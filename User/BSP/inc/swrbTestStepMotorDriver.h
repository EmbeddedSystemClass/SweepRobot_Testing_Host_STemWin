#ifndef __SWRB_TEST_STEP_MOTOR_DRIVER_H
#define __SWRB_TEST_STEP_MOTOR_DRIVER_H

#include "stm32f4xx.h"

#define STEP_MOTOR_DRIVER_GPIO_PERIPH_ID    RCC_AHB1Periph_GPIOA
#define STEP_MOTOR_DRIVER_GPIO              GPIOA
#define STEP_MOTOR_DRIVER_PWM_OUT_PIN       GPIO_Pin_6
#define STEP_MOTOR_DRIVER_DIR_PIN           GPIO_Pin_5
#define STEP_MOTOR_DRIVER_EN_OUT_PIN        GPIO_Pin_4


#define STEP_MOTOR_DRIVER_24V_CTRL_GPIO_PERIPH_ID   RCC_AHB1Periph_GPIOG
#define STEP_MOTOR_DRIVER_24V_CTRL_GPIO             GPIOG
#define STEP_MOTOR_DRIVER_24V_CTRL_PIN              GPIO_Pin_7

#ifdef VERSION_1_3
#define STEP_MOTOR_DRIVER_POS_DETECT_GPIO_PERIPH_ID RCC_AHB1Periph_GPIOB
#define STEP_MOTOR_DRIVER_POS_DETECT_GPIO           GPIOB
#define STEP_MOTOR_DRIVER_POS_DETECT_PIN            GPIO_Pin_5
#else
#define STEP_MOTOR_DRIVER_POS_DETECT_GPIO_PERIPH_ID RCC_AHB1Periph_GPIOG
#define STEP_MOTOR_DRIVER_POS_DETECT_GPIO           GPIOG
#define STEP_MOTOR_DRIVER_POS_DETECT_PIN            GPIO_Pin_6
#endif


#define STEP_MOTOR_DRIVER_PWM_OUT_PIN_SOURCE    GPIO_PinSource6
#define STEP_MOTOR_DRIVER_GPIO_AF_PPP       GPIO_AF_TIM3
#define STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_PERIPH_ID  RCC_APB1Periph_TIM3
#define STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM      TIM3
#define STEP_MOTOR_DRVIER_GPIO_PWM_OUT_TIM_IRQn  TIM3_IRQn
#define STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM_INT   STM32F4xx_INT_TIM3

#define STEP_MOTOR_24V_ON()     GPIO_WriteBit(STEP_MOTOR_DRIVER_24V_RELAY_CTRL_GPIO, STEP_MOTOR_DIRVER_24V_RELAY_CTRL_PIN, Bit_RESET)
#define STEP_MOTOR_24V_OFF()    GPIO_WriteBit(STEP_MOTOR_DRIVER_24V_RELAY_CTRL_GPIO, STEP_MOTOR_DIRVER_24V_RELAY_CTRL_PIN, Bit_SET)

#define STEP_MOTOR_TIM_SET_RUN()           TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, ENABLE);
#define STEP_MOTOR_TIM_SET_STOP()          TIM_Cmd(STEP_MOTOR_DRIVER_GPIO_PWM_OUT_TIM, DISABLE);

#define STEP_MOTOR_DIR_SET_FORWARD()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_DIR_PIN, (BitAction)STEP_MOTOR_DIR_FORWARD)
#define STEP_MOTOR_DIR_SET_BACKWARD() GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_DIR_PIN, (BitAction)STEP_MOTOR_DIR_BACKWARD)

#define STEP_MOTOR_EN_OUT_ENABLE()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_EN_OUT_PIN, Bit_SET)
#define STEP_MOTOR_EN_OUT_DISABLE()  GPIO_WriteBit(STEP_MOTOR_DRIVER_GPIO, STEP_MOTOR_DRIVER_EN_OUT_PIN, Bit_RESET)

#define STEP_MOTOR_PWR_ON()     GPIO_WriteBit(STEP_MOTOR_DRIVER_24V_CTRL_GPIO, STEP_MOTOR_DRIVER_24V_CTRL_PIN, Bit_RESET)
#define STEP_MOTOR_PWR_OFF()    GPIO_WriteBit(STEP_MOTOR_DRIVER_24V_CTRL_GPIO, STEP_MOTOR_DRIVER_24V_CTRL_PIN, Bit_SET)

#define STEP_MOTOR_POS_DETECT_SIGN  GPIO_ReadInputDataBit(STEP_MOTOR_DRIVER_POS_DETECT_GPIO, STEP_MOTOR_DRIVER_POS_DETECT_PIN)

typedef void (*stepMotorDriverISRCB_t)(void);

extern stepMotorDriverISRCB_t stepMotorDriverISRCB;

#define STEP_MOTOR_ISR_CB_REG(f)            do{stepMotorDriverISRCB=f;}while(0)
#define STEP_MOTOR_ISR_CB_DEREG()           do{stepMotorDriverISRCB=NULL;}while(0)

enum STEP_MOTOR_MODE{
    STEP_MOTOR_MODE_UNKNOWN,
    STEP_MOTOR_MODE_STOP,
    STEP_MOTOR_MODE_RUN_POS,
    STEP_MOTOR_MODE_RUN_STEP,
    STEP_MOTOR_MODE_RUN,
    STEP_MOTOR_MODE_HOMING,
    STEP_MOTOR_MODE_BOUND,
};

enum STEP_MOTOR_DIR{
    STEP_MOTOR_DIR_FORWARD,
    STEP_MOTOR_DIR_BACKWARD,
};

enum STEP_MOTOR_POS{
    STEP_MOTOR_POS_UNKNOWN,
    STEP_MOTOR_POS_HOME,
    STEP_MOTOR_POS_MARK1,
    STEP_MOTOR_POS_BOUND,
};

void SweepRobotTest_StepMotorDriverGPIOInit(void);
void SweepRobotTest_StepMotorMoveSteps(float speed, u16 steps);
void SweepRobotTest_StepMotorModeSetExec(enum STEP_MOTOR_MODE mode);
void SweepRobotTest_StepMotorModeSet(enum STEP_MOTOR_MODE mode);
enum STEP_MOTOR_MODE SweepRobotTest_StepMotorModeGet(void);
void SweepRobotTest_StepMotorPwrOn(void);
void SweepRobotTest_StepMotorPwrOff(void);
FunctionalState SweepRobotTest_StepMotorPwrStateGet(void);
void SweepRobotTest_StepMotorDirSet(enum STEP_MOTOR_DIR dir);
enum STEP_MOTOR_DIR SweepRobotTest_StepMotorDirGet(void);
void SweepRobotTest_StepMotorSpeedSet(float speed);
float SweepRobotTest_StepMotorSpeedGet(void);
int SweepRobotTest_StepMotorStepsGet(void);
void SweepRobotTest_StepMotorEnStateSet(FunctionalState enState);
FunctionalState SweepRobotTest_StepMotorEnStateGet(void);
void SweepRobotTest_StepMotorDriverReset(void);
enum STEP_MOTOR_POS SweepRobotTest_StepMotorPosGet(void);
int32_t SweepRobotTest_StepMotorPosStepCntGet(void);
void SweepRobotTest_StepMotorGoHome(void);
void SweepRobotTest_StepMotorSetIdle(void);

#endif
