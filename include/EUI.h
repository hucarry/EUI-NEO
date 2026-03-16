#ifndef EUI_H_
#define EUI_H_

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef EUI_ENABLE_GLFW_OPENGL_BACKEND
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <GLFW/glfw3.h>
#ifdef _WIN32
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#endif

#include <array>
#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif
#endif

namespace eui {

struct Color {
    float r{1.0f};
    float g{1.0f};
    float b{1.0f};
    float a{1.0f};
};

inline Color rgba(float r, float g, float b, float a = 1.0f) {
    return Color{r, g, b, a};
}

inline Color mix(const Color& lhs, const Color& rhs, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return Color{
        lhs.r + (rhs.r - lhs.r) * t,
        lhs.g + (rhs.g - lhs.g) * t,
        lhs.b + (rhs.b - lhs.b) * t,
        lhs.a + (rhs.a - lhs.a) * t,
    };
}

struct Rect {
    float x{0.0f};
    float y{0.0f};
    float w{0.0f};
    float h{0.0f};

    bool contains(float px, float py) const {
        return px >= x && px <= x + w && py >= y && py <= y + h;
    }
};

enum class ThemeMode {
    Light,
    Dark,
};

enum class ButtonStyle {
    Primary,
    Secondary,
    Ghost,
};

struct Theme {
    Color background{};
    Color panel{};
    Color panel_border{};
    Color text{};
    Color muted_text{};
    Color primary{};
    Color primary_text{};
    Color secondary{};
    Color secondary_hover{};
    Color secondary_active{};
    Color track{};
    Color track_fill{};
    Color outline{};
    Color input_bg{};
    Color input_border{};
    Color focus_ring{};
    float radius{8.0f};
};

inline Theme make_theme(ThemeMode mode, const Color& primary) {
    Theme theme{};
    theme.primary = primary;
    theme.radius = 8.0f;

    if (mode == ThemeMode::Dark) {
        theme.background = rgba(0.05f, 0.07f, 0.10f, 1.0f);
        theme.panel = rgba(0.09f, 0.12f, 0.16f, 1.0f);
        theme.panel_border = rgba(0.18f, 0.23f, 0.30f, 1.0f);
        theme.text = rgba(0.92f, 0.95f, 0.98f, 1.0f);
        theme.muted_text = rgba(0.63f, 0.70f, 0.79f, 1.0f);
        theme.primary_text = rgba(0.06f, 0.10f, 0.17f, 1.0f);
        theme.secondary = rgba(0.15f, 0.20f, 0.27f, 1.0f);
        theme.secondary_hover = mix(theme.secondary, theme.primary, 0.18f);
        theme.secondary_active = mix(theme.secondary, theme.primary, 0.32f);
        theme.track = rgba(0.18f, 0.23f, 0.31f, 1.0f);
        theme.track_fill = theme.primary;
        theme.outline = rgba(0.25f, 0.31f, 0.40f, 1.0f);
        theme.input_bg = rgba(0.08f, 0.11f, 0.15f, 1.0f);
        theme.input_border = mix(rgba(0.26f, 0.33f, 0.42f, 1.0f), theme.primary, 0.20f);
        theme.focus_ring = mix(theme.primary, rgba(1.0f, 1.0f, 1.0f, 1.0f), 0.18f);
    } else {
        theme.background = rgba(0.96f, 0.97f, 0.99f, 1.0f);
        theme.panel = rgba(1.0f, 1.0f, 1.0f, 1.0f);
        theme.panel_border = rgba(0.84f, 0.88f, 0.93f, 1.0f);
        theme.text = rgba(0.11f, 0.15f, 0.22f, 1.0f);
        theme.muted_text = rgba(0.41f, 0.47f, 0.58f, 1.0f);
        theme.primary_text = rgba(0.96f, 0.98f, 1.0f, 1.0f);
        theme.secondary = rgba(0.92f, 0.94f, 0.97f, 1.0f);
        theme.secondary_hover = mix(theme.secondary, theme.primary, 0.12f);
        theme.secondary_active = mix(theme.secondary, theme.primary, 0.24f);
        theme.track = rgba(0.90f, 0.92f, 0.96f, 1.0f);
        theme.track_fill = theme.primary;
        theme.outline = rgba(0.80f, 0.85f, 0.92f, 1.0f);
        theme.input_bg = rgba(1.0f, 1.0f, 1.0f, 1.0f);
        theme.input_border = mix(rgba(0.79f, 0.84f, 0.91f, 1.0f), theme.primary, 0.28f);
        theme.focus_ring = mix(theme.primary, rgba(1.0f, 1.0f, 1.0f, 1.0f), 0.10f);
    }
    return theme;
}

struct InputState {
    float mouse_x{0.0f};
    float mouse_y{0.0f};
    float mouse_wheel_y{0.0f};
    bool mouse_down{false};
    bool mouse_pressed{false};
    bool mouse_released{false};

    bool mouse_right_down{false};
    bool mouse_right_pressed{false};
    bool mouse_right_released{false};

    bool key_backspace{false};
    bool key_delete{false};
    bool key_enter{false};
    bool key_escape{false};
    bool key_left{false};
    bool key_right{false};
    bool key_home{false};
    bool key_end{false};
    bool key_select_all{false};
    bool key_copy{false};
    bool key_cut{false};
    bool key_paste{false};
    bool key_shift{false};
    std::string text_input{};
    std::string clipboard_text{};
    double time_seconds{0.0};
};

enum class CommandType {
    FilledRect,
    RectOutline,
    Text,
};

enum class TextAlign {
    Left,
    Center,
    Right,
};

struct DrawCommand {
    CommandType type{CommandType::FilledRect};
    Rect rect{};
    Rect clip_rect{};
    Color color{};
    std::uint32_t text_offset{0};
    std::uint32_t text_length{0};
    float font_size{13.0f};
    TextAlign align{TextAlign::Left};
    float radius{0.0f};
    float thickness{1.0f};
    bool has_clip{false};
    std::uint64_t hash{0ull};
};

class Context {
public:
    Context() {
        commands_.reserve(1024);
        scope_stack_.reserve(24);
        text_arena_.reserve(16 * 1024);
        input_buffer_.reserve(64);
        theme_.radius = corner_radius_;
    }

    void set_theme_mode(ThemeMode mode) {
        if (theme_mode_ == mode) {
            return;
        }
        theme_mode_ = mode;
        refresh_theme();
    }

    ThemeMode theme_mode() const {
        return theme_mode_;
    }

    void set_primary_color(const Color& color) {
        primary_color_ = color;
        refresh_theme();
    }

    void set_corner_radius(float radius) {
        corner_radius_ = std::clamp(radius, 0.0f, 28.0f);
        theme_.radius = corner_radius_;
    }

    float corner_radius() const {
        return corner_radius_;
    }

    const Theme& theme() const {
        return theme_;
    }

    void begin_frame(float width, float height, const InputState& input) {
        frame_width_ = std::max(1.0f, width);
        frame_height_ = std::max(1.0f, height);
        input_ = input;
        repaint_requested_ = false;
        if (has_prev_input_time_) {
            const double dt = input_.time_seconds - prev_input_time_;
            ui_dt_ = std::clamp(static_cast<float>(dt), 1.0f / 240.0f, 0.10f);
        } else {
            ui_dt_ = 1.0f / 60.0f;
            has_prev_input_time_ = true;
        }
        prev_input_time_ = input_.time_seconds;
        commands_.clear();
        text_arena_.clear();
        flush_row();
        scope_stack_.clear();
        clip_stack_.clear();
        waterfall_ = WaterfallState{};

        content_x_ = 16.0f;
        content_width_ = frame_width_ - 32.0f;
        cursor_y_ = 16.0f;
        panel_id_seed_ = 1469598103934665603ull;
    }

    const std::vector<DrawCommand>& end_frame() {
        finalize_frame_state();
        return commands_;
    }

    void take_frame(std::vector<DrawCommand>& out_commands, std::vector<char>& out_text_arena) {
        finalize_frame_state();
        out_commands.clear();
        out_text_arena.clear();
        out_commands.swap(commands_);
        out_text_arena.swap(text_arena_);
        const std::size_t cmd_cap = out_commands.capacity();
        const std::size_t text_cap = out_text_arena.capacity();
        commands_.clear();
        text_arena_.clear();
        if (commands_.capacity() < cmd_cap) {
            commands_.reserve(cmd_cap);
        }
        if (text_arena_.capacity() < text_cap) {
            text_arena_.reserve(text_cap);
        }
    }

    const std::vector<char>& text_arena() const {
        return text_arena_;
    }

    const std::vector<DrawCommand>& commands() const {
        return commands_;
    }

private:
    void finalize_frame_state() {
        flush_row();
        scope_stack_.clear();
        clip_stack_.clear();
        waterfall_ = WaterfallState{};
        active_slider_id_ = input_.mouse_down ? active_slider_id_ : 0;
        active_textarea_scroll_id_ = input_.mouse_down ? active_textarea_scroll_id_ : 0;
        if (!input_.mouse_down) {
            active_scroll_drag_id_ = 0;
            active_scrollbar_drag_id_ = 0;
        }
    }

public:
    void begin_panel(std::string_view title, float x, float y, float width, float padding = 14.0f,
                     float radius = 10.0f) {
        flush_row();
        scope_stack_.clear();
        waterfall_ = WaterfallState{};

        const float panel_w = std::max(140.0f, width);
        const float panel_h = std::max(120.0f, frame_height_ - y - 20.0f);
        panel_rect_ = Rect{x, y, panel_w, panel_h};

        if (radius >= 0.0f) {
            add_filled_rect(panel_rect_, theme_.panel, radius);
            add_outline_rect(panel_rect_, theme_.panel_border, radius);
        }

        content_x_ = x + padding;
        content_width_ = std::max(20.0f, panel_w - 2.0f * padding);
        cursor_y_ = y + padding;

        panel_id_seed_ = hash_sv(title);
        if (!title.empty()) {
            const float title_font = std::clamp(theme_.radius * 1.4f, 14.0f, 24.0f);
            const float title_h = title_font + 2.0f;
            add_text(title, Rect{content_x_, cursor_y_, content_width_, title_h}, theme_.text, title_font,
                     TextAlign::Left);
            cursor_y_ += title_h + 6.0f;
        }
    }

    void end_panel() {
        flush_row();
        scope_stack_.clear();
        waterfall_ = WaterfallState{};
    }

    void begin_card(std::string_view title, float height = 0.0f, float padding = 14.0f,
                    float radius = 8.0f) {
        padding = std::clamp(padding, 6.0f, 28.0f);
        const float min_height = std::max(42.0f, height);
        bool in_waterfall = false;
        int column_index = -1;
        Rect card{};
        if (waterfall_.active && !row_.active) {
            in_waterfall = true;
            float best_y = waterfall_.column_y.empty() ? cursor_y_ : waterfall_.column_y[0];
            int best_idx = 0;
            for (int i = 1; i < waterfall_.columns; ++i) {
                const float y = waterfall_.column_y[static_cast<std::size_t>(i)];
                if (y < best_y) {
                    best_y = y;
                    best_idx = i;
                }
            }
            column_index = best_idx;
            const float x =
                content_x_ + static_cast<float>(best_idx) * (waterfall_.item_width + waterfall_.gap);
            card = Rect{x, best_y, waterfall_.item_width, min_height};
        } else {
            card = next_rect(min_height);
        }
        const std::size_t fill_cmd_index = commands_.size();
        add_filled_rect(card, theme_.panel, radius);
        const std::size_t outline_cmd_index = commands_.size();
        add_outline_rect(card, theme_.panel_border, radius);
        const bool had_outer_row = row_.active;
        const RowState outer_row = row_;

        scope_stack_.push_back(ScopeState{
            ScopeKind::Card,
            content_x_,
            content_width_,
            0.0f,
            fill_cmd_index,
            outline_cmd_index,
            card.y,
            min_height,
            padding,
            had_outer_row,
            outer_row,
            in_waterfall,
            column_index,
        });

        content_x_ = card.x + padding;
        content_width_ = std::max(10.0f, card.w - 2.0f * padding);
        cursor_y_ = card.y + padding;
        row_ = RowState{};

        if (!title.empty()) {
            const float title_font = std::clamp(theme_.radius * 1.4f, 14.0f, 24.0f);
            const float title_h = title_font + 2.0f;
            add_text(title, Rect{content_x_, cursor_y_, content_width_, title_h}, theme_.text, title_font,
                     TextAlign::Left);
            cursor_y_ += title_h + 6.0f;
        }
    }

    void end_card() {
        flush_row();
        if (scope_stack_.empty()) {
            return;
        }
        restore_scope();
    }

    void label(std::string_view text, float font_size = 13.0f, bool muted = false) {
        const float height = font_size + 6.0f;
        const Rect rect = next_rect(height);
        add_text(text, Rect{rect.x, rect.y, rect.w, height}, muted ? theme_.muted_text : theme_.text,
                 font_size, TextAlign::Left);
    }

    void spacer(float height = 8.0f) {
        flush_row();
        cursor_y_ += std::max(0.0f, height);
    }

    bool button(std::string_view label, ButtonStyle style = ButtonStyle::Secondary, float height = 34.0f) {
        const Rect rect = next_rect(height);
        const bool hovered = rect.contains(input_.mouse_x, input_.mouse_y);
        const bool held = hovered && input_.mouse_down;
        bool icon_like = false;
        if (!label.empty() && label.size() <= 4u) {
            icon_like = true;
            for (char ch : label) {
                const unsigned char uch = static_cast<unsigned char>(ch);
                if (uch < 0x80u && std::isalnum(uch)) {
                    icon_like = false;
                    break;
                }
            }
        }
        const float text_size = icon_like ? std::clamp(rect.h * 0.72f, 16.0f, 34.0f)
                                          : std::clamp(rect.h * 0.38f, 12.0f, 28.0f);

        // For labels like "<icon>  TEXT", draw icon/text separately to keep visual vertical alignment.
        bool icon_text_combo = false;
        std::string_view icon_part{};
        std::string_view text_part{};
        if (!label.empty() && static_cast<unsigned char>(label.front()) >= 0x80u) {
            std::size_t split = label.find("  ");
            if (split == std::string_view::npos) {
                split = label.find(' ');
            }
            if (split != std::string_view::npos) {
                std::size_t text_start = split;
                while (text_start < label.size() && label[text_start] == ' ') {
                    text_start += 1u;
                }
                if (split > 0u && text_start < label.size()) {
                    icon_part = label.substr(0u, split);
                    text_part = label.substr(text_start);
                    icon_text_combo = true;
                }
            }
        }

        Color fill = theme_.secondary;
        Color text_color = theme_.text;
        if (style == ButtonStyle::Primary) {
            fill = theme_.primary;
            text_color = theme_.primary_text;
        } else if (style == ButtonStyle::Ghost) {
            fill = hovered ? mix(theme_.secondary, theme_.panel, 0.5f) : theme_.panel;
            text_color = theme_.text;
        }

        if (held) {
            fill = mix(fill, theme_.secondary_active, 0.35f);
        } else if (hovered && style != ButtonStyle::Ghost) {
            fill = mix(fill, theme_.secondary_hover, 0.30f);
        }

        add_filled_rect(rect, fill, theme_.radius);
        add_outline_rect(rect, style == ButtonStyle::Primary ? theme_.primary : theme_.outline, theme_.radius);
        if (icon_text_combo) {
            const float pad = std::clamp(rect.h * 0.24f, 8.0f, 14.0f);
            const Rect icon_rect{
                rect.x + pad,
                rect.y,
                std::max(14.0f, rect.h - pad * 0.2f),
                rect.h,
            };
            const float icon_size = std::clamp(rect.h * 0.60f, 13.0f, 34.0f);
            const float text_x = icon_rect.x + icon_rect.w + std::max(4.0f, pad * 0.45f);
            const Rect text_rect{
                text_x,
                rect.y,
                std::max(0.0f, rect.x + rect.w - text_x - pad),
                rect.h,
            };
            const float combo_text_size = std::clamp(rect.h * 0.35f, 11.0f, 24.0f);
            add_text(icon_part, icon_rect, text_color, icon_size, TextAlign::Center);
            add_text(text_part, text_rect, text_color, combo_text_size, TextAlign::Left);
        } else {
            add_text(label, Rect{rect.x, rect.y, rect.w, rect.h}, text_color, text_size, TextAlign::Center);
        }

        return hovered && input_.mouse_pressed;
    }

    bool tab(std::string_view label, bool selected, float height = 30.0f) {
        const Rect rect = next_rect(height);
        const bool hovered = rect.contains(input_.mouse_x, input_.mouse_y);
        const bool held = hovered && input_.mouse_down;
        const float text_size = std::clamp(rect.h * 0.42f, 13.0f, 26.0f);

        Color fill = selected ? mix(theme_.primary, theme_.panel, 0.74f) : theme_.secondary;
        if (selected && hovered) {
            fill = mix(theme_.primary, theme_.panel, 0.66f);
        } else if (!selected && hovered) {
            fill = theme_.secondary_hover;
        }
        if (held) {
            fill = mix(fill, theme_.secondary_active, selected ? 0.22f : 0.35f);
        }

        const float tab_radius = std::max(0.0f, theme_.radius - 2.0f);
        add_filled_rect(rect, fill, tab_radius);
        add_outline_rect(rect, selected ? theme_.primary : mix(theme_.outline, theme_.panel, 0.6f), tab_radius,
                         selected ? 1.4f : 1.0f);
        add_text(label, Rect{rect.x, rect.y, rect.w, rect.h},
                 selected ? theme_.text : theme_.muted_text, text_size, TextAlign::Center);

        return hovered && input_.mouse_pressed;
    }

