# EUI-NEO 全量重绘与 GPU/CPU 性能优化说明

## 目标

- 保持当前界面结果不变，不通过降低分辨率、降低刷新率、删除动画来“优化”。
- 优先做底层优化，减少无意义的全量重绘、重复提交和驱动开销。
- 明确哪些方案已经验证安全，哪些方案已经验证会破坏显示，防止以后回退。

---

## 当前稳定基线

当前项目的稳定渲染路线是：

- 事件驱动重绘。
- `RenderLayer::Backdrop` 单独走 layer cache。
- `CompositeLayers(...)` 只合成 `Backdrop`。
- 普通前景节点仍在最终屏幕绘制阶段输出。
- 非 `blur` 节点可走 `Renderer::DrawCachedSurface(...)` 的节点级缓存。
- `blur` 节点直接绘制，不走普通节点缓存。

当前基线对应文件：

- `main.cpp`
- `src/EUINEO.cpp`
- `src/ui/UIContext.cpp`
- `src/ui/UINode.h`

---

## 已落地且保留的优化

下面这些已经进代码，并且当前保留。

### 1. 主循环改成事件驱动输入同步

位置：

- `main.cpp`

已做内容：

- 鼠标位置改为通过 `glfwSetCursorPosCallback(...)` 更新。
- framebuffer/window 尺寸改为通过回调更新。
- 去掉每帧 `glfwGetFramebufferSize(...)`。
- 去掉每帧 `glfwGetCursorPos(...)`。
- 去掉每帧 `glfwGetWindowSize(...)`。
- 去掉每帧 `glfwGetMouseButton(...)` 轮询。
- `mouseClicked` / `mouseRightClicked` 在帧尾统一清零。

收益：

- 降低了纯 CPU 轮询成本。
- 减少了交互空转时的主线程开销。

### 2. 文本测量缓存

位置：

- `src/EUINEO.cpp`

已做内容：

- 保留 `TextWidthCache`。
- 新增 `TextBoundsCache`，缓存 `MeasureTextBounds(...)` 的结果。
- 新字形加载时同时清理宽度缓存和 bounds 缓存。

收益：

- 降低了布局和对齐过程中重复文本测量的 CPU 成本。

### 3. 文本绘制状态切换收敛

位置：

- `src/EUINEO.cpp`

已做内容：

- `DrawTextStr(...)` 中 `TextVBO` 改为每次字符串绘制只绑定一次。
- `TextModeLoc` 只在 SDF/非 SDF 模式变化时更新。
- glyph 纹理只在纹理 id 变化时重绑。
- 不再在每个字符串绘制结束时做无意义的 buffer / texture unbind。

收益：

- 降低了文字多的页面在交互时的 driver 开销。

### 4. 帧级 uniform 下沉到 `BeginFrame()`

位置：

- `src/EUINEO.cpp`

已做内容：

- `iTime`
- `iResolution`
- 采样器 uniform 初始化

这些不再每个矩形都重复提交，而是放到初始化或 `BeginFrame()`。

收益：

- 降低了 `DrawRect(...)` 热路径中的 uniform 提交量。

### 5. Polygon VBO 容量复用

位置：

- `src/EUINEO.cpp`

已做内容：

- 为 `PolygonVBO` 增加容量记录 `PolygonVBOCapacity`。
- 顶点数据增长时才重新 `glBufferData(...)`。
- 常规更新改为 `glBufferSubData(...)`。

收益：

- 避免 polygon 动画或反复绘制时每次重新分配 VBO。

### 6. Label 节点接入缓存表面

位置：

- `src/components/Label.h`

已做内容：

- `LabelNode::usesCachedSurface()` 由 `false` 改为 `true`。

收益：

- 侧边栏 hover、页面滚动、普通重绘时，不再把所有标签文本逐 glyph 重画。

### 7. 节点缓存允许“只平移不重绘”

位置：

- `src/EUINEO.cpp`

已做内容：

- `Renderer::DrawCachedSurface(...)` 只在以下情况重建离屏表面：
  - 节点内容 dirty
  - 缓存还没准备好
  - 节点尺寸变化
- 纯位置变化只复用旧纹理并重新 composite。

