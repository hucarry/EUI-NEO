#ifndef EUI_ENABLE_GLFW_OPENGL_BACKEND
#define EUI_ENABLE_GLFW_OPENGL_BACKEND 1
#endif
#include "EUI.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>

#if defined(_WIN32)
#include <shellapi.h>
#endif

// Global controls for quick tuning.
const char* g_window_title = "EUI Demo";
int g_window_width = 960;
int g_window_height = 710;
float g_content_margin = 20.0f;
float g_content_top = 18.0f;
const char* g_text_font_family = "Segoe UI";
const char* g_text_font_file = nullptr;
const char* g_icon_font_family = "Segoe MDL2 Assets";
const char* g_icon_font_file = nullptr;

eui::Color g_theme_primary = eui::rgba(0.282f, 0.827f, 0.835f, 1.0f);
float g_theme_radius = 10.0f;
bool g_vsync = true;
bool g_animate_progress = false;
double g_max_fps = 240.0;

struct DemoState {
    bool dark_mode{true};
    int active_tab{0};
    int dropdown_item{0};
    bool react_selected{true};
    bool vue_selected{false};
    bool svelte_selected{true};
    bool advanced_open{true};
    bool debug_mode{true};

    float opacity{65.0f};
    float exposure{128.0f};
    float log_level{5.0f};
    float gamma{2.20f};
    float loading_ratio{0.45f};
};

std::string g_story_text =
    u8"\uE713 \uE8A5 \uE80F  icon row | \U0001F44F \U0001F31F \U0001F642 \U0001F680 \u2728\n"
    u8"tiny hands clap \U0001F44F\U0001F44F, smiles shine \U0001F31F\U0001F31F, hearts glow \u2665 and vibes stay warm.\n"
    u8"toolbox line: \U0001F527 \U0001F6E0 \U0001F4CC \U0001F4DD \U0001F4AC \u2705 \U0001F3AF \U0001F525\n"
    u8"mail + sparkles: \u2709 \u2728 and everyone says hi \U0001F44B with bright eyes \U0001F60A.\n"
    u8"design notes: keep rhythm, keep contrast, keep spacing, keep corners clean.\n"
    u8"interaction notes: smooth hover, stable click, readable text, clear hierarchy.\n"
    u8"team notes: small progress every day, one bug less, one smile more \U0001F31F.\n"
    u8"cute line: little paws tap tap, little hands clap clap \U0001F44F\U0001F44F, everybody shines.\n"
    u8"status line: build \U0001F680 test \u2705 review \U0001F4DD ship \U0001F389 repeat.\n"
    u8"bonus icons: \u2600 \u2601 \u2615 \u2699 \u2764 \U0001F525 \U0001F44D \U0001F680 \U0001F31F.\n"
    u8"long text keeps wrapping at the edge and you can scroll to read more lines.\n"
    u8"have a great day, keep coding, and let every screen feel friendly \U0001F60A";

static void open_external_url(const char* url) {
    if (url == nullptr || url[0] == '\0') {
        return;
    }
#if defined(_WIN32)
    ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    const std::string command = "open \"" + std::string(url) + "\"";
    std::system(command.c_str());
#else
    const std::string command = "xdg-open \"" + std::string(url) + "\" >/dev/null 2>&1";
    std::system(command.c_str());
#endif
}

