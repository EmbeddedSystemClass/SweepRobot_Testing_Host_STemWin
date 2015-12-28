#include "swrbTestStepMotorDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "delay.h"

#define STEP_MOTOR_DRIVER_GPIO_RCC          RCC_AHB1Periph_GPIOA
#define STEP_MOTOR_DRIVER_GPIO              GPIOA
#define STEP_MOTOR_DRIVER_PWM_OUT_PIN       GPIO_Pin_6
#define STEP_MOTOR_DRIVER_DIR_PIN           GPIO_Pin_5
#define STEP_MOTOR_DRIVER_EN_OUT_PIN        GPIO_Pin_4
#define STEP_MOTOR_DRIVER_PWM_OUT_PIN_SOURCE    GPIO_PinSource6
#define STEP_MOTOR_DRIVER_GPIO_AF_PPP       GPIO_AF_TIM3
#define STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_RCC  RCC_APB1Periph_TIM3
#define STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM      TIM3

#define STEP_MOTOR_MODE_SET_RUN()           TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, ENABLE);
#define STEP_MOTOR_MODE_SET_HARD_STOP()     TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);
#define STEP_MOTOR_MODE_SET_SOFT_STOP()     TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);

#define STEP_MOTOR_DIR_SET_FORWARD()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_DIR_PIN, (BitAction)STEP_MOTOR_DIR_FORWARD)
#define STEP_MOTOR_DIR_SET_BACKWARD() GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_DIR_PIN, (BitAction)STEP_MOTOR_DIR_BACKWARD)

#define STEP_MOTOR_EN_OUT_ENABLE()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_EN_OUT_PIN, Bit_SET)
#define STEP_MOTOR_EN_OUT_DISABLE()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_EN_OUT_PIN, Bit_RESET)

typedef struct{
    enum STEP_MOTOR_MODE mode;
    enum STEP_MOTOR_DIR dir;
    enum STEP_MOTOR_SPEED speed;
    enum STEP_MOTOR_POS pos;
    FunctionalState enState;
}StepMotorDriver_TypeDef;
StepMotorDriver_TypeDef stepMotor;

/* STEP MOTOR DRIVER GPIO INIT */
void SweepRobotTest_StepMotorDriverGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

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
    
    TIM_DeInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, ENABLE);
    
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);
    
    TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);
    
    SweepRobotTest_StepMotorDriverReset();
}

void SweepRobotTest_StepMotorModeSet(enum STEP_MOTOR_MODE mode)
{
    if(mode == STEP_MOTOR_MODE_RUN){
        stepMotor.mode = STEP_MOTOR_MODE_RUN;
        STEP_MOTOR_EN_OUT_DISABLE();
        STEP_MOTOR_MODE_SET_RUN();
    }else if(mode == STEP_MOTOR_MODE_SOFT_STOP){
        stepMotor.mode = STEP_MOTOR_MODE_SOFT_STOP;
        STEP_MOTOR_MODE_SET_SOFT_STOP();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_HARD_STOP){
        stepMotor.mode = STEP_MOTOR_MODE_HARD_STOP;
        STEP_MOTOR_MODE_SET_HARD_STOP();
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_ON){
        stepMotor.mode = STEP_MOTOR_MODE_ON;
        TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);
        STEP_MOTOR_EN_OUT_ENABLE();
    }else if(mode == STEP_MOTOR_MODE_OFF){
        stepMotor.mode = STEP_MOTOR_MODE_OFF;
        STEP_MOTOR_MODE_SET_HARD_STOP();
        TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, 0);
        STEP_MOTOR_EN_OUT_ENABLE();
    }else{
        stepMotor.mode = STEP_MOTOR_MODE_UNKNOWN;
        STEP_MOTOR_EN_OUT_ENABLE();
    }
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

void SweepRobotTest_StepMotorRun(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_RUN;
    STEP_MOTOR_MODE_SET_RUN();
}

void SweepRobotTest_StepMotorSoftStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_SOFT_STOP;
    STEP_MOTOR_MODE_SET_SOFT_STOP();
}

void SweepRobotTest_StepMotorHardStop(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_HARD_STOP;
    STEP_MOTOR_MODE_SET_HARD_STOP();
}

void SweepRobotTest_StepMotorOn(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_ON;
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, STEP_MOTOR_PWM_PULSE_WIDTH);
}

void SweepRobotTest_StepMotorOff(void)
{
    stepMotor.mode = STEP_MOTOR_MODE_OFF;
    STEP_MOTOR_MODE_SET_HARD_STOP();
    TIM_SetCompare1(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, 0);
}

void SweepRobotTest_StepMotorMoveSteps(u16 steps)
{
    
}

void SweepRobotTest_StepMotorGotoPos(u8 pos)
{
    
}

void SweepRobotTest_StepMotorDriverReset(void)
{
    SweepRobotTest_StepMotorModeSet(STEP_MOTOR_MODE_ON);
    SweepRobotTest_StepMotorDirSet(STEP_MOTOR_DIR_FORWARD);
    SweepRobotTest_StepMotorSpeedSet(STEP_MOTOR_SPEED_MID);
    SweepRobotTest_StepMotorEnStateSet(ENABLE);
}

