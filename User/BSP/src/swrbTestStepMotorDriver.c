#include "swrbTestStepMotorDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"

#include "delay.h"

typedef struct{
    enum STEP_MOTOR_MODE mode;
    enum STEP_MOTOR_DIR dir;
    enum STEP_MOTOR_SPEED speed;
    enum STEP_MOTOR_POS pos;
    uint16_t    step;
    FunctionalState enState;
}StepMotorDriver_TypeDef;
StepMotorDriver_TypeDef stepMotor;

/* ISR METHOD */
//static uint16_t gStepMotorEXPStep = 0;

static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_ISR(void);
static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR(void);
static void SweepRobotTest_StepMotorSoftStop(void);

/* STEP MOTOR DRIVER GPIO AND TIMER INIT */
void SweepRobotTest_StepMotorDriverGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(STEP_MOTOR_DRIVER_GPIO_RCC, ENABLE);

    GPIO_PinAFConfig(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_PWM_OUT_PIN_SOURCE, STEP_MOTOR_DRIVER_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DRIVER_PWM_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(STEP_MOTOR_DRIVER_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DRIVER_DIR_PIN | STEP_MOTOR_DRIVER_EN_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(STEP_MOTOR_DRIVER_GPIO, &GPIO_InitStructure);

    /* MASTER TIMER INIT */
    TIM_DeInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 16800-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 1;
    TIM_OC2Init(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM,&TIM_OCInitStructure);

    TIM_ITConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=STEP_MOTOR_DRVIER_GPIO_PWM_OUT_MASTER_TIM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    plat_int_reg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_INT, STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_ISR);

    TIM_SelectOnePulseMode(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, TIM_OPMode_Single);
    TIM_OC2PreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, TIM_OCPreload_Enable);
    TIM_SelectOutputTrigger(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, TIM_TRGOSource_OC2Ref);

    TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, DISABLE);

    /* SLAVE TIMER INIT */
    TIM_DeInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1680-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_OCPreload_Enable);
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);

    TIM_SelectSlaveMode(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_SlaveMode_Gated);
    TIM_SelectInputTrigger(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_TS_ITR2);
    TIM_SelectMasterSlaveMode(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_MasterSlaveMode_Enable);

    /* ISR METHOD */
    TIM_ITConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_IT_Update, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel=STEP_MOTOR_DRVIER_GPIO_PWM_OUT_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    plat_int_reg_cb(STM32F4xx_INT_TIM3, STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR);

    TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, ENABLE);

    SweepRobotTest_StepMotorDriverReset();
}

static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM_ISR(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_HARD_STOP;
    STEP_MOTOR_EN_OUT_ENABLE();
}

static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR(void)
{
    stepMotor.step++;
}

static void SweepRobotTest_StepMotorRun(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_RUN;
    STEP_MOTOR_MODE_SET_RUN();
}

static void SweepRobotTest_StepMotorSoftStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_SOFT_STOP;
    STEP_MOTOR_MODE_SET_SOFT_STOP();
}

static void SweepRobotTest_StepMotorHardStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_HARD_STOP;
    STEP_MOTOR_MODE_SET_HARD_STOP();
}

static void SweepRobotTest_StepMotorOn(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_ON;
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);
}

static void SweepRobotTest_StepMotorOff(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_OFF;
    STEP_MOTOR_MODE_SET_HARD_STOP();
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, 0);
}

/* ISR METHOD */
//void SweepRobotTest_StepMotorMoveSteps2(u16 steps)
//{
//    gStepMotorEXPStep = steps;

//    TIM_ITConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_IT_Update, ENABLE);
//    SweepRobotTest_StepMotorRun();
//}