int main() {
    DemoState state{};
    eui::demo::AppOptions options{};
    options.title = g_window_title;
    options.width = g_window_width;
    options.height = g_window_height;
    options.vsync = g_vsync;
    options.continuous_render = false;
    options.max_fps = g_max_fps;
    options.text_font_family = g_text_font_family;
    options.text_font_file = g_text_font_file;
    options.icon_font_family = g_icon_font_family;
    options.icon_font_file = g_icon_font_file;
    options.enable_icon_font_fallback = true;

    return eui::demo::run(
        [&](eui::demo::FrameContext frame) {
            auto& ui = frame.ui;
            const float ui_scale = std::max(1.0f, frame.dpi_scale);
            const auto dp = [ui_scale](float value) { return value * ui_scale; };

            if (g_animate_progress) {
                state.loading_ratio += frame.dt * 0.20f;
                if (state.loading_ratio > 1.0f) {
                    state.loading_ratio = 0.0f;
                }
                frame.request_next_frame();
            }

            ui.set_theme_mode(state.dark_mode ? eui::ThemeMode::Dark : eui::ThemeMode::Light);
            ui.set_primary_color(g_theme_primary);
            ui.set_corner_radius(g_theme_radius * ui_scale);

            const float content_margin = std::max(0.0f, g_content_margin) * ui_scale;
            const float content_width =
                std::max(dp(240.0f), static_cast<float>(frame.framebuffer_w) - content_margin * 2.0f);
            ui.begin_panel("", content_margin, g_content_top * ui_scale, content_width, 0.0f, -1.0f);

            ui.begin_waterfall(2, dp(8.0f));

            ui.begin_card("OVERVIEW", 0.0f, dp(8.0f));
            ui.label("EUI SYSTEM", dp(22.0f), false);
            char dpi_text[40];
            std::snprintf(dpi_text, sizeof(dpi_text), "DPI %.2fx", ui_scale);
            ui.begin_row(7, dp(6.0f));
            ui.label(dpi_text, dp(12.0f), true);
            ui.label("", dp(12.0f), true);
            ui.label("", dp(12.0f), true);
            ui.label("", dp(12.0f), true);
            ui.label("", dp(12.0f), true);
            ui.label("", dp(12.0f), true);
            const char* theme_icon = state.dark_mode ? u8"\u2600" : u8"\u263D";
            if (ui.button(theme_icon, eui::ButtonStyle::Secondary, dp(24.0f))) {
                state.dark_mode = !state.dark_mode;
            }
            ui.end_row();
            ui.end_card();

            ui.begin_card("CONTACT");
            ui.begin_row(2, dp(8.0f));
            if (ui.button("@sudoevolve", eui::ButtonStyle::Secondary, dp(32.0f))) {
                open_external_url("https://github.com/sudoevolve");
            }
            ui.input_readonly("", "sudoevolve@gmail.com", dp(32.0f));
            ui.end_row();
            ui.end_card();

            ui.begin_card("ICON DEMO", dp(200.0f));
            ui.label("SYSTEM ICONS 2 x 4", dp(11.0f), true);
            ui.begin_row(2, dp(8.0f));
            ui.button(u8"\uE700  MENU", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.button(u8"\uE710  BACK", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.end_row();
            ui.begin_row(2, dp(8.0f));
            ui.button(u8"\uE711  FORWARD", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.button(u8"\uE721  SEARCH", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.end_row();
            ui.begin_row(2, dp(8.0f));
            ui.button(u8"\uE713  SETTINGS", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.button(u8"\uE8A5  FAVORITE", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.end_row();
            ui.begin_row(2, dp(8.0f));
            ui.button(u8"\uE80F  MAIL", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.button(u8"\uE8BB  CONTACT", eui::ButtonStyle::Secondary, dp(30.0f));
            ui.end_row();
            ui.end_card();

            ui.begin_card("TEXT EDITOR");
            ui.text_area("STORY", g_story_text, dp(140.0f));
            ui.end_card();

            ui.end_waterfall();
            ui.spacer(dp(2.0f));

            ui.begin_waterfall(2, dp(8.0f));
            ui.begin_card("TAB SELECTION");
            ui.label("SINGLE SELECT (RADIO)", dp(11.0f), true);
            ui.begin_row(3, dp(6.0f));
            if (ui.tab("EFFICIENCY", state.active_tab == 0, dp(30.0f))) {
                state.active_tab = 0;
            }
            if (ui.tab("QUALITY", state.active_tab == 1, dp(30.0f))) {
                state.active_tab = 1;
            }
            if (ui.tab("SPEED", state.active_tab == 2, dp(30.0f))) {
                state.active_tab = 2;
            }
            ui.end_row();
            ui.spacer(dp(6.0f));
            ui.label("MULTI SELECT (CHECKBOX)", dp(11.0f), true);
            ui.begin_row(3, dp(6.0f));
            if (ui.tab("REACT", state.react_selected, dp(30.0f))) {
                state.react_selected = !state.react_selected;
            }
            if (ui.tab("VUE", state.vue_selected, dp(30.0f))) {
                state.vue_selected = !state.vue_selected;
            }
            if (ui.tab("SVELTE", state.svelte_selected, dp(30.0f))) {
                state.svelte_selected = !state.svelte_selected;
            }
            ui.end_row();
            ui.end_card();
            ui.begin_card("SLIDERS");
            ui.label("RIGHT-CLICK VALUE TO EDIT DIRECTLY", dp(11.0f), true);
            ui.slider_float("OPACITY", state.opacity, 0.0f, 100.0f, 0, dp(40.0f));
            ui.slider_float("EXPOSURE", state.exposure, 0.0f, 255.0f, 0, dp(40.0f));
            ui.end_card();
            ui.begin_card("BUTTONS");
            ui.begin_row(3, dp(8.0f));
            ui.button("PRIMARY ACTION", eui::ButtonStyle::Primary, dp(34.0f));
            ui.button("SECONDARY", eui::ButtonStyle::Secondary, dp(34.0f));
            ui.button("GHOST", eui::ButtonStyle::Ghost, dp(34.0f));
            ui.end_row();
            ui.end_card();
            ui.begin_card("DROPDOWN PANEL");
            if (ui.begin_dropdown("ADVANCED SETTINGS", state.advanced_open, 0.0f, dp(10.0f))) {
                if (ui.begin_scroll_area("ADVANCED_CARD_LIST", dp(130.0f))) {
                    static const char* kRows[7] = {
                        "CARD 01  Renderer Budget",
                        "CARD 02  Runtime Trace",
                        "CARD 03  Font Atlas",
                        "CARD 04  Input Capture",
                        "CARD 05  Async Upload",
                        "CARD 06  VSync Profile",
                        "CARD 07  Hot Reload",
                    };
                    for (int i = 0; i < 7; ++i) {
                        const eui::ButtonStyle style =
                            (state.dropdown_item == i) ? eui::ButtonStyle::Primary : eui::ButtonStyle::Secondary;
                        if (ui.button(kRows[i], style, dp(34.0f))) {
                            state.dropdown_item = i;
                        }
                    }
                    ui.end_scroll_area();
                }
                ui.end_dropdown();
            }
            ui.end_card();
            ui.begin_card("INPUT + PROGRESS");
            ui.input_float("GAMMA", state.gamma, 0.10f, 4.00f, 2, dp(34.0f));
            ui.spacer(dp(8.0f));
            const char* play_icon = g_animate_progress ? u8"\u23F8" : u8"\u25B6";
            if (ui.button(play_icon, eui::ButtonStyle::Secondary, dp(28.0f))) {
                g_animate_progress = !g_animate_progress;
            }
            ui.spacer(dp(6.0f));
            ui.progress("LOADING ASSETS...", state.loading_ratio, dp(8.0f));
            ui.end_card();
            ui.end_waterfall();

            ui.end_panel();
        },
        options);
}

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}
#endif
