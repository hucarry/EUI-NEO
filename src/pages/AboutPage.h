#pragma once

#include "../EUINEO.h"
#include "../ui/UIContext.h"
#include "../ui/ThemeTokens.h"
#include <algorithm>
#include <string>

namespace EUINEO {

class AboutPage {
public:
    static void Compose(UIContext& ui, const std::string& idPrefix, const RectFrame& bounds) {
        if (bounds.width <= 0.0f || bounds.height <= 0.0f) {
            return;
        }

        const PageVisualTokens visuals = CurrentPageVisuals();
        const PageHeaderLayout header = ComposePageHeader(
            ui,
            idPrefix,
            bounds,
            "About",
            "EUI-NEO"
        );

        const RectFrame viewport{
            bounds.x,
            header.contentY,
            bounds.width,
            std::max(0.0f, bounds.y + bounds.height - header.contentY)
        };
        if (viewport.height <= 0.0f) {
            return;
        }

        ComposePageSection(ui, idPrefix + ".section", viewport);

        const float iconSize = std::max(96.0f, std::min({220.0f, viewport.width * 0.3f, viewport.height * 0.3f}));
        const float footerHeight = 128.0f;
        const float buttonGap = 20.0f;
        const float sideInset = std::max(48.0f, visuals.sectionInset * 2.0f);
        const float rowWidth = std::max(260.0f, std::min(560.0f, viewport.width - sideInset * 2.0f));
        const float buttonWidth = std::max(120.0f, (rowWidth - buttonGap) * 0.5f);
        const float buttonHeight = 44.0f;
        const float clusterHeight = iconSize + 18.0f + buttonHeight;
        const float availableHeight = std::max(0.0f, viewport.height - footerHeight);
        const float clusterY = viewport.y + std::max(16.0f, (availableHeight - clusterHeight) * 0.5f);
        const float iconX = viewport.x + (viewport.width - iconSize) * 0.5f;
        const float rowX = viewport.x + (viewport.width - rowWidth) * 0.5f;
        const float primaryY = clusterY + iconSize + 18.0f;
        const float outlineX = rowX + buttonWidth + buttonGap;

        ui.image(idPrefix + ".icon")
            .position(iconX, clusterY)
            .size(iconSize, iconSize)
            .path("docs/icon.svg")
            .rounding(18.0f)
            .build();

        ui.button(idPrefix + ".primary")
            .position(rowX, primaryY)
            .size(buttonWidth, buttonHeight)
            .text("GitHub")
            .icon("\xF0\x9F\x93\x8E")
            .style(ButtonStyle::Primary)
            .onClick([] {
                OpenExternalUrl("https://github.com/sudoevolve/EUI");
            })
            .build();

        ui.button(idPrefix + ".outline")
            .position(outlineX, primaryY)
            .size(buttonWidth, buttonHeight)
            .text("Join Group")
            .icon("\xEF\x83\x80")
            .style(ButtonStyle::Outline)
            .onClick([] {
                OpenExternalUrl("https://qm.qq.com/q/4aaM5gqisg");
            })
            .build();

        const std::string licenseTitle = "License";
        const std::string licenseLine1 = "Copyright @2026 SudoEvolve";
        const std::string licenseLine2 = "Licensed under apeach2.0";
        const std::string licenseLine3 = "(Apeach Public License 2.0)";
        const RectFrame titleBounds = Renderer::MeasureTextBounds(licenseTitle, 24.0f / 24.0f);
        const RectFrame licenseBounds1 = Renderer::MeasureTextBounds(licenseLine1, 16.0f / 24.0f);
        const RectFrame licenseBounds2 = Renderer::MeasureTextBounds(licenseLine2, 16.0f / 24.0f);
        const RectFrame licenseBounds3 = Renderer::MeasureTextBounds(licenseLine3, 16.0f / 24.0f);
        const float titleX = viewport.x + (viewport.width - titleBounds.width) * 0.5f;
        const float line1X = viewport.x + (viewport.width - licenseBounds1.width) * 0.5f;
        const float line2X = viewport.x + (viewport.width - licenseBounds2.width) * 0.5f;
        const float line3X = viewport.x + (viewport.width - licenseBounds3.width) * 0.5f;
        const float line3Y = viewport.y + viewport.height - 14.0f - licenseBounds3.y - licenseBounds3.height;
        const float line2Y = line3Y - 22.0f;
        const float line1Y = line2Y - 22.0f;
        const float titleY = line1Y - 30.0f;

        ui.label(idPrefix + ".license.title")
            .text(licenseTitle)
            .position(titleX, titleY)
            .fontSize(24.0f)
            .color(visuals.titleColor)
            .build();

        ui.label(idPrefix + ".license.line1")
            .text(licenseLine1)
            .position(line1X, line1Y)
            .fontSize(16.0f)
            .color(visuals.subtitleColor)
            .build();

        ui.label(idPrefix + ".license.line2")
            .text(licenseLine2)
            .position(line2X, line2Y)
            .fontSize(16.0f)
            .color(visuals.bodyColor)
            .build();

        ui.label(idPrefix + ".license.line3")
            .text(licenseLine3)
            .position(line3X, line3Y)
            .fontSize(16.0f)
            .color(visuals.bodyColor)
            .build();
    }
};

} // namespace EUINEO
