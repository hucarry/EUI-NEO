#ifndef EUI_ENABLE_GLFW_OPENGL_BACKEND
#define EUI_ENABLE_GLFW_OPENGL_BACKEND 1
#endif
#include "EUI.h"

#include <algorithm>
#include <array>
#include <string>

enum class SidebarPage {
    Overview,
    Library,
    Inbox,
    Settings,
};

struct SidebarDemoState {
    bool dark_mode{true};
    SidebarPage active_page{SidebarPage::Overview};
    float page_anim{1.0f};
    float sync_progress{0.72f};
    float release_progress{0.58f};
    float inbox_progress{0.34f};
};

static bool draw_sidebar_entry(eui::Context& ui, SidebarDemoState& state, SidebarPage page, const char* label,
                               float height) {
    std::string button_label;
    button_label.reserve(std::char_traits<char>::length(label) + 1u);
    button_label.push_back('\t');
    button_label += label;
    const bool selected = state.active_page == page;
    if (ui.button(button_label, selected ? eui::ButtonStyle::Primary : eui::ButtonStyle::Ghost, height)) {
        if (!selected) {
            state.active_page = page;
            state.page_anim = 0.0f;
        }
        return true;
    }
    return false;
}

static void draw_metric_card(eui::Context& ui, float scale, const char* title, const char* value, const char* note) {
    const auto dp = [scale](float v) { return v * scale; };
    ui.begin_card("", dp(118.0f), dp(10.0f));
    ui.label(title, dp(11.0f), true);
    ui.label(value, dp(22.0f), false);
    ui.label(note, dp(12.0f), true);
    ui.end_card();
}

static void draw_overview_page(eui::Context& ui, SidebarDemoState& state, float scale) {
    const auto dp = [scale](float v) { return v * scale; };
    ui.begin_card("OVERVIEW", 0.0f, dp(12.0f));
    ui.label("A minimal sidebar + content layout with page entry animation.", dp(12.0f), true);
    ui.spacer(dp(6.0f));
    ui.begin_row(3, dp(8.0f));
    draw_metric_card(ui, scale, "Workspace Sync", "72%", "Assets ready for review");
    draw_metric_card(ui, scale, "Release Readiness", "58%", "Docs and QA still pending");
    draw_metric_card(ui, scale, "Live Inbox", "14", "New notes since this morning");
    ui.end_row();
    ui.spacer(dp(8.0f));
    ui.progress("Workspace Sync", state.sync_progress, dp(8.0f));
    ui.progress("Release Readiness", state.release_progress, dp(8.0f));
    ui.progress("Inbox Processing", state.inbox_progress, dp(8.0f));
    ui.end_card();

    ui.begin_card("TODAY", 0.0f, dp(12.0f));
    ui.text_area_readonly(
        "Focus",
        "Refine motion curves for navigation.\n"
        "Polish sidebar spacing and hierarchy.\n"
        "Check icon and text weight balance.\n"
        "Keep transitions short and stable.",
        dp(150.0f));
    ui.end_card();
}

static void draw_library_page(eui::Context& ui, SidebarDemoState&, float scale) {
    const auto dp = [scale](float v) { return v * scale; };
    ui.begin_card("LIBRARY", 0.0f, dp(12.0f));
    ui.label("A content page can stay mostly static while the panel itself animates in.", dp(12.0f), true);
    ui.input_readonly("Current Collection", "Brand Motion Kit", dp(34.0f), false, 1.0f, false);
    ui.input_readonly("Latest Export", "sidebar-transition-v4.fig", dp(34.0f), false, 1.0f, false);
    ui.progress("Asset Coverage", 0.83f, dp(8.0f));
    ui.end_card();

    ui.begin_card("PINNED NOTES", 0.0f, dp(12.0f));
    ui.text_area_readonly(
        "Notes",
        "Use soft reveal on entry.\n"
        "Avoid hover zoom on navigation items.\n"
        "Keep the right page readable even during motion.\n"
        "Prefer subtle vertical drift over large transforms.",
        dp(170.0f));
    ui.end_card();
}

static void draw_inbox_page(eui::Context& ui, SidebarDemoState&, float scale) {
    const auto dp = [scale](float v) { return v * scale; };
    ui.begin_card("INBOX", 0.0f, dp(12.0f));
    ui.label("Message stack with compact density.", dp(12.0f), true);
    ui.input_readonly("08:42", "Design review moved to 10:30", dp(34.0f), false, 1.0f, false);
    ui.input_readonly("09:15", "Sidebar motion pass approved", dp(34.0f), false, 1.0f, false);
    ui.input_readonly("09:50", "Need final QA build after polish", dp(34.0f), false, 1.0f, false);
    ui.progress("Unread Cleared", 0.46f, dp(8.0f));
    ui.end_card();

    ui.begin_card("QUEUE", 0.0f, dp(12.0f));
    ui.label("Keep timing short so page switching feels immediate.", dp(12.0f), true);
    ui.begin_row(2, dp(8.0f));
    draw_metric_card(ui, scale, "Priority", "05", "Requires response");
    draw_metric_card(ui, scale, "Resolved", "19", "Closed today");
    ui.end_row();
    ui.end_card();
}

