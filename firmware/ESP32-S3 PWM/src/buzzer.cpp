#include "buzzer.h"

static const int LEDC_CHANNEL = 4;
static const int LEDC_RES_BITS = 8;
static const int OFF_DUTY = 0;
static const int ON_DUTY = 128;

void setupBuzzer()
{
	ledcAttachPin(CTL_BUZZ, LEDC_CHANNEL);
  ledcSetup(LEDC_CHANNEL, 2000, LEDC_RES_BITS);
  ledcWrite(LEDC_CHANNEL, OFF_DUTY);
}

static void buzzerPlay(uint32_t freq)
{
	ledcSetup(LEDC_CHANNEL, freq, LEDC_RES_BITS);
	ledcWrite(LEDC_CHANNEL, ON_DUTY);
	// delay(ms);
	// ledcWrite(LEDC_CHANNEL, OFF_DUTY);
}

void buzzerOff()
{
	ledcWrite(LEDC_CHANNEL, OFF_DUTY);
}

void buzzerCut()
{
  // buzzerPlay(4000);
  buzzerPlay(300);
}

void buzzerCoag()
{
	// buzzerPlay(4500);
  buzzerPlay(400);
}

void buzzerError()
{
	buzzerPlay(600);
	delay(200);
	buzzerPlay(750);
  delay(200);
  buzzerOff();
}