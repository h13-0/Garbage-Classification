#include "vl53l0x_it.h"
#include "vl53l0x_easy_api.h"

//中断模式参数结构体
typedef struct
{
    const int VL53L0X_Mode;//模式
    uint32_t ThreshLow;    //下限值
    uint32_t ThreshHigh;   //上限值
} AlrmMode_t;

AlrmMode_t AlarmModes = {

    VL53L0X_GPIOFUNCTIONALITY_THRESHOLD_CROSSED_OUT,// value < thresh_low OR value > thresh_high
    Thresh_Low<<16,
              Thresh_High<<16
};

typedef __packed struct
{
    uint8_t  isApertureSpads;             //ApertureSpads值
    uint8_t  VhvSettings;                 //VhvSettings值
    uint8_t  PhaseCal;                    //PhaseCal值
    uint32_t XTalkCalDistance;            //XTalkCalDistance值
    uint32_t XTalkCompensationRateMegaCps;//XTalkCompensationRateMegaCps值
    uint32_t CalDistanceMilliMeter;       //CalDistanceMilliMeter值
    int32_t  OffsetMicroMeter;            //OffsetMicroMeter值
    uint32_t refSpadCount;                //refSpadCount值

} _vl53l0x_adjust;

extern _vl53l0x_adjust Vl53l0x_adjust;

//vl53l0x模式配置参数集
typedef __packed struct
{
    FixPoint1616_t signalLimit;    //Signal极限数值
    FixPoint1616_t sigmaLimit;     //Sigmal极限数值
    uint32_t timingBudget;         //采样时间周期
    uint8_t preRangeVcselPeriod ;  //VCSEL脉冲周期
    uint8_t finalRangeVcselPeriod ;//VCSEL脉冲周期范围

} mode_data;

//中断配置初始化
static void exti_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			    //使能按键WK_UP所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		    //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				    //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

}

//外部中断服务函数
void EXTI4_IRQHandler(void)
{
    printf("TOF Raised the interrupt\r\n");
		//我不知道你要挂多少设备 所以下面这句你自己去调用吧
    //VL53L0X_ClearInterruptMask(dev,0);
    EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位
}

extern mode_data Mode_data[];

//vl53l0x中断测量模式测试
//dev:设备I2C参数结构体
//mode: 0:默认;1:高精度;2:长距离;3:高速
VL53L0X_Error vl53l0x_interrupt_start(VL53L0X_Dev_t *dev,uint8_t mode)
{
    VL53L0X_Error status=VL53L0X_ERROR_NONE;//工作状态

    exti_init();//中断初始化

    vl53l0x_reset(dev);//复位vl53l0x(频繁切换工作模式容易导致采集距离数据不准，需加上这一代码)
    status = VL53L0X_StaticInit(dev);
    if(status!=VL53L0X_ERROR_NONE) return status;

    status= VL53L0X_SetReferenceSpads(dev,Vl53l0x_adjust.refSpadCount,Vl53l0x_adjust.isApertureSpads);//设定Spads校准值
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status= VL53L0X_SetRefCalibration(dev,Vl53l0x_adjust.VhvSettings,Vl53l0x_adjust.PhaseCal);//设定Ref校准值
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status=VL53L0X_SetOffsetCalibrationDataMicroMeter(dev,Vl53l0x_adjust.OffsetMicroMeter);//设定偏移校准值
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status=VL53L0X_SetXTalkCompensationRateMegaCps(dev,Vl53l0x_adjust.XTalkCompensationRateMegaCps);//设定串扰校准值
    if(status!=VL53L0X_ERROR_NONE) return status;

    status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);//使能连续测量模式
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetInterMeasurementPeriodMilliSeconds(dev,Mode_data[mode].timingBudget);//设置内部周期测量时间
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,1);//使能SIGMA范围检查
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//使能信号速率范围检查
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//设定SIGMA范围
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//设定信号速率范围范围
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev,Mode_data[mode].timingBudget);//设定完整测距最长时间
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod);//设定VCSEL脉冲周期
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod);//设定VCSEL脉冲周期范围
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_StopMeasurement(dev);//停止测量
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetInterruptThresholds(dev,VL53L0X_DEVICEMODE_CONTINUOUS_RANGING,AlarmModes.ThreshLow, AlarmModes.ThreshHigh);//设定触发中断上、下限值
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_SetGpioConfig(dev,0,VL53L0X_DEVICEMODE_CONTINUOUS_RANGING,AlarmModes.VL53L0X_Mode,VL53L0X_INTERRUPTPOLARITY_LOW);//设定触发中断模式 下降沿
    if(status!=VL53L0X_ERROR_NONE) return status;
    delay_ms(2);
    status = VL53L0X_ClearInterruptMask(dev,0);//清除VL53L0X中断标志位
    return status;
}
