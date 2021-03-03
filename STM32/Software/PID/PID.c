#include <PID.h>
#include <usart.h>

int16_t incPIDcalc(PIDtypedef* PIDx,double currentpoint)
{
    double Error = (*PIDx).setpoint - currentpoint;

    double Result;

    (*PIDx).sum_error = (*PIDx).sum_error + Error;

    Result =

        (*PIDx).proportion * (Error) +                //P ��������

        (*PIDx).integral * (*PIDx).sum_error +           //I ��������

        (*PIDx).derivative * (Error - (*PIDx).error);    //D ΢������

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