static void draw_settings_page(eui::Context& ui, SidebarDemoState& state, float scale) {
    const auto dp = [scale](float v) { return v * scale; };
    ui.begin_card("SETTINGS", 0.0f, dp(12.0f));
    ui.input_readonly("Theme", state.dark_mode ? "Dark" : "Light", dp(34.0f), false, 1.0f, false);
    ui.input_readonly("Navigation Motion", "Fade + slight upward drift", dp(34.0f), false, 1.0f, false);
    ui.input_readonly("Hover Behavior", "No scale, color only", dp(34.0f), false, 1.0f, false);
    ui.progress("Visual Polish", 0.91f, dp(8.0f));
    ui.end_card();

    ui.begin_card("GUIDELINES", 0.0f, dp(12.0f));
    ui.text_area_readonly(
        "Rules",
        "Keep hover feedback readable but stable.\n"
        "Use press motion sparingly.\n"
        "Animate container reveal, not every pixel.\n"
        "Let the content settle quickly after the switch.",
        dp(150.0f));
    ui.end_card();
}

int main() {
    SidebarDemoState state{};
    eui::demo::AppOptions options{};
    options.title = "EUI Sidebar Navigation Demo";
    options.width = 1080;
    options.height = 720;
    options.vsync = true;
    options.continuous_render = false;
    options.max_fps = 120.0;

    return eui::demo::run(
        [&](eui::demo::FrameContext frame) {
            auto& ui = frame.ui;
            const float scale = std::max(1.0f, frame.dpi_scale);
            const auto dp = [scale](float v) { return v * scale; };

            ui.set_theme_mode(state.dark_mode ? eui::ThemeMode::Dark : eui::ThemeMode::Light);
            ui.set_primary_color(eui::rgba(0.30f, 0.74f, 0.77f, 1.0f));
            ui.set_corner_radius(12.0f * scale);

            const float margin = dp(18.0f);
            const float gap = dp(14.0f);
            const float top = dp(18.0f);
            const float available_h = std::max(dp(520.0f), static_cast<float>(frame.framebuffer_h) - top - margin);
            const float sidebar_w = dp(220.0f);
            const float content_x = margin + sidebar_w + gap;
            const float content_w =
                std::max(dp(320.0f), static_cast<float>(frame.framebuffer_w) - content_x - margin);

            ui.begin_panel("", margin, top, sidebar_w, 0.0f, -1.0f);
            ui.begin_card("", available_h, dp(12.0f));
            ui.label("EUI NAV", dp(25.0f), false);
            ui.label("Minimal sidebar and page switch sample", dp(12.0f), true);
            ui.spacer(dp(10.0f));
            ui.label("PAGES", dp(11.0f), true);
            draw_sidebar_entry(ui, state, SidebarPage::Overview, u8"\uF015  Overview", dp(34.0f));
            draw_sidebar_entry(ui, state, SidebarPage::Library, u8"\uF07B  Library", dp(34.0f));
            draw_sidebar_entry(ui, state, SidebarPage::Inbox, u8"\uF0E0  Inbox", dp(34.0f));
            draw_sidebar_entry(ui, state, SidebarPage::Settings, u8"\uF013  Settings", dp(34.0f));
            ui.spacer(dp(10.0f));
            ui.progress("Sync", state.sync_progress, dp(6.0f));
            ui.spacer(dp(8.0f));
            if (ui.button(state.dark_mode ? "Light Theme" : "Dark Theme", eui::ButtonStyle::Secondary, dp(34.0f))) {
                state.dark_mode = !state.dark_mode;
            }
            ui.end_card();
            ui.end_panel();

            const float reveal = state.page_anim * state.page_anim * (3.0f - 2.0f * state.page_anim);
            const float page_alpha = std::clamp(reveal, 0.0f, 1.0f);
            const float page_offset_y = (1.0f - reveal) * dp(12.0f);

            ui.set_global_alpha(page_alpha);
            ui.begin_panel("", content_x, top + page_offset_y, content_w, 0.0f, -1.0f);
            switch (state.active_page) {
            case SidebarPage::Overview:
                draw_overview_page(ui, state, scale);
                break;
            case SidebarPage::Library:
                draw_library_page(ui, state, scale);
                break;
            case SidebarPage::Inbox:
                draw_inbox_page(ui, state, scale);
                break;
            case SidebarPage::Settings:
                draw_settings_page(ui, state, scale);
                break;
            }
            ui.end_panel();
            ui.set_global_alpha(1.0f);

            if (state.page_anim < 1.0f) {
                state.page_anim = std::min(1.0f, state.page_anim + frame.dt * 2.4f);
                frame.request_next_frame();
            }
        },
        options);
}

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return main();
}
#endif
