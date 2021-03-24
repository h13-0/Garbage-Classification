#ifndef _CONVEYOR_H_
#define _CONVEYOR_H_

//Configs
#define CONVEYOR_DIR_PORT   GPIOA
#define CONVEYOR_DIR_PIN    GPIO_Pin_0
#define CONVEYOR_STEP_PORT  GPIOA
#define CONVEYOR_STEP_PIN   GPIO_Pin_1
#define CONVEYOR_DELAY_PER_HALF_SETP 5000


//APIs

/**
  * @brief  Init Conveyor.
  * @param  None
  * @retval None
	* @note   None
  */
void ConveyorInit(void);

void ConveyorForward(void);

#endif
