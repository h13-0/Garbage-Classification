#ifndef _SLIDER_H_
#define _SLIDER_H_

//Configs

//限制最大步数, 也就是行程
#define STEP_LIMIT 1500  //28cm

#define SLIDER_DELAY 1000

//Stepper
#define SLIDER_DIR_PORT   GPIOC
#define SLIDER_DIR_PIN    GPIO_Pin_13
#define SLIDER_STEP_PORT  GPIOC
#define SLIDER_STEP_PIN   GPIO_Pin_14
#define SLIDER_DELAY_PER_HALF_SETP 5000

//Servo
//#define SLIDER_SERVO_INIT() 
#define SLIDER_SERVO_ROTATE(angle) TIM_SetCompare1(TIM4, angle)

#define LEFT_ANGLE       245
#define LEFT_SHAKE_ANGLE 220

#define MIDDLE_ANGLE 160

#define RIGHT_ANGLE  			75
#define RIGHT_SHAKE_ANGLE 100


//Limit sensor
#define LIMIT_SENSOR_RCC_ENABLE() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define LIMIT_SENDOR_GPIO_MODE GPIO_Mode_IPD

#define FORWARD_TRIGGRR_PORT GPIOA
#define FORWARD_TRIGGRR_PIN  GPIO_Pin_11

#define BACKWARD_TRIGGRR_PORT GPIOA
#define BACKWARD_TRIGGRR_PIN  GPIO_Pin_12

#define TRIGGERED_LEVEL Bit_RESET

//APIs

/**
  * @brief  Init Slider.
  * @param  None
  * @retval None
	* @note   None
  */
void SliderInit(void);

/**
  * @brief  Move the slide to zero.
  * @param  None
  * @retval None
	* @note   None
  */
void SliderReset(void);

/**
  * @brief  Place Recyclable trash.
  * @param  None
  * @retval None
	* @note   None
  */
void RecyclableWaste(void);

/**
  * @brief  Place Kitchen trash.
  * @param  None
  * @retval None
	* @note   None
  */
void KitchenWaste(void);

/**
  * @brief  Place Other trash.
  * @param  None
  * @retval None
	* @note   The slider does not need to move at zero.
  */
void OtherWaste(void);

/**
  * @brief  Place Harmful trash.
  * @param  None
  * @retval None
	* @note   The slider does not need to move at zero.
  */
void HarmfulWaste(void);

#endif
