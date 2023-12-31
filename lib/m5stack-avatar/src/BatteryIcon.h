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
    {
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

    void drawBatteryIcon02(M5Canvas *spi, uint32_t x, uint32_t y, uint16_t fgcolor, uint16_t bgcolor, float offset, BatteryIconStatus batteryIconStatus, int32_t batteryLevel)
    {
      // 一度、balloonの表示を出さないとキャラクタ設定が反映されない？？
      spi->setFont(&fonts::lgfxJapanGothic_16);
      // spi->setFont(&fonts::lgfxJapanGothicP_12);
      // spi->setFont(&fonts::efontJA_10);

      spi->setTextColor(fgcolor, bgcolor); // （文字色, 背景色）
      spi->setTextDatum(top_left);
      spi->setCursor(0, 0);
      // --------------------------------------------------------------

      String chargeState = "";
      if (batteryIconStatus == BatteryIconStatus::Ncharging)
        chargeState = "   充電中";

      String baLevelText;
      // 桁揃え
      if(batteryLevel>=100)     baLevelText = "電池：";
      else if(batteryLevel>=10) baLevelText = "電池： ";
      else if(batteryLevel>=0)  baLevelText = "電池：  ";
      else return;

      spi->print( baLevelText + String(batteryLevel, DEC) + "\% " + chargeState);
    }

    void drawBatteryIcon03(M5Canvas *spi, uint32_t x, uint32_t y, uint16_t fgcolor, uint16_t bgcolor, float offset, BatteryIconStatus batteryIconStatus, int32_t batteryLevel, String msg)
    {
      // spi->setFont(&fonts::lgfxJapanGothic_16);
      spi->setFont(&fonts::lgfxJapanGothicP_12);
      // spi->setFont(&fonts::efontJA_10);

      spi->setTextColor(fgcolor, bgcolor); // （文字色, 背景色）
      spi->setTextDatum(top_left);
      spi->setCursor(0, 0);

      // Serial.println("Line Msg = " + msg);
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
      // if (ctx->getBatteryIconStatus() != BatteryIconStatus::invisible) {
      auto iStatus = ctx->getBatteryIconStatus();
      // enum BatteryIconStatus { discharging, charging, invisible, unknown, numMD, LineDispMD };

      if (iStatus == BatteryIconStatus::discharging || iStatus == BatteryIconStatus::charging || iStatus == BatteryIconStatus::unknown)
      {
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        float offset = ctx->getBreath();
        int32_t batteryLevel = ctx->getBatteryLevel();
        drawBatteryIcon(spi, 285, 5, primaryColor, bgColor, -offset, ctx->getBatteryIconStatus(), batteryLevel);
      }
      else if (iStatus == BatteryIconStatus::Ndischarging || iStatus == BatteryIconStatus::Ncharging || iStatus == BatteryIconStatus::numMD)
      {
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        float offset = ctx->getBreath();
        int32_t batteryLevel = ctx->getBatteryLevel();
        drawBatteryIcon02(spi, 285, 5, primaryColor, bgColor, -offset, ctx->getBatteryIconStatus(), batteryLevel);
      }
      else if (iStatus == BatteryIconStatus::lineDispMD)
      {
        uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
        uint16_t bgColor = ctx->getColorDepth() == 1 ? ERACER_COLOR : ctx->getColorPalette()->get(COLOR_BACKGROUND);
        float offset = ctx->getBreath();
        int32_t batteryLevel = ctx->getBatteryLevel();
        String msg = ctx->getBatteryLineText();
        drawBatteryIcon03(spi, 285, 5, primaryColor, bgColor, -offset, ctx->getBatteryIconStatus(), batteryLevel, msg );
      }
    };
  };

} // namespace m5avatar

#endif // BATTERYICON_H_
