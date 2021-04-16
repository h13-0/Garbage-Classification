#include "Slider.h"

/**********项目所需库***********/
#include "Stepper.h"
#include "SystemClock.h"

static Stepper_TypeDef stepperSlider;

void SliderInit()
{
		stepperSlider = Stepper_Init(SLIDER_DIR_PORT, SLIDER_DIR_PIN, SLIDER_STEP_PORT, SLIDER_STEP_PIN);
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
	
		LIMIT_SENSOR_RCC_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Mode = LIMIT_SENDOR_GPIO_MODE;
	
		GPIO_InitStructure.GPIO_Pin = FORWARD_TRIGGRR_PIN;
		GPIO_Init(FORWARD_TRIGGRR_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = BACKWARD_TRIGGRR_PIN;
		GPIO_Init(BACKWARD_TRIGGRR_PORT, &GPIO_InitStructure);
	
		SliderReset();
}

void SliderReset()
{
		if(GPIO_ReadInputDataBit(BACKWARD_TRIGGRR_PORT,BACKWARD_TRIGGRR_PIN) == TRIGGERED_LEVEL)
			return;
		
		Stepper_BackwardMode(&stepperSlider);
		
		for(uint16_t i = 0; i < STEP_LIMIT; i++)
		{
				Stepper_SingleStep(&stepperSlider, SLIDER_DELAY_PER_HALF_SETP);
				if(GPIO_ReadInputDataBit(BACKWARD_TRIGGRR_PORT,BACKWARD_TRIGGRR_PIN) == TRIGGERED_LEVEL)
						break;
		}
}

void RecyclableWaste()
{
		Stepper_Forward(&stepperSlider, 200, SLIDER_DELAY_PER_HALF_SETP);
	
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
	
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		SliderReset();
}

void KitchenWaste()
{
		Stepper_Forward(&stepperSlider, 200, SLIDER_DELAY_PER_HALF_SETP);
		
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
	
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		SliderReset();
}

void OtherWaste()
{
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Forward(&stepperSlider, STEP_LIMIT, SLIDER_DELAY_PER_HALF_SETP);
		
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(LEFT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
	
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		SliderReset();
}

void HarmfulWaste()
{
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		Stepper_Forward(&stepperSlider, STEP_LIMIT, SLIDER_DELAY_PER_HALF_SETP);
		
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
		SLIDER_SERVO_ROTATE(RIGHT_SHAKE_ANGLE);
		delay_ms(SLIDER_DELAY / 4);
	
		SLIDER_SERVO_ROTATE(MIDDLE_ANGLE);
		SliderReset();
}