    bool slider_float(std::string_view label, float& value, float min_value, float max_value, int decimals = -1,
                      float height = 40.0f) {
        if (max_value < min_value) {
            std::swap(max_value, min_value);
        }

        const int value_decimals = resolve_decimals(min_value, max_value, decimals);
        const std::uint64_t id = id_for(label) ^ 0x62e2ac4d9d45f7b1ull;
        const Rect rect = next_rect(height);
        const float radius = theme_.radius;
        const float label_font = std::clamp(rect.h * 0.36f, 13.0f, 24.0f);
        const float value_font = std::max(12.0f, label_font - 0.5f);
        const float value_padding = std::clamp(rect.h * 0.15f, 6.0f, 12.0f);
        const float value_box_w = std::clamp(rect.h * 1.8f, 64.0f, 128.0f);
        const Rect value_box{
            rect.x + rect.w - value_box_w - value_padding,
            rect.y + value_padding,
            value_box_w,
            rect.h - value_padding * 2.0f,
        };

        const bool hovered = rect.contains(input_.mouse_x, input_.mouse_y);
        const bool value_hovered = value_box.contains(input_.mouse_x, input_.mouse_y);

        if (input_.mouse_right_pressed && value_hovered) {
            start_text_input(id, format_float(value, value_decimals), true);
        }

        bool changed = false;
        if (is_text_input_active(id)) {
            update_mouse_selection(value_box, value_font, true, value_padding, value_hovered);
            consume_numeric_typing(min_value < 0.0f, true);
            clamp_live_numeric_buffer(min_value, max_value, value_decimals);
            if (input_.key_escape) {
                stop_text_input();
            } else if (input_.key_enter || (input_.mouse_pressed && !value_hovered)) {
                changed = commit_text_input(value, min_value, max_value) || changed;
            }
        }

        if (hovered && input_.mouse_pressed && !value_hovered && !is_text_input_active(id)) {
            active_slider_id_ = id;
        }

        if (active_slider_id_ == id) {
            if (input_.mouse_down) {
                float t = (input_.mouse_x - rect.x) / rect.w;
                t = std::clamp(t, 0.0f, 1.0f);
                const float new_value = min_value + (max_value - min_value) * t;
                if (std::fabs(new_value - value) > 1e-6f) {
                    value = new_value;
                    changed = true;
                }
            }
            if (!input_.mouse_down || input_.mouse_released) {
                active_slider_id_ = 0;
            }
        }

        const float range = std::max(1e-6f, max_value - min_value);
        const float t = std::clamp((value - min_value) / range, 0.0f, 1.0f);
        const float inner_x = rect.x + 1.0f;
        const float inner_y = rect.y + 1.0f;
        const float inner_w = std::max(0.0f, rect.w - 2.0f);
        const float inner_h = std::max(0.0f, rect.h - 2.0f);
        const float thumb_w = std::clamp(rect.h * 0.24f, 4.0f, 10.0f);
        const float thumb_center_x = inner_x + inner_w * t;
        const float thumb_x = std::clamp(thumb_center_x - thumb_w * 0.5f, inner_x,
                                         inner_x + std::max(0.0f, inner_w - thumb_w));
        float fill_right = inner_x + inner_w * t;
        if (t > 1e-4f) {
            fill_right = std::min(inner_x + inner_w, fill_right + thumb_w * 0.25f);
        }
        if (t >= 0.9999f) {
            fill_right = inner_x + inner_w;
        }
        const Rect fill{
            inner_x,
            inner_y,
            std::max(0.0f, fill_right - inner_x),
            inner_h,
        };
        const Rect thumb{
            thumb_x,
            inner_y,
            thumb_w,
            inner_h,
        };
        const bool thumb_hovered = thumb.contains(input_.mouse_x, input_.mouse_y);

        add_filled_rect(rect, theme_.secondary, radius);
        if (fill.w > 0.0f && fill.h > 0.0f) {
            const float fill_radius = std::max(
                0.0f, std::min(std::min(radius - 1.0f, fill.h * 0.5f), fill.w * 0.5f));
            add_filled_rect(fill, mix(theme_.primary, theme_.secondary, 0.75f), fill_radius);
        }
        add_outline_rect(rect, hovered ? theme_.primary : theme_.outline, radius);
        const float thumb_radius = std::max(0.0f, std::min(theme_.radius - 1.0f, std::min(thumb.w, thumb.h) * 0.5f));
        Color thumb_color = theme_.primary;
        if (active_slider_id_ == id) {
            thumb_color = mix(theme_.primary, theme_.panel, 0.18f);
        } else if (thumb_hovered) {
            thumb_color = mix(theme_.primary, theme_.panel, 0.10f);
        }
        add_filled_rect(thumb, thumb_color, thumb_radius);

        add_text(label,
                 Rect{rect.x + value_padding, rect.y,
                      rect.w - value_box.w - value_padding * 2.0f, rect.h},
                 theme_.text, label_font, TextAlign::Left);

        const bool editing = is_text_input_active(id);
        add_filled_rect(value_box, editing ? theme_.input_bg : mix(theme_.input_bg, theme_.secondary, 0.25f),
                        theme_.radius - 2.0f);
        add_outline_rect(value_box, editing ? theme_.focus_ring : theme_.input_border, theme_.radius - 2.0f,
                         editing ? 1.5f : 1.0f);

        if (editing) {
            draw_text_input_content(value_box, value_font, true, value_padding, theme_.text,
                                    mix(theme_.primary, theme_.input_bg, 0.55f));
        } else {
            std::string value_text = format_float(value, value_decimals);
            if (value_text.empty()) {
                value_text = "0";
            }
            add_text(value_text,
                     Rect{value_box.x + value_padding, value_box.y,
                          value_box.w - value_padding * 2.0f, value_box.h},
                     theme_.muted_text, value_font, TextAlign::Right);
        }

        return changed;
    }

    bool input_float(std::string_view label, float& value, float min_value, float max_value, int decimals = 2,
                     float height = 34.0f) {
        if (max_value < min_value) {
            std::swap(max_value, min_value);
        }

        const Rect rect = next_rect(height);
        const float label_font = std::clamp(rect.h * 0.40f, 13.0f, 24.0f);
        const float value_font = std::max(12.0f, label_font - 0.5f);
        const float input_padding = std::clamp(rect.h * 0.18f, 6.0f, 12.0f);
        const Rect label_rect{
            rect.x,
            rect.y,
            rect.w * 0.46f,
            rect.h,
        };
        const Rect input_rect{
            rect.x + rect.w * 0.50f,
            rect.y + input_padding * 0.5f,
            rect.w * 0.50f,
            rect.h - input_padding,
        };
        const std::uint64_t id = id_for(label) ^ 0x95b6a4cb4123be3full;

        add_text(label, label_rect, theme_.text, label_font, TextAlign::Left);

        const bool hovered = input_rect.contains(input_.mouse_x, input_.mouse_y);
        if (hovered && input_.mouse_pressed) {
            start_text_input(id, format_float(value, std::max(0, decimals)), true);
        }

        bool changed = false;
        if (is_text_input_active(id)) {
            update_mouse_selection(input_rect, value_font, true, input_padding, hovered);
            consume_numeric_typing(min_value < 0.0f, true);
            clamp_live_numeric_buffer(min_value, max_value, std::max(0, decimals));
            if (input_.key_escape) {
                stop_text_input();
            } else if (input_.key_enter || (input_.mouse_pressed && !hovered)) {
                changed = commit_text_input(value, min_value, max_value) || changed;
            }
        }

        const bool editing = is_text_input_active(id);
        add_filled_rect(input_rect, theme_.input_bg, theme_.radius - 2.0f);
        add_outline_rect(input_rect, editing ? theme_.focus_ring : theme_.input_border, theme_.radius - 2.0f,
                         editing ? 1.5f : 1.0f);

        if (editing) {
            draw_text_input_content(input_rect, value_font, true, input_padding, theme_.text,
                                    mix(theme_.primary, theme_.input_bg, 0.55f));
        } else {
            std::string value_text = format_float(value, std::max(0, decimals));
            if (value_text.empty()) {
                value_text = "0";
            }
            add_text(value_text,
                     Rect{input_rect.x + input_padding, input_rect.y,
                          input_rect.w - input_padding * 2.0f, input_rect.h},
                     theme_.text, value_font, TextAlign::Right);
        }

        return changed;
    }

    void input_readonly(std::string_view label, std::string_view value, float height = 34.0f,
                        bool align_right = false, float value_font_scale = 1.0f, bool muted = true) {
        const Rect rect = next_rect(height);
        const float label_font = std::clamp(rect.h * 0.40f, 13.0f, 24.0f);
        const float base_value_font = std::clamp(rect.h * 0.44f, 12.0f, 56.0f);
        const float scale = std::clamp(value_font_scale, 0.5f, 2.2f);
        const float value_font = std::clamp(base_value_font * scale, 12.0f, 72.0f);
        const float input_padding = std::clamp(rect.h * 0.18f, 6.0f, 12.0f);
        const bool has_label = !label.empty();
        const Rect label_rect{
            rect.x,
            rect.y,
            has_label ? rect.w * 0.34f : 0.0f,
            rect.h,
        };
        const Rect input_rect = has_label
                                    ? Rect{
                                          rect.x + rect.w * 0.36f,
                                          rect.y + input_padding * 0.5f,
                                          rect.w * 0.64f,
                                          rect.h - input_padding,
                                      }
                                    : Rect{
                                          rect.x,
                                          rect.y + input_padding * 0.5f,
                                          rect.w,
                                          rect.h - input_padding,
                                      };

        if (has_label) {
            add_text(label, label_rect, theme_.text, label_font, TextAlign::Left);
        }
        add_filled_rect(input_rect, mix(theme_.input_bg, theme_.secondary, 0.08f), theme_.radius - 2.0f);
        add_outline_rect(input_rect, theme_.input_border, theme_.radius - 2.0f, 1.0f);
        add_text(value,
                 Rect{input_rect.x + input_padding, input_rect.y,
                      input_rect.w - input_padding * 2.0f, input_rect.h},
                 muted ? theme_.muted_text : theme_.text, value_font, align_right ? TextAlign::Right : TextAlign::Left);
    }

    struct ScrollAreaOptions {
        float padding{6.0f};
        float scrollbar_width{7.0f};
        float min_thumb_height{18.0f};
        float wheel_step{30.0f};
        float drag_sensitivity{1.0f};
        float inertia_friction{14.0f};
        float bounce_strength{28.0f};
        float bounce_damping{18.0f};
        float overscroll_limit{20.0f};
        bool enable_drag{true};
        bool show_scrollbar{true};
        bool draw_background{true};
    };

    bool begin_scroll_area(std::string_view label, float height,
                           const ScrollAreaOptions& options = ScrollAreaOptions{}) {
        const Rect outer = next_rect(std::max(40.0f, height));
        const float pad = std::clamp(options.padding, 2.0f, 24.0f);
        const bool show_scrollbar = options.show_scrollbar;
        const float scrollbar_w = show_scrollbar ? std::clamp(options.scrollbar_width, 4.0f, 20.0f) : 0.0f;
        const float lane_gap = show_scrollbar ? std::max(3.0f, pad * 0.45f) : 0.0f;
        const Rect viewport{
            outer.x + pad,
            outer.y + pad,
            std::max(10.0f, outer.w - pad * 2.0f - scrollbar_w - lane_gap),
            std::max(10.0f, outer.h - pad * 2.0f),
        };
        const Rect track{
            viewport.x + viewport.w + lane_gap,
            viewport.y,
            scrollbar_w,
            viewport.h,
        };

        const std::uint64_t id = id_for(label) ^ 0x3cd71be2458fe12bull;
        ScrollAreaState& state = scroll_area_state_[id];
        const float content_h = std::max(viewport.h, state.content_height);
        const float max_scroll = std::max(0.0f, content_h - viewport.h);
        const bool hovered_outer = outer.contains(input_.mouse_x, input_.mouse_y);
        const bool hovered_view = viewport.contains(input_.mouse_x, input_.mouse_y);

        const float before_scroll = state.scroll;
        const float before_velocity = state.velocity;
        bool needs_animation = false;

        if (hovered_view && std::fabs(input_.mouse_wheel_y) > 1e-6f) {
            const float wheel_delta = input_.mouse_wheel_y * std::max(12.0f, options.wheel_step);
            state.scroll -= wheel_delta;
            const float dt = std::max(1e-4f, ui_dt_);
            const float impulse_velocity = (-wheel_delta / dt) * 0.10f;
            state.velocity = state.velocity * 0.30f + impulse_velocity;
            needs_animation = true;
        }

        const float thumb_h = max_scroll > 0.0f ? std::max(std::max(12.0f, options.min_thumb_height),
                                                           viewport.h * (viewport.h / std::max(viewport.h + 1.0f, content_h)))
                                                : viewport.h;
        const float thumb_travel = std::max(0.0f, track.h - thumb_h);
        const float clamped_scroll = std::clamp(state.scroll, 0.0f, max_scroll);
        const float thumb_y = track.y + (max_scroll > 0.0f ? (clamped_scroll / max_scroll) * thumb_travel : 0.0f);
        const Rect thumb{
            track.x,
            thumb_y,
            track.w,
            thumb_h,
        };
        const bool hovered_thumb =
            show_scrollbar && max_scroll > 0.0f && thumb.contains(input_.mouse_x, input_.mouse_y);

        if (input_.mouse_pressed) {
            if (hovered_thumb) {
                active_scrollbar_drag_id_ = id;
                active_scrollbar_drag_offset_ = input_.mouse_y - thumb.y;
                state.velocity = 0.0f;
            } else if (options.enable_drag && hovered_view) {
                active_scroll_drag_id_ = id;
                active_scroll_drag_last_y_ = input_.mouse_y;
                state.velocity = 0.0f;
            }
        }

        if (active_scrollbar_drag_id_ == id) {
            if (input_.mouse_down && max_scroll > 0.0f) {
                const float new_thumb_y =
                    std::clamp(input_.mouse_y - active_scrollbar_drag_offset_, track.y, track.y + thumb_travel);
                const float t = thumb_travel > 1e-5f ? (new_thumb_y - track.y) / thumb_travel : 0.0f;
                state.scroll = t * max_scroll;
                state.velocity = 0.0f;
                needs_animation = true;
            } else if (!input_.mouse_down) {
                active_scrollbar_drag_id_ = 0u;
            }
        }

        if (active_scroll_drag_id_ == id) {
            if (input_.mouse_down) {
                const float dy = input_.mouse_y - active_scroll_drag_last_y_;
                active_scroll_drag_last_y_ = input_.mouse_y;
                const float delta = -dy * std::max(0.1f, options.drag_sensitivity);
                state.scroll += delta;
                const float dt = std::max(1e-4f, ui_dt_);
                const float inst_velocity = delta / dt;
                state.velocity = state.velocity * 0.60f + inst_velocity * 0.40f;
                needs_animation = true;
            } else {
                active_scroll_drag_id_ = 0u;
            }
        }

        const bool dragging =
            active_scroll_drag_id_ == id || active_scrollbar_drag_id_ == id;
        const float overscroll =
            max_scroll > 1e-4f ? std::max(0.0f, options.overscroll_limit) : 0.0f;
        if (!dragging) {
            const float friction = std::clamp(options.inertia_friction, 0.0f, 80.0f);
            const float spring = std::max(0.0f, options.bounce_strength);
            const float damping = std::max(0.0f, options.bounce_damping);
            const bool out_of_bounds = state.scroll < 0.0f || state.scroll > max_scroll;
            if (out_of_bounds) {
                const float target = state.scroll < 0.0f ? 0.0f : max_scroll;
                const float displacement = target - state.scroll;
                state.velocity += displacement * spring * ui_dt_;
                state.velocity *= std::exp(-damping * ui_dt_);
            } else {
                state.velocity *= std::exp(-friction * ui_dt_);
            }
            state.scroll += state.velocity * ui_dt_;
            if (std::fabs(state.velocity) < 6.0f && !out_of_bounds) {
                state.velocity = 0.0f;
            }
            if (out_of_bounds) {
                const float target = state.scroll < 0.0f ? 0.0f : max_scroll;
                if (std::fabs(target - state.scroll) < 0.25f && std::fabs(state.velocity) < 8.0f) {
                    state.scroll = target;
                    state.velocity = 0.0f;
                }
            }
        }
        if (max_scroll <= 1e-4f) {
            state.scroll = 0.0f;
            state.velocity = 0.0f;
        }
        state.scroll = std::clamp(state.scroll, -overscroll, max_scroll + overscroll);

        const bool scroll_changed = std::fabs(state.scroll - before_scroll) > 0.05f;
        const bool velocity_changed = std::fabs(state.velocity - before_velocity) > 5.0f;
        const bool still_animating =
            std::fabs(state.velocity) > 6.0f ||
            state.scroll < -0.25f || state.scroll > max_scroll + 0.25f;
        if (scroll_changed || velocity_changed || needs_animation || still_animating) {
            repaint_requested_ = true;
        }

        if (options.draw_background) {
            add_filled_rect(outer, mix(theme_.input_bg, theme_.secondary, 0.10f), theme_.radius - 2.0f);
            add_outline_rect(outer, hovered_outer ? theme_.focus_ring : theme_.input_border, theme_.radius - 2.0f,
                             hovered_outer ? 1.3f : 1.0f);
        }

        if (show_scrollbar) {
            const float draw_scroll = std::clamp(state.scroll, 0.0f, max_scroll);
            const float draw_thumb_y =
                track.y + (max_scroll > 0.0f ? (draw_scroll / max_scroll) * thumb_travel : 0.0f);
            const Rect draw_thumb{
                track.x,
                draw_thumb_y,
                track.w,
                thumb_h,
            };
            const bool has_scroll = max_scroll > 1e-4f;
            const Color track_color = mix(theme_.secondary, theme_.panel, 0.45f);
            const Color thumb_color = has_scroll
                                          ? ((hovered_thumb || active_scrollbar_drag_id_ == id)
                                                 ? theme_.primary
                                                 : mix(theme_.primary, theme_.panel, 0.40f))
                                          : mix(theme_.outline, theme_.panel, 0.55f);
            add_filled_rect(track, track_color, std::max(0.0f, std::min(track.w, track.h) * 0.5f));
            add_filled_rect(draw_thumb, thumb_color, std::max(0.0f, std::min(draw_thumb.w, draw_thumb.h) * 0.5f));
        }

        const bool had_outer_row = row_.active;
        const RowState outer_row = row_;
        scope_stack_.push_back(ScopeState{
            ScopeKind::ScrollArea,
            content_x_,
            content_width_,
            0.0f,
            0u,
            0u,
            outer.y,
            outer.h,
            0.0f,
            had_outer_row,
            outer_row,
            false,
            -1,
            id,
            viewport,
            viewport.y - state.scroll,
            true,
        });

        push_clip_rect(viewport);
        content_x_ = viewport.x;
        content_width_ = viewport.w;
        cursor_y_ = viewport.y - state.scroll;
        row_ = RowState{};
        return true;
    }

    void end_scroll_area() {
        flush_row();
        if (scope_stack_.empty()) {
            return;
        }
        if (scope_stack_.back().kind != ScopeKind::ScrollArea) {
            return;
        }
        restore_scope();
    }

