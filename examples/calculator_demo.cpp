#ifndef EUI_ENABLE_GLFW_OPENGL_BACKEND
#define EUI_ENABLE_GLFW_OPENGL_BACKEND 1
#endif
#include "EUI.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

struct CalculatorState {
    std::string history{"0"};
    std::string display{"0"};
    double accumulator{0.0};
    bool has_accumulator{false};
    char pending_op{0};
    bool reset_input{false};
    bool history_locked{false};
    bool dark_mode{false};
};

static const char* calc_op_text(char op) {
    switch (op) {
        case '+':
            return "+";
        case '-':
            return "-";
        case '*':
            return u8"\u00D7";
        case '/':
            return u8"\u00F7";
        default:
            return "";
    }
}

static std::string calc_format(double value) {
    if (!std::isfinite(value)) {
        return "Error";
    }
    if (std::fabs(value) < 1e-12) {
        value = 0.0;
    }
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%.12g", value);
    return std::string(buffer);
}

static double calc_parse(const std::string& text) {
    if (text == "Error") {
        return 0.0;
    }
    try {
        const double parsed = std::stod(text);
        if (std::isfinite(parsed)) {
            return parsed;
        }
    } catch (...) {
    }
    return 0.0;
}

static bool calc_apply(double& lhs, double rhs, char op) {
    switch (op) {
        case '+':
            lhs += rhs;
            return true;
        case '-':
            lhs -= rhs;
            return true;
        case '*':
            lhs *= rhs;
            return true;
        case '/':
            if (std::fabs(rhs) < 1e-12) {
                return false;
            }
            lhs /= rhs;
            return true;
        default:
            return true;
    }
}

static void calc_clear(CalculatorState& state) {
    state.history = "0";
    state.display = "0";
    state.accumulator = 0.0;
    state.has_accumulator = false;
    state.pending_op = 0;
    state.reset_input = false;
    state.history_locked = false;
}

