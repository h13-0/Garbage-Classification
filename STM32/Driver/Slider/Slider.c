#include "Slider.h"

#include "Stepper.h"
#include "SystemClock.h"

static Stepper_TypeDef stepperSlider;

void SliderInit()
{
		stepperSlider = Stepper_Init(SLIDER_DIR_PORT, SLIDER_DIR_PIN, SLIDER_STEP_PORT, SLIDER_STEP_PIN);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		SliderReset();
}

void SliderReset()
{
		//Stepper_ForwardMode(&stepperSlider);
		
		
	
}

void RecyclableWaste()
{
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Forward(&stepperSlider, 1500, 5000);
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(500);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Backward(&stepperSlider, 1500, 5000);
}

void KitchenWaste()
{
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Forward(&stepperSlider, 1500, 5000);
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(500);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Backward(&stepperSlider, 1500, 5000);
}

void OtherWaste()
{
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(500);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
}

void HarmfulWaste()
{
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(500);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
}

void SliderTest()
{
		RecyclableWaste();
		delay_ms(2000);
		KitchenWaste();
		delay_ms(2000);
		OtherWaste();
		delay_ms(2000);
		HarmfulWaste();
		delay_ms(2000);
}