    bool text_area(std::string_view label, std::string& text, float height = 170.0f) {
        const std::string original_text = text;
        const Rect rect = next_rect(std::max(96.0f, height));
        const float label_font = std::clamp(rect.h * 0.12f, 12.0f, 18.0f);
        const float text_font = std::clamp(rect.h * 0.13f, 13.0f, 22.0f);
        const float outer_pad = std::clamp(rect.h * 0.04f, 6.0f, 12.0f);
        const float line_h = text_font + 5.0f;

        const Rect label_rect{
            rect.x + outer_pad,
            rect.y + outer_pad,
            rect.w - outer_pad * 2.0f,
            label_font,
        };
        const Rect box_rect{
            rect.x + outer_pad,
            label_rect.y + label_rect.h + 6.0f,
            rect.w - outer_pad * 2.0f,
            rect.h - (label_rect.h + outer_pad + 12.0f),
        };
        const float radius = std::max(2.0f, theme_.radius - 2.0f);
        const float scrollbar_w = 8.0f;
        const float text_pad = std::clamp(rect.h * 0.03f, 6.0f, 10.0f);
        const float content_w = std::max(24.0f, box_rect.w - text_pad * 2.0f - 2.0f);
        const float char_w = std::max(3.6f, text_font * 0.42f);
        const int max_chars_per_line =
            std::max(1, static_cast<int>(std::floor(content_w / char_w)));
        const Rect content_clip{
            box_rect.x + text_pad,
            box_rect.y + text_pad,
            content_w,
            std::max(24.0f, box_rect.h - text_pad * 2.0f),
        };

        const std::uint64_t id = id_for(label) ^ 0x5f8d37aa44c2e391ull;
        TextAreaState& state = text_area_state_[id];
        const bool hovered_box = box_rect.contains(input_.mouse_x, input_.mouse_y);
        bool editing = is_text_input_active(id);

        if (input_.mouse_pressed) {
            if (hovered_box) {
                if (!editing) {
                    start_text_input(id, text, false);
                }
                editing = is_text_input_active(id);
            } else if (editing) {
                text = input_buffer_;
                stop_text_input();
                editing = false;
            }
        }

        if (editing) {
            if (input_.key_escape) {
                stop_text_input();
                editing = false;
            } else {
                consume_plain_typing(true);
                text = input_buffer_;
            }
        }

        const std::string_view text_view = editing ? std::string_view(input_buffer_) : std::string_view(text);
        const auto count_chars = [&](std::size_t start, std::size_t end) -> int {
            if (end <= start) {
                return 0;
            }
            start = std::min(start, text_view.size());
            end = std::min(end, text_view.size());
            int count = 0;
            std::size_t idx = start;
            while (idx < end) {
                idx = utf8_next_index(text_view, idx);
                count += 1;
            }
            return count;
        };

        struct WrappedLine {
            std::size_t start{0u};
            std::size_t len{0u};
            int chars{0};
        };
        std::vector<WrappedLine> lines;
        lines.reserve(64);
        std::size_t line_start = 0u;
        std::size_t index = 0u;
        int line_chars = 0;
        while (index < text_view.size()) {
            const unsigned char ch = static_cast<unsigned char>(text_view[index]);
            if (ch == '\r') {
                index += 1u;
                continue;
            }
            if (ch == '\n') {
                lines.push_back(WrappedLine{line_start, index - line_start, line_chars});
                index += 1u;
                line_start = index;
                line_chars = 0;
                continue;
            }
            if (line_chars >= max_chars_per_line) {
                lines.push_back(WrappedLine{line_start, index - line_start, line_chars});
                line_start = index;
                line_chars = 0;
            }
            index = utf8_next_index(text_view, index);
            line_chars += 1;
        }
        lines.push_back(WrappedLine{line_start, text_view.size() - line_start, line_chars});
        if (lines.empty()) {
            lines.push_back(WrappedLine{0u, 0u, 0});
        }

        const float viewport_h = content_clip.h;
        const float total_h = static_cast<float>(lines.size()) * line_h;
        const float max_scroll = std::max(0.0f, total_h - viewport_h);
        const Rect track{
            box_rect.x + box_rect.w - text_pad - scrollbar_w,
            box_rect.y + text_pad,
            scrollbar_w,
            viewport_h,
        };
        const float thumb_h = max_scroll > 0.0f
                                  ? std::max(18.0f, viewport_h * (viewport_h / std::max(viewport_h + 1.0f, total_h)))
                                  : viewport_h;
        const float thumb_travel = std::max(0.0f, track.h - thumb_h);
        const float thumb_y =
            track.y + (max_scroll > 0.0f ? (state.scroll / max_scroll) * thumb_travel : 0.0f);
        const Rect thumb{track.x, thumb_y, track.w, thumb_h};
        const bool hovered_thumb = thumb.contains(input_.mouse_x, input_.mouse_y);

        if (hovered_box && std::fabs(input_.mouse_wheel_y) > 1e-6f) {
            state.scroll = std::clamp(state.scroll - input_.mouse_wheel_y * line_h * 2.0f, 0.0f, max_scroll);
        } else {
            state.scroll = std::clamp(state.scroll, 0.0f, max_scroll);
        }

        auto caret_from_point = [&](float mouse_x, float mouse_y) -> std::size_t {
            if (lines.empty()) {
                return 0u;
            }
            const float local_y = std::clamp(mouse_y - content_clip.y + state.scroll, 0.0f, total_h);
            std::size_t line_index =
                static_cast<std::size_t>(std::floor(local_y / line_h));
            line_index = std::min(line_index, lines.size() - 1u);
            const WrappedLine& line = lines[line_index];
            const float rel_x = std::max(0.0f, mouse_x - content_clip.x);
            const int char_index =
                std::clamp(static_cast<int>(std::floor(rel_x / char_w + 0.5f)), 0, line.chars);
            std::size_t pos = line.start;
            for (int i = 0; i < char_index && pos < line.start + line.len; ++i) {
                pos = utf8_next_index(text_view, pos);
            }
            return std::min(pos, line.start + line.len);
        };

        auto resolve_caret_visual = [&](std::size_t caret_pos) -> std::pair<std::size_t, int> {
            if (lines.empty()) {
                return {0u, 0};
            }
            const std::size_t clamped_caret = std::min(caret_pos, text_view.size());
            for (std::size_t i = 0; i < lines.size(); ++i) {
                const std::size_t line_start_idx = lines[i].start;
                const std::size_t line_end_idx = line_start_idx + lines[i].len;
                const bool has_next = i + 1u < lines.size();
                const std::size_t next_start = has_next ? lines[i + 1u].start : line_end_idx;

                if (clamped_caret < line_start_idx) {
                    return {i, 0};
                }
                if (clamped_caret < line_end_idx) {
                    return {i, count_chars(line_start_idx, clamped_caret)};
                }
                if (clamped_caret == line_end_idx) {
                    // Wrapped lines share a boundary; prefer next line start to avoid end/start jitter.
                    if (has_next && next_start == line_end_idx) {
                        return {i + 1u, 0};
                    }
                    return {i, count_chars(line_start_idx, clamped_caret)};
                }
                if (has_next && clamped_caret < next_start) {
                    return {i, lines[i].chars};
                }
            }
            return {lines.size() - 1u, lines.back().chars};
        };

        if (editing) {
            if (input_.mouse_pressed && content_clip.contains(input_.mouse_x, input_.mouse_y) &&
                !hovered_thumb) {
                const std::size_t caret = caret_from_point(input_.mouse_x, input_.mouse_y);
                caret_pos_ = caret;
                selection_start_ = caret;
                selection_end_ = caret;
                drag_anchor_ = caret;
                drag_selecting_ = true;
            }
            if (drag_selecting_ && input_.mouse_down) {
                const float clamped_x = std::clamp(input_.mouse_x, content_clip.x, content_clip.x + content_clip.w);
                const float clamped_y = std::clamp(input_.mouse_y, content_clip.y, content_clip.y + content_clip.h);
                const std::size_t caret = caret_from_point(clamped_x, clamped_y);
                caret_pos_ = caret;
                selection_start_ = drag_anchor_;
                selection_end_ = caret;
            }
            if (input_.mouse_released) {
                drag_selecting_ = false;
            }

            const auto caret_visual = resolve_caret_visual(caret_pos_);
            const std::size_t caret_line = caret_visual.first;
            const float caret_top = static_cast<float>(caret_line) * line_h;
            if (caret_top < state.scroll) {
                state.scroll = caret_top;
            } else if (caret_top + line_h > state.scroll + viewport_h) {
                state.scroll = caret_top + line_h - viewport_h;
            }
            state.scroll = std::clamp(state.scroll, 0.0f, max_scroll);
        }

        add_text(label, label_rect, theme_.muted_text, label_font, TextAlign::Left);
        add_filled_rect(box_rect, mix(theme_.input_bg, theme_.secondary, 0.08f), radius);
        add_outline_rect(box_rect, (hovered_box || editing) ? theme_.focus_ring : theme_.input_border, radius,
                         (hovered_box || editing) ? 1.3f : 1.0f);

        if (input_.mouse_pressed && hovered_thumb) {
            active_textarea_scroll_id_ = id;
            active_textarea_drag_offset_ = input_.mouse_y - thumb.y;
        }
        if (active_textarea_scroll_id_ == id) {
            if (input_.mouse_down && max_scroll > 0.0f) {
                const float new_thumb_y =
                    std::clamp(input_.mouse_y - active_textarea_drag_offset_, track.y, track.y + thumb_travel);
                const float t = thumb_travel > 1e-5f ? (new_thumb_y - track.y) / thumb_travel : 0.0f;
                state.scroll = std::clamp(t * max_scroll, 0.0f, max_scroll);
            } else if (!input_.mouse_down) {
                active_textarea_scroll_id_ = 0u;
            }
        }

        add_filled_rect(track, mix(theme_.secondary, theme_.panel, 0.45f), 3.0f);
        add_filled_rect(thumb, hovered_thumb ? theme_.primary : mix(theme_.primary, theme_.panel, 0.40f), 3.0f);

        const float line_offset = std::fmod(state.scroll, line_h);
        std::size_t first_line = static_cast<std::size_t>(std::floor(state.scroll / line_h));
        first_line = std::min(first_line, lines.size() - 1u);
        const Color selection_color = mix(theme_.primary, theme_.input_bg, 0.55f);

        if (editing && has_selection()) {
            const std::size_t sel_l = selection_left();
            const std::size_t sel_r = selection_right();
            float y = content_clip.y - line_offset;
            for (std::size_t i = first_line; i < lines.size(); ++i) {
                if (y > content_clip.y + content_clip.h) {
                    break;
                }
                const WrappedLine& line = lines[i];
                const std::size_t line_start_idx = line.start;
                const std::size_t line_end_idx = line.start + line.len;
                const std::size_t hi_l = std::max(sel_l, line_start_idx);
                const std::size_t hi_r = std::min(sel_r, line_end_idx);
                if (hi_r > hi_l && y + line_h >= content_clip.y) {
                    const int before_chars = count_chars(line_start_idx, hi_l);
                    const int selected_chars = count_chars(hi_l, hi_r);
                    Rect sel_rect{
                        content_clip.x + static_cast<float>(before_chars) * char_w,
                        y + 1.0f,
                        static_cast<float>(selected_chars) * char_w,
                        std::max(2.0f, line_h - 2.0f),
                    };
                    Rect clipped{};
                    if (intersect_rects(sel_rect, content_clip, clipped)) {
                        add_filled_rect(clipped, selection_color, 2.0f);
                    }
                }
                y += line_h;
            }
        }

        float y = content_clip.y - line_offset;
        for (std::size_t i = first_line; i < lines.size(); ++i) {
            if (y > content_clip.y + content_clip.h) {
                break;
            }
            if (y + line_h >= content_clip.y) {
                const WrappedLine& line = lines[i];
                add_text(text_view.substr(line.start, line.len), Rect{content_clip.x, y, content_w, line_h},
                         theme_.text, text_font, TextAlign::Left, &content_clip);
            }
            y += line_h;
        }

        if (editing && should_show_caret()) {
            const auto caret_visual = resolve_caret_visual(caret_pos_);
            const std::size_t caret_line = caret_visual.first;
            const int caret_chars = caret_visual.second;
            const float caret_y = content_clip.y - line_offset + static_cast<float>(caret_line) * line_h;
            const float caret_x =
                std::min(content_clip.x + content_clip.w - 1.0f, content_clip.x + static_cast<float>(caret_chars) * char_w);
            Rect caret_rect{
                caret_x,
                caret_y + 1.0f,
                std::max(1.4f, text_font * 0.08f),
                std::max(2.0f, line_h - 2.0f),
            };
            Rect clipped{};
            if (intersect_rects(caret_rect, content_clip, clipped)) {
                add_filled_rect(clipped, theme_.text, 0.0f);
            }
        }

        return text != original_text;
    }

    void text_area_readonly(std::string_view label, std::string_view text, float height = 170.0f) {
        const Rect rect = next_rect(std::max(96.0f, height));
        const float label_font = std::clamp(rect.h * 0.12f, 12.0f, 18.0f);
        const float text_font = std::clamp(rect.h * 0.13f, 13.0f, 20.0f);
        const float outer_pad = std::clamp(rect.h * 0.04f, 6.0f, 12.0f);
        const float line_h = text_font + 5.0f;

        const Rect label_rect{
            rect.x + outer_pad,
            rect.y + outer_pad,
            rect.w - outer_pad * 2.0f,
            label_font,
        };
        const Rect box_rect{
            rect.x + outer_pad,
            label_rect.y + label_rect.h + 6.0f,
            rect.w - outer_pad * 2.0f,
            rect.h - (label_rect.h + outer_pad + 12.0f),
        };
        const float radius = std::max(2.0f, theme_.radius - 2.0f);
        const float scrollbar_w = 8.0f;
        const float text_pad = std::clamp(rect.h * 0.03f, 6.0f, 10.0f);
        const float content_w = std::max(24.0f, box_rect.w - text_pad * 2.0f - 2.0f);
        const Rect content_clip{
            box_rect.x + text_pad,
            box_rect.y + text_pad,
            content_w,
            std::max(24.0f, box_rect.h - text_pad * 2.0f),
        };
        const float wrap_char_w = std::max(3.6f, text_font * 0.42f);
        const int max_chars_per_line =
            std::max(1, static_cast<int>(std::floor(content_w / wrap_char_w)));

        std::vector<std::pair<std::size_t, std::size_t>> lines;
        lines.reserve(64);
        std::size_t line_start = 0u;
        std::size_t index = 0u;
        int line_chars = 0;
        while (index < text.size()) {
            const unsigned char ch = static_cast<unsigned char>(text[index]);
            if (ch == '\r') {
                index += 1u;
                continue;
            }
            if (ch == '\n') {
                lines.emplace_back(line_start, index - line_start);
                index += 1u;
                line_start = index;
                line_chars = 0;
                continue;
            }
            if (line_chars >= max_chars_per_line) {
                lines.emplace_back(line_start, index - line_start);
                line_start = index;
                line_chars = 0;
            }
            index = utf8_next_index(text, index);
            line_chars += 1;
        }
        lines.emplace_back(line_start, text.size() - line_start);
        if (lines.empty()) {
            lines.emplace_back(0u, 0u);
        }

        const float viewport_h = std::max(24.0f, box_rect.h - text_pad * 2.0f);
        const float total_h = static_cast<float>(lines.size()) * line_h;
        const float max_scroll = std::max(0.0f, total_h - viewport_h);

        const std::uint64_t id = id_for(label) ^ 0x5f8d37aa44c2e391ull;
        TextAreaState& state = text_area_state_[id];
        const bool hovered_box = box_rect.contains(input_.mouse_x, input_.mouse_y);
        if (hovered_box && std::fabs(input_.mouse_wheel_y) > 1e-6f) {
            state.scroll = std::clamp(state.scroll - input_.mouse_wheel_y * line_h * 2.0f, 0.0f, max_scroll);
        } else {
            state.scroll = std::clamp(state.scroll, 0.0f, max_scroll);
        }

        add_text(label, label_rect, theme_.muted_text, label_font, TextAlign::Left);
        add_filled_rect(box_rect, mix(theme_.input_bg, theme_.secondary, 0.08f), radius);
        add_outline_rect(box_rect, hovered_box ? theme_.focus_ring : theme_.input_border, radius,
                         hovered_box ? 1.3f : 1.0f);

        const Rect track{
            box_rect.x + box_rect.w - text_pad - scrollbar_w,
            box_rect.y + text_pad,
            scrollbar_w,
            viewport_h,
        };
        const float thumb_h = max_scroll > 0.0f
                                  ? std::max(18.0f, viewport_h * (viewport_h / std::max(viewport_h + 1.0f, total_h)))
                                  : viewport_h;
        const float thumb_travel = std::max(0.0f, track.h - thumb_h);
        const float thumb_y =
            track.y + (max_scroll > 0.0f ? (state.scroll / max_scroll) * thumb_travel : 0.0f);
        const Rect thumb{track.x, thumb_y, track.w, thumb_h};
        const bool hovered_thumb = thumb.contains(input_.mouse_x, input_.mouse_y);
        if (input_.mouse_pressed && hovered_thumb) {
            active_textarea_scroll_id_ = id;
            active_textarea_drag_offset_ = input_.mouse_y - thumb.y;
        }
        if (active_textarea_scroll_id_ == id) {
            if (input_.mouse_down && max_scroll > 0.0f) {
                const float new_thumb_y =
                    std::clamp(input_.mouse_y - active_textarea_drag_offset_, track.y, track.y + thumb_travel);
                const float t = thumb_travel > 1e-5f ? (new_thumb_y - track.y) / thumb_travel : 0.0f;
                state.scroll = std::clamp(t * max_scroll, 0.0f, max_scroll);
            } else if (!input_.mouse_down) {
                active_textarea_scroll_id_ = 0u;
            }
        }

        add_filled_rect(track, mix(theme_.secondary, theme_.panel, 0.45f), 3.0f);
        add_filled_rect(thumb, hovered_thumb ? theme_.primary : mix(theme_.primary, theme_.panel, 0.40f), 3.0f);

        const float line_offset = std::fmod(state.scroll, line_h);
        std::size_t first_line = static_cast<std::size_t>(std::floor(state.scroll / line_h));
        float y = box_rect.y + text_pad - line_offset;
        for (std::size_t i = first_line; i < lines.size(); ++i) {
            if (y > box_rect.y + box_rect.h - text_pad) {
                break;
            }
            const auto [start, len] = lines[i];
            if (y + line_h >= box_rect.y + text_pad) {
                add_text(text.substr(start, len),
                         Rect{content_clip.x, y, content_w, line_h},
                         theme_.text, text_font, TextAlign::Left, &content_clip);
            }
            y += line_h;
        }
    }

    void progress(std::string_view label, float ratio, float height = 10.0f) {
        ratio = std::clamp(ratio, 0.0f, 1.0f);
        const float label_h = std::clamp(height * 1.6f, 14.0f, 26.0f);
        const float text_gap = std::max(8.0f, label_h + 4.0f);
        const Rect rect = next_rect(height + text_gap);

        add_text(label, Rect{rect.x, rect.y, rect.w * 0.7f, label_h}, theme_.muted_text, label_h,
                 TextAlign::Left);

        char pct_text[32];
        std::snprintf(pct_text, sizeof(pct_text), "%.0f%%", ratio * 100.0f);
        add_text(pct_text, Rect{rect.x + rect.w * 0.7f, rect.y, rect.w * 0.3f, label_h}, theme_.text, label_h,
                 TextAlign::Right);

        const Rect track{rect.x, rect.y + text_gap, rect.w, std::max(4.0f, height)};
        const Rect fill{
            track.x + 1.0f,
            track.y + 1.0f,
            std::max(0.0f, track.w * ratio - 2.0f),
            std::max(0.0f, track.h - 2.0f),
        };
        const float track_radius = track.h * 0.5f;
        add_filled_rect(track, theme_.track, track_radius);
        if (fill.w > 0.0f && fill.h > 0.0f) {
            const float fill_radius = std::max(
                0.0f, std::min(std::min(track_radius - 1.0f, fill.h * 0.5f), fill.w * 0.5f));
            add_filled_rect(fill, theme_.track_fill, fill_radius);
        }
    }

    bool begin_dropdown(std::string_view label, bool& open, float body_height = 104.0f,
                        float padding = 10.0f) {
        const float header_height = std::max(34.0f, padding * 3.0f);
        const Rect header = next_rect(header_height);
        const bool hovered = header.contains(input_.mouse_x, input_.mouse_y);
        if (hovered && input_.mouse_pressed) {
            open = !open;
        }

        Color fill = theme_.panel;
        if (hovered) {
            fill = mix(theme_.panel, theme_.secondary_hover, 0.35f);
        }

        add_filled_rect(header, fill, theme_.radius);
        add_outline_rect(header, theme_.outline, theme_.radius);
        const float header_font = std::clamp(header.h * 0.38f, 13.0f, 24.0f);
        const float header_pad = std::clamp(header.h * 0.28f, 10.0f, 22.0f);
        add_text(label,
                 Rect{header.x + header_pad, header.y,
                      header.w - header_pad * 2.0f - header_font, header.h},
                 theme_.text, header_font, TextAlign::Left);
        add_text(open ? "V" : ">",
                 Rect{header.x + header_pad, header.y,
                      header.w - header_pad * 2.0f, header.h},
                 theme_.muted_text, header_font, TextAlign::Right);

        if (!open) {
            return false;
        }

        flush_row();

        body_height = std::max(36.0f, body_height);
        padding = std::clamp(padding, 4.0f, 24.0f);

        const Rect body{content_x_, cursor_y_, content_width_, body_height};
        const std::size_t fill_cmd_index = commands_.size();
        add_filled_rect(body, mix(theme_.panel, theme_.secondary, 0.35f), theme_.radius);
        const std::size_t outline_cmd_index = commands_.size();
        add_outline_rect(body, theme_.outline, theme_.radius);
        const bool had_outer_row = row_.active;
        const RowState outer_row = row_;

        scope_stack_.push_back(ScopeState{
            ScopeKind::DropdownBody,
            content_x_,
            content_width_,
            0.0f,
            fill_cmd_index,
            outline_cmd_index,
            body.y,
            body_height,
            padding,
            had_outer_row,
            outer_row,
            false,
            -1,
        });
        content_x_ = body.x + padding;
        content_width_ = std::max(10.0f, body.w - 2.0f * padding);
        cursor_y_ = body.y + padding;
        row_ = RowState{};

        return true;
    }

    void end_dropdown() {
        flush_row();
        if (scope_stack_.empty()) {
            return;
        }
        restore_scope();
    }

    void begin_row(int columns, float gap = 8.0f) {
        flush_row();
        row_.active = true;
        row_.columns = std::max(1, columns);
        row_.index = 0;
        row_.gap = std::max(0.0f, gap);
        row_.y = cursor_y_;
        row_.max_height = 0.0f;
    }

