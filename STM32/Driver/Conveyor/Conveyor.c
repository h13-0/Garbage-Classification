#include "Conveyor.h"

#include "Stepper.h"

static Stepper_TypeDef Conveyor;

void ConveyorInit()
{
		Conveyor = Stepper_Init(CONVEYOR_DIR_PORT, CONVEYOR_DIR_PIN, CONVEYOR_STEP_PORT, CONVEYOR_STEP_PIN);
}

void ConveyorForward()
{
		Stepper_ForwardMode(&Conveyor);
		while(1)
			Stepper_SingleStep(&Conveyor, CONVEYOR_DELAY_PER_HALF_SETP);
}
