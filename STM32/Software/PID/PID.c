#include <PID.h>
#include <usart.h>

int16_t incPIDcalc(PIDtypedef* PIDx,double currentpoint)
{
    double Error = (*PIDx).setpoint - currentpoint;

    double Result;

    (*PIDx).sum_error = (*PIDx).sum_error + Error;

    Result =

        (*PIDx).proportion * (Error) +                //P 比例运算

        (*PIDx).integral * (*PIDx).sum_error +           //I 积分运算

        (*PIDx).derivative * (Error - (*PIDx).error);    //D 微分运算

    (*PIDx).error = Error;
	
	//printf("Result:%f\r\n",Result);

    if(Result <= 32767)
        if(Result >= -32768)
            return Result;
				else
					  return -32768;
    else
        return 32767;
}
