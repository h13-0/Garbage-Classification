#ifndef _BAFFLE_H_
#define _BAFFLE_H_

#define Left_Baffle_Open() TIM_SetCompare2(TIM4, 220)
#define Left_Baffle_Close() TIM_SetCompare2(TIM4, 50)


#define Right_Baffle_Open() TIM_SetCompare3(TIM4, 87)
#define Right_Baffle_Close() TIM_SetCompare3(TIM4, 250)

#define Middle_Baffle_Rotate(angle) TIM_SetCompare4(TIM4, angle)

#endif
