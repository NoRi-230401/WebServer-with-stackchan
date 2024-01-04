// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "Avatar.h"
namespace m5avatar
{

  unsigned int seed = 0;

  // TODO(meganetaaan): make read-only
  DriveContext::DriveContext(Avatar *avatar) : avatar{avatar} {}

  Avatar *DriveContext::getAvatar() { return avatar; }

  TaskHandle_t drawTaskHandle;

  void drawLoop(void *args)
  {
    DriveContext *ctx = reinterpret_cast<DriveContext *>(args);
    Avatar *avatar = ctx->getAvatar();
    while (avatar->isDrawing())
    {
      if (avatar->isDrawing())
      {
        avatar->draw();
      }
      vTaskDelay(33);
    }
    vTaskDelete(NULL);
  }

  void facialLoop(void *args)
  {
    int c = 0;
    DriveContext *ctx = reinterpret_cast<DriveContext *>(args);
    Avatar *avatar = ctx->getAvatar();
    uint32_t saccade_interval = 1000;
    uint32_t blink_interval = 1000;
    unsigned long last_saccade_millis = 0;
    unsigned long last_blink_millis = 0;
    bool eye_open = true;
    float vertical = 0.0f;
    float horizontal = 0.0f;
    float breath = 0.0f;
    while (avatar->isDrawing())
    {

      if ((millis() - last_saccade_millis) > saccade_interval)
      {
        vertical = rand_r(&seed) / (RAND_MAX / 2.0) - 1;
        horizontal = rand_r(&seed) / (RAND_MAX / 2.0) - 1;
        avatar->setGaze(vertical, horizontal);
        saccade_interval = 500 + 100 * random(20);
        last_saccade_millis = millis();
      }

      if ((millis() - last_blink_millis) > blink_interval)
      {
        if (eye_open)
        {
          avatar->setEyeOpenRatio(1);
          blink_interval = 2500 + 100 * random(20);
        }
        else
        {
          avatar->setEyeOpenRatio(0);
          blink_interval = 300 + 10 * random(20);
        }
        eye_open = !eye_open;
        last_blink_millis = millis();
      }
      c = (c + 1) % 100;
      breath = sin(c * 2 * PI / 100.0);
      avatar->setBreath(breath);
      vTaskDelay(33);
    }
    vTaskDelete(NULL);
  }

  Avatar::Avatar() : Avatar(new Face()) {}

  Avatar::Avatar(Face *face)
      : face{face},
        _isDrawing{false},
        expression{Expression::Neutral},
        breath{0},
        eyeOpenRatio{1},
        mouthOpenRatio{0},
        gazeV{0},
        gazeH{0},
        rotation{0},
        scale{1},
        palette{ColorPalette()},
        speechText{""},
        colorDepth{1},
        batteryLineText{""},
        batteryIconStatus{BatteryIconStatus::invisible} {}

  Avatar::~Avatar()
  {
    delete face;
  }

  void Avatar::setFace(Face *face) { this->face = face; }

  Face *Avatar::getFace() const { return face; }

  void Avatar::addTask(TaskFunction_t f, const char *name, const uint32_t stack_size, UBaseType_t priority, TaskHandle_t *const task_handle, const BaseType_t core_id)
  {
    DriveContext *ctx = new DriveContext(this);
    // TODO(meganetaaan): set a task handler
    xTaskCreateUniversal(f,           /* Function to implement the task */
                         name,        /* Name of the task */
                         stack_size,  /* Stack size in words */
                         ctx,         /* Task input parameter */
                         priority,    /* Priority of the task */
                         task_handle, /* Task handle. */
                         core_id);    /* Core No*/
  }

  void Avatar::init(int colorDepth)
  {
    // for compatibility with older version
    start(colorDepth);
  }

  void Avatar::stop() { _isDrawing = false; }

  void Avatar::suspend()
  {
    vTaskSuspend(drawTaskHandle);
  }

  void Avatar::resume()
  {
    vTaskResume(drawTaskHandle);
  }

  void Avatar::start(int colorDepth)
  {
    // if the task already started, don't create another task;
    if (_isDrawing)
      return;
    _isDrawing = true;

    this->colorDepth = colorDepth;
    DriveContext *ctx = new DriveContext(this);
    // TODO(meganetaaan): keep handle of these tasks
    xTaskCreateUniversal(drawLoop,        /* Function to implement the task */
                         "drawLoop",      /* Name of the task */
                         2048,            /* Stack size in words */
                         ctx,             /* Task input parameter */
                         1,               /* Priority of the task */
                         &drawTaskHandle, /* Task handle. */
                         APP_CPU_NUM);

    xTaskCreateUniversal(facialLoop,   /* Function to implement the task */
                         "facialLoop", /* Name of the task */
                         1024,         /* Stack size in words */
                         ctx,          /* Task input parameter */
                         2,            /* Priority of the task */
                         NULL,         /* Task handle. */
                         APP_CPU_NUM);
  }

