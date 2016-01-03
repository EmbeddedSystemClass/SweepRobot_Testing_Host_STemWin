#include "sweeprobot_testing_stepmotor_task.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "swrbTestStepMotorDriver.h"
#include "stm32f4xx_it.h"

#include "includes.h"

static u16 gSwrbStepMotorTaskInitDlyTimeMSec = 10;
static u16 gSwrbStepMotorTaskDlyTimeSec = 1;

static void SweepRobot_StepMotorOnePulseTimerISR(void)
{
    STEP_MOTOR_EN_OUT_ENABLE();
    SweepRobotTest_StepMotorModeSet(STEP_MOTOR_MODE_STOP);
    OSTaskResume(SWRB_STEPMOTOR_TASK_PRIO);
}

static void SweepRobot_StepMotorTaskInit(void)
{
//    gSwrbTestRuningTaskPrio = SWRB_STEPMOTOR_TASK_PRIO;

    plat_int_reg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_INT, SweepRobot_StepMotorOnePulseTimerISR);
}

static void SweepRobot_StepMotorTaskMoveProc(void)
{
    SweepRobotTest_StepMotorMoveSteps(1,800);
    OSTaskSuspend(OS_PRIO_SELF);
}

static void SweepRobot_StepMotorTaskMoveFinishProc(void)
{
    plat_int_dereg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_INT);
    gSwrbTestTaskRunCnt = 0;
    OSTaskSuspend(OS_PRIO_SELF);
}

void SweepRobot_StepMotorTask(void *pdata)
{
    while(1){
        gSwrbTestTaskRunCnt++;

        if(gSwrbTestTaskRunCnt == 1){
            SweepRobot_StepMotorTaskInit();
        }

        if(gSwrbTestTaskRunCnt > 1){
            SweepRobot_StepMotorTaskMoveProc();
        }

        if(gSwrbTestTaskRunCnt > 3){
            SweepRobot_StepMotorTaskMoveFinishProc();
        }
        
        if(gSwrbTestTaskRunCnt == 1){
            OSTimeDlyHMSM(0,0,0,gSwrbStepMotorTaskInitDlyTimeMSec);
        }else{
            OSTimeDlyHMSM(0,0,gSwrbStepMotorTaskDlyTimeSec,0);
        }
    }
}