int main() {
    CalculatorState state{};
    eui::demo::AppOptions options{};
    options.title = "EUI Calculator Demo";
    options.width = 430;
    options.height = 600;
    options.vsync = true;
    options.continuous_render = false;
    options.max_fps = 120.0;

    return eui::demo::run(
        [&](eui::demo::FrameContext frame) {
            auto& ui = frame.ui;
            const float ui_scale = std::max(1.0f, frame.dpi_scale);
            const auto dp = [ui_scale](float value) { return value * ui_scale; };

            auto set_error = [&]() {
                state.history = "Error";
                state.display = "Error";
                state.accumulator = 0.0;
                state.has_accumulator = false;
                state.pending_op = 0;
                state.reset_input = true;
                state.history_locked = false;
            };

            auto sync_history = [&]() {
                if (state.display == "Error") {
                    state.history = "Error";
                    return;
                }
                if (state.pending_op != 0 && state.has_accumulator) {
                    const std::string lhs = calc_format(state.accumulator);
                    const std::string op = calc_op_text(state.pending_op);
                    if (state.reset_input) {
                        state.history = lhs + " " + op;
                    } else {
                        state.history = lhs + " " + op + " " + state.display;
                    }
                    return;
                }
                if (!state.history_locked) {
                    state.history = state.display;
                }
            };

            auto input_digit = [&](char ch) {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                if (state.history_locked) {
                    state.history_locked = false;
                }
                if (state.reset_input) {
                    state.display = (ch == '.') ? "0." : std::string(1, ch);
                    state.reset_input = false;
                    if (state.pending_op == 0) {
                        state.has_accumulator = false;
                    }
                    sync_history();
                    return;
                }
                if (ch == '.') {
                    if (state.display.find('.') == std::string::npos) {
                        state.display.push_back('.');
                    }
                    sync_history();
                    return;
                }
                if (state.display == "0") {
                    state.display = std::string(1, ch);
                } else if (state.display.size() < 18u) {
                    state.display.push_back(ch);
                }
                sync_history();
            };

            auto input_operator = [&](char op) {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                state.history_locked = false;
                const double current = calc_parse(state.display);
                if (!state.has_accumulator) {
                    state.accumulator = current;
                    state.has_accumulator = true;
                } else if (state.pending_op != 0 && !state.reset_input) {
                    if (!calc_apply(state.accumulator, current, state.pending_op)) {
                        set_error();
                        return;
                    }
                    state.display = calc_format(state.accumulator);
                }
                state.pending_op = op;
                state.reset_input = true;
                sync_history();
            };

            auto eval_equal = [&]() {
                if (state.pending_op == 0) {
                    return;
                }
                const double lhs_before = state.accumulator;
                const char op_before = state.pending_op;
                double rhs = calc_parse(state.display);
                if (state.reset_input && state.has_accumulator) {
                    rhs = state.accumulator;
                }
                if (!state.has_accumulator) {
                    state.accumulator = rhs;
                    state.has_accumulator = true;
                }
                if (!calc_apply(state.accumulator, rhs, state.pending_op)) {
                    set_error();
                    return;
                }
                state.display = calc_format(state.accumulator);
                state.history = calc_format(lhs_before) + " " + calc_op_text(op_before) + " " +
                                calc_format(rhs) + " =";
                state.pending_op = 0;
                state.has_accumulator = false;
                state.reset_input = true;
                state.history_locked = true;
            };

            auto clear_entry = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                    return;
                }
                state.display = "0";
                state.reset_input = false;
                state.history_locked = false;
                sync_history();
            };

            auto input_percent = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                state.display = calc_format(calc_parse(state.display) * 0.01);
                state.reset_input = false;
                state.history_locked = false;
                sync_history();
            };

            auto input_inverse = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                const double value = calc_parse(state.display);
                if (std::fabs(value) < 1e-12) {
                    set_error();
                    return;
                }
                state.display = calc_format(1.0 / value);
                state.reset_input = false;
                state.history_locked = false;
                sync_history();
            };

            auto input_square = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                const double value = calc_parse(state.display);
                state.display = calc_format(value * value);
                state.reset_input = false;
                state.history_locked = false;
                sync_history();
            };

            auto input_sqrt = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                }
                const double value = calc_parse(state.display);
                if (value < 0.0) {
                    set_error();
                    return;
                }
                state.display = calc_format(std::sqrt(value));
                state.reset_input = false;
                state.history_locked = false;
                sync_history();
            };

            auto backspace = [&]() {
                if (state.display == "Error") {
                    calc_clear(state);
                    return;
                }
                if (state.reset_input) {
                    state.display = "0";
                    state.reset_input = false;
                    state.history_locked = false;
                    sync_history();
                    return;
                }
                if (state.display.size() <= 1u) {
                    state.display = "0";
                    state.history_locked = false;
                    sync_history();
                    return;
                }
                state.display.pop_back();
                if (state.display.empty() || state.display == "-") {
                    state.display = "0";
                }
                state.history_locked = false;
                sync_history();
            };

            ui.set_theme_mode(state.dark_mode ? eui::ThemeMode::Dark : eui::ThemeMode::Light);
            ui.set_primary_color(eui::rgba(0.30f, 0.74f, 0.77f, 1.0f));
            ui.set_corner_radius(14.0f * ui_scale);

            const float margin = dp(18.0f);
            const float content_width =
                std::max(dp(240.0f), static_cast<float>(frame.framebuffer_w) - margin * 2.0f);
            ui.begin_panel("", margin, dp(14.0f), content_width, 0.0f, -1.0f);

            ui.begin_card("CALCULATOR", 0.0f, dp(10.0f));
            ui.input_readonly("", state.history, dp(34.0f), true, 0.95f, true);
            ui.input_readonly("", state.display, dp(88.0f), true, 1.35f, false);
            ui.spacer(dp(8.0f));

            const float key_h = dp(62.0f);
            const float key_gap = dp(8.0f);

            ui.begin_row(4, key_gap);
            if (ui.button("%", eui::ButtonStyle::Secondary, key_h)) {
                input_percent();
            }
            if (ui.button("CE", eui::ButtonStyle::Secondary, key_h)) {
                clear_entry();
            }
            if (ui.button("C", eui::ButtonStyle::Secondary, key_h)) {
                calc_clear(state);
            }
            if (ui.button(u8"\u232B", eui::ButtonStyle::Secondary, key_h)) {
                backspace();
            }
            ui.end_row();

            ui.begin_row(4, key_gap);
            if (ui.button("1/x", eui::ButtonStyle::Secondary, key_h)) {
                input_inverse();
            }
            if (ui.button(u8"x\u00B2", eui::ButtonStyle::Secondary, key_h)) {
                input_square();
            }
            if (ui.button(u8"\u00B2\u221Ax", eui::ButtonStyle::Secondary, key_h)) {
                input_sqrt();
            }
            if (ui.button(u8"\u00F7", eui::ButtonStyle::Secondary, key_h)) {
                input_operator('/');
            }
            ui.end_row();

            ui.begin_row(4, key_gap);
            if (ui.button("7", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('7');
            }
            if (ui.button("8", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('8');
            }
            if (ui.button("9", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('9');
            }
            if (ui.button(u8"\u00D7", eui::ButtonStyle::Secondary, key_h)) {
                input_operator('*');
            }
            ui.end_row();

            ui.begin_row(4, key_gap);
            if (ui.button("4", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('4');
            }
            if (ui.button("5", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('5');
            }
            if (ui.button("6", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('6');
            }
            if (ui.button("-", eui::ButtonStyle::Secondary, key_h)) {
                input_operator('-');
            }
            ui.end_row();

            ui.begin_row(4, key_gap);
            if (ui.button("1", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('1');
            }
            if (ui.button("2", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('2');
            }
            if (ui.button("3", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('3');
            }
            if (ui.button("+", eui::ButtonStyle::Secondary, key_h)) {
                input_operator('+');
            }
            ui.end_row();

            ui.begin_row(4, key_gap);
            if (ui.button(state.dark_mode ? u8"\u2600" : u8"\u263D", eui::ButtonStyle::Secondary, key_h)) {
                state.dark_mode = !state.dark_mode;
            }
            if (ui.button("0", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('0');
            }
            if (ui.button(".", eui::ButtonStyle::Secondary, key_h)) {
                input_digit('.');
            }
            if (ui.button("=", eui::ButtonStyle::Primary, key_h)) {
                eval_equal();
            }
            ui.end_row();

            ui.end_card();
            ui.end_panel();
        },
        options);
}

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}
#endif