收益：

- 滚动和整体平移时，大量节点不再因为位置变化而重画内容。

### 8. 安全范围内恢复 blur cache 复用

位置：

- `src/EUINEO.cpp`

已做内容：

- `CanReuseBlurCache(...)` 现在只在安全路径返回 `true`：
  - 默认 framebuffer
  - 或 `RenderLayer::Backdrop`
  - 且不是 custom surface

收益：

- 降低静态毛玻璃场景中的重复 blur 成本。

限制：

- 不允许把这个逻辑直接扩展到任意 layer 或任意 custom surface。

### 9. MSVC 构建参数冲突修正

位置：

- `CMakeLists.txt`

已做内容：

- 删除了目标级 `/O1`。
- 保留 Release 自带的 `/O2`。

原因：

- `/O1` 与 Release 的 `/O2` 同时存在时，MSVC 会报 `D9025`，而且最后实际会被 `/O1` 覆盖。

---

## 已验证错误的路线，禁止再引入

下面这些方案已经验证会破坏显示或造成残影，不要再回到这些路线。

### 1. 所有 layer 都走整层 FBO 合成

禁止内容：

- `Content`
- `Chrome`
- `Popup`

都走整层离屏缓存再统一 composite。

历史问题：

- 控件 hover 才出现。
- 鼠标移开就消失。
- 残影。
- 蓝色残留。
- 黑屏。
- 错误裁剪。
- 合成顺序错乱。

结论：

- 当前只允许 `Backdrop` 走 layer cache。

### 2. 在普通 layer draw 路径里重新套节点缓存链

错误尝试：

- 把 `DrawCachedSurface(...)` 再嵌进 `draw(RenderLayer layer)` 的前景 layer 路线里。

结论：

- 节点缓存只适合最终前景绘制阶段使用。
- 不要再把它和多 layer FBO 合成链混用。

### 3. 鼠标移动回调里直接 `RequestRepaint()`

必须保持：

- 鼠标移动只更新状态和 `State.pointerMoved`。
- 是否真正重绘，由 `Update()` 和渲染器判断。

### 4. 回到脏矩形/局部回收/遮挡传播系统

当前项目已经证明：

- 这条路线复杂度高。
- 容易导致残影和局部不刷新。
- 后续维护成本非常差。

---

## 当前仍然存在的热点

以下热点是当前用户侧已经观察到的真实问题。

### 1. 侧边栏 hover 仍然可能有较高 GPU 占用

现象：

- 侧边栏按钮 hover 时，GPU 占用仍可能达到约 15%。

当前判断：

- 虽然文本和部分节点已经走缓存，但前景层仍是最终屏幕直接绘制。
- 侧边栏 hover 会触发前景重绘。
- 如果页面本身前景节点很多，仍会有明显 GPU 压力。

### 2. 滚动区域移动时 GPU 占用可能很高

现象：

- 滚动区域一动，GPU 占用可能到 60%。

当前判断：

- `ScrollAreaNode` 每次滚动都可能触发 `requestComposeRebuild()`。
- 页面结构虽然没变，但 compose 路线会重新跑。
- 即使节点缓存已经允许纯平移复用，滚动期间仍有大量节点需要重新参与布局与绘制链判断。

关键文件：

- `src/components/ScrollArea.h`
- `src/ui/UIContext.cpp`
- `src/pages/TypographyPage.h`

### 3. Slider 拖动会直接打脏 Backdrop

现象：

- 首页滑块拖动时 GPU 占用明显上升。

根因：

- `src/pages/MainPage.h` 中 `glassPanel("content")` 的 `blur(...)` 直接依赖 `progressValue_`。
- `SetProgressValue(...)` 会调用：

```cpp
Renderer::InvalidateLayer(RenderLayer::Backdrop);
Renderer::RequestRepaint(0.18f);
```

也就是说：

- slider 每次变化
- 都会让整块玻璃背景重新进 `Backdrop`
- 然后重新跑毛玻璃相关路径

这不是 slider 控件本身慢，而是它驱动了大面积 blur。

### 4. 大面积 blur 仍是最大 GPU 成本来源

当前主要 blur 区域：