    void begin_columns(int columns, float gap = 8.0f) {
        begin_row(columns, gap);
    }

    void begin_waterfall(int columns, float gap = 8.0f) {
        flush_row();
        waterfall_.active = true;
        waterfall_.columns = std::max(1, columns);
        waterfall_.gap = std::max(0.0f, gap);
        waterfall_.start_y = cursor_y_;
        const float total_gap = waterfall_.gap * static_cast<float>(waterfall_.columns - 1);
        waterfall_.item_width =
            std::max(10.0f, (content_width_ - total_gap) / static_cast<float>(waterfall_.columns));
        waterfall_.column_y.assign(static_cast<std::size_t>(waterfall_.columns), cursor_y_);
    }

    void end_row() {
        flush_row();
    }

    void end_columns() {
        end_row();
    }

    void end_waterfall() {
        if (!waterfall_.active) {
            return;
        }
        float max_y = waterfall_.start_y;
        for (float y : waterfall_.column_y) {
            max_y = std::max(max_y, y);
        }
        cursor_y_ = max_y;
        waterfall_ = WaterfallState{};
    }

    bool consume_clipboard_write(std::string& out_text) {
        if (!clipboard_write_pending_) {
            return false;
        }
        out_text = clipboard_write_text_;
        clipboard_write_text_.clear();
        clipboard_write_pending_ = false;
        return true;
    }

    bool consume_repaint_request() {
        const bool requested = repaint_requested_;
        repaint_requested_ = false;
        return requested;
    }

private:
    void refresh_theme() {
        theme_ = make_theme(theme_mode_, primary_color_);
        theme_.radius = corner_radius_;
    }

    enum class ScopeKind {
        Card,
        DropdownBody,
        ScrollArea,
    };

    struct RowState {
        bool active{false};
        int columns{1};
        int index{0};
        float gap{8.0f};
        float y{0.0f};
        float max_height{0.0f};
    };

    struct WaterfallState {
        bool active{false};
        int columns{1};
        float gap{8.0f};
        float start_y{0.0f};
        float item_width{0.0f};
        std::vector<float> column_y{};
    };

    struct TextAreaState {
        float scroll{0.0f};
    };

    struct ScrollAreaState {
        float scroll{0.0f};
        float velocity{0.0f};
        float content_height{0.0f};
    };

    struct ScopeState {
        ScopeKind kind{ScopeKind::Card};
        float content_x{0.0f};
        float content_width{0.0f};
        float cursor_y_after{0.0f};
        std::size_t fill_cmd_index{0};
        std::size_t outline_cmd_index{0};
        float top_y{0.0f};
        float min_height{0.0f};
        float padding{0.0f};
        bool had_outer_row{false};
        RowState outer_row{};
        bool in_waterfall{false};
        int column_index{-1};
        std::uint64_t scroll_state_id{0u};
        Rect scroll_viewport{};
        float scroll_content_origin_y{0.0f};
        bool pushed_clip{false};
    };

    static std::uint64_t hash_sv(std::string_view text) {
        std::uint64_t value = 1469598103934665603ull;
        for (char ch : text) {
            value ^= static_cast<std::uint8_t>(ch);
            value *= 1099511628211ull;
        }
        return value;
    }

    static std::uint64_t hash_mix(std::uint64_t hash, std::uint64_t value) {
        hash ^= value;
        hash *= 1099511628211ull;
        return hash;
    }

    static std::uint32_t float_bits(float value) {
        std::uint32_t out = 0u;
        std::memcpy(&out, &value, sizeof(out));
        return out;
    }

    static std::uint64_t hash_rect(const Rect& rect) {
        std::uint64_t hash = 1469598103934665603ull;
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(rect.x)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(rect.y)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(rect.w)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(rect.h)));
        return hash;
    }

    static std::uint64_t hash_color(const Color& color) {
        std::uint64_t hash = 1469598103934665603ull;
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(color.r)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(color.g)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(color.b)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(color.a)));
        return hash;
    }

    static std::uint64_t hash_command_base(const DrawCommand& cmd) {
        std::uint64_t hash = 1469598103934665603ull;
        hash = hash_mix(hash, static_cast<std::uint64_t>(cmd.type));
        hash = hash_mix(hash, hash_rect(cmd.rect));
        hash = hash_mix(hash, hash_color(cmd.color));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(cmd.font_size)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(cmd.align));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(cmd.radius)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(float_bits(cmd.thickness)));
        hash = hash_mix(hash, static_cast<std::uint64_t>(cmd.has_clip ? 1u : 0u));
        if (cmd.has_clip) {
            hash = hash_mix(hash, hash_rect(cmd.clip_rect));
        }
        return hash;
    }

    std::uint64_t id_for(std::string_view label) const {
        const std::uint64_t key = hash_sv(label);
        return panel_id_seed_ ^ (key + 0x9e3779b97f4a7c15ull + (panel_id_seed_ << 6u) +
                                 (panel_id_seed_ >> 2u));
    }

    static bool parse_float_text(const std::string& text, float& out_value) {
        if (text.empty() || text == "-" || text == "." || text == "-.") {
            return false;
        }
        char* end = nullptr;
        const float parsed = std::strtof(text.c_str(), &end);
        if (end == text.c_str() || (end != nullptr && *end != '\0') || !std::isfinite(parsed)) {
            return false;
        }
        out_value = parsed;
        return true;
    }

    static int resolve_decimals(float min_value, float max_value, int requested) {
        if (requested >= 0) {
            return std::clamp(requested, 0, 4);
        }
        const float span = std::fabs(max_value - min_value);
        if (span <= 1.0f) {
            return 2;
        }
        if (span <= 10.0f) {
            return 1;
        }
        return 0;
    }

    static std::string format_float(float value, int decimals) {
        decimals = std::clamp(decimals, 0, 4);
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%.*f", decimals, value);
        return std::string(buffer);
    }

    static bool intersect_rects(const Rect& lhs, const Rect& rhs, Rect& out) {
        const float x1 = std::max(lhs.x, rhs.x);
        const float y1 = std::max(lhs.y, rhs.y);
        const float x2 = std::min(lhs.x + lhs.w, rhs.x + rhs.w);
        const float y2 = std::min(lhs.y + lhs.h, rhs.y + rhs.h);
        if (x2 <= x1 || y2 <= y1) {
            out = Rect{};
            return false;
        }
        out = Rect{x1, y1, x2 - x1, y2 - y1};
        return true;
    }

    bool resolve_effective_clip(const Rect* requested_clip, Rect& out_clip, bool& has_clip) const {
        has_clip = false;
        if (requested_clip != nullptr) {
            out_clip = *requested_clip;
            has_clip = true;
        }
        if (!clip_stack_.empty()) {
            if (has_clip) {
                Rect merged{};
                if (!intersect_rects(out_clip, clip_stack_.back(), merged)) {
                    return false;
                }
                out_clip = merged;
            } else {
                out_clip = clip_stack_.back();
                has_clip = true;
            }
        }
        return true;
    }

    bool apply_clip_to_command(DrawCommand& cmd, const Rect* requested_clip = nullptr) const {
        Rect effective_clip{};
        bool has_clip = false;
        if (!resolve_effective_clip(requested_clip, effective_clip, has_clip)) {
            return false;
        }
        if (!has_clip) {
            return true;
        }
        Rect visible{};
        if (!intersect_rects(cmd.rect, effective_clip, visible)) {
            return false;
        }
        cmd.has_clip = true;
        cmd.clip_rect = effective_clip;
        return true;
    }

    void push_clip_rect(const Rect& rect) {
        Rect clipped = rect;
        if (!clip_stack_.empty()) {
            Rect merged{};
            if (intersect_rects(clipped, clip_stack_.back(), merged)) {
                clipped = merged;
            } else {
                clipped = Rect{};
            }
        }
        clip_stack_.push_back(clipped);
    }

    void pop_clip_rect() {
        if (!clip_stack_.empty()) {
            clip_stack_.pop_back();
        }
    }

    static std::size_t utf8_next_index(std::string_view text, std::size_t index) {
        if (index >= text.size()) {
            return text.size();
        }
        const unsigned char lead = static_cast<unsigned char>(text[index]);
        if (lead < 0x80u) {
            return index + 1u;
        }
        if ((lead >> 5u) == 0x6u && index + 1u < text.size()) {
            const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
            if ((b1 & 0xC0u) == 0x80u) {
                return index + 2u;
            }
        }
        if ((lead >> 4u) == 0xEu && index + 2u < text.size()) {
            const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
            const unsigned char b2 = static_cast<unsigned char>(text[index + 2u]);
            if ((b1 & 0xC0u) == 0x80u && (b2 & 0xC0u) == 0x80u) {
                return index + 3u;
            }
        }
        if ((lead >> 3u) == 0x1Eu && index + 3u < text.size()) {
            const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
            const unsigned char b2 = static_cast<unsigned char>(text[index + 2u]);
            const unsigned char b3 = static_cast<unsigned char>(text[index + 3u]);
            if ((b1 & 0xC0u) == 0x80u && (b2 & 0xC0u) == 0x80u && (b3 & 0xC0u) == 0x80u) {
                return index + 4u;
            }
        }
        return index + 1u;
    }

    static std::size_t utf8_prev_index(std::string_view text, std::size_t index) {
        if (index == 0u || text.empty()) {
            return 0u;
        }
        std::size_t i = std::min(index, text.size());
        i -= 1u;
        while (i > 0u) {
            const unsigned char ch = static_cast<unsigned char>(text[i]);
            if ((ch & 0xC0u) != 0x80u) {
                break;
            }
            i -= 1u;
        }
        return i;
    }

    static float approx_char_width(float font_size) {
        return std::max(5.0f, font_size * 0.58f);
    }

    float text_origin_x(const Rect& input_rect, float font_size, bool align_right, float padding,
                        std::size_t text_len) const {
        const float char_w = approx_char_width(font_size);
        const float text_w = static_cast<float>(text_len) * char_w;
        if (!align_right) {
            return input_rect.x + padding;
        }
        const float desired = input_rect.x + input_rect.w - padding - text_w;
        return std::max(input_rect.x + padding, desired);
    }

    std::size_t caret_from_mouse_x(const Rect& input_rect, float font_size, bool align_right, float padding,
                                   std::size_t text_len, float mouse_x) const {
        const float char_w = approx_char_width(font_size);
        const float origin_x = text_origin_x(input_rect, font_size, align_right, padding, text_len);
        const float rel_x = mouse_x - origin_x;
        if (rel_x <= 0.0f) {
            return 0u;
        }
        const std::size_t raw = static_cast<std::size_t>(std::floor(rel_x / char_w + 0.5f));
        return std::min(raw, text_len);
    }

    bool has_selection() const {
        return selection_start_ != selection_end_;
    }

    std::size_t selection_left() const {
        return std::min(selection_start_, selection_end_);
    }

    std::size_t selection_right() const {
        return std::max(selection_start_, selection_end_);
    }

    void clear_selection() {
        selection_start_ = caret_pos_;
        selection_end_ = caret_pos_;
    }

    void request_clipboard_write(std::string text) {
        clipboard_write_text_ = std::move(text);
        clipboard_write_pending_ = true;
    }

    void ensure_edit_state_bounds() {
        const std::size_t size = input_buffer_.size();
        caret_pos_ = std::min(caret_pos_, size);
        selection_start_ = std::min(selection_start_, size);
        selection_end_ = std::min(selection_end_, size);
    }

    void set_caret(std::size_t pos, bool extend_selection) {
        const std::size_t clamped = std::min(pos, input_buffer_.size());
        if (extend_selection) {
            if (!has_selection()) {
                selection_start_ = caret_pos_;
            }
            caret_pos_ = clamped;
            selection_end_ = caret_pos_;
            return;
        }
        caret_pos_ = clamped;
        clear_selection();
    }

    void select_all_text() {
        caret_pos_ = input_buffer_.size();
        selection_start_ = 0u;
        selection_end_ = caret_pos_;
    }

    void erase_selection() {
        if (!has_selection()) {
            return;
        }
        const std::size_t left = selection_left();
        const std::size_t right = selection_right();
        if (right > left && right <= input_buffer_.size()) {
            input_buffer_.erase(left, right - left);
            caret_pos_ = left;
        }
        clear_selection();
    }

    void insert_at_caret(char ch) {
        erase_selection();
        input_buffer_.insert(input_buffer_.begin() + static_cast<std::ptrdiff_t>(caret_pos_), ch);
        caret_pos_ += 1u;
        clear_selection();
    }

    void insert_text_at_caret(std::string_view text) {
        if (text.empty()) {
            return;
        }
        erase_selection();
        input_buffer_.insert(caret_pos_, text.data(), text.size());
        caret_pos_ += text.size();
        clear_selection();
    }

    static std::string sanitize_paste_text(std::string_view text, bool multiline) {
        std::string out;
        out.reserve(text.size());
        for (std::size_t i = 0u; i < text.size(); ++i) {
            const char ch = text[i];
            if (ch == '\r') {
                if (multiline) {
                    out.push_back('\n');
                    if (i + 1u < text.size() && text[i + 1u] == '\n') {
                        i += 1u;
                    }
                }
                continue;
            }
            if (ch == '\n') {
                if (multiline) {
                    out.push_back('\n');
                }
                continue;
            }
            const unsigned char uch = static_cast<unsigned char>(ch);
            if (uch < 0x20u && ch != '\t') {
                continue;
            }
            out.push_back(ch);
        }
        return out;
    }

    bool is_numeric_char_allowed(char ch, bool allow_negative, bool allow_decimal) const {
        if (ch >= '0' && ch <= '9') {
            return true;
        }
        if (allow_decimal && ch == '.') {
            return input_buffer_.find('.') == std::string::npos;
        }
        if (allow_negative && ch == '-') {
            return input_buffer_.find('-') == std::string::npos && caret_pos_ == 0u;
        }
        return false;
    }

    void insert_filtered_numeric_text(std::string_view text, bool allow_negative, bool allow_decimal) {
        for (char ch : text) {
            if (!is_numeric_char_allowed(ch, allow_negative, allow_decimal)) {
                continue;
            }

            if (ch == '.' && (input_buffer_.empty() || input_buffer_ == "-")) {
                if (input_buffer_ == "-") {
                    if (caret_pos_ == 0u) {
                        caret_pos_ = 1u;
                    }
                    insert_at_caret('0');
                } else if (input_buffer_.empty()) {
                    insert_at_caret('0');
                }
            }
            insert_at_caret(ch);
        }
    }

    void delete_backward() {
        if (has_selection()) {
            erase_selection();
            return;
        }
        if (caret_pos_ == 0u || input_buffer_.empty()) {
            return;
        }
        const std::size_t prev = utf8_prev_index(input_buffer_, caret_pos_);
        input_buffer_.erase(prev, caret_pos_ - prev);
        caret_pos_ = prev;
        clear_selection();
    }

    void delete_forward() {
        if (has_selection()) {
            erase_selection();
            return;
        }
        if (caret_pos_ >= input_buffer_.size()) {
            return;
        }
        const std::size_t next = utf8_next_index(input_buffer_, caret_pos_);
        input_buffer_.erase(caret_pos_, next - caret_pos_);
        clear_selection();
    }

    void move_caret_left(bool extend_selection) {
        if (!extend_selection && has_selection()) {
            set_caret(selection_left(), false);
            return;
        }
        if (caret_pos_ == 0u) {
            if (!extend_selection) {
                clear_selection();
            }
            return;
        }
        set_caret(utf8_prev_index(input_buffer_, caret_pos_), extend_selection);
    }

    void move_caret_right(bool extend_selection) {
        if (!extend_selection && has_selection()) {
            set_caret(selection_right(), false);
            return;
        }
        if (caret_pos_ >= input_buffer_.size()) {
            if (!extend_selection) {
                clear_selection();
            }
            return;
        }
        set_caret(utf8_next_index(input_buffer_, caret_pos_), extend_selection);
    }

    void move_caret_home(bool extend_selection) {
        set_caret(0u, extend_selection);
    }

    void move_caret_end(bool extend_selection) {
        set_caret(input_buffer_.size(), extend_selection);
    }

    void start_text_input(std::uint64_t id, const std::string& initial_value, bool select_all = true) {
        if (active_input_id_ == id) {
            return;
        }
        active_input_id_ = id;
        input_buffer_ = initial_value;
        caret_pos_ = input_buffer_.size();
        selection_start_ = caret_pos_;
        selection_end_ = caret_pos_;
        if (select_all) {
            select_all_text();
        }
        drag_selecting_ = false;
    }

    bool is_text_input_active(std::uint64_t id) const {
        return active_input_id_ == id;
    }

    void stop_text_input() {
        active_input_id_ = 0u;
        input_buffer_.clear();
        caret_pos_ = 0u;
        selection_start_ = 0u;
        selection_end_ = 0u;
        drag_selecting_ = false;
    }

    void update_mouse_selection(const Rect& input_rect, float font_size, bool align_right, float padding,
                                bool hovered) {
        if (hovered && input_.mouse_pressed) {
            const std::size_t caret =
                caret_from_mouse_x(input_rect, font_size, align_right, padding, input_buffer_.size(), input_.mouse_x);
            caret_pos_ = caret;
            selection_start_ = caret;
            selection_end_ = caret;
            drag_anchor_ = caret;
            drag_selecting_ = true;
        }

        if (drag_selecting_ && input_.mouse_down) {
            const std::size_t caret =
                caret_from_mouse_x(input_rect, font_size, align_right, padding, input_buffer_.size(), input_.mouse_x);
            caret_pos_ = caret;
            selection_start_ = drag_anchor_;
            selection_end_ = caret;
        }

        if (input_.mouse_released) {
            drag_selecting_ = false;
        }
    }

    void consume_numeric_typing(bool allow_negative, bool allow_decimal) {
        const bool extend_selection = input_.key_shift;

        if (input_.key_select_all) {
            select_all_text();
        }
        if (input_.key_copy && has_selection()) {
            request_clipboard_write(input_buffer_.substr(selection_left(), selection_right() - selection_left()));
        }
        if (input_.key_cut && has_selection()) {
            request_clipboard_write(input_buffer_.substr(selection_left(), selection_right() - selection_left()));
            erase_selection();
        }
        if (input_.key_paste) {
            erase_selection();
            insert_filtered_numeric_text(input_.clipboard_text, allow_negative, allow_decimal);
        }

        if (input_.key_home) {
            move_caret_home(extend_selection);
        }
        if (input_.key_end) {
            move_caret_end(extend_selection);
        }
        if (input_.key_left) {
            move_caret_left(extend_selection);
        }
        if (input_.key_right) {
            move_caret_right(extend_selection);
        }
        if (input_.key_backspace) {
            delete_backward();
        }
        if (input_.key_delete) {
            delete_forward();
        }

        if (!input_.text_input.empty()) {
            erase_selection();
            for (char ch : input_.text_input) {
                if (is_numeric_char_allowed(ch, allow_negative, allow_decimal)) {
                    if (ch == '.' && (input_buffer_.empty() || input_buffer_ == "-")) {
                        if (input_buffer_ == "-") {
                            insert_at_caret('0');
                        } else if (input_buffer_.empty()) {
                            insert_at_caret('0');
                        }
                    }
                    insert_at_caret(ch);
                }
            }
        }
        ensure_edit_state_bounds();
    }

    void consume_plain_typing(bool multiline) {
        const bool extend_selection = input_.key_shift;

        if (input_.key_select_all) {
            select_all_text();
        }
        if (input_.key_copy && has_selection()) {
            request_clipboard_write(input_buffer_.substr(selection_left(), selection_right() - selection_left()));
        }
        if (input_.key_cut && has_selection()) {
            request_clipboard_write(input_buffer_.substr(selection_left(), selection_right() - selection_left()));
            erase_selection();
        }
        if (input_.key_paste) {
            const std::string pasted = sanitize_paste_text(input_.clipboard_text, multiline);
            if (!pasted.empty()) {
                insert_text_at_caret(pasted);
            }
        }

        if (input_.key_home) {
            move_caret_home(extend_selection);
        }
        if (input_.key_end) {
            move_caret_end(extend_selection);
        }
        if (input_.key_left) {
            move_caret_left(extend_selection);
        }
        if (input_.key_right) {
            move_caret_right(extend_selection);
        }
        if (input_.key_backspace) {
            delete_backward();
        }
        if (input_.key_delete) {
            delete_forward();
        }

        if (multiline && input_.key_enter) {
            insert_at_caret('\n');
        }

        if (!input_.text_input.empty()) {
            insert_text_at_caret(sanitize_paste_text(input_.text_input, multiline));
        }
        ensure_edit_state_bounds();
    }

    bool commit_text_input(float& value, float min_value, float max_value) {
        float parsed = value;
        const bool valid = parse_float_text(input_buffer_, parsed);
        if (valid) {
            parsed = std::clamp(parsed, min_value, max_value);
        }
        stop_text_input();
        if (!valid) {
            return false;
        }
        if (std::fabs(parsed - value) <= 1e-6f) {
            return false;
        }
        value = parsed;
        return true;
    }

    void clamp_live_numeric_buffer(float min_value, float max_value, int decimals) {
        if (input_buffer_.empty() || input_buffer_ == "-" || input_buffer_ == "." ||
            input_buffer_ == "-.") {
            return;
        }

        if (input_buffer_.size() > 24u) {
            input_buffer_.resize(24u);
        }

        decimals = std::clamp(decimals, 0, 6);
        const std::size_t dot_pos = input_buffer_.find('.');
        if (dot_pos != std::string::npos && decimals >= 0) {
            const std::size_t max_len = dot_pos + 1u + static_cast<std::size_t>(decimals);
            if (input_buffer_.size() > max_len) {
                input_buffer_.resize(max_len);
            }
        }

        float parsed = 0.0f;
        if (!parse_float_text(input_buffer_, parsed)) {
            return;
        }

        const float clamped = std::clamp(parsed, min_value, max_value);
        if (std::fabs(clamped - parsed) > 1e-6f) {
            input_buffer_ = format_float(clamped, decimals);
            caret_pos_ = input_buffer_.size();
            clear_selection();
        }
        ensure_edit_state_bounds();
    }

    bool should_show_caret() const {
        // 1.2s full cycle, 0.6s on / 0.6s off.
        return std::fmod(input_.time_seconds, 1.2) < 0.6;
    }

    void draw_text_input_content(const Rect& input_rect, float font_size, bool align_right, float padding,
                                 const Color& text_color, const Color& selection_color) {
        const std::string_view text_view = std::string_view(input_buffer_);
        const float char_w = approx_char_width(font_size);
        const float origin_x =
            text_origin_x(input_rect, font_size, align_right, padding, text_view.size());

        if (has_selection()) {
            const std::size_t sel_l = selection_left();
            const std::size_t sel_r = selection_right();
            const Rect selection_rect{
                origin_x + static_cast<float>(sel_l) * char_w,
                input_rect.y + 4.0f,
                static_cast<float>(sel_r - sel_l) * char_w,
                std::max(2.0f, input_rect.h - 8.0f),
            };
            if (selection_rect.w > 0.0f) {
                add_filled_rect(selection_rect, selection_color, 2.0f);
            }
        }

        add_text(text_view, Rect{origin_x, input_rect.y, input_rect.w - padding * 2.0f, input_rect.h}, text_color,
                 font_size, TextAlign::Left);

        if (should_show_caret()) {
            const Rect caret_rect{
                origin_x + static_cast<float>(caret_pos_) * char_w,
                input_rect.y + 4.0f,
                1.3f,
                std::max(2.0f, input_rect.h - 8.0f),
            };
            add_filled_rect(caret_rect, text_color, 0.0f);
        }
    }

    void restore_scope() {
        const ScopeState scope = scope_stack_.back();
        scope_stack_.pop_back();

        if (scope.kind == ScopeKind::Card || scope.kind == ScopeKind::DropdownBody) {
            const float needed_height = std::max(0.0f, (cursor_y_ - scope.top_y) + scope.padding);
            const float final_height = std::max(scope.min_height, needed_height);
            if (scope.fill_cmd_index < commands_.size()) {
                commands_[scope.fill_cmd_index].rect.h = final_height;
                commands_[scope.fill_cmd_index].hash = hash_command_base(commands_[scope.fill_cmd_index]);
            }
            if (scope.outline_cmd_index < commands_.size()) {
                commands_[scope.outline_cmd_index].rect.h = final_height;
                commands_[scope.outline_cmd_index].hash = hash_command_base(commands_[scope.outline_cmd_index]);
            }

            content_x_ = scope.content_x;
            content_width_ = scope.content_width;
            if (scope.in_waterfall && waterfall_.active && scope.column_index >= 0 &&
                scope.column_index < static_cast<int>(waterfall_.column_y.size())) {
                waterfall_.column_y[static_cast<std::size_t>(scope.column_index)] =
                    scope.top_y + final_height + item_spacing_;
                cursor_y_ = waterfall_.start_y;
            } else if (scope.had_outer_row) {
                row_ = scope.outer_row;
                row_.max_height = std::max(row_.max_height, final_height);
                cursor_y_ = row_.y;
                if (row_.index >= row_.columns) {
                    flush_row();
                }
            } else {
                cursor_y_ = scope.top_y + final_height + item_spacing_;
            }
            return;
        }

        if (scope.kind == ScopeKind::ScrollArea) {
            if (scope.pushed_clip) {
                pop_clip_rect();
            }

            auto it = scroll_area_state_.find(scope.scroll_state_id);
            if (it != scroll_area_state_.end()) {
                const float measured_height =
                    std::max(0.0f, cursor_y_ - scope.scroll_content_origin_y - item_spacing_);
                it->second.content_height = std::max(scope.scroll_viewport.h, measured_height);
                const float max_scroll =
                    std::max(0.0f, it->second.content_height - scope.scroll_viewport.h);
                const float hard_bound = std::max(24.0f, scope.scroll_viewport.h);
                it->second.scroll = std::clamp(it->second.scroll, -hard_bound, max_scroll + hard_bound);
            }

            content_x_ = scope.content_x;
            content_width_ = scope.content_width;
            if (scope.had_outer_row) {
                row_ = scope.outer_row;
                row_.max_height = std::max(row_.max_height, scope.min_height);
                cursor_y_ = row_.y;
                if (row_.index >= row_.columns) {
                    flush_row();
                }
            } else {
                cursor_y_ = scope.top_y + scope.min_height;
            }
            return;
        }

        content_x_ = scope.content_x;
        content_width_ = scope.content_width;
        cursor_y_ = scope.cursor_y_after;
    }

    void flush_row() {
        if (!row_.active) {
            return;
        }
        if (row_.index > 0) {
            cursor_y_ = row_.y + row_.max_height + item_spacing_;
        } else {
            cursor_y_ = row_.y;
        }
        row_ = RowState{};
    }

    Rect next_rect(float height) {
        height = std::max(2.0f, height);

        if (!row_.active) {
            Rect rect{content_x_, cursor_y_, content_width_, height};
            cursor_y_ += height + item_spacing_;
            return rect;
        }

        if (row_.index >= row_.columns) {
            row_.y += row_.max_height + item_spacing_;
            row_.index = 0;
            row_.max_height = 0.0f;
        }

        const float total_gap = row_.gap * static_cast<float>(row_.columns - 1);
        const float item_width =
            std::max(10.0f, (content_width_ - total_gap) / static_cast<float>(row_.columns));
        Rect rect{
            content_x_ + static_cast<float>(row_.index) * (item_width + row_.gap),
            row_.y,
            item_width,
            height,
        };

        row_.max_height = std::max(row_.max_height, height);
        row_.index += 1;

        return rect;
    }

    void add_filled_rect(const Rect& rect, const Color& color, float radius = 0.0f) {
        DrawCommand cmd;
        cmd.type = CommandType::FilledRect;
        cmd.rect = rect;
        cmd.color = color;
        cmd.radius = std::max(0.0f, radius);
        if (!apply_clip_to_command(cmd, nullptr)) {
            return;
        }
        cmd.hash = hash_command_base(cmd);
        commands_.push_back(std::move(cmd));
    }

    void add_outline_rect(const Rect& rect, const Color& color, float radius = 0.0f,
                          float thickness = 1.0f) {
        DrawCommand cmd;
        cmd.type = CommandType::RectOutline;
        cmd.rect = rect;
        cmd.color = color;
        cmd.radius = std::max(0.0f, radius);
        cmd.thickness = std::max(1.0f, thickness);
        if (!apply_clip_to_command(cmd, nullptr)) {
            return;
        }
        cmd.hash = hash_command_base(cmd);
        commands_.push_back(std::move(cmd));
    }

    void add_text(std::string_view text, const Rect& rect, const Color& color, float font_size,
                  TextAlign align, const Rect* clip_rect = nullptr) {
        DrawCommand cmd;
        cmd.type = CommandType::Text;
        cmd.rect = rect;
        cmd.color = color;
        cmd.font_size = std::max(8.0f, font_size);
        cmd.align = align;
        if (!apply_clip_to_command(cmd, clip_rect)) {
            return;
        }

        const std::uint32_t offset = static_cast<std::uint32_t>(text_arena_.size());
        const std::uint32_t length = static_cast<std::uint32_t>(text.size());
        text_arena_.insert(text_arena_.end(), text.begin(), text.end());
        cmd.text_offset = offset;
        cmd.text_length = length;
        cmd.hash = hash_mix(hash_command_base(cmd), hash_sv(text));
        cmd.hash = hash_mix(cmd.hash, static_cast<std::uint64_t>(length));
        commands_.push_back(std::move(cmd));
    }

    float frame_width_{1280.0f};
    float frame_height_{720.0f};
    InputState input_{};

    ThemeMode theme_mode_{ThemeMode::Dark};
    Color primary_color_{0.25f, 0.55f, 1.0f, 1.0f};
    float corner_radius_{8.0f};
    Theme theme_{make_theme(theme_mode_, primary_color_)};

    std::vector<DrawCommand> commands_{};
    std::vector<ScopeState> scope_stack_{};
    std::vector<char> text_arena_{};

    RowState row_{};
    WaterfallState waterfall_{};
    float content_x_{16.0f};
    float content_width_{800.0f};
    float cursor_y_{16.0f};
    float item_spacing_{8.0f};

    std::uint64_t panel_id_seed_{1469598103934665603ull};
    std::uint64_t active_slider_id_{0};
    std::uint64_t active_input_id_{0};
    std::uint64_t active_textarea_scroll_id_{0};
    std::uint64_t active_scroll_drag_id_{0};
    std::uint64_t active_scrollbar_drag_id_{0};
    std::string input_buffer_{};
    float active_textarea_drag_offset_{0.0f};
    float active_scroll_drag_last_y_{0.0f};
    float active_scrollbar_drag_offset_{0.0f};
    double prev_input_time_{0.0};
    bool has_prev_input_time_{false};
    float ui_dt_{1.0f / 60.0f};
    std::size_t caret_pos_{0u};
    std::size_t selection_start_{0u};
    std::size_t selection_end_{0u};
    std::size_t drag_anchor_{0u};
    bool drag_selecting_{false};
    bool repaint_requested_{false};
    bool clipboard_write_pending_{false};
    std::string clipboard_write_text_{};
    std::unordered_map<std::uint64_t, TextAreaState> text_area_state_{};
    std::unordered_map<std::uint64_t, ScrollAreaState> scroll_area_state_{};
    std::vector<Rect> clip_stack_{};
    Rect panel_rect_{};
};