/* ONE PULSE MODE METHOD*/
void SweepRobotTest_StepMotorMoveSteps(int period_ms, u16 steps)
{
    uint32_t masterTimerPeriod = 0;
    uint32_t slaveTimerPeriod = 0;

    masterTimerPeriod = (period_ms*steps)*10;       //((period_ms*1000)*steps)/100
    slaveTimerPeriod = period_ms*100;               //(period_ms*1000/10

    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_MASTER_TIM, masterTimerPeriod);
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, slaveTimerPeriod);

    stepMotor.mode = STEP_MOTOR_MODE_RUN_STEP;

    STEP_MOTOR_EN_OUT_DISABLE();

    STEP_MOTOR_MODE_SET_RUN();
}

void SweepRobotTest_StepMotorModeSetRun(enum STEP_MOTOR_MODE mode)
{
    if(mode == STEP_MOTOR_MODE_RUN){
        STEP_MOTOR_EN_OUT_DISABLE();
        SweepRobotTest_StepMotorRun();
    }else if(mode == STEP_MOTOR_MODE_SOFT_STOP){
        SweepRobotTest_StepMotorSoftStop();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_HARD_STOP){
        SweepRobotTest_StepMotorHardStop();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_ON){
        SweepRobotTest_StepMotorOn();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_OFF){
        SweepRobotTest_StepMotorOff();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else{
        stepMotor.mode = STEP_MOTOR_MODE_UNKNOWN;
        STEP_MOTOR_EN_OUT_ENABLE();
    }
}

void SweepRobotTest_StepMotorModeSet(enum STEP_MOTOR_MODE mode)
{
    stepMotor.mode = mode;
}

enum STEP_MOTOR_MODE SweepRobotTest_StepMotorModeGet(void)
{
    return stepMotor.mode;
}

void SweepRobotTest_StepMotorDirSet(enum STEP_MOTOR_DIR dir)
{
    if(dir == STEP_MOTOR_DIR_FORWARD){
        stepMotor.dir = STEP_MOTOR_DIR_FORWARD;
        STEP_MOTOR_DIR_SET_FORWARD();
    }else if(dir == STEP_MOTOR_DIR_BACKWARD){
        stepMotor.dir = STEP_MOTOR_DIR_BACKWARD;
        STEP_MOTOR_DIR_SET_BACKWARD();
    }else{

    }
}

enum STEP_MOTOR_DIR SweepRobotTest_StepMotorDirGet(void)
{
    return stepMotor.dir;
}

static void SweepRobotTest_StepMotorEnEnable(void)
{
    STEP_MOTOR_EN_OUT_ENABLE();
}

static void SweepRobotTest_StepMotorEnDisable(void)
{
    STEP_MOTOR_EN_OUT_DISABLE();
}

void SweepRobotTest_StepMotorEnStateSet(FunctionalState enState)
{
    if(enState == ENABLE){
        stepMotor.enState = ENABLE;
        SweepRobotTest_StepMotorEnEnable();
    }else if(enState == DISABLE){
        stepMotor.enState = DISABLE;
        SweepRobotTest_StepMotorEnDisable();
    }else{

    }
}

FunctionalState SweepRobotTest_StepMotorEnStateGet(void)
{
    return stepMotor.enState;
}

void SweepRobotTest_StepMotorSpeedSet(enum STEP_MOTOR_SPEED speed)
{
    stepMotor.speed = speed;
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, speed);
}

enum STEP_MOTOR_SPEED SweepRobotTest_StepMotorSpeedGet(void)
{
    return stepMotor.speed;
}

int SweepRobotTest_StepMotorStepsGet(void)
{
    int stepCnt;
    
    stepCnt = stepMotor.step;
    
    return stepCnt;
}

void SweepRobotTest_StepMotorGotoPos(u8 pos)
{

}

void SweepRobotTest_StepMotorDriverReset(void)
{
    SweepRobotTest_StepMotorModeSet(STEP_MOTOR_MODE_ON);
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_FORWARD);
    SweepRobotTest_StepMotorSpeedSet(STEP_MOTOR_SPEED_LOW);
    SweepRobotTest_StepMotorEnStateSet(ENABLE);
}

