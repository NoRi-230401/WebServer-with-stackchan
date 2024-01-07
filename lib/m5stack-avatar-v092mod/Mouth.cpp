// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "Mouth.h"

#ifndef _min
#define _min(a, b) std::min(a, b)
#endif

namespace m5avatar {

Mouth::Mouth(uint16_t minWidth, uint16_t maxWidth, uint16_t minHeight,
             uint16_t maxHeight)
    : minWidth{minWidth},
      maxWidth{maxWidth},
      minHeight{minHeight},
      maxHeight{maxHeight} {}

void Mouth::draw(M5Canvas *spi, BoundingRect rect, DrawContext *ctx) {
  uint16_t primaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_PRIMARY);
  float breath = _min(1.0f, ctx->getBreath());
  float openRatio = ctx->getMouthOpenRatio();
  int h = minHeight + (maxHeight - minHeight) * openRatio;
  int w = minWidth + (maxWidth - minWidth) * (1 - openRatio);
  int x = rect.getLeft() - w / 2;
  int y = rect.getTop() - h / 2 + breath * 2;
  spi->fillRect(x, y, w, h, primaryColor);
  
  // (カスタマイズ) ほっぺを表示 （つゆきぱぱさん）
  uint16_t secondaryColor = ctx->getColorDepth() == 1 ? 1 : ctx->getColorPalette()->get(COLOR_SECONDARY);
  int cheeks_r = static_cast<int>(M5.Display.height() / 20);
  int cheeks_x1 = cheeks_r * 4;
  int cheeks_x2 = M5.Display.width() - cheeks_r * 4;
  int cheeks_y = y + h / 2;
  spi->fillCircle(cheeks_x1, cheeks_y, cheeks_r, secondaryColor);
  spi->fillCircle(cheeks_x2, cheeks_y, cheeks_r, secondaryColor);
 
}


}  // namespace m5avatar
