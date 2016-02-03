#include "swrbTestStepMotorDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"

#include "malloc.h"

#include "delay.h"

#define STEP_MOTOR_PWM_INIT_ARR     100
#define STEP_MOTOR_PWM_INIT_PULSE_WIDTH      STEP_MOTOR_PWM_INIT_ARR/2

#define STEP_MOTOR_MIN_SPEED        1
#define STEP_MOTOR_MAX_SPEED        50
#define STEP_MOTOR_ACCELERATION     0.005
#define STEP_MOTOR_DECELERATION     0.005

#define STEP_MOTOR_STEPS_PER_REV    1600

#define STEP_MOTOR_MAX_STEPS        40000

stepMotorDriverISRCB_t stepMotorDriverISRCB = NULL;

typedef struct{
    float maxSpeed;
    float minSpeed;
    float acceleration;
    float deceleration;
    float runSpeed;
    float expSpeed;
    uint32_t expSteps;
    uint32_t runStepCnt;
    int32_t posStepCnt;
}StepMotorDriverCtrl_TypeDef;

typedef struct{
    enum STEP_MOTOR_MODE mode;
    enum STEP_MOTOR_DIR dir;
    enum STEP_MOTOR_POS pos;
    StepMotorDriverCtrl_TypeDef ctrl;
    FunctionalState enState;
    FunctionalState pwrState;
}StepMotorDriver_TypeDef;
StepMotorDriver_TypeDef stepMotor;

static void StepMotorDriver_PWMTimerISR(void);
static void SweepRobotTest_StepMotorStop(void);

void SweepRobotTest_StepMotorDriverConf(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_STOP;
    stepMotor.dir = STEP_MOTOR_DIR_BACKWARD;
    stepMotor.pos = STEP_MOTOR_POS_UNKNOWN;
    stepMotor.ctrl.minSpeed = STEP_MOTOR_MIN_SPEED;
    stepMotor.ctrl.maxSpeed = STEP_MOTOR_MAX_SPEED;
    stepMotor.ctrl.acceleration = STEP_MOTOR_ACCELERATION;
    stepMotor.ctrl.deceleration = STEP_MOTOR_DECELERATION;
    stepMotor.ctrl.runSpeed = 0;
    stepMotor.ctrl.expSpeed = 0;
    stepMotor.ctrl.expSteps = 0;
}

void SweepRobotTest_StepMotorDriverReset(void)
{
    SweepRobotTest_StepMotorPwrOff();
    SweepRobotTest_StepMotorDriverConf();
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_BACKWARD);
    SweepRobotTest_StepMotorEnStateSet(ENABLE);
}

/* STEP MOTOR DRIVER GPIO AND TIMER INIT */
void SweepRobotTest_StepMotorDriverGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_PERIPH_ID, ENABLE);
    RCC_AHB1PeriphClockCmd(STEP_MOTOR_DRIVER_GPIO_PERIPH_ID, ENABLE);

    GPIO_PinAFConfig(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_PWM_OUT_PIN_SOURCE, STEP_MOTOR_DRIVER_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DRIVER_PWM_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(STEP_MOTOR_DRIVER_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DRIVER_DIR_PIN | STEP_MOTOR_DRIVER_EN_OUT_PIN | STEP_MOTOR_DRIVER_24V_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(STEP_MOTOR_DRIVER_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DRIVER_POS_DETECT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
#ifdef _USE_ACTUAL_POS_DETECT_KEY
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
#else
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
#endif
    GPIO_Init(STEP_MOTOR_DRIVER_GPIO, &GPIO_InitStructure);

    /* SLAVE TIMER INIT */
    TIM_DeInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = STEP_MOTOR_PWM_INIT_ARR-1;
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
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_INIT_PULSE_WIDTH);

    TIM_ITConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel=STEP_MOTOR_DRVIER_GPIO_PWM_OUT_TIM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    plat_int_reg_cb(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_INT, StepMotorDriver_PWMTimerISR);

    TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);

    SweepRobotTest_StepMotorDriverReset();
}

