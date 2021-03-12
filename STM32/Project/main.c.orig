#include "string.h"

#include "stm32f10x.h"

#include "SystemClock.h"
#include "usart.h"
#include "TIM_Init.h"
#include "Slider.h"
#include "USART1_IRQ.h"

//#define STM32_PROJRCT_DEBUG

int main()
{
		//初始化Systick系统时钟
		SystemClockInit();
	
		//初始化串口
		uart_init(115200);
	
		//由于TIM4是多个组件共用的, 所以先初始化TIM4
		TIM4_PWM_Init(1999, 719);

		//初始化Slider
		SliderInit();

		while(1)
		{
				uint8_t buf[USART1_BUF_LEN];
				uint16_t len = USART1_ReadLine(buf);
				if(len > 0)
				{
						#ifdef STM32_PROJRCT_DEBUG
						printf("len: %d\r\n", len);
						printf("Command: %s\r\n", buf);
						#endif
					
						char *key = NULL;
						key = strtok((char *)buf, ":");
						
						//命令可能有效
						if(key != NULL)
						{
								//挡板命令
								//Baffle:${class}:${cmd}
								if(!strncmp(key, "Baffle", 6))
								{
										char *classes = NULL;
										classes = strtok(NULL, ":");
										if(classes != NULL)
										{
												//Baffle:Left:${cmd}
												if(!strncmp(classes, "Left", 4))
												{
														char *cmd = NULL;
														cmd = strtok(NULL, ":");
														if(cmd != NULL)
														{
																//Baffle:Left:Open
																if(!strncmp(cmd, "Open", 4))
																{
																		
																		printf("Baffle:Left:Opened\r\n");
																}
																
																//Baffle:Left:Close
																else if(!strncmp(cmd, "Close", 5))
																{
																		
																		printf("Baffle:Left:Closed\r\n");
																}
																
														}
												}
												//Baffle:Right:${cmd}
												else if(!strncmp(classes, "Right", 5))
												{
														char *cmd = NULL;
														cmd = strtok(NULL, ":");
														if(cmd != NULL)
														{
																//Baffle:Right:Open
																if(!strncmp(cmd, "Open", 4))
																{
																		
																		printf("Baffle:Right:Opened\r\n");
																}
																
																//Baffle:Left:Close
																else if(!strncmp(cmd, "Close", 5))
																{
																		
																		printf("Baffle:Right:Closed\r\n");
																}
																
														}
												}
												//Baffle:Middle:${cmd}
												else if(!strncmp(classes, "Middle", 6))
												{
														char *cmd = NULL;
														cmd = strtok(NULL, ":");
														if(cmd != NULL)
														{
																//Baffle:Middle:Open
																if(!strncmp(cmd, "Open", 4))
																{
																		
																		printf("Baffle:Middle:Opened\r\n");
																}
																
																//Baffle:Left:Close
																else if(!strncmp(cmd, "Close", 5))
																{
																		
																		printf("Baffle:Middle:Closed\r\n");
																}
																
														}
												}
										}
								}
								
								//传送带命令
								//Conveyor:${cmd}
								else if(!strncmp(key, "Conveyor", 8))
								{
										
								}
								
								//放置垃圾命令
								//Slider:${class}
								else if(!strncmp(key, "Slider", 6))
								{
										char *classes = NULL;
										classes = strtok(NULL, ":");
										if(classes != NULL)
										{	
												//Slider:Recycle
												if(!strncmp(classes, "Recycle", 7))
												{
														RecyclableWaste();
														printf("Slider:OK\r\n");
												}
												
												//Slider:Kitchen
												else if(!strncmp(classes, "Kitchen", 7))
												{
														KitchenWaste();
														printf("Slider:OK\r\n");
												}
												
												//Slider:Harmful
												else if(!strncmp(classes, "Harmful", 7))
												{
														HarmfulWaste();
														printf("Slider:OK\r\n");
												}
												
												//Slider:Other
												else if(!strncmp(classes, "Other", 5))
												{
														OtherWaste();
														printf("Slider:OK\r\n");
												}
										}
								}
						}
				}
		}
}
