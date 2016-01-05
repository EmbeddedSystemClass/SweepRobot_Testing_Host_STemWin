#include "sweeprobot_testing_stepmotor_task.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "swrbTestStepMotorDriver.h"
#include "stm32f4xx_it.h"

#include "includes.h"

static u16 gSwrbStepMotorTaskInitDlyTimeMSec = 10;
static u16 gSwrbStepMotorTaskDlyTimeSec = 1;

static void SweepRobot_StepMotorTaskInit(void)
{
    
}

static void SweepRobot_StepMotorTaskMoveProc(void)
{
    SweepRobotTest_StepMotorMoveSteps(1,800);
    OSTaskSuspend(OS_PRIO_SELF);
}

static void SweepRobot_StepMotorTaskMoveFinishProc(void)
{
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
