/**********C语言标准库**********/

/**********STM32标准库**********/

/**********项目所需库***********/
#include "vl53l0x_easy_api.h"
#include "SystemClock.h"
#include "sys.h"
#include "usart.h"

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

_vl53l0x_adjust Vl53l0x_adjust;           //与vl53l0x_it.c共用

void Vl53l0x_adjust_init()
{
    Vl53l0x_adjust.isApertureSpads = 0;
    Vl53l0x_adjust.VhvSettings     = 33;
    Vl53l0x_adjust.PhaseCal        = 1;
    Vl53l0x_adjust.XTalkCalDistance = 100;
    Vl53l0x_adjust.XTalkCompensationRateMegaCps = 0;
    Vl53l0x_adjust.CalDistanceMilliMeter = 100<<16;
    Vl53l0x_adjust.OffsetMicroMeter = -3000;
    Vl53l0x_adjust.refSpadCount = 4;
}

//vl53l0x模式配置参数集
typedef __packed struct
{
    FixPoint1616_t signalLimit;    //Signal极限数值
    FixPoint1616_t sigmaLimit;     //Sigmal极限数值
    uint32_t timingBudget;         //采样时间周期
    uint8_t preRangeVcselPeriod ;  //VCSEL脉冲周期
    uint8_t finalRangeVcselPeriod ;//VCSEL脉冲周期范围

} mode_data;

//VL53L0X各测量模式参数, 与vl53l0x_it.c共用
//0：默认;1:高精度;2:长距离;3:高速
mode_data Mode_data[]=
{
    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(18*65536),
        33000,
        14,
        10
    },//默认

    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(18*65536),
        200000,
        14,
        10
    },//高精度

    {   (FixPoint1616_t)(0.1*65536),
        (FixPoint1616_t)(60*65536),
        33000,
        18,
        14
    },//长距离

    {   (FixPoint1616_t)(0.25*65536),
        (FixPoint1616_t)(32*65536),
        20000,
        14,
        10
    },//高速

};

//配置VL53L0X设备I2C地址
//dev:设备I2C参数结构体
//newaddr:设备新I2C地址
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev,uint8_t newaddr)
{
    uint16_t Id;
    uint8_t FinalAddress;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    u8 sta=0x00;

    FinalAddress = newaddr;

    if(FinalAddress==dev->I2cDevAddr)//新设备I2C地址与旧地址一致,直接退出
        return VL53L0X_ERROR_NONE;
    //在进行第一个寄存器访问之前设置I2C标准模式(400Khz)
    Status = VL53L0X_WrByte(dev,0x88,0x00);
    if(Status!=VL53L0X_ERROR_NONE)
    {
        sta=0x01;//设置I2C标准模式出错
        return Status;
    }
    //尝试使用默认的0x52地址读取一个寄存器
    Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
    if(Status!=VL53L0X_ERROR_NONE)
    {
        sta=0x02;//读取寄存器出错
        return Status;
    }
    if(Id == 0xEEAA)
    {
        //设置设备新的I2C地址
        Status = VL53L0X_SetDeviceAddress(dev,FinalAddress);
        if(Status!=VL53L0X_ERROR_NONE)
        {
            sta=0x03;//设置I2C地址出错
            return Status;
        }
        //修改参数结构体的I2C地址
        dev->I2cDevAddr = FinalAddress;
        //检查新的I2C地址读写是否正常
        Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
        if(Status!=VL53L0X_ERROR_NONE)
        {
            sta=0x04;//新I2C地址读写出错
            return Status;
        }
    }

    if(sta!=0)
        printf("sta:0x%x\r\n",sta);
    return Status;
}

//初始化vl53l0x
//dev:设备I2C参数结构体
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    VL53L0X_Dev_t *pMyDevice = dev;

    RCC_Enable();

    GPIO_InitStructure.GPIO_Pin = Xshut_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Xshut_Port, &GPIO_InitStructure);

    pMyDevice->I2cDevAddr = VL53L0X_Addr;//I2C地址(上电默认0x52)
    pMyDevice->comms_type = 1;           //I2C通信模式
    pMyDevice->comms_speed_khz = 400;    //I2C通信速率

    VL53L0X_i2c_init();//初始化IIC总线

    VL53L0X_Xshut=0;//失能VL53L0X
    delay_ms(30);
    VL53L0X_Xshut=1;//使能VL53L0X,让传感器处于工作
    delay_ms(30);

    vl53l0x_Addr_set(pMyDevice,0x54);//设置VL53L0X传感器I2C地址
    if(Status!=VL53L0X_ERROR_NONE) return Status;
    Status = VL53L0X_DataInit(pMyDevice);//设备初始化
    if(Status!=VL53L0X_ERROR_NONE) return Status;
    delay_ms(2);

    Vl53l0x_adjust_init();

    return Status;
}

//vl53l0x复位函数
//dev:设备I2C参数结构体
void vl53l0x_reset(VL53L0X_Dev_t *dev)
{
    uint8_t addr;
    addr = dev->I2cDevAddr;//保存设备原I2C地址
    VL53L0X_Xshut=0;//失能VL53L0X
    delay_ms(30);
    VL53L0X_Xshut=1;//使能VL53L0X,让传感器处于工作(I2C地址会恢复默认0X52)
    delay_ms(30);
    dev->I2cDevAddr=0x52;
    vl53l0x_Addr_set(dev,addr);//设置VL53L0X传感器原来上电前原I2C地址
    VL53L0X_DataInit(dev);
}

