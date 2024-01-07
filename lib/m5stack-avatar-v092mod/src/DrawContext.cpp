// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "DrawContext.h"
namespace m5avatar {

// DrawContext
// --- statusLine Add ----
DrawContext::DrawContext(Expression expression, float breath,
                         ColorPalette* const palette, Gaze gaze,
                         float eyeOpenRatio, float mouthOpenRatio,
                         String speechText, BatteryIconStatus batteryIcon, String statusLineText,
                         int32_t batteryLevel, const lgfx::IFont* speechFont, const lgfx::IFont* statusLineFont)
    : DrawContext(expression, breath, palette, gaze, eyeOpenRatio, mouthOpenRatio, speechText, 0, 1, 1, BatteryIconStatus::invisible, statusLineText,0, speechFont, statusLineFont ){};


// --- statusLine Add ----
DrawContext::DrawContext(Expression expression, float breath,
                         ColorPalette* const palette, Gaze gaze,
                         float eyeOpenRatio, float mouthOpenRatio,
                         String speechText, float rotation, float scale, int colorDepth, BatteryIconStatus batteryIconStatus, String statusLineText,
                         int32_t batteryLevel, const lgfx::IFont* speechFont, const lgfx::IFont* statusLineFont) 
    : expression{expression},
      breath{breath},
      eyeOpenRatio{eyeOpenRatio},
      mouthOpenRatio{mouthOpenRatio},
      gaze{gaze},
      palette{palette},
      speechText{speechText},
      rotation{rotation},
      scale{scale},
      colorDepth{colorDepth},
      batteryIconStatus(batteryIconStatus),
      statusLineText{statusLineText},
      batteryLevel(batteryLevel),
      speechFont{speechFont},
      statusLineFont{statusLineFont}{}

Expression DrawContext::getExpression() const { return expression; }

float DrawContext::getMouthOpenRatio() const { return mouthOpenRatio; }

float DrawContext::getEyeOpenRatio() const { return eyeOpenRatio; }

float DrawContext::getBreath() const { return breath; }

float DrawContext::getRotation() const { return rotation; }

float DrawContext::getScale() const { return scale; }

String  DrawContext::getspeechText() const { return speechText; }

Gaze DrawContext::getGaze() const { return gaze; }

ColorPalette* const DrawContext::getColorPalette() const { return palette; }

int DrawContext::getColorDepth() const { return colorDepth; }

const lgfx::IFont* DrawContext::getSpeechFont() const { return speechFont; }

BatteryIconStatus DrawContext::getBatteryIconStatus() const { return batteryIconStatus; }

int32_t DrawContext::getBatteryLevel() const { return batteryLevel; }

// --------- statusLine --------------
const lgfx::IFont* DrawContext::getStatusLineFont() const { return statusLineFont; }

String DrawContext::getStatusLineText() const { return statusLineText; }
// -----------------------------------

}  // namespace m5avatar