- `src/pages/MainPage.h` 的 `glassPanel("content")`

这块一旦失效，会显著拉高 GPU 开销。

---

## 当前必须遵守的规则

### 状态规则

- 只有真实状态变化才改页面状态。
- 只有真实视觉变化才 `requestVisualRepaint()`。
- 不要因为 hover 就改页面级 `stateVersion_`。
- 不要把小交互升级成整页 recompose。

### Backdrop 规则

- 只有真的影响玻璃背景、背景装饰、主题背景时，才 `InvalidateLayer(RenderLayer::Backdrop)`。
- 普通前景 hover、文字变化、按钮状态变化，不要顺手打脏 `Backdrop`。

### 缓存规则

- 稳定 key 是前提。
- `paintBounds()` 必须准确。
- 节点纯平移时应尽量复用缓存，不要重绘内容。
- blur 节点默认视为昂贵节点。

### 连续更新规则

- `wantsContinuousUpdate()` 只能在动画、拖动、光标闪烁等真实需要时返回 `true`。
- 动画停止后必须自动降回 `false`。

---

## 后续优化优先级

下面这些是当前最值得继续做的方向。

### 优先级 1：拆掉滚动中的不必要 recompose

目标：

- 滚动偏移变化时，只更新 offset 和必要视觉，不整页重 compose。

重点文件：

- `src/components/ScrollArea.h`
- `src/ui/UIContext.cpp`

### 优先级 2：继续压缩大面积 blur 成本

目标：

- 在不改画面结果的前提下，减少 `Backdrop` 反复重算成本。

重点文件：

- `src/EUINEO.cpp`
- `src/pages/MainPage.h`

### 优先级 3：给前景重绘增加运行时统计

目标：

- 不是继续猜测，而是直接输出：
  - 哪个 layer 变脏
  - 哪些节点频繁 dirty
  - 哪类交互触发了 Backdrop 失效

建议位置：

- `src/EUINEO.cpp`
- `src/ui/UIContext.cpp`
- `main.cpp`

### 优先级 4：评估前景层安全缓存方案

注意：

- 不是回到“所有 layer 整层 FBO 合成”。
- 而是先通过统计确认真正高成本的前景区域，再设计可控的缓存边界。

---

## 出现问题时的排查顺序

### 出现高 GPU/CPU 时

按这个顺序查：

1. 是否把鼠标移动回调改回了直接 `RequestRepaint()`。
2. 是否某个组件长期 `wantsContinuousUpdate() == true`。
3. 是否普通前景变化误打脏了 `Backdrop`。
4. 是否滚动链路把纯偏移升级成了 `requestComposeRebuild()`。
5. 是否 blur 面积过大且频繁失效。
6. 是否节点 key 不稳定导致缓存失效。
7. 是否 `paintBounds()` 过大导致缓存开销被放大。

### 出现残影/蓝色残留/黑屏时

按这个顺序查：

1. 是否又把 `Content/Chrome/Popup` 改回整层 FBO 合成。
2. 是否又把节点缓存嵌回了 layer draw 路线。
3. `BeginFrame()` 是否仍然先 `glDisable(GL_SCISSOR_TEST)`。
4. 是否在错误的 custom surface / layer 状态里 composite 纹理。
5. `paintBounds()` 是否过小导致缓存内容被裁掉。

### 出现 hover 才显示、移开消失时

先看：

1. 是否重新引入了多 layer 整层缓存。
2. 是否 layer dirty/ready 生命周期被改坏。
3. 是否前景层没有在最终屏幕路径正确绘制。

---

## 结论

EUI-NEO 当前稳定路线不是脏矩形系统，也不是多 layer 全量 FBO 合成，而是：

- 事件驱动重绘
- `Backdrop` 单层缓存
- 普通节点在最终屏幕阶段绘制
- 非 blur 节点做节点级缓存
- 文本测量和绘制热路径压缩
- 纯平移复用缓存
- 精确 `paintBounds()`
- 只在真实变化时持续更新

后续继续优化时，优先盯住这三个热点：

- `ScrollArea` 的 recompose 链
- `MainPage` 中由 slider 驱动的 `Backdrop` 失效
- 大面积 blur 的重算成本
