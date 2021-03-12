#ifndef _SLIDER_H_
#define _SLIDER_H_

//Configs

//限制最大步数, 也就是行程
#define STEP_LIMIT 1500

//Stepper
#define SLIDER_DIR_PORT   GPIOC
#define SLIDER_DIR_PIN    GPIO_Pin_13
#define SLIDER_STEP_PORT  GPIOC
#define SLIDER_STEP_PIN   GPIO_Pin_14

//Servo
//#define SLIDER_SERVO_INIT() 
#define SLIDER_SERVO_ROTATE(angle) TIM_SetCompare1(TIM4, angle)
#define LEFT_ANGLE   70
#define MIDDLE_ANGLE 158
#define RIGHT_ANGLE  240

void SliderTest(void);

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