#ifdef EUI_ENABLE_GLFW_OPENGL_BACKEND

namespace demo {

struct AppOptions {
    int width{1150};
    int height{820};
    const char* title{"EUI Demo"};
    bool vsync{true};
    bool continuous_render{false};
    double idle_wait_seconds{0.25};
    double max_fps{60.0};
    const char* text_font_family{"Segoe UI"};
    const char* text_font_file{nullptr};
    const char* icon_font_family{"Segoe MDL2 Assets"};
    const char* icon_font_file{nullptr};
    bool enable_icon_font_fallback{true};
};

struct FrameContext {
    Context& ui;
    GLFWwindow* window{nullptr};
    float dt{0.0f};
    int framebuffer_w{1};
    int framebuffer_h{1};
    int window_w{1};
    int window_h{1};
    float dpi_scale_x{1.0f};
    float dpi_scale_y{1.0f};
    float dpi_scale{1.0f};
    bool* repaint_flag{nullptr};

    void request_next_frame() const {
        if (repaint_flag != nullptr) {
            *repaint_flag = true;
        }
    }
};

namespace detail {

using Glyph = std::array<std::uint8_t, 7>;
using Point = std::array<float, 2>;
using TextureId = unsigned int;

struct RuntimeState {
    std::string text_input{};
    double scroll_y_accum{0.0};
    bool prev_left_mouse{false};
    bool prev_right_mouse{false};
    bool prev_backspace{false};
    bool prev_delete{false};
    bool prev_enter{false};
    bool prev_escape{false};
    bool prev_left_key{false};
    bool prev_right_key{false};
    bool prev_home_key{false};
    bool prev_end_key{false};
    bool prev_a_key{false};
    bool prev_c_key{false};
    bool prev_v_key{false};
    bool prev_x_key{false};
    bool prev_left_shift{false};
    bool prev_right_shift{false};
    double prev_mouse_x{0.0};
    double prev_mouse_y{0.0};
    bool has_prev_mouse{false};
    int prev_framebuffer_w{0};
    int prev_framebuffer_h{0};

    std::vector<DrawCommand> curr_commands{};
    std::vector<char> curr_text_arena{};
    std::vector<Rect> dirty_regions{};
    std::vector<DrawCommand> prev_commands{};
    std::vector<char> prev_text_arena{};
    Color prev_bg{};
    std::uint64_t prev_frame_hash{0ull};
    bool has_prev_frame{false};

    TextureId cache_texture{0};
    int cache_w{0};
    int cache_h{0};
    bool has_cache{false};
};

inline void text_input_callback(GLFWwindow* window, unsigned int codepoint) {
    RuntimeState* state = static_cast<RuntimeState*>(glfwGetWindowUserPointer(window));
    if (state == nullptr) {
        return;
    }
    if (codepoint < 32u && codepoint != '\t') {
        return;
    }
    if (codepoint <= 0x7Fu) {
        state->text_input.push_back(static_cast<char>(codepoint));
    } else if (codepoint <= 0x7FFu) {
        state->text_input.push_back(static_cast<char>(0xC0u | ((codepoint >> 6u) & 0x1Fu)));
        state->text_input.push_back(static_cast<char>(0x80u | (codepoint & 0x3Fu)));
    } else if (codepoint <= 0xFFFFu) {
        state->text_input.push_back(static_cast<char>(0xE0u | ((codepoint >> 12u) & 0x0Fu)));
        state->text_input.push_back(static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3Fu)));
        state->text_input.push_back(static_cast<char>(0x80u | (codepoint & 0x3Fu)));
    } else if (codepoint <= 0x10FFFFu) {
        state->text_input.push_back(static_cast<char>(0xF0u | ((codepoint >> 18u) & 0x07u)));
        state->text_input.push_back(static_cast<char>(0x80u | ((codepoint >> 12u) & 0x3Fu)));
        state->text_input.push_back(static_cast<char>(0x80u | ((codepoint >> 6u) & 0x3Fu)));
        state->text_input.push_back(static_cast<char>(0x80u | (codepoint & 0x3Fu)));
    }
}

inline void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    RuntimeState* state = static_cast<RuntimeState*>(glfwGetWindowUserPointer(window));
    if (state == nullptr) {
        return;
    }
    state->scroll_y_accum += yoffset;
}

inline bool float_eq(float lhs, float rhs, float eps = 1e-5f) {
    return std::fabs(lhs - rhs) <= eps;
}

inline bool color_eq(const Color& lhs, const Color& rhs, float eps = 1e-4f) {
    return float_eq(lhs.r, rhs.r, eps) && float_eq(lhs.g, rhs.g, eps) &&
           float_eq(lhs.b, rhs.b, eps) && float_eq(lhs.a, rhs.a, eps);
}

inline bool rect_eq(const Rect& lhs, const Rect& rhs, float eps = 0.01f) {
    return float_eq(lhs.x, rhs.x, eps) && float_eq(lhs.y, rhs.y, eps) &&
           float_eq(lhs.w, rhs.w, eps) && float_eq(lhs.h, rhs.h, eps);
}

inline bool rect_intersects(const Rect& lhs, const Rect& rhs) {
    if (lhs.w <= 0.0f || lhs.h <= 0.0f || rhs.w <= 0.0f || rhs.h <= 0.0f) {
        return false;
    }
    return lhs.x < rhs.x + rhs.w && lhs.x + lhs.w > rhs.x && lhs.y < rhs.y + rhs.h &&
           lhs.y + lhs.h > rhs.y;
}

inline bool rect_intersection(const Rect& lhs, const Rect& rhs, Rect& out) {
    const float x1 = std::max(lhs.x, rhs.x);
    const float y1 = std::max(lhs.y, rhs.y);
    const float x2 = std::min(lhs.x + lhs.w, rhs.x + rhs.w);
    const float y2 = std::min(lhs.y + lhs.h, rhs.y + rhs.h);
    if (x2 <= x1 || y2 <= y1) {
        out = Rect{};
        return false;
    }
    out = Rect{x1, y1, x2 - x1, y2 - y1};
    return true;
}

inline Rect rect_union(const Rect& lhs, const Rect& rhs) {
    const float x1 = std::min(lhs.x, rhs.x);
    const float y1 = std::min(lhs.y, rhs.y);
    const float x2 = std::max(lhs.x + lhs.w, rhs.x + rhs.w);
    const float y2 = std::max(lhs.y + lhs.h, rhs.y + rhs.h);
    return Rect{x1, y1, std::max(0.0f, x2 - x1), std::max(0.0f, y2 - y1)};
}

inline bool command_payload_equal(const DrawCommand& lhs, const DrawCommand& rhs,
                                  const std::vector<char>& lhs_arena,
                                  const std::vector<char>& rhs_arena) {
    (void)lhs_arena;
    (void)rhs_arena;
    return lhs.hash == rhs.hash;
}

inline Rect expanded_and_clamped(const Rect& rect, int width, int height, float expand_px = 2.0f) {
    Rect out{
        rect.x - expand_px,
        rect.y - expand_px,
        rect.w + expand_px * 2.0f,
        rect.h + expand_px * 2.0f,
    };
    out.x = std::clamp(out.x, 0.0f, static_cast<float>(width));
    out.y = std::clamp(out.y, 0.0f, static_cast<float>(height));
    const float right = std::clamp(out.x + out.w, 0.0f, static_cast<float>(width));
    const float bottom = std::clamp(out.y + out.h, 0.0f, static_cast<float>(height));
    out.w = std::max(0.0f, right - out.x);
    out.h = std::max(0.0f, bottom - out.y);
    return out;
}

inline std::uint64_t hash_frame_payload(const std::vector<DrawCommand>& commands, const Color& bg) {
    std::uint64_t hash = 1469598103934665603ull;
    auto mix = [](std::uint64_t h, std::uint64_t v) {
        h ^= v;
        h *= 1099511628211ull;
        return h;
    };
    auto float_to_u32 = [](float value) {
        std::uint32_t out = 0u;
        std::memcpy(&out, &value, sizeof(out));
        return out;
    };

    hash = mix(hash, static_cast<std::uint64_t>(float_to_u32(bg.r)));
    hash = mix(hash, static_cast<std::uint64_t>(float_to_u32(bg.g)));
    hash = mix(hash, static_cast<std::uint64_t>(float_to_u32(bg.b)));
    hash = mix(hash, static_cast<std::uint64_t>(float_to_u32(bg.a)));
    hash = mix(hash, static_cast<std::uint64_t>(commands.size()));
    for (const DrawCommand& cmd : commands) {
        hash = mix(hash, cmd.hash);
    }
    return hash;
}