void StepMotorDriver_PWMTimerISR(void)
{
    float stepMotorTimerPeriod = 0;

    stepMotor.ctrl.runStepCnt++;
    
    if(stepMotor.dir == STEP_MOTOR_DIR_FORWARD){
        stepMotor.ctrl.posStepCnt--;
    }else{
        stepMotor.ctrl.posStepCnt++;
    }

#ifdef _USE_ACTUAL_POS_DETECT_KEY
    if( !STEP_MOTOR_POS_DETECT_SIGN && (stepMotor.dir == STEP_MOTOR_DIR_FORWARD) ){
#else
    if( STEP_MOTOR_POS_DETECT_SIGN && (stepMotor.dir == STEP_MOTOR_DIR_BACKWARD) ){
#endif
        stepMotor.ctrl.runStepCnt = 0;
        stepMotor.ctrl.posStepCnt = 0;
        stepMotor.mode = STEP_MOTOR_MODE_STOP;
        stepMotor.pos = STEP_MOTOR_POS_HOME;
        STEP_MOTOR_TIM_SET_STOP();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else{
        if(stepMotor.mode == STEP_MOTOR_MODE_RUN_POS){
            
        }else if(stepMotor.mode == STEP_MOTOR_MODE_RUN_STEP){
            if(stepMotor.ctrl.runStepCnt >= stepMotor.ctrl.expSteps || stepMotor.ctrl.runStepCnt >= STEP_MOTOR_MAX_STEPS){
                stepMotor.ctrl.runStepCnt = 0;
                stepMotor.mode = STEP_MOTOR_MODE_STOP;
                STEP_MOTOR_TIM_SET_STOP();
                STEP_MOTOR_EN_OUT_ENABLE();
            }
        }else if( stepMotor.mode == STEP_MOTOR_MODE_RUN ){
            if(stepMotor.ctrl.runStepCnt > STEP_MOTOR_MAX_STEPS){
                stepMotor.ctrl.runStepCnt = 0;
                stepMotor.mode = STEP_MOTOR_MODE_STOP;
                STEP_MOTOR_TIM_SET_STOP();
                STEP_MOTOR_EN_OUT_ENABLE();
            }
        }else if( stepMotor.mode == STEP_MOTOR_MODE_HOMING ){
            
        }else{
            
        }
    }
    
    if(NULL != stepMotorDriverISRCB){
        stepMotorDriverISRCB();
    }
    
    /* Acceleration and Deceleration procees when step motor is running */
    if(stepMotor.mode != STEP_MOTOR_MODE_STOP){
        if( (stepMotor.ctrl.runStepCnt < (stepMotor.ctrl.expSpeed - stepMotor.ctrl.minSpeed)/stepMotor.ctrl.acceleration) && (stepMotor.ctrl.runStepCnt < stepMotor.ctrl.expSteps/2) ){
            if(stepMotor.ctrl.runSpeed < stepMotor.ctrl.expSpeed){
                stepMotor.ctrl.runSpeed += stepMotor.ctrl.acceleration;

                stepMotorTimerPeriod = 625/stepMotor.ctrl.runSpeed;
                TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)stepMotorTimerPeriod);
                TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)(stepMotorTimerPeriod/2));
            }
        }else if(stepMotor.ctrl.runStepCnt > (stepMotor.ctrl.expSteps - (stepMotor.ctrl.runSpeed - stepMotor.ctrl.minSpeed)/stepMotor.ctrl.deceleration)){
            if(stepMotor.ctrl.runSpeed > stepMotor.ctrl.minSpeed){
                stepMotor.ctrl.runSpeed -= stepMotor.ctrl.deceleration;

                stepMotorTimerPeriod = 625/stepMotor.ctrl.runSpeed;
                TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)stepMotorTimerPeriod);
                TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)(stepMotorTimerPeriod/2));
            }
        }
    }
}

void SweepRobotTest_StepMotorPwrOn(void)
{
    stepMotor.pwrState = ENABLE;
    STEP_MOTOR_PWR_ON();
}

void SweepRobotTest_StepMotorPwrOff(void)
{
    stepMotor.pwrState = DISABLE;
    STEP_MOTOR_PWR_OFF();
}

FunctionalState SweepRobotTest_StepMotorPwrStateGet(void)
{
    return stepMotor.pwrState;
}

static void SweepRobotTest_StepMotorRun(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_RUN;
    stepMotor.ctrl.expSteps = STEP_MOTOR_MAX_STEPS;
    STEP_MOTOR_TIM_SET_RUN();
}

static void SweepRobotTest_StepMotorStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_STOP;
    stepMotor.ctrl.expSteps = 0;
    STEP_MOTOR_TIM_SET_STOP();
}