  void Avatar::draw()
  {
    Gaze g = Gaze(this->gazeV, this->gazeH);
    DrawContext *ctx = new DrawContext(this->expression, this->breath,
                                       &this->palette, g, this->eyeOpenRatio,
                                       this->mouthOpenRatio, this->speechText,
                                       this->rotation, this->scale, this->colorDepth, this->batteryIconStatus, this->batteryLineText, this->batteryLevel, this->speechFont, this->statusLineFont);
    face->draw(ctx);
    delete ctx;
  }

  bool Avatar::isDrawing() { return _isDrawing; }

  void Avatar::setExpression(Expression expression)
  {
    suspend();
    this->expression = expression;
    resume();
  }

  Expression Avatar::getExpression()
  {
    return this->expression;
  }

  void Avatar::setBreath(float breath) { this->breath = breath; }

  float Avatar::getBreath()
  {
    return this->breath;
  }

  void Avatar::setRotation(float radian) { this->rotation = radian; }

  void Avatar::setScale(float scale) { this->scale = scale; }

  void Avatar::setPosition(int top, int left)
  {
    this->getFace()->getBoundingRect()->setPosition(top, left);
  }

  void Avatar::setColorPalette(ColorPalette cp) { palette = cp; }

  ColorPalette Avatar::getColorPalette(void) const { return this->palette; }

  void Avatar::setMouthOpenRatio(float ratio) { this->mouthOpenRatio = ratio; }

  void Avatar::setEyeOpenRatio(float ratio) { this->eyeOpenRatio = ratio; }

  void Avatar::setGaze(float vertical, float horizontal)
  {
    this->gazeV = vertical;
    this->gazeH = horizontal;
  }

  void Avatar::getGaze(float *vertical, float *horizontal)
  {
    *vertical = this->gazeV;
    *horizontal = this->gazeH;
  }

  void Avatar::setSpeechText(const char *speechText)
  {
    this->speechText = String(speechText);
  }

  void Avatar::setSpeechFont(const lgfx::IFont *speechFont)
  {
    this->speechFont = speechFont;
  }

  void Avatar::setStatusLineFont(const lgfx::IFont *statusLineFont)
  {
    this->statusLineFont = statusLineFont;
  }

  void Avatar::setBatteryIcon(bool batteryIcon)
  {
    if (!batteryIcon) {
      batteryIconStatus = BatteryIconStatus::invisible;
    } else {
      batteryIconStatus = BatteryIconStatus::unknown;
    }
  }

  void Avatar::setBatteryIcon(bool batteryIcon, int8_t batteryIconMode)
  {
    if (!batteryIcon) 
    {
      batteryIconStatus = BatteryIconStatus::invisible;
      return;
    }

    switch (batteryIconMode)
    {
    case BATTERY_MD_ICON:
      batteryIconStatus = BatteryIconStatus::unknown;
      break;

    case BATTERY_MD_NUM:
      batteryIconStatus = BatteryIconStatus::numMD;
      break;

    case BATTERY_MD_LINE_DISP:
      batteryIconStatus = BatteryIconStatus::lineDispMD;
      break;

    case BATTERY_MD_INVISIBLE:
    default:
      batteryIconStatus = BatteryIconStatus::invisible;
      break;
    }
  }

  void Avatar::setBatteryStatus(bool isCharging, int32_t batteryLevel)
  {
    // if (this->batteryIconStatus != BatteryIconStatus::invisible) {
    //   if (isCharging) {
    //     this->batteryIconStatus = BatteryIconStatus::charging;
    //   } else {
    //     this->batteryIconStatus = BatteryIconStatus::discharging;
    //   }
    //   this->batteryLevel = batteryLevel;
    // }

    auto iStatus = this->batteryIconStatus;
    if (iStatus == BatteryIconStatus::invisible)
      return;

    if(iStatus==BatteryIconStatus::charging ||iStatus==BatteryIconStatus::discharging ||iStatus==BatteryIconStatus::unknown )
    {
      if (isCharging)
        this->batteryIconStatus = BatteryIconStatus::charging;
      else
        this->batteryIconStatus = BatteryIconStatus::discharging;
    }
    else if(iStatus==BatteryIconStatus::Ncharging ||iStatus==BatteryIconStatus::Ndischarging ||iStatus==BatteryIconStatus::numMD)
    {
      if (isCharging)
        this->batteryIconStatus = BatteryIconStatus::Ncharging;
      else
        this->batteryIconStatus = BatteryIconStatus::Ndischarging;
    }

    this->batteryLevel = batteryLevel; 
  }

  void Avatar::setBatteryStatus(bool isCharging, int32_t batteryLevel, String lineText )
  {
    setBatteryStatus(isCharging, batteryLevel);

    // Serial.println("setBatteryStatus() LineMsg=" + LineMsg);
    auto iStatus = this->batteryIconStatus;
    if (iStatus == BatteryIconStatus::lineDispMD)
    {
      this->batteryLineText = lineText;
    }

    // Serial.println("setBatteryStatus() batteryLineText = " + this->batteryLineText);
    
  }

  void Avatar::setBatteryLineText( String lineText )
  {
    this->batteryLineText = lineText;
  }


} // namespace m5avatar