inline void append_dirty_rect(const Rect& rect, int width, int height, std::vector<Rect>& out_regions,
                              std::size_t max_regions = 16u) {
    Rect region = expanded_and_clamped(rect, width, height);
    if (region.w <= 0.0f || region.h <= 0.0f) {
        return;
    }

    for (Rect& existing : out_regions) {
        if (rect_intersects(existing, region)) {
            existing = rect_union(existing, region);
            return;
        }
    }

    if (out_regions.size() < max_regions) {
        out_regions.push_back(region);
        return;
    }

    // Region budget exceeded: collapse to one bounding dirty area to cap CPU overhead.
    Rect merged = out_regions.front();
    for (std::size_t i = 1; i < out_regions.size(); ++i) {
        merged = rect_union(merged, out_regions[i]);
    }
    merged = rect_union(merged, region);
    out_regions.clear();
    out_regions.push_back(expanded_and_clamped(merged, width, height));
}

inline void merge_dirty_overlaps(std::vector<Rect>& regions, int width, int height) {
    if (regions.size() < 2u) {
        return;
    }
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t i = 0; i < regions.size() && !changed; ++i) {
            for (std::size_t j = i + 1; j < regions.size(); ++j) {
                if (rect_intersects(regions[i], regions[j])) {
                    regions[i] = expanded_and_clamped(rect_union(regions[i], regions[j]), width, height, 0.0f);
                    regions.erase(regions.begin() + static_cast<std::ptrdiff_t>(j));
                    changed = true;
                    break;
                }
            }
        }
    }
}

inline bool compute_dirty_regions(const std::vector<DrawCommand>& commands,
                                  const std::vector<char>& text_arena, const RuntimeState& runtime,
                                  const Color& bg, int width, int height, bool force_full,
                                  std::vector<Rect>& out_regions) {
    (void)text_arena;
    out_regions.clear();
    if (force_full || !runtime.has_prev_frame || !color_eq(bg, runtime.prev_bg)) {
        out_regions.push_back(Rect{0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)});
        return true;
    }

    const std::size_t max_count = std::max(commands.size(), runtime.prev_commands.size());
    for (std::size_t i = 0; i < max_count; ++i) {
        const bool has_curr = i < commands.size();
        const bool has_prev = i < runtime.prev_commands.size();

        if (has_curr && has_prev &&
            command_payload_equal(commands[i], runtime.prev_commands[i], text_arena, runtime.prev_text_arena)) {
            continue;
        }

        if (has_curr) {
            append_dirty_rect(commands[i].rect, width, height, out_regions);
        }
        if (has_prev) {
            append_dirty_rect(runtime.prev_commands[i].rect, width, height, out_regions);
        }
    }

    if (out_regions.empty()) {
        return false;
    }

    merge_dirty_overlaps(out_regions, width, height);
    return true;
}

struct IRect {
    int x{0};
    int y{0};
    int w{0};
    int h{0};
};

inline IRect to_gl_rect(const Rect& rect, int framebuffer_w, int framebuffer_h) {
    int x = static_cast<int>(std::floor(rect.x));
    int y_top = static_cast<int>(std::floor(rect.y));
    int w = static_cast<int>(std::ceil(rect.w));
    int h = static_cast<int>(std::ceil(rect.h));

    x = std::clamp(x, 0, framebuffer_w);
    y_top = std::clamp(y_top, 0, framebuffer_h);
    w = std::clamp(w, 0, framebuffer_w - x);
    h = std::clamp(h, 0, framebuffer_h - y_top);
    int y = framebuffer_h - (y_top + h);
    y = std::clamp(y, 0, framebuffer_h);
    return IRect{x, y, w, h};
}

