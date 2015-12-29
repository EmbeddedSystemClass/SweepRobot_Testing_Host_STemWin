#ifndef __SWRB_TEST_STEP_MOTOR_DRIVER_H
#define __SWRB_TEST_STEP_MOTOR_DRIVER_H

#include "stm32f4xx.h"

#define STEP_MOTOR_SPEED_LOW_PERIOD     3000
#define STEP_MOTOR_SPEED_MID_PERIOD     2000
#define STEP_MOTOR_SPEED_HIGH_PERIOD    1000

#define STEP_MOTOR_PWM_PULSE_WIDTH      50      //50*10us = 500us

enum STEP_MOTOR_MODE{
    STEP_MOTOR_MODE_UNKNOWN,
    STEP_MOTOR_MODE_OFF,
    STEP_MOTOR_MODE_ON,
    STEP_MOTOR_MODE_HARD_STOP,
    STEP_MOTOR_MODE_SOFT_STOP,
    STEP_MOTOR_MODE_RUN_STEP,
    STEP_MOTOR_MODE_RUN,
    STEP_MOTOR_MODE_HOME,
    STEP_MOTOR_MODE_BOUND,
};

enum STEP_MOTOR_DIR{
    STEP_MOTOR_DIR_FORWARD,
    STEP_MOTOR_DIR_BACKWARD,
};

enum STEP_MOTOR_SPEED{
    STEP_MOTOR_SPEED_LOW = STEP_MOTOR_SPEED_LOW_PERIOD,
    STEP_MOTOR_SPEED_MID = STEP_MOTOR_SPEED_MID_PERIOD,
    STEP_MOTOR_SPEED_HIGH = STEP_MOTOR_SPEED_HIGH_PERIOD,
};

enum STEP_MOTOR_POS{
    STEP_MOTOR_POS_UNKNOWN,
    STEP_MOTOR_POS_HOME,
    STEP_MOTOR_POS_MARK1,
    STEP_MOTOR_POS_BOUND,
};

void SweepRobotTest_StepMotorDriverGPIOInit(void);

void SweepRobotTest_StepMotorMoveSteps(int period_ms, u16 steps);

void SweepRobotTest_StepMotorModeSet(enum STEP_MOTOR_MODE mode);
enum STEP_MOTOR_MODE SweepRobotTest_StepMotorModeGet(void);
void SweepRobotTest_StepMotorDirSet(enum STEP_MOTOR_DIR dir);
enum STEP_MOTOR_DIR SweepRobotTest_StepMotorDirGet(void);
void SweepRobotTest_StepMotorSpeedSet(enum STEP_MOTOR_SPEED speed);
enum STEP_MOTOR_SPEED SweepRobotTest_StepMotorSpeedGet(void);
void SweepRobotTest_StepMotorEnStateSet(FunctionalState enState);
FunctionalState SweepRobotTest_StepMotorEnStateGet(void);
void SweepRobotTest_StepMotorDriverReset(void);

#endif