//VL53L0X 测量模式配置
//dev:设备I2C参数结构体
//mode: 0:默认;1:高精度;2:长距离
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode)
{

    VL53L0X_Error status = VL53L0X_ERROR_NONE;

    vl53l0x_reset(dev);//复位vl53l0x(频繁切换工作模式容易导致采集距离数据不准，需加上这一代码)
    status = VL53L0X_StaticInit(dev);

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
    delay_ms(2);

    status = VL53L0X_SetDeviceMode(dev,VL53L0X_DEVICEMODE_SINGLE_RANGING);//使能单次测量模式
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

    return status;
}



VL53L0X_Error vl53l0x_adjust(VL53L0X_Dev_t *dev)
{
    VL53L0X_DeviceError Status = VL53L0X_ERROR_NONE;

    uint32_t refSpadCount;
    uint8_t  isApertureSpads;
    uint32_t XTalkCalDistance = 100;
    uint32_t XTalkCompensationRateMegaCps;
    uint32_t CalDistanceMilliMeter = 100<<16;
    int32_t  OffsetMicroMeter;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    u8 i=0;

    VL53L0X_StaticInit(dev);//数值恢复默认,传感器处于空闲状态
    //SPADS校准----------------------------
spads:
    delay_ms(10);
    printf("The SPADS Calibration Start...\r\n");
    Status = VL53L0X_PerformRefSpadManagement(dev,&refSpadCount,&isApertureSpads);//执行参考Spad管理
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("refSpadCount = %d\r\n",refSpadCount);
        Vl53l0x_adjust.refSpadCount = refSpadCount;
        printf("isApertureSpads = %d\r\n",isApertureSpads);
        Vl53l0x_adjust.isApertureSpads = isApertureSpads;
        printf("The SPADS Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjust_num) return Status;
        printf("SPADS Calibration Error,Restart this step\r\n");
        goto spads;
    }
    //设备参考校准---------------------------------------------------
ref:
    delay_ms(10);
    printf("The Ref Calibration Start...\r\n");
    Status = VL53L0X_PerformRefCalibration(dev,&VhvSettings,&PhaseCal);//Ref参考校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("VhvSettings = %d\r\n",VhvSettings);
        Vl53l0x_adjust.VhvSettings = VhvSettings;
        printf("PhaseCal = %d\r\n",PhaseCal);
        Vl53l0x_adjust.PhaseCal = PhaseCal;
        printf("The Ref Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjust_num) return Status;
        printf("Ref Calibration Error,Restart this step\r\n");
        goto ref;
    }
    //偏移校准------------------------------------------------
offset:
    delay_ms(10);
    printf("Offset Calibration:need a white target,in black space,and the distance keep 100mm!\r\n");
    printf("The Offset Calibration Start...\r\n");
    Status = VL53L0X_PerformOffsetCalibration(dev,CalDistanceMilliMeter,&OffsetMicroMeter);//偏移校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("CalDistanceMilliMeter = %d mm\r\n",CalDistanceMilliMeter);
        Vl53l0x_adjust.CalDistanceMilliMeter = CalDistanceMilliMeter;
        printf("OffsetMicroMeter = %d mm\r\n",OffsetMicroMeter);
        Vl53l0x_adjust.OffsetMicroMeter = OffsetMicroMeter;
        printf("The Offset Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjust_num) return Status;
        printf("Offset Calibration Error,Restart this step\r\n");
        goto offset;
    }
    //串扰校准-----------------------------------------------------
xtalk:
    delay_ms(20);
    printf("Cross Talk Calibration:need a grey target\r\n");
    printf("The Cross Talk Calibration Start...\r\n");
    Status = VL53L0X_PerformXTalkCalibration(dev,XTalkCalDistance,&XTalkCompensationRateMegaCps);//串扰校准
    if(Status == VL53L0X_ERROR_NONE)
    {
        printf("XTalkCalDistance = %d mm\r\n",XTalkCalDistance);
        Vl53l0x_adjust.XTalkCalDistance = XTalkCalDistance;
        printf("XTalkCompensationRateMegaCps = %d\r\n",XTalkCompensationRateMegaCps);
        Vl53l0x_adjust.XTalkCompensationRateMegaCps = XTalkCompensationRateMegaCps;
        printf("The Cross Talk Calibration Finish...\r\n\r\n");
        i=0;
    }
    else
    {
        i++;
        if(i==adjust_num) return Status;
        printf("Cross Talk Calibration Error,Restart this step\r\n");
        goto xtalk;
    }

    printf("Copy the following code to vl53l0x_easy_api.c's Vl53l0x_adjust_init() function\r\n\r\n");

    printf("Vl53l0x_adjust.isApertureSpads: %d;\r\n",isApertureSpads);
    printf("Vl53l0x_adjust.VhvSettings: %d;\r\n",VhvSettings);
    printf("Vl53l0x_adjust.PhaseCal %d;\r\n",PhaseCal);
    printf("Vl53l0x_adjust.XTalkCalDistance: %d;\r\n",XTalkCalDistance);
    printf("Vl53l0x_adjust.XTalkCompensationRateMegaCps: %d;\r\n",XTalkCompensationRateMegaCps);
    printf("Vl53l0x_adjust.CalDistanceMilliMeter: %d;\r\n",CalDistanceMilliMeter);
    printf("Vl53l0x_adjust.OffsetMicroMeter: %d;\r\n",OffsetMicroMeter);
    printf("Vl53l0x_adjust.refSpadCount: %d;\r\n",refSpadCount);
    printf("\r\n");

    printf("All the Calibration has Finished!\r\n");
    printf("Calibration is successful!!\r\n");

    return Status;
}