inline void ensure_cache_texture(RuntimeState& runtime, int width, int height) {
    if (runtime.cache_texture == 0u) {
        glGenTextures(1, &runtime.cache_texture);
    }
    if (runtime.cache_w == width && runtime.cache_h == height && runtime.has_cache) {
        return;
    }
    runtime.cache_w = width;
    runtime.cache_h = height;
    runtime.has_cache = false;

    glBindTexture(GL_TEXTURE_2D, runtime.cache_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

inline void copy_full_to_cache(RuntimeState& runtime, int width, int height) {
    if (runtime.cache_texture == 0u) {
        return;
    }
    glBindTexture(GL_TEXTURE_2D, runtime.cache_texture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    runtime.has_cache = true;
}

inline void copy_region_to_cache(RuntimeState& runtime, const IRect& gl_rect) {
    if (runtime.cache_texture == 0u || gl_rect.w <= 0 || gl_rect.h <= 0) {
        return;
    }
    glBindTexture(GL_TEXTURE_2D, runtime.cache_texture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, gl_rect.x, gl_rect.y, gl_rect.x, gl_rect.y, gl_rect.w, gl_rect.h);
    runtime.has_cache = true;
}

inline void draw_cache_texture(const RuntimeState& runtime, int width, int height) {
    if (!runtime.has_cache || runtime.cache_texture == 0u) {
        return;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(width), static_cast<double>(height), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, runtime.cache_texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(static_cast<float>(width), 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(static_cast<float>(width), static_cast<float>(height));
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, static_cast<float>(height));
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

#ifdef _WIN32
inline std::wstring utf8_to_wide(std::string_view text) {
    if (text.empty()) {
        return std::wstring{};
    }
    const int count =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()), nullptr, 0);
    if (count <= 0) {
        std::wstring fallback;
        fallback.reserve(text.size());
        for (char ch : text) {
            fallback.push_back(static_cast<unsigned char>(ch));
        }
        return fallback;
    }
    std::wstring out(static_cast<std::size_t>(count), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()), out.data(),
                        count);
    return out;
}

inline bool utf8_next(std::string_view text, std::size_t& index, std::uint32_t& codepoint) {
    if (index >= text.size()) {
        return false;
    }
    const unsigned char lead = static_cast<unsigned char>(text[index]);
    if (lead < 0x80u) {
        codepoint = lead;
        index += 1u;
        return true;
    }
    if ((lead >> 5u) == 0x6u && index + 1u < text.size()) {
        const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
        if ((b1 & 0xC0u) == 0x80u) {
            codepoint = (static_cast<std::uint32_t>(lead & 0x1Fu) << 6u) |
                        static_cast<std::uint32_t>(b1 & 0x3Fu);
            index += 2u;
            return true;
        }
    }
    if ((lead >> 4u) == 0xEu && index + 2u < text.size()) {
        const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
        const unsigned char b2 = static_cast<unsigned char>(text[index + 2u]);
        if ((b1 & 0xC0u) == 0x80u && (b2 & 0xC0u) == 0x80u) {
            codepoint = (static_cast<std::uint32_t>(lead & 0x0Fu) << 12u) |
                        (static_cast<std::uint32_t>(b1 & 0x3Fu) << 6u) |
                        static_cast<std::uint32_t>(b2 & 0x3Fu);
            index += 3u;
            return true;
        }
    }
    if ((lead >> 3u) == 0x1Eu && index + 3u < text.size()) {
        const unsigned char b1 = static_cast<unsigned char>(text[index + 1u]);
        const unsigned char b2 = static_cast<unsigned char>(text[index + 2u]);
        const unsigned char b3 = static_cast<unsigned char>(text[index + 3u]);
        if ((b1 & 0xC0u) == 0x80u && (b2 & 0xC0u) == 0x80u && (b3 & 0xC0u) == 0x80u) {
            codepoint = (static_cast<std::uint32_t>(lead & 0x07u) << 18u) |
                        (static_cast<std::uint32_t>(b1 & 0x3Fu) << 12u) |
                        (static_cast<std::uint32_t>(b2 & 0x3Fu) << 6u) |
                        static_cast<std::uint32_t>(b3 & 0x3Fu);
            index += 4u;
            return true;
        }
    }
    codepoint = static_cast<std::uint32_t>(lead);
    index += 1u;
    return true;
}

class Win32FontRenderer {
public:
    explicit Win32FontRenderer(const AppOptions& options)
        : text_family_(utf8_to_wide(options.text_font_family != nullptr ? options.text_font_family : "Segoe UI")),
          icon_family_(utf8_to_wide(options.icon_font_family != nullptr ? options.icon_font_family
                                                                       : "Segoe MDL2 Assets")),
          text_font_file_(utf8_to_wide(options.text_font_file != nullptr ? options.text_font_file : "")),
          icon_font_file_(utf8_to_wide(options.icon_font_file != nullptr ? options.icon_font_file : "")),
          enable_icon_fallback_(options.enable_icon_font_fallback) {}

    ~Win32FontRenderer() {
        if (wglGetCurrentContext() != nullptr) {
            release_gl_resources();
        }
        for (auto& pair : fonts_) {
            if (pair.second.handle != nullptr) {
                DeleteObject(pair.second.handle);
                pair.second.handle = nullptr;
            }
        }
        for (const std::wstring& path : loaded_private_fonts_) {
            RemoveFontResourceExW(path.c_str(), FR_PRIVATE, nullptr);
        }
    }

    Win32FontRenderer(const Win32FontRenderer&) = delete;
    Win32FontRenderer& operator=(const Win32FontRenderer&) = delete;

    void release_gl_resources() const {
        for (auto& pair : fonts_) {
            auto& glyphs = pair.second.glyphs;
            for (auto& glyph_pair : glyphs) {
                GlyphData& glyph = glyph_pair.second;
                if (glyph.list_id != 0u) {
                    glDeleteLists(glyph.list_id, 1);
                    glyph.list_id = 0u;
                }
            }
            glyphs.clear();
        }
    }

    bool draw_text(const DrawCommand& cmd, std::string_view text) const {
        if (text.empty()) {
            return true;
        }

        HDC hdc = wglGetCurrentDC();
        if (hdc == nullptr) {
            return false;
        }

        ensure_private_fonts_loaded();

        // Outline path appears visually smaller than legacy bitmap; lift render px for parity.
        const int font_px = std::max(8, static_cast<int>(std::round(cmd.font_size * 1.2f)));
        FontInstance* text_font = ensure_font(false, font_px, hdc);
        if (text_font == nullptr) {
            return false;
        }
        FontInstance* icon_font = enable_icon_fallback_ ? ensure_font(true, font_px, hdc) : nullptr;

        std::vector<GlyphData> glyphs;
        glyphs.reserve(text.size());
        float width = 0.0f;
        float max_above_baseline = 0.0f;
        float max_below_baseline = 0.0f;

        std::size_t index = 0u;
        while (index < text.size()) {
            std::uint32_t cp = 0u;
            if (!utf8_next(text, index, cp)) {
                break;
            }
            const GlyphData* glyph = resolve_glyph(cp, text_font, icon_font, hdc);
            if (glyph == nullptr || !glyph->valid) {
                continue;
            }
            glyphs.push_back(*glyph);
            width += glyph->advance;
            max_above_baseline = std::max(max_above_baseline, glyph->ascent);
            max_below_baseline =
                std::max(max_below_baseline, std::max(0.0f, glyph->line_height - glyph->ascent));
        }

        if (glyphs.empty()) {
            return false;
        }

        float x = cmd.rect.x;
        if (cmd.align == TextAlign::Center) {
            x += std::max(0.0f, (cmd.rect.w - width) * 0.5f);
        } else if (cmd.align == TextAlign::Right) {
            x += std::max(0.0f, cmd.rect.w - width);
        }
        const float resolved_text_h =
            std::max(1.0f, max_above_baseline + max_below_baseline);
        const float baseline_y =
            cmd.rect.y + std::max(0.0f, (cmd.rect.h - resolved_text_h) * 0.5f) + max_above_baseline;

        glColor4f(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        float pen_x = x;
        for (const GlyphData& glyph : glyphs) {
            if (glyph.list_id != 0u) {
                if (glyph.is_bitmap) {
                    glRasterPos2f(pen_x, baseline_y);
                    glCallList(glyph.list_id);
                } else {
                    glPushMatrix();
                    glTranslatef(pen_x, baseline_y, 0.0f);
                    // wgl outline glyph coordinates are y-up; UI projection here is y-down.
                    const float scale = std::max(1.0f, glyph.outline_scale);
                    glScalef(scale, -scale, 1.0f);
                    glCallList(glyph.list_id);
                    glPopMatrix();
                }
            }
            pen_x += glyph.advance;
        }
        return true;
    }

private:
    struct GlyphData {
        unsigned int list_id{0u};
        float advance{0.0f};
        float ascent{0.0f};
        float line_height{0.0f};
        float outline_scale{1.0f};
        bool is_bitmap{false};
        bool valid{false};
    };

    struct FontInstance {
        HFONT handle{nullptr};
        int ascent{0};
        int line_height{0};
        int pixel_size{0};
        std::unordered_map<std::uint32_t, GlyphData> glyphs{};
    };

    static int to_utf16(std::uint32_t cp, wchar_t out[2]) {
        if (cp <= 0xFFFFu) {
            out[0] = static_cast<wchar_t>(cp);
            return 1;
        }
        out[0] = L'\0';
        out[1] = L'\0';
        return 0;
    }

    static bool is_private_use_codepoint(std::uint32_t cp) {
        return cp >= 0xE000u && cp <= 0xF8FFu;
    }

    static bool is_known_icon_face(const std::wstring& face_name) {
        return face_name == L"Segoe Fluent Icons" || face_name == L"Segoe MDL2 Assets" ||
               face_name == L"Segoe UI Symbol";
    }

    static bool wide_iequals(const std::wstring& lhs, const std::wstring& rhs) {
        if (lhs.empty() || rhs.empty()) {
            return false;
        }
        return CompareStringOrdinal(lhs.c_str(), -1, rhs.c_str(), -1, TRUE) == CSTR_EQUAL;
    }

    std::string font_key(bool icon_font, int px) const {
        return std::string(icon_font ? "icon:" : "text:") + std::to_string(px);
    }

    void ensure_private_fonts_loaded() const {
        if (private_fonts_loaded_) {
            return;
        }
        private_fonts_loaded_ = true;

        auto load_private = [&](const std::wstring& path) {
            if (path.empty()) {
                return;
            }
            const int added = AddFontResourceExW(path.c_str(), FR_PRIVATE, nullptr);
            if (added > 0) {
                loaded_private_fonts_.push_back(path);
            }
        };
        load_private(text_font_file_);
        load_private(icon_font_file_);
    }

    FontInstance* ensure_font(bool icon_font, int px, HDC hdc) const {
        const std::string key = font_key(icon_font, px);
        auto it = fonts_.find(key);
        if (it != fonts_.end()) {
            return &it->second;
        }

        auto create_font_instance =
            [&](const std::wstring& family, DWORD charset, bool* out_is_known_icon_face,
                std::wstring* out_resolved_face) -> FontInstance {
            FontInstance instance{};
            if (family.empty()) {
                if (out_is_known_icon_face != nullptr) {
                    *out_is_known_icon_face = false;
                }
                if (out_resolved_face != nullptr) {
                    out_resolved_face->clear();
                }
                return instance;
            }

            HFONT font = CreateFontW(-px, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, charset, OUT_TT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                     family.c_str());
            if (font == nullptr) {
                if (out_is_known_icon_face != nullptr) {
                    *out_is_known_icon_face = false;
                }
                if (out_resolved_face != nullptr) {
                    out_resolved_face->clear();
                }
                return instance;
            }

            HGDIOBJ old = SelectObject(hdc, font);
            TEXTMETRICW tm{};
            GetTextMetricsW(hdc, &tm);
            wchar_t face_name[LF_FACESIZE]{};
            GetTextFaceW(hdc, LF_FACESIZE, face_name);
            SelectObject(hdc, old);

            if (out_is_known_icon_face != nullptr) {
                *out_is_known_icon_face = is_known_icon_face(face_name);
            }
            if (out_resolved_face != nullptr) {
                *out_resolved_face = face_name;
            }

            instance.handle = font;
            instance.ascent = std::max(1, static_cast<int>(tm.tmAscent));
            instance.line_height = std::max(1, static_cast<int>(tm.tmHeight));
            instance.pixel_size = std::max(1, px);
            return instance;
        };

        FontInstance instance{};
        if (icon_font) {
            std::vector<std::wstring> candidates;
            candidates.reserve(4);
            auto add_unique = [&](const std::wstring& family) {
                if (family.empty()) {
                    return;
                }
                for (const std::wstring& existing : candidates) {
                    if (existing == family) {
                        return;
                    }
                }
                candidates.push_back(family);
            };

            add_unique(icon_family_);
            add_unique(L"Segoe Fluent Icons");
            add_unique(L"Segoe MDL2 Assets");
            add_unique(L"Segoe UI Symbol");

            for (const std::wstring& family : candidates) {
                bool known_icon_face = false;
                std::wstring resolved_face{};
                FontInstance candidate =
                    create_font_instance(family, DEFAULT_CHARSET, &known_icon_face, &resolved_face);
                if (candidate.handle == nullptr) {
                    continue;
                }

                const bool exact_family_match = wide_iequals(resolved_face, family);
                const bool usable_icon_face = known_icon_face || exact_family_match;
                if (usable_icon_face) {
                    instance = std::move(candidate);
                    break;
                }
                DeleteObject(candidate.handle);
            }
        } else {
            instance = create_font_instance(text_family_, DEFAULT_CHARSET, nullptr, nullptr);
        }

        if (instance.handle == nullptr) {
            return nullptr;
        }

        auto [inserted_it, _] = fonts_.emplace(key, std::move(instance));
        return &inserted_it->second;
    }

    GlyphData* ensure_glyph(FontInstance* font, std::uint32_t cp, HDC hdc) const {
        if (font == nullptr) {
            return nullptr;
        }

        auto it = font->glyphs.find(cp);
        if (it != font->glyphs.end()) {
            return &it->second;
        }

        GlyphData glyph{};
        wchar_t wide[2] = {L'\0', L'\0'};
        const int wide_len = to_utf16(cp, wide);
        if (wide_len == 1) {
            HGDIOBJ old = SelectObject(hdc, font->handle);
            bool glyph_available = true;
            if (is_private_use_codepoint(cp)) {
                WORD glyph_index = 0xFFFFu;
                const DWORD glyph_query =
                    GetGlyphIndicesW(hdc, wide, 1, &glyph_index, GGI_MARK_NONEXISTING_GLYPHS);
                glyph_available = glyph_query != GDI_ERROR && glyph_index != 0xFFFFu;
            }

            if (glyph_available) {
                const unsigned int list_id = glGenLists(1);
                bool built = false;
                const DWORD glyph_code = static_cast<DWORD>(wide[0]);
                if (list_id != 0u) {
                    GLYPHMETRICSFLOAT gm{};
                    if (wglUseFontOutlinesW(hdc, glyph_code, 1u, list_id, 0.0f, 0.0f,
                                            WGL_FONT_POLYGONS, &gm) != FALSE) {
                        const bool outline_metrics_ok =
                            gm.gmfBlackBoxX > 0.0f && gm.gmfBlackBoxY > 0.0f;
                        const float outline_scale = static_cast<float>(std::max(1, font->pixel_size));
                        glyph.advance = std::max(0.0f, gm.gmfCellIncX * outline_scale);
                        if (glyph.advance <= 0.0f) {
                            SIZE size{};
                            GetTextExtentPoint32W(hdc, wide, 1, &size);
                            glyph.advance = std::max(0.0f, static_cast<float>(size.cx));
                        }
                        if (outline_metrics_ok && glyph.advance >= 0.25f) {
                            const float above = std::max(0.0f, gm.gmfptGlyphOrigin.y * outline_scale);
                            const float below =
                                std::max(0.0f, (gm.gmfBlackBoxY - gm.gmfptGlyphOrigin.y) * outline_scale);
                            glyph.ascent = above;
                            glyph.line_height = std::max(1.0f, above + below);
                            if (glyph.line_height < 1.0f || glyph.ascent < 0.0f) {
                                glyph.ascent = static_cast<float>(std::max(1, font->ascent));
                                glyph.line_height = static_cast<float>(std::max(1, font->line_height));
                            }
                            glyph.outline_scale = outline_scale;
                            glyph.list_id = list_id;
                            glyph.is_bitmap = false;
                            glyph.valid = true;
                            built = true;
                        }
                    }
                }
                if (!built && list_id != 0u && wglUseFontBitmapsW(hdc, glyph_code, 1u, list_id) != FALSE) {
                    SIZE size{};
                    GetTextExtentPoint32W(hdc, wide, 1, &size);
                    glyph.advance = std::max(0.0f, static_cast<float>(size.cx));
                    glyph.ascent = static_cast<float>(std::max(1, font->ascent));
                    glyph.line_height = static_cast<float>(std::max(1, font->line_height));
                    glyph.outline_scale = 1.0f;
                    glyph.list_id = list_id;
                    glyph.is_bitmap = true;
                    glyph.valid = true;
                    built = true;
                }

                if (!built && list_id != 0u) {
                    glDeleteLists(list_id, 1);
                }
            }
            SelectObject(hdc, old);
        }

        auto [inserted_it, _] = font->glyphs.emplace(cp, glyph);
        return &inserted_it->second;
    }

    const GlyphData* resolve_glyph(std::uint32_t cp, FontInstance* text_font, FontInstance* icon_font,
                                   HDC hdc) const {
        auto try_font = [&](FontInstance* font) -> GlyphData* {
            GlyphData* glyph = ensure_glyph(font, cp, hdc);
            if (glyph != nullptr && glyph->valid) {
                return glyph;
            }
            return nullptr;
        };

        const bool prefer_icon_font = enable_icon_fallback_ && icon_font != nullptr && is_private_use_codepoint(cp);
        if (prefer_icon_font) {
            if (GlyphData* icon_glyph = try_font(icon_font)) {
                return icon_glyph;
            }
            return nullptr;
        } else {
            if (GlyphData* text_glyph = try_font(text_font)) {
                return text_glyph;
            }
            if (enable_icon_fallback_ && icon_font != nullptr) {
                if (GlyphData* icon_glyph = try_font(icon_font)) {
                    return icon_glyph;
                }
            }
        }

        GlyphData* fallback_glyph = ensure_glyph(text_font, static_cast<std::uint32_t>('?'), hdc);
        if (fallback_glyph != nullptr && fallback_glyph->valid) {
            return fallback_glyph;
        }
        return nullptr;
    }

    std::wstring text_family_{};
    std::wstring icon_family_{};
    std::wstring text_font_file_{};
    std::wstring icon_font_file_{};
    bool enable_icon_fallback_{true};

    mutable bool private_fonts_loaded_{false};
    mutable std::unordered_map<std::string, FontInstance> fonts_{};
    mutable std::vector<std::wstring> loaded_private_fonts_{};
};
#endif

inline const Glyph& glyph_for(char ch) {
    static const Glyph kUnknown = {0x1E, 0x11, 0x02, 0x04, 0x04, 0x00, 0x04};
    static const Glyph kSpace = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const Glyph kDot = {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C};
    static const Glyph kMinus = {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00};
    static const Glyph kGreater = {0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10};
    static const Glyph kPipe = {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
    static const Glyph kPercent = {0x19, 0x19, 0x02, 0x04, 0x08, 0x13, 0x13};
    static const Glyph kSlash = {0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00};

    static const Glyph k0 = {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E};
    static const Glyph k1 = {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E};
    static const Glyph k2 = {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F};
    static const Glyph k3 = {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E};
    static const Glyph k4 = {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02};
    static const Glyph k5 = {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E};
    static const Glyph k6 = {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E};
    static const Glyph k7 = {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08};
    static const Glyph k8 = {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E};
    static const Glyph k9 = {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C};

    static const Glyph kA = {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
    static const Glyph kB = {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E};
    static const Glyph kC = {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F};
    static const Glyph kD = {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E};
    static const Glyph kE = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F};
    static const Glyph kF = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10};
    static const Glyph kG = {0x0F, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0F};
    static const Glyph kH = {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
    static const Glyph kI = {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E};
    static const Glyph kJ = {0x07, 0x02, 0x02, 0x02, 0x12, 0x12, 0x0C};
    static const Glyph kK = {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11};
    static const Glyph kL = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};
    static const Glyph kM = {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11};
    static const Glyph kN = {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11};
    static const Glyph kO = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
    static const Glyph kP = {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10};
    static const Glyph kQ = {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D};
    static const Glyph kR = {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};
    static const Glyph kS = {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E};
    static const Glyph kT = {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
    static const Glyph kU = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
    static const Glyph kV = {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04};
    static const Glyph kW = {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A};
    static const Glyph kX = {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11};
    static const Glyph kY = {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04};
    static const Glyph kZ = {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F};

    switch (ch) {
        case ' ':
            return kSpace;
        case '.':
            return kDot;
        case '-':
            return kMinus;
        case '>':
            return kGreater;
        case '|':
            return kPipe;
        case '%':
            return kPercent;
        case '/':
            return kSlash;
        case '0':
            return k0;
        case '1':
            return k1;
        case '2':
            return k2;
        case '3':
            return k3;
        case '4':
            return k4;
        case '5':
            return k5;
        case '6':
            return k6;
        case '7':
            return k7;
        case '8':
            return k8;
        case '9':
            return k9;
        case 'A':
            return kA;
        case 'B':
            return kB;
        case 'C':
            return kC;
        case 'D':
            return kD;
        case 'E':
            return kE;
        case 'F':
            return kF;
        case 'G':
            return kG;
        case 'H':
            return kH;
        case 'I':
            return kI;
        case 'J':
            return kJ;
        case 'K':
            return kK;
        case 'L':
            return kL;
        case 'M':
            return kM;
        case 'N':
            return kN;
        case 'O':
            return kO;
        case 'P':
            return kP;
        case 'Q':
            return kQ;
        case 'R':
            return kR;
        case 'S':
            return kS;
        case 'T':
            return kT;
        case 'U':
            return kU;
        case 'V':
            return kV;
        case 'W':
            return kW;
        case 'X':
            return kX;
        case 'Y':
            return kY;
        case 'Z':
            return kZ;
        default:
            return kUnknown;
    }
}

inline void gl_set_color(const Color& color) {
    glColor4f(color.r, color.g, color.b, color.a);
}

inline int build_rounded_points(const Rect& rect, float radius, Point* out_points, int max_points) {
    if (max_points < 4) {
        return 0;
    }

    radius = std::clamp(radius, 0.0f, std::min(rect.w, rect.h) * 0.5f);
    if (radius <= 0.0f) {
        out_points[0] = Point{rect.x, rect.y};
        out_points[1] = Point{rect.x + rect.w, rect.y};
        out_points[2] = Point{rect.x + rect.w, rect.y + rect.h};
        out_points[3] = Point{rect.x, rect.y + rect.h};
        return 4;
    }

    const float left = rect.x;
    const float right = rect.x + rect.w;
    const float top = rect.y;
    const float bottom = rect.y + rect.h;
    const int steps = std::clamp(static_cast<int>(radius * 0.65f), 3, 10);
    const float kPi = 3.1415926f;

    int count = 0;
    auto push_point = [&](float x, float y) {
        if (count < max_points) {
            out_points[count++] = Point{x, y};
        }
    };
    auto add_arc = [&](float cx, float cy, float start_angle, float end_angle, bool include_first) {
        for (int i = 0; i <= steps; ++i) {
            if (i == 0 && !include_first) {
                continue;
            }
            const float t = static_cast<float>(i) / static_cast<float>(steps);
            const float angle = start_angle + (end_angle - start_angle) * t;
            push_point(cx + std::cos(angle) * radius, cy + std::sin(angle) * radius);
        }
    };

    add_arc(left + radius, top + radius, kPi, 1.5f * kPi, true);
    add_arc(right - radius, top + radius, 1.5f * kPi, 2.0f * kPi, false);
    add_arc(right - radius, bottom - radius, 0.0f, 0.5f * kPi, false);
    add_arc(left + radius, bottom - radius, 0.5f * kPi, kPi, false);
    return count;
}

inline void draw_filled_rect(const Rect& rect, float radius) {
    if (radius <= 0.0f) {
        glBegin(GL_QUADS);
        glVertex2f(rect.x, rect.y);
        glVertex2f(rect.x + rect.w, rect.y);
        glVertex2f(rect.x + rect.w, rect.y + rect.h);
        glVertex2f(rect.x, rect.y + rect.h);
        glEnd();
        return;
    }

    std::array<Point, 64> points{};
    const int count = build_rounded_points(rect, radius, points.data(), static_cast<int>(points.size()));
    if (count < 3) {
        return;
    }

    float center_x = 0.0f;
    float center_y = 0.0f;
    for (int i = 0; i < count; ++i) {
        center_x += points[static_cast<std::size_t>(i)][0];
        center_y += points[static_cast<std::size_t>(i)][1];
    }
    center_x /= static_cast<float>(count);
    center_y /= static_cast<float>(count);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(center_x, center_y);
    for (int i = 0; i < count; ++i) {
        const Point& p = points[static_cast<std::size_t>(i)];
        glVertex2f(p[0], p[1]);
    }
    glVertex2f(points[0][0], points[0][1]);
    glEnd();
}

inline void draw_outline_rect(const Rect& rect, float radius, float thickness) {
    if (radius <= 0.0f) {
        glLineWidth(std::max(1.0f, thickness));
        glBegin(GL_LINE_LOOP);
        glVertex2f(rect.x, rect.y);
        glVertex2f(rect.x + rect.w, rect.y);
        glVertex2f(rect.x + rect.w, rect.y + rect.h);
        glVertex2f(rect.x, rect.y + rect.h);
        glEnd();
        glLineWidth(1.0f);
        return;
    }

    std::array<Point, 64> points{};
    const int count = build_rounded_points(rect, radius, points.data(), static_cast<int>(points.size()));
    if (count < 3) {
        return;
    }

    glLineWidth(std::max(1.0f, thickness));
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < count; ++i) {
        const Point& p = points[static_cast<std::size_t>(i)];
        glVertex2f(p[0], p[1]);
    }
    glEnd();
    glLineWidth(1.0f);
}

inline float text_width(std::size_t glyph_count, float scale) {
    if (glyph_count == 0u) {
        return 0.0f;
    }
    const float advance = 6.0f * scale;
    return advance * static_cast<float>(glyph_count) - scale;
}

inline void draw_text_bitmap(const DrawCommand& cmd, std::string_view text) {
    if (text.empty()) {
        return;
    }

    const float scale = std::max(1.0f, cmd.font_size / 8.0f);
    const float advance = 6.0f * scale;
    const float width = text_width(text.size(), scale);
    const float glyph_h = 7.0f * scale;

    float x = cmd.rect.x;
    if (cmd.align == TextAlign::Center) {
        x += std::max(0.0f, (cmd.rect.w - width) * 0.5f);
    } else if (cmd.align == TextAlign::Right) {
        x += std::max(0.0f, cmd.rect.w - width);
    }
    const float y = cmd.rect.y + std::max(0.0f, (cmd.rect.h - glyph_h) * 0.5f);

    struct BitmapVertex {
        float x;
        float y;
        float r;
        float g;
        float b;
        float a;
    };
    std::vector<BitmapVertex> verts;
    verts.reserve(text.size() * 7u * 5u * 4u);
    auto push_v = [&](float px, float py) {
        verts.push_back(BitmapVertex{px, py, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a});
    };

    float pen_x = x;
    for (char raw_ch : text) {
        const char ch = static_cast<char>(std::toupper(static_cast<unsigned char>(raw_ch)));
        if (ch == ' ') {
            pen_x += advance;
            continue;
        }

        const Glyph& glyph = glyph_for(ch);
        for (std::size_t row = 0; row < glyph.size(); ++row) {
            const std::uint8_t bits = glyph[row];
            for (int col = 0; col < 5; ++col) {
                const std::uint8_t mask = static_cast<std::uint8_t>(1u << (4 - col));
                if ((bits & mask) == 0u) {
                    continue;
                }
                const float px = pen_x + static_cast<float>(col) * scale;
                const float py = y + static_cast<float>(row) * scale;
                push_v(px, py);
                push_v(px + scale, py);
                push_v(px + scale, py + scale);
                push_v(px, py + scale);
            }
        }
        pen_x += advance;
    }
    if (!verts.empty()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(BitmapVertex), &verts[0].x);
        glColorPointer(4, GL_FLOAT, sizeof(BitmapVertex), &verts[0].r);
        glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(verts.size()));
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

class Renderer {
public:
    explicit Renderer(const AppOptions& options = {})
#ifdef _WIN32
        : win32_font_renderer_(options)
#endif
    {}

    void release_gl_resources() const {
#ifdef _WIN32
        win32_font_renderer_.release_gl_resources();
#endif
    }

    void render(const std::vector<DrawCommand>& commands, const std::vector<char>& text_arena, int width,
                int height, const Rect* clip_rect = nullptr) const {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, static_cast<double>(width), static_cast<double>(height), 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        const bool has_outer_clip = clip_rect != nullptr;
        const IRect outer_gl_clip = has_outer_clip ? to_gl_rect(*clip_rect, width, height) : IRect{};
        const bool outer_clip_valid = !has_outer_clip || (outer_gl_clip.w > 0 && outer_gl_clip.h > 0);
        if (!outer_clip_valid) {
            return;
        }

        auto irect_equal = [](const IRect& lhs, const IRect& rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
        };

        bool scissor_enabled = false;
        IRect active_scissor{};
        auto apply_scissor = [&](const IRect* desired) {
            if (desired == nullptr) {
                if (scissor_enabled) {
                    glDisable(GL_SCISSOR_TEST);
                    scissor_enabled = false;
                }
                return;
            }
            if (desired->w <= 0 || desired->h <= 0) {
                if (scissor_enabled) {
                    glDisable(GL_SCISSOR_TEST);
                    scissor_enabled = false;
                }
                return;
            }
            if (!scissor_enabled) {
                glEnable(GL_SCISSOR_TEST);
                glScissor(desired->x, desired->y, desired->w, desired->h);
                active_scissor = *desired;
                scissor_enabled = true;
                return;
            }
            if (!irect_equal(active_scissor, *desired)) {
                glScissor(desired->x, desired->y, desired->w, desired->h);
                active_scissor = *desired;
            }
        };

        visible_indices_.clear();
        if (clip_rect == nullptr || commands.size() < 96u) {
            visible_indices_.reserve(commands.size());
            for (std::size_t i = 0; i < commands.size(); ++i) {
                if (clip_rect == nullptr || rect_intersects(commands[i].rect, *clip_rect)) {
                    visible_indices_.push_back(i);
                }
            }
        } else {
            const int tile_px = 128;
            const int cols = std::max(1, (width + tile_px - 1) / tile_px);
            const int rows = std::max(1, (height + tile_px - 1) / tile_px);
            const std::size_t bucket_count = static_cast<std::size_t>(cols * rows);
            if (spatial_buckets_.size() != bucket_count) {
                spatial_buckets_.assign(bucket_count, std::vector<std::uint32_t>{});
            } else {
                for (std::vector<std::uint32_t>& bucket : spatial_buckets_) {
                    bucket.clear();
                }
            }

            for (std::size_t i = 0; i < commands.size(); ++i) {
                const Rect& rect = commands[i].rect;
                if (rect.w <= 0.0f || rect.h <= 0.0f || !rect_intersects(rect, *clip_rect)) {
                    continue;
                }

                const int x0 = std::clamp(static_cast<int>(std::floor(rect.x / tile_px)), 0, cols - 1);
                const int y0 = std::clamp(static_cast<int>(std::floor(rect.y / tile_px)), 0, rows - 1);
                const int x1 = std::clamp(static_cast<int>(std::floor((rect.x + rect.w) / tile_px)), 0, cols - 1);
                const int y1 = std::clamp(static_cast<int>(std::floor((rect.y + rect.h) / tile_px)), 0, rows - 1);
                for (int y = y0; y <= y1; ++y) {
                    for (int x = x0; x <= x1; ++x) {
                        spatial_buckets_[static_cast<std::size_t>(y * cols + x)].push_back(
                            static_cast<std::uint32_t>(i));
                    }
                }
            }

            if (spatial_marks_.size() < commands.size()) {
                spatial_marks_.assign(commands.size(), 0u);
                spatial_mark_id_ = 1u;
            }
            spatial_mark_id_ += 1u;
            if (spatial_mark_id_ == 0u) {
                std::fill(spatial_marks_.begin(), spatial_marks_.end(), 0u);
                spatial_mark_id_ = 1u;
            }

            visible_indices_.reserve(commands.size() / 4u + 8u);
            const int clip_x0 = std::clamp(static_cast<int>(std::floor(clip_rect->x / tile_px)), 0, cols - 1);
            const int clip_y0 = std::clamp(static_cast<int>(std::floor(clip_rect->y / tile_px)), 0, rows - 1);
            const int clip_x1 =
                std::clamp(static_cast<int>(std::floor((clip_rect->x + clip_rect->w) / tile_px)), 0, cols - 1);
            const int clip_y1 =
                std::clamp(static_cast<int>(std::floor((clip_rect->y + clip_rect->h) / tile_px)), 0, rows - 1);
            for (int y = clip_y0; y <= clip_y1; ++y) {
                for (int x = clip_x0; x <= clip_x1; ++x) {
                    const std::vector<std::uint32_t>& bucket =
                        spatial_buckets_[static_cast<std::size_t>(y * cols + x)];
                    for (std::uint32_t idx : bucket) {
                        if (spatial_marks_[idx] == spatial_mark_id_) {
                            continue;
                        }
                        spatial_marks_[idx] = spatial_mark_id_;
                        visible_indices_.push_back(static_cast<std::size_t>(idx));
                    }
                }
            }
            std::sort(visible_indices_.begin(), visible_indices_.end());
        }

        apply_scissor(nullptr);

        struct BatchVertex {
            float x;
            float y;
            float r;
            float g;
            float b;
            float a;
        };
        enum class PendingBatch {
            None,
            Filled,
            Outline,
        };
        PendingBatch pending_batch = PendingBatch::None;
        float pending_outline_thickness = 1.0f;
        std::vector<BatchVertex> filled_batch{};
        std::vector<BatchVertex> outline_batch{};
        filled_batch.reserve(1024);
        outline_batch.reserve(1024);

        auto push_colored_vertex = [](std::vector<BatchVertex>& batch, float x, float y, const Color& color) {
            batch.push_back(BatchVertex{x, y, color.r, color.g, color.b, color.a});
        };

        auto append_filled_rect = [&](const DrawCommand& cmd) {
            if (cmd.radius <= 0.0f) {
                const float x0 = cmd.rect.x;
                const float y0 = cmd.rect.y;
                const float x1 = cmd.rect.x + cmd.rect.w;
                const float y1 = cmd.rect.y + cmd.rect.h;
                push_colored_vertex(filled_batch, x0, y0, cmd.color);
                push_colored_vertex(filled_batch, x1, y0, cmd.color);
                push_colored_vertex(filled_batch, x1, y1, cmd.color);
                push_colored_vertex(filled_batch, x0, y0, cmd.color);
                push_colored_vertex(filled_batch, x1, y1, cmd.color);
                push_colored_vertex(filled_batch, x0, y1, cmd.color);
                return;
            }

            std::array<Point, 64> points{};
            const int count = build_rounded_points(cmd.rect, cmd.radius, points.data(),
                                                   static_cast<int>(points.size()));
            if (count < 3) {
                return;
            }
            float center_x = 0.0f;
            float center_y = 0.0f;
            for (int i = 0; i < count; ++i) {
                center_x += points[static_cast<std::size_t>(i)][0];
                center_y += points[static_cast<std::size_t>(i)][1];
            }
            center_x /= static_cast<float>(count);
            center_y /= static_cast<float>(count);

            for (int i = 0; i < count; ++i) {
                const int next = (i + 1) % count;
                push_colored_vertex(filled_batch, center_x, center_y, cmd.color);
                push_colored_vertex(filled_batch, points[static_cast<std::size_t>(i)][0],
                                    points[static_cast<std::size_t>(i)][1], cmd.color);
                push_colored_vertex(filled_batch, points[static_cast<std::size_t>(next)][0],
                                    points[static_cast<std::size_t>(next)][1], cmd.color);
            }
        };

        auto append_outline_rect = [&](const DrawCommand& cmd) {
            if (cmd.radius <= 0.0f) {
                const float x0 = cmd.rect.x;
                const float y0 = cmd.rect.y;
                const float x1 = cmd.rect.x + cmd.rect.w;
                const float y1 = cmd.rect.y + cmd.rect.h;
                push_colored_vertex(outline_batch, x0, y0, cmd.color);
                push_colored_vertex(outline_batch, x1, y0, cmd.color);
                push_colored_vertex(outline_batch, x1, y0, cmd.color);
                push_colored_vertex(outline_batch, x1, y1, cmd.color);
                push_colored_vertex(outline_batch, x1, y1, cmd.color);
                push_colored_vertex(outline_batch, x0, y1, cmd.color);
                push_colored_vertex(outline_batch, x0, y1, cmd.color);
                push_colored_vertex(outline_batch, x0, y0, cmd.color);
                return;
            }

            std::array<Point, 64> points{};
            const int count = build_rounded_points(cmd.rect, cmd.radius, points.data(),
                                                   static_cast<int>(points.size()));
            if (count < 3) {
                return;
            }
            for (int i = 0; i < count; ++i) {
                const int next = (i + 1) % count;
                push_colored_vertex(outline_batch, points[static_cast<std::size_t>(i)][0],
                                    points[static_cast<std::size_t>(i)][1], cmd.color);
                push_colored_vertex(outline_batch, points[static_cast<std::size_t>(next)][0],
                                    points[static_cast<std::size_t>(next)][1], cmd.color);
            }
        };

        auto flush_filled_batch = [&]() {
            if (filled_batch.empty()) {
                return;
            }
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(2, GL_FLOAT, sizeof(BatchVertex), &filled_batch[0].x);
            glColorPointer(4, GL_FLOAT, sizeof(BatchVertex), &filled_batch[0].r);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(filled_batch.size()));
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            filled_batch.clear();
        };

        auto flush_outline_batch = [&]() {
            if (outline_batch.empty()) {
                return;
            }
            glLineWidth(std::max(1.0f, pending_outline_thickness));
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(2, GL_FLOAT, sizeof(BatchVertex), &outline_batch[0].x);
            glColorPointer(4, GL_FLOAT, sizeof(BatchVertex), &outline_batch[0].r);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(outline_batch.size()));
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glLineWidth(1.0f);
            outline_batch.clear();
        };

        auto flush_pending_batch = [&]() {
            if (pending_batch == PendingBatch::Filled) {
                flush_filled_batch();
            } else if (pending_batch == PendingBatch::Outline) {
                flush_outline_batch();
            }
            pending_batch = PendingBatch::None;
        };

        auto scissor_matches = [&](const IRect* desired) {
            if (desired == nullptr) {
                return !scissor_enabled;
            }
            if (!scissor_enabled) {
                return false;
            }
            return irect_equal(active_scissor, *desired);
        };

        for (std::size_t draw_idx : visible_indices_) {
            const DrawCommand& cmd = commands[draw_idx];
            if (clip_rect != nullptr && !rect_intersects(cmd.rect, *clip_rect)) {
                continue;
            }
            IRect cmd_gl_clip{};
            const IRect* desired_clip = nullptr;
            if (cmd.has_clip) {
                Rect effective_clip = cmd.clip_rect;
                if (has_outer_clip) {
                    Rect clipped{};
                    if (!rect_intersection(effective_clip, *clip_rect, clipped)) {
                        continue;
                    }
                    effective_clip = clipped;
                }
                cmd_gl_clip = to_gl_rect(effective_clip, width, height);
                if (cmd_gl_clip.w <= 0 || cmd_gl_clip.h <= 0) {
                    continue;
                }
                desired_clip = &cmd_gl_clip;
            } else if (has_outer_clip) {
                desired_clip = &outer_gl_clip;
            }

            switch (cmd.type) {
                case CommandType::FilledRect:
                    if (!scissor_matches(desired_clip)) {
                        flush_pending_batch();
                        apply_scissor(desired_clip);
                    }
                    if (pending_batch != PendingBatch::Filled) {
                        flush_pending_batch();
                        pending_batch = PendingBatch::Filled;
                    }
                    append_filled_rect(cmd);
                    break;
                case CommandType::RectOutline:
                    if (!scissor_matches(desired_clip)) {
                        flush_pending_batch();
                        apply_scissor(desired_clip);
                    }
                    if (pending_batch != PendingBatch::Outline ||
                        !float_eq(pending_outline_thickness, std::max(1.0f, cmd.thickness), 1e-3f)) {
                        flush_pending_batch();
                        pending_batch = PendingBatch::Outline;
                        pending_outline_thickness = std::max(1.0f, cmd.thickness);
                    }
                    append_outline_rect(cmd);
                    break;
                case CommandType::Text: {
                    flush_pending_batch();
                    apply_scissor(desired_clip);
                    std::string_view text{};
                    const std::size_t offset = static_cast<std::size_t>(cmd.text_offset);
                    const std::size_t length = static_cast<std::size_t>(cmd.text_length);
                    if (offset + length <= text_arena.size()) {
                        text = std::string_view(text_arena.data() + offset, length);
                    }

#ifdef _WIN32
                    if (!win32_font_renderer_.draw_text(cmd, text)) {
                        draw_text_bitmap(cmd, text);
                    }
#else
                    draw_text_bitmap(cmd, text);
#endif
                    break;
                }
            }
        }

        flush_pending_batch();
        apply_scissor(nullptr);
    }

#ifdef _WIN32
private:
    mutable Win32FontRenderer win32_font_renderer_;
#endif
private:
    mutable std::vector<std::vector<std::uint32_t>> spatial_buckets_{};
    mutable std::vector<std::uint32_t> spatial_marks_{};
    mutable std::vector<std::size_t> visible_indices_{};
    mutable std::uint32_t spatial_mark_id_{1u};
};

}  // namespace detail

