/**********C���Ա�׼��**********/
#include "string.h"

/**********STM32��׼��**********/
#include "stm32f10x.h"

/**********��Ŀ�����***********/
#include "SystemClock.h"
#include "usart.h"
#include "TIM_Init.h"
#include "Slider.h"
#include "USART1_IRQ.h"
#include "CapacityDetector.h"
#include "Conveyor.h"
#include "Baffle.h"

//#define STM32_PROJRCT_DEBUG

int main()
{
    //��ʼ��Systickϵͳʱ��
    SystemClockInit();

    //��ʼ������
    uart_init(115200);

    //����TIM4�Ƕ��������õ�, �����ȳ�ʼ��TIM4
    TIM4_PWM_Init(1999, 719);
	
		//��ʼ�����������
		CapacityDetectorInit();

    //��ʼ��Slider
    SliderInit();

		//��ʼ��Conveyor
		ConveyorInit();
		
		//�رճ���
		//Right_Baffle_Close();
		Right_Baffle_Open();
		//Left_Baffle_Close();
		Left_Baffle_Open();
	
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

            //���������Ч
            if(key != NULL)
            {
                //��������
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
																		//����keil ����nop������
																		__nop();
																		Left_Baffle_Open();
                                }

                                //Baffle:Left:Close
                                else if(!strncmp(cmd, "Close", 5))
                                {
																		__nop();
																		Left_Baffle_Close();
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
																		__nop();
																		Right_Baffle_Open();
                                }

                                //Baffle:Left:Close
                                else if(!strncmp(cmd, "Close", 5))
                                {
																		__nop();
																		Right_Baffle_Close();
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

                //���ʹ�����
                //Conveyor:${cmd}
                else if(!strncmp(key, "Conveyor", 8))
                {
										char *cmd = NULL;
										cmd = strtok(NULL, ":");
										if(cmd != NULL)
										{
												//Conveyor:Forward
												if(!strncmp(cmd, "Forward", 7))
												{
														ConveyorForward();
												}
												
												//Conveyor:Backward
												else if(!strncmp(cmd, "Backward", 8))
												{
														ConveyorBackward();
												}
												
												//Conveyor:Stop
												else if(!strncmp(cmd, "Stop", 4))
												{
														ConveyorStop();
												}
										}
                }

                //������������
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
        } else {
						//û��ָ���ʱ����в��, 3��ִ��һ��, 12��һ�ֻ�
						static uint64_t time = 0;
						//������ó��������޷���ȡ����, �����
						CapacityDetector_Handler();
						
						if(Get_ms() - time > 3000)
						{
								time = Get_ms();
								double rec = 0, kit = 0, oth = 0, harm = 0;
								CapacityDetector_getValue(&rec, &kit, &oth, &harm);
							
								//����ʣ������
							
								printf("Capacity:Recycle:%lf\r\n", rec);
								printf("Capacity:Kitchen:%lf\r\n", kit);
								printf("Capacity:Other:%lf\r\n", oth);
								printf("Capacity:Harmful:%lf\r\n", harm);
						}
				}
    }
}