/*
 * @param speed: revolution per sec ,1revolution=1600steps at 8 div, max=625
 * @param steps: expect steps to move
 */
void SweepRobotTest_StepMotorMoveSteps(float speed, u16 steps)
{
    float stepMotorTimerPeriod = 0;

    /* with Acc and Dec */
    if(speed > stepMotor.ctrl.maxSpeed){
        stepMotor.ctrl.expSpeed = stepMotor.ctrl.maxSpeed;
    }else if(speed < stepMotor.ctrl.minSpeed) {
        stepMotor.ctrl.expSpeed = stepMotor.ctrl.minSpeed;
    }else{
        stepMotor.ctrl.expSpeed = speed;
    }
    stepMotor.ctrl.expSteps = steps;

    stepMotor.ctrl.runSpeed = stepMotor.ctrl.minSpeed;

    stepMotorTimerPeriod = 1000000/(stepMotor.ctrl.runSpeed*STEP_MOTOR_STEPS_PER_REV);
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)stepMotorTimerPeriod);

    stepMotor.mode = STEP_MOTOR_MODE_RUN_STEP;

    STEP_MOTOR_EN_OUT_DISABLE();

    stepMotor.ctrl.runStepCnt = 0;
    STEP_MOTOR_TIM_SET_RUN();
}

void SweepRobotTest_StepMotorModeSetRun(enum STEP_MOTOR_MODE mode)
{
    if(mode == STEP_MOTOR_MODE_RUN){
        STEP_MOTOR_EN_OUT_DISABLE();
        SweepRobotTest_StepMotorRun();
    }else if(mode == STEP_MOTOR_MODE_STOP){
        SweepRobotTest_StepMotorStop();
        stepMotor.ctrl.runStepCnt = 0;
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
    }else
        ;
}

enum STEP_MOTOR_DIR SweepRobotTest_StepMotorDirGet(void)
{
    return stepMotor.dir;
}

static void SweepRobotTest_StepMotorEnEnable(void)
{
    stepMotor.enState = ENABLE;
    STEP_MOTOR_EN_OUT_ENABLE();
}

static void SweepRobotTest_StepMotorEnDisable(void)
{
    stepMotor.enState = DISABLE;
    STEP_MOTOR_EN_OUT_DISABLE();
}

void SweepRobotTest_StepMotorEnStateSet(FunctionalState enState)
{
    if(enState == ENABLE){
        SweepRobotTest_StepMotorEnEnable();
    }else if(enState == DISABLE){
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
    float stepMotorTimerPeriod;

    stepMotor.ctrl.runSpeed = speed;
    stepMotorTimerPeriod = 1000000/(speed*STEP_MOTOR_STEPS_PER_REV);
    TIM_SetAutoreload(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, (uint32_t)stepMotorTimerPeriod);
}

float SweepRobotTest_StepMotorSpeedGet(void)
{
    return stepMotor.ctrl.runSpeed;
}

int SweepRobotTest_StepMotorStepsGet(void)
{
    return stepMotor.ctrl.runStepCnt;
}

void SweepRobotTest_StepMotorPosSet(enum STEP_MOTOR_POS pos)
{
    stepMotor.pos = pos;
}

enum STEP_MOTOR_POS SweepRobotTest_StepMotorPosGet(void)
{
    return stepMotor.pos;
}

int32_t SweepRobotTest_StepMotorPosStepCntGet(void)
{
    return stepMotor.ctrl.posStepCnt;
}

/* TODO: Add Absolute Position Move function */
void SweepRobotTest_StepMotorGotoPos(enum STEP_MOTOR_POS pos)
{
    switch(pos){
        case STEP_MOTOR_POS_HOME:
            break;
        case STEP_MOTOR_POS_MARK1:
            break;
        default:break;
    }
}

void SweepRobotTest_StepMotorGoHome(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_HOMING;
    
#ifdef _USE_ACTUAL_POS_DETECT_KEY
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_FORWARD);
#else
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_BACKWARD);
#endif
    SweepRobotTest_StepMotorModeSetRun(STEP_MOTOR_MODE_RUN);
}

void SweepRobotTest_StepMotorSetIdle(void)
{
    STEP_MOTOR_EN_OUT_ENABLE();
    SweepRobotTest_StepMotorStop();
    stepMotor.ctrl.runStepCnt = 0;
}

