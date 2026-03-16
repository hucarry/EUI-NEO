# EUI（中文说明）

[English README](readme.md)

EUI 是一个轻量、头文件式（header-only）的 C++ UI 工具包，偏重实际可用的即时模式（immediate-mode）工作流。
核心 API 在 `include/EUI.h` 中，只负责产出绘制命令。
当启用 `EUI_ENABLE_GLFW_OPENGL_BACKEND` 时，可使用内置的 GLFW + OpenGL demo 运行时。

## 预览

![Preview](preview/0.jpg)
![Preview](preview/1.jpg)

## 项目现状分析（基于当前代码）

### 1）整体架构

- **核心层（`eui::Context`）**
  - 即时模式 UI API，输出 `DrawCommand` + 文本缓冲区（text arena）。
  - 核心不强依赖 GLFW/OpenGL，可独立接入你自己的渲染后端。
- **可选运行时层（`eui::demo`）**
  - 负责窗口循环、输入采集、DPI 计算、剪贴板桥接、帧调度。
  - 通过 `FrameContext` 回调驱动你写的 UI。
- **渲染层（同在 `EUI.h`）**
  - OpenGL 指令渲染，支持裁剪与批处理。
  - Windows 下使用 GDI/wgl 字形列表渲染文字，并支持图标字体回退。
  - 非 Windows 路径保留了内置位图字体回退逻辑。

### 2）当前渲染链路

1. `ui.begin_frame(...)`
2. 构建控件与布局
3. `ui.take_frame(...)` 取出命令和文本缓冲
4. 对整帧命令做 hash
5. 与上一帧相同则跳过重绘
6. 不同则与上一帧计算脏矩形
7. 仅重绘脏区（scissor），其余区域复用缓存纹理

### 3）已落地的性能机制

- 默认事件驱动渲染（`continuous_render = false`）。
- 帧级 hash 快速早退，避免无意义 GPU 重绘。
- 前后帧命令差异计算脏区。
- 缓存帧缓冲纹理 + 脏区局部刷新。
- Core 层有 clip stack 与命令级裁剪。
- 命令量较大时启用基于 tile 的命令分桶。

## 已实现功能（v0.1.0）

### 主题

- `ThemeMode`（`Light` / `Dark`）
- 主色（`set_primary_color`）
- 圆角（`set_corner_radius`）

### 布局

- `begin_panel` / `end_panel`
- `begin_card` / `end_card`
- `begin_row` / `end_row`
- `begin_columns` / `end_columns`
- `begin_waterfall` / `end_waterfall`
- `spacer`

### 控件

- `label`
- `button`（`Primary`、`Secondary`、`Ghost`）
- `tab`
- `slider_float`（拖动 + 右键数值编辑）
- `input_float`（光标、选区、`Ctrl+A/C/V/X`）
- `input_readonly`
  - 支持 `align_right`、`value_font_scale`、`muted`
- `progress`
- `begin_dropdown` / `end_dropdown`
- `begin_scroll_area` / `end_scroll_area`
  - 支持拖拽、滚轮、惯性、回弹、滚动条参数
- `text_area`（可编辑、可选择、光标、滚动）
- `text_area_readonly`

### 输出与集成

- `end_frame()` 返回 `std::vector<DrawCommand>`
- `take_frame(...)` 可高效转移本帧命令缓冲
- `text_arena()` 返回文本绘制命令使用的文本存储

## 仓库结构

```text
EUI/
|- include/
|  `- EUI.h
|- examples/
|  |- basic_demo.cpp
|  `- calculator_demo.cpp
|- CMakeLists.txt
|- index.html
|- readme.md
`- readme.zh-CN.md
```

## 构建

推荐使用 `Ninja` 生成器。

### 1）仅构建核心（不依赖 GLFW）

```bash
cmake -S . -B build -G Ninja -DEUI_BUILD_EXAMPLES=OFF
cmake --build build
```

会生成：

- `EUI::eui`（interface）

### 2）构建示例（GLFW + OpenGL）

```bash
cmake -S . -B build -G Ninja -DEUI_BUILD_EXAMPLES=ON
cmake --build build
```

当 OpenGL 与 GLFW 都可用时，会生成：

- `eui_demo`（`examples/basic_demo.cpp`）
- `eui_calculator_demo`（`examples/calculator_demo.cpp`）

重要 CMake 选项：

```bash
-DEUI_BUILD_EXAMPLES=ON|OFF
-DEUI_STRICT_WARNINGS=ON|OFF
-DEUI_FETCH_GLFW_FROM_GIT=ON|OFF
-DEUI_GLFW_GIT_TAG=3.4
```

若网络或 Git 不可用：

```bash
cmake -S . -B build -G Ninja -DEUI_BUILD_EXAMPLES=ON -DEUI_FETCH_GLFW_FROM_GIT=OFF
```

## 运行示例

```bash
# 主 demo
cmake --build build --target eui_demo

# 计算器 demo
cmake --build build --target eui_calculator_demo
```

## 核心最小用法

```cpp
#include "EUI.h"

eui::Context ui;
eui::InputState input{};

float value = 0.5f;
bool advanced_open = false;

ui.begin_frame(1280.0f, 720.0f, input);
ui.begin_panel("Demo", 20.0f, 20.0f, 640.0f);

ui.begin_row(2, 8.0f);
ui.button("Run", eui::ButtonStyle::Primary);
ui.input_float("Value", value, 0.0f, 1.0f, 2);
ui.end_row();

if (ui.begin_dropdown("Advanced", advanced_open, 80.0f)) {
    ui.progress("Loading", 0.42f);
    ui.end_dropdown();
}

ui.end_panel();

const auto& commands = ui.end_frame();
const auto& text_arena = ui.text_arena();
```

## 可选 Demo 运行时用法

```cpp
#define EUI_ENABLE_GLFW_OPENGL_BACKEND 1
#include "EUI.h"

int main() {
    eui::demo::AppOptions options{};
    options.width = 960;
    options.height = 710;
    options.title = "EUI Demo";
    options.vsync = true;
    options.continuous_render = false;
    options.max_fps = 240.0;

    options.text_font_family = "Segoe UI";
    options.icon_font_family = "Segoe MDL2 Assets";
    options.enable_icon_font_fallback = true;

    return eui::demo::run(
        [&](eui::demo::FrameContext frame) {
            auto& ui = frame.ui;
            ui.set_theme_mode(eui::ThemeMode::Dark);

            ui.begin_panel("Demo", 20.0f, 20.0f, 320.0f);
            ui.label("Hello EUI");
            ui.end_panel();

            // 事件驱动模式下，如需动画请主动请求下一帧。
            frame.request_next_frame();
        },
        options
    );
}
```

## 说明

- `index.html` 是视觉原型参考，不参与 C++ 构建产物。
- 建议源码统一使用 UTF-8，避免 Windows 工具链下 C4819 和字符串乱码问题。
