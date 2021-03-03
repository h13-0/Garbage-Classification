


# 头文件API表

inv_mpu.h
```
static int set_int_enable(unsigned char enable) 模块中断使能函数
int mpu_reg_dump(void) 测试打印函数
int mpu_read_reg(unsigned char reg, unsigned char *data) 3.向芯片读寄存器值，除了MEMERY和FIFO
int mpu_init(void) MPU6050的初始化
int mpu_lp_accel_mode(unsigned char rate) 进入低功耗模式
int mpu_get_gyro_reg(short *data, unsigned long *timestamp) 获取新的原始陀螺仪数据
int mpu_get_accel_reg(short *data, unsigned long *timestamp获取新的原始加速度数据
int mpu_get_temperature(long *data, unsigned long *timestamp) 获取新的温度数据
int mpu_set_accel_bias(const long *accel_bias) 偏差配置函数
int mpu_reset_fifo(void) 重置FIFO函数
int mpu_get_gyro_fsr(unsigned short *fsr) 获得陀螺仪全尺寸范围函数
int mpu_set_gyro_fsr(unsigned short fsr) 设置陀螺仪全尺寸范围函数
int mpu_get_accel_fsr(unsigned char *fsr) 获得加速度全尺寸范围函数
int mpu_set_accel_fsr(unsigned char fsr) 配置加速度全尺寸范围函数
int mpu_get_lpf(unsigned short *lpf) .获得DLPF范围函数
int mpu_set_lpf(unsigned short lpf) 配置DLPF范围函数
int mpu_get_sample_rate(unsigned short *rate) 获得采样频率范围函数
int mpu_set_sample_rate(unsigned short rate) 配置采样频率范围函数
int mpu_get_compass_sample_rate(unsigned short *rate) 获得罗盘采样频率范围函数
int mpu_set_compass_sample_rate(unsigned short rate) 配置罗盘采样频率范围函数
int mpu_get_gyro_sens(float *sens) 获得陀螺仪灵敏度比例因子函数
int mpu_get_accel_sens(unsigned short *sens) 获得加速计灵敏度比例因子函数
int mpu_get_fifo_config(unsigned char *sensors) 获得开启的FIFO通道函数
int mpu_configure_fifo(unsigned char sensors) 配置开启FIFO通道函数
int mpu_get_power_state(unsigned char *power_on) 获得芯片工作状态
int mpu_set_sensors(unsigned char sensors) 配置传感器的时钟和工作状态函数
int mpu_get_int_status(short *status).获得中断状态函数
int mpu_read_fifo(short *gyro, short *accel, unsigned long *timestamp,unsigned char *sensors, unsigned char *more) 获得FIFO数据函数
int mpu_read_fifo_stream(unsigned short length, unsigned char *data,unsigned char *more) 获得FIFO数据长度函数
int mpu_set_bypass(unsigned char bypass_on) 设置旁路模式函数
int mpu_set_int_level(unsigned char active_low) 设置中断优先级函数
int mpu_set_int_latched(unsigned char enable) 设置中断锁存函数-
设置自检函数
static int get_st_biases(long *gyro, long *accel, unsigned char hw_test) 获取所有的偏差值函数
int mpu_run_self_test(long *gyro, long *accel) 行自检值函数
int mpu_write_mem(unsigned short mem_addr, unsigned short length,unsigned char *data) 向DMP写记忆函数
int mpu_read_mem(unsigned short mem_addr, unsigned short length,unsigned char *data) 向DMP读记忆函数
int mpu_load_firmware(unsigned short length, const unsigned char *firmware,unsigned short start_addr, unsigned short sample_rate) 加载并验证DMP映像函数
int mpu_set_dmp_state(unsigned char enable) DMP状态控制函数
int mpu_get_dmp_state(unsigned char *enabled) DMP状态读取函数
```