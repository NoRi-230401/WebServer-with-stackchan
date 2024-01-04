// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef BATTERYICON_H_
#define BATTERYICON_H_
#include <M5GFX.h>
#include <M5Unified.h>
#include "DrawContext.h"
#include "Drawable.h"

namespace m5avatar
{

  class BatteryIcon final : public Drawable
  {
  private:
    void drawBatteryIcon(M5Canvas *spi, uint32_t x, uint32_t y, uint16_t fgcolor, uint16_t bgcolor, float offset, BatteryIconStatus batteryIconStatus, int32_t batteryLevel)
    { // draw : battery Icon
      spi->drawRect(x, y + 5, 5, 5, fgcolor);
      spi->drawRect(x + 5, y, 30, 15, fgcolor);
      int battery_width = 30 * (float)(batteryLevel / 100.0f);
      spi->fillRect(x + 5 + 30 - battery_width, y, battery_width, 15, fgcolor);
      if (batteryIconStatus == BatteryIconStatus::charging)
      {
        spi->fillTriangle(x + 20, y, x + 15, y + 8, x + 20, y + 8, bgcolor);
        spi->fillTriangle(x + 18, y + 7, x + 18, y + 15, x + 23, y + 7, bgcolor);
        spi->drawLine(x + 20, y, x + 15, y + 8, fgcolor);
        spi->drawLine(x + 20, y, x + 20, y + 7, fgcolor);
        spi->drawLine(x + 18, y + 15, x + 23, y + 7, fgcolor);
        spi->drawLine(x + 18, y + 8, x + 18, y + 15, fgcolor);
      }
    }

    void drawBatteryLvlNum(M5Canvas *spi, uint16_t fgcolor, uint16_t bgcolor,
       BatteryIconStatus batteryIconStatus, int32_t batteryLevel, const lgfx::IFont *font)
    {// draw : batteryLevel number(%) 
      spi->setFont(font);
      spi->setTextColor(fgcolor, bgcolor);
      spi->setTextDatum(top_left);
      spi->setCursor(0, 0);
      
      String chargeState = "";
      if (batteryIconStatus == BatteryIconStatus::Ncharging)
        chargeState = "charging";

      char levelTxt[30];
      sprintf(levelTxt,"Battery:%3d", batteryLevel);
      spi->print( String(levelTxt) + "\%   " + chargeState);
    }

    void drawStatusLineMsg(M5Canvas *spi, uint16_t fgcolor, uint16_t bgcolor, String msg, const lgfx::IFont *font) 
    {// draw : statusLine Text
      spi->setFont(font);
      spi->setTextColor(fgcolor, bgcolor);
      spi->setTextDatum(top_left);
      spi->setCursor(0, 0);
      spi->print( msg );
    }

  public:
    // constructor
    BatteryIcon() = default;
    ~BatteryIcon() = default;
    BatteryIcon(const BatteryIcon &other) = default;
    BatteryIcon &operator=(const BatteryIcon &other) = default;
    void draw(M5Canvas *spi, BoundingRect rect, DrawContext *ctx) override
    {
      auto iStatus = ctx->getBatteryIconStatus();

      if (iStatus == BatteryIconStatus::discharging || iStatus == BatteryIconStatus::charging || iStatus == BatteryIconStatus::unknown)
      {// batteryLevel ICON disp
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        float offset = ctx->getBreath();
        int32_t batteryLevel = ctx->getBatteryLevel();
        drawBatteryIcon(spi, 285, 5, primaryColor, bgColor, -offset, ctx->getBatteryIconStatus(), batteryLevel);
      }
      else if (iStatus == BatteryIconStatus::Ndischarging || iStatus == BatteryIconStatus::Ncharging || iStatus == BatteryIconStatus::numMD)
      {// batteryLevel number(%) disp
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        int32_t batteryLevel = ctx->getBatteryLevel();
        const lgfx::IFont *font = ctx->getStatusLineFont();
        drawBatteryLvlNum(spi, primaryColor, bgColor, ctx->getBatteryIconStatus(), batteryLevel, font);
      }
      else if (iStatus == BatteryIconStatus::lineDispMD)
      {// status line mode
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        String msg = ctx->getBatteryLineText();
        const lgfx::IFont *font = ctx->getStatusLineFont();
        drawStatusLineMsg(spi, primaryColor, bgColor, msg , font);
      }
    };
  };

} // namespace m5avatar

#endif // BATTERYICON_H_