template <typename BuildUiFn>
int run(BuildUiFn&& build_ui, const AppOptions& options = {}) {
#ifdef _WIN32
    // Best-effort: opt into system/per-monitor DPI awareness before creating the GLFW window.
    HMODULE user32 = GetModuleHandleW(L"user32.dll");
    if (user32 != nullptr) {
        using SetProcessDpiAwarenessContextFn = BOOL(WINAPI*)(HANDLE);
        auto set_context = reinterpret_cast<SetProcessDpiAwarenessContextFn>(
            GetProcAddress(user32, "SetProcessDpiAwarenessContext"));
        if (set_context != nullptr) {
            const HANDLE kPerMonitorAwareV2 = reinterpret_cast<HANDLE>(-4);
            const HANDLE kPerMonitorAware = reinterpret_cast<HANDLE>(-3);
            if (set_context(kPerMonitorAwareV2) == FALSE) {
                set_context(kPerMonitorAware);
            }
        } else {
            using SetProcessDPIAwareFn = BOOL(WINAPI*)();
            auto set_legacy = reinterpret_cast<SetProcessDPIAwareFn>(GetProcAddress(user32, "SetProcessDPIAware"));
            if (set_legacy != nullptr) {
                set_legacy();
            }
        }
    }
#endif

    if (glfwInit() == 0) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef GLFW_SCALE_TO_MONITOR
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(options.width, options.height, options.title, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(options.vsync ? 1 : 0);

    detail::RuntimeState runtime{};
    glfwSetWindowUserPointer(window, &runtime);
    glfwSetCharCallback(window, detail::text_input_callback);
    glfwSetScrollCallback(window, detail::scroll_callback);

    detail::Renderer renderer(options);
    Context ui;
    double previous_time = glfwGetTime();
    double next_frame_deadline = previous_time;
    bool redraw_needed = true;

    while (glfwWindowShouldClose(window) == 0) {
        if (options.continuous_render || redraw_needed) {
            if (options.max_fps > 0.0) {
                const double now_wait = glfwGetTime();
                const double wait_s = next_frame_deadline - now_wait;
                if (wait_s > 0.0005) {
                    glfwWaitEventsTimeout(wait_s);
                } else {
                    glfwPollEvents();
                }
            } else {
                glfwPollEvents();
            }
        } else {
            glfwWaitEventsTimeout(std::max(0.001, options.idle_wait_seconds));
        }

        int framebuffer_w = 1;
        int framebuffer_h = 1;
        glfwGetFramebufferSize(window, &framebuffer_w, &framebuffer_h);

        int window_w = 1;
        int window_h = 1;
        glfwGetWindowSize(window, &window_w, &window_h);

        double mouse_x = 0.0;
        double mouse_y = 0.0;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        const float framebuffer_scale_x =
            window_w > 0 ? static_cast<float>(framebuffer_w) / static_cast<float>(window_w) : 1.0f;
        const float framebuffer_scale_y =
            window_h > 0 ? static_cast<float>(framebuffer_h) / static_cast<float>(window_h) : 1.0f;

        float content_scale_x = 1.0f;
        float content_scale_y = 1.0f;
#if defined(GLFW_VERSION_MAJOR) && ((GLFW_VERSION_MAJOR > 3) || (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 3))
        glfwGetWindowContentScale(window, &content_scale_x, &content_scale_y);
#endif

        float window_dpi_scale = 1.0f;
#ifdef _WIN32
        HWND hwnd = glfwGetWin32Window(window);
        if (hwnd != nullptr) {
            HMODULE user32_module = GetModuleHandleW(L"user32.dll");
            if (user32_module != nullptr) {
                using GetDpiForWindowFn = UINT(WINAPI*)(HWND);
                auto get_dpi_for_window =
                    reinterpret_cast<GetDpiForWindowFn>(GetProcAddress(user32_module, "GetDpiForWindow"));
                if (get_dpi_for_window != nullptr) {
                    const UINT dpi = get_dpi_for_window(hwnd);
                    if (dpi > 0u) {
                        window_dpi_scale = static_cast<float>(dpi) / 96.0f;
                    }
                } else {
                    HDC dc = GetDC(hwnd);
                    if (dc != nullptr) {
                        const int dpi_x = GetDeviceCaps(dc, LOGPIXELSX);
                        if (dpi_x > 0) {
                            window_dpi_scale = static_cast<float>(dpi_x) / 96.0f;
                        }
                        ReleaseDC(hwnd, dc);
                    }
                }
            }
        }
#endif

        const float dpi_scale_x =
            std::max(framebuffer_scale_x, std::max(std::max(0.5f, content_scale_x), window_dpi_scale));
        const float dpi_scale_y =
            std::max(framebuffer_scale_y, std::max(std::max(0.5f, content_scale_y), window_dpi_scale));

        const bool left_mouse_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        const bool right_mouse_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        const bool backspace_down = glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
        const bool delete_down = glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS;
        const bool enter_down = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) ||
                                (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS);
        const bool escape_down = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        const bool left_down = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
        const bool right_down = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        const bool home_down = glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS;
        const bool end_down = glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS;
        const bool a_down = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        const bool c_down = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
        const bool v_down = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
        const bool x_down = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
        const bool left_shift_down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
        const bool right_shift_down = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        const bool ctrl_down = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
                               (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);

        InputState input{};
        input.mouse_x = static_cast<float>(mouse_x) * framebuffer_scale_x;
        input.mouse_y = static_cast<float>(mouse_y) * framebuffer_scale_y;
        input.mouse_wheel_y = static_cast<float>(runtime.scroll_y_accum);
        input.mouse_down = left_mouse_down;
        input.mouse_pressed = left_mouse_down && !runtime.prev_left_mouse;
        input.mouse_released = !left_mouse_down && runtime.prev_left_mouse;
        input.mouse_right_down = right_mouse_down;
        input.mouse_right_pressed = right_mouse_down && !runtime.prev_right_mouse;
        input.mouse_right_released = !right_mouse_down && runtime.prev_right_mouse;
        input.key_backspace = backspace_down && !runtime.prev_backspace;
        input.key_delete = delete_down && !runtime.prev_delete;
        input.key_enter = enter_down && !runtime.prev_enter;
        input.key_escape = escape_down && !runtime.prev_escape;
        input.key_left = left_down && !runtime.prev_left_key;
        input.key_right = right_down && !runtime.prev_right_key;
        input.key_home = home_down && !runtime.prev_home_key;
        input.key_end = end_down && !runtime.prev_end_key;
        input.key_shift = left_shift_down || right_shift_down;
        input.key_select_all = ctrl_down && a_down && !runtime.prev_a_key;
        input.key_copy = ctrl_down && c_down && !runtime.prev_c_key;
        input.key_cut = ctrl_down && x_down && !runtime.prev_x_key;
        input.key_paste = ctrl_down && v_down && !runtime.prev_v_key;
        input.text_input = runtime.text_input;
        if (input.key_paste) {
            const char* clipboard = glfwGetClipboardString(window);
            if (clipboard != nullptr) {
                input.clipboard_text = clipboard;
            } else {
                input.key_paste = false;
            }
        }

        const bool mouse_moved = !runtime.has_prev_mouse ||
                                 std::fabs(mouse_x - runtime.prev_mouse_x) > 0.5 ||
                                 std::fabs(mouse_y - runtime.prev_mouse_y) > 0.5;
        const bool framebuffer_changed =
            framebuffer_w != runtime.prev_framebuffer_w || framebuffer_h != runtime.prev_framebuffer_h;

        runtime.text_input.clear();
        runtime.scroll_y_accum = 0.0;
        runtime.prev_left_mouse = left_mouse_down;
        runtime.prev_right_mouse = right_mouse_down;
        runtime.prev_backspace = backspace_down;
        runtime.prev_delete = delete_down;
        runtime.prev_enter = enter_down;
        runtime.prev_escape = escape_down;
        runtime.prev_left_key = left_down;
        runtime.prev_right_key = right_down;
        runtime.prev_home_key = home_down;
        runtime.prev_end_key = end_down;
        runtime.prev_a_key = a_down;
        runtime.prev_c_key = c_down;
        runtime.prev_v_key = v_down;
        runtime.prev_x_key = x_down;
        runtime.prev_left_shift = left_shift_down;
        runtime.prev_right_shift = right_shift_down;
        runtime.prev_mouse_x = mouse_x;
        runtime.prev_mouse_y = mouse_y;
        runtime.has_prev_mouse = true;
        runtime.prev_framebuffer_w = framebuffer_w;
        runtime.prev_framebuffer_h = framebuffer_h;

        const double now = glfwGetTime();
        const float dt = static_cast<float>(now - previous_time);
        previous_time = now;
        input.time_seconds = now;

        const bool input_event =
            input.mouse_pressed || input.mouse_released || input.mouse_right_pressed ||
            input.mouse_right_released || input.key_backspace || input.key_delete || input.key_enter ||
            input.key_escape || input.key_left || input.key_right || input.key_home || input.key_end ||
            input.key_select_all || input.key_copy || input.key_cut || input.key_paste ||
            std::fabs(input.mouse_wheel_y) > 1e-6f ||
            !input.text_input.empty();
        const bool render_this_frame =
            options.continuous_render || redraw_needed || framebuffer_changed || mouse_moved || input_event;
        if (!render_this_frame) {
            continue;
        }

        ui.begin_frame(static_cast<float>(framebuffer_w), static_cast<float>(framebuffer_h), input);
        bool request_next_frame = false;
        const float dpi_scale = std::max(1.0f, std::min(dpi_scale_x, dpi_scale_y));
        FrameContext frame_ctx{
            ui,
            window,
            dt,
            framebuffer_w,
            framebuffer_h,
            window_w,
            window_h,
            dpi_scale_x,
            dpi_scale_y,
            dpi_scale,
            &request_next_frame,
        };
        build_ui(frame_ctx);
        if (ui.consume_repaint_request()) {
            request_next_frame = true;
        }
        ui.take_frame(runtime.curr_commands, runtime.curr_text_arena);
        std::string clipboard_write_text;
        if (ui.consume_clipboard_write(clipboard_write_text)) {
            glfwSetClipboardString(window, clipboard_write_text.c_str());
        }
        redraw_needed = request_next_frame;

        const Color bg = ui.theme().background;
        const auto& commands = runtime.curr_commands;
        const auto& text_arena = runtime.curr_text_arena;
        detail::ensure_cache_texture(runtime, framebuffer_w, framebuffer_h);
        const std::uint64_t frame_hash = detail::hash_frame_payload(commands, bg);

        const bool force_full_redraw =
            framebuffer_changed || !runtime.has_prev_frame || !runtime.has_cache;
        bool has_dirty = false;
        runtime.dirty_regions.clear();
        if (!force_full_redraw && runtime.has_prev_frame && runtime.prev_frame_hash == frame_hash) {
            has_dirty = false;
        } else {
            has_dirty =
                detail::compute_dirty_regions(commands, text_arena, runtime, bg, framebuffer_w, framebuffer_h,
                                              force_full_redraw, runtime.dirty_regions);
        }

        if (!force_full_redraw && !has_dirty) {
            runtime.prev_commands.swap(runtime.curr_commands);
            runtime.prev_text_arena.swap(runtime.curr_text_arena);
            runtime.prev_bg = bg;
            runtime.prev_frame_hash = frame_hash;
            runtime.has_prev_frame = true;
            if (options.max_fps > 0.0) {
                const double frame_interval = 1.0 / options.max_fps;
                next_frame_deadline = glfwGetTime() + frame_interval;
            }
            continue;
        }

        if (force_full_redraw) {
            glDisable(GL_SCISSOR_TEST);
            glClearColor(bg.r, bg.g, bg.b, bg.a);
            glClear(GL_COLOR_BUFFER_BIT);
            renderer.render(commands, text_arena, framebuffer_w, framebuffer_h, nullptr);
            detail::copy_full_to_cache(runtime, framebuffer_w, framebuffer_h);
        } else {
            detail::draw_cache_texture(runtime, framebuffer_w, framebuffer_h);
            for (const Rect& dirty : runtime.dirty_regions) {
                const detail::IRect gl_dirty = detail::to_gl_rect(dirty, framebuffer_w, framebuffer_h);
                if (gl_dirty.w > 0 && gl_dirty.h > 0) {
                    glEnable(GL_SCISSOR_TEST);
                    glScissor(gl_dirty.x, gl_dirty.y, gl_dirty.w, gl_dirty.h);
                    glClearColor(bg.r, bg.g, bg.b, bg.a);
                    glClear(GL_COLOR_BUFFER_BIT);
                    renderer.render(commands, text_arena, framebuffer_w, framebuffer_h, &dirty);
                    detail::copy_region_to_cache(runtime, gl_dirty);
                }
            }
            glDisable(GL_SCISSOR_TEST);
        }

        runtime.prev_commands.swap(runtime.curr_commands);
        runtime.prev_text_arena.swap(runtime.curr_text_arena);
        runtime.prev_bg = bg;
        runtime.prev_frame_hash = frame_hash;
        runtime.has_prev_frame = true;
        glfwSwapBuffers(window);

        if (options.max_fps > 0.0) {
            const double frame_interval = 1.0 / options.max_fps;
            next_frame_deadline = glfwGetTime() + frame_interval;
        }
    }

    renderer.release_gl_resources();
    if (runtime.cache_texture != 0u) {
        glDeleteTextures(1, &runtime.cache_texture);
        runtime.cache_texture = 0u;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

}  // namespace demo

#endif  // EUI_ENABLE_GLFW_OPENGL_BACKEND

}  // namespace eui

#endif  // EUI_H_
