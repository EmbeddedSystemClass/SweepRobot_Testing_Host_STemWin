#include "sweeprobot_testing_stepmotor_task.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"
#include "swrbTestStepMotorDriver.h"
#include "stm32f4xx_it.h"

#include "includes.h"

static u16 gSwrbStepMotorRunCnt = 0;

#define SWRB_STEPMOTOR_TASK_OSTIMEDLY_TIME_SEC  2 

static void SweepRobot_StepMotorTaskISR(void)
{
    STEP_MOTOR_EN_OUT_ENABLE();
    OSTaskResume(SWRB_STEPMOTOR_TASK_PRIO);
}

static void SweepRobot_StepMotorTaskInit(void)
{
    gSwrbTestRuningTaskPrio = SWRB_STEPMOTOR_TASK_PRIO;
    
    plat_int_reg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_INT, SweepRobot_StepMotorTaskISR);
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
        
        OSTimeDlyHMSM(0,0,SWRB_STEPMOTOR_TASK_OSTIMEDLY_TIME_SEC,0);
    }
}
