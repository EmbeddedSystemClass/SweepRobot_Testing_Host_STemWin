#include "swrbTestStepMotorDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"

#include "malloc.h"

#include "delay.h"

#define STEP_MOTOR_MIN_SPEED        1
#define STEP_MOTOR_MAX_SPEED        10
#define STEP_MOTOR_ACCELERATION     0.005
#define STEP_MOTOR_DECELERATION     0.01

#define STEP_MOTOR_STEPS_PER_REV    1600

typedef struct{
    float maxspeed;
    float minspeed;
    float acceleration;
    float deceleration;
    float runspeed;
    float expspeed;
    uint32_t expsteps;
    uint32_t stepcnt;
}StepMotorDriverCtrl_TypeDef;

typedef struct{
    enum STEP_MOTOR_MODE mode;
    enum STEP_MOTOR_DIR dir;
    enum STEP_MOTOR_POS pos;
    StepMotorDriverCtrl_TypeDef ctrl;
    FunctionalState enState;
}StepMotorDriver_TypeDef;
StepMotorDriver_TypeDef stepMotor;

static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR(void);
static void SweepRobotTest_StepMotorStop(void);

void SweepRobotTest_StepMotorDriverConf(void)
{
    stepMotor.ctrl.minspeed = STEP_MOTOR_MIN_SPEED;
    stepMotor.ctrl.maxspeed = STEP_MOTOR_MAX_SPEED;
    stepMotor.ctrl.acceleration = STEP_MOTOR_ACCELERATION;
    stepMotor.ctrl.deceleration = STEP_MOTOR_DECELERATION;
    stepMotor.ctrl.runspeed = 0;
    stepMotor.ctrl.expspeed = 0;
    stepMotor.ctrl.expsteps = 0;
}

void SweepRobotTest_StepMotorDriverReset(void)
{
    SweepRobotTest_StepMotorDriverConf();
    SweepRobotTest_StepMotorModeSet(STEP_MOTOR_MODE_ON);
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_FORWARD);
    SweepRobotTest_StepMotorEnStateSet(ENABLE);
}

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

    /* SLAVE TIMER INIT */
    TIM_DeInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = 100-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_OCInitStructure);

    TIM_ARRPreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, ENABLE);
    TIM_OC1PreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_OCPreload_Enable);
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);

    TIM_ITConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=STEP_MOTOR_DRVIER_GPIO_PWM_OUT_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    plat_int_reg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_INT, STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR);

    TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);

    SweepRobotTest_StepMotorDriverReset();
}

static void STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_ISR(void)
{
    float slaveTimerPeriod = 0;

    stepMotor.ctrl.stepcnt++;

    if(stepMotor.ctrl.stepcnt >= stepMotor.ctrl.expsteps){
        stepMotor.ctrl.stepcnt = 0;
        STEP_MOTOR_MODE_SET_STOP();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else{
        if( (stepMotor.ctrl.stepcnt < (stepMotor.ctrl.expspeed - stepMotor.ctrl.minspeed)/stepMotor.ctrl.acceleration) && (stepMotor.ctrl.stepcnt < stepMotor.ctrl.expsteps/2) ){
            if(stepMotor.ctrl.runspeed < stepMotor.ctrl.expspeed){
                stepMotor.ctrl.runspeed += stepMotor.ctrl.acceleration;

                slaveTimerPeriod = 625/stepMotor.ctrl.runspeed;
                TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)slaveTimerPeriod);
                TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)(slaveTimerPeriod/2));
            }
        }else if(stepMotor.ctrl.stepcnt > (stepMotor.ctrl.expsteps - (stepMotor.ctrl.runspeed - stepMotor.ctrl.minspeed)/stepMotor.ctrl.deceleration)){
            if(stepMotor.ctrl.runspeed > stepMotor.ctrl.minspeed){
                stepMotor.ctrl.runspeed -= stepMotor.ctrl.deceleration;

                slaveTimerPeriod = 625/stepMotor.ctrl.runspeed;
                TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)slaveTimerPeriod);
                TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)(slaveTimerPeriod/2));
            }
        }
    }
}

static void SweepRobotTest_StepMotorRun(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_RUN;
    STEP_MOTOR_MODE_SET_RUN();
}

static void SweepRobotTest_StepMotorStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_STOP;
    STEP_MOTOR_MODE_SET_STOP();
}

static void SweepRobotTest_StepMotorOn(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_ON;
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);
}

static void SweepRobotTest_StepMotorOff(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_OFF;
    STEP_MOTOR_MODE_SET_STOP();
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, 0);
}

/*
 * @param speed: revolution per sec ,1revolution=1600steps at 8 div, max=625
 * @param steps: expect steps to move
 */
void SweepRobotTest_StepMotorMoveSteps(float speed, u16 steps)
{
    float slaveTimerPeriod = 0;

    /* with Acc and Dec */
    if(speed > stepMotor.ctrl.maxspeed){
        stepMotor.ctrl.expspeed = stepMotor.ctrl.maxspeed;
    }else if(speed < stepMotor.ctrl.minspeed) {
        stepMotor.ctrl.expspeed = stepMotor.ctrl.minspeed;
    }else{
        stepMotor.ctrl.expspeed = speed;
    }
    stepMotor.ctrl.expsteps = steps;

    stepMotor.ctrl.runspeed = stepMotor.ctrl.minspeed;

    slaveTimerPeriod = 1000000/(stepMotor.ctrl.runspeed*STEP_MOTOR_STEPS_PER_REV);
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)slaveTimerPeriod);

    stepMotor.mode = STEP_MOTOR_MODE_RUN_STEP;

    STEP_MOTOR_EN_OUT_DISABLE();

    stepMotor.ctrl.stepcnt = 0;
    STEP_MOTOR_MODE_SET_RUN();
}

void SweepRobotTest_StepMotorModeSetRun(enum STEP_MOTOR_MODE mode)
{
    if(mode == STEP_MOTOR_MODE_RUN){
        STEP_MOTOR_EN_OUT_DISABLE();
        SweepRobotTest_StepMotorRun();
    }else if(mode == STEP_MOTOR_MODE_STOP){
        SweepRobotTest_StepMotorStop();
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

void SweepRobotTest_StepMotorSpeedSet(float speed)
{
    float slaveTimerPeriod;

    stepMotor.ctrl.runspeed = speed;
    slaveTimerPeriod = 1000000/(speed*STEP_MOTOR_STEPS_PER_REV);
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)slaveTimerPeriod);
}

float SweepRobotTest_StepMotorSpeedGet(void)
{
    return stepMotor.ctrl.runspeed;
}

int SweepRobotTest_StepMotorStepsGet(void)
{
    return stepMotor.ctrl.stepcnt;
}

void SweepRobotTest_StepMotorGotoPos(u8 pos)
{

}

