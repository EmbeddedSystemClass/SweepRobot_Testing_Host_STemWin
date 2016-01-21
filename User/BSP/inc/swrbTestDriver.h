#ifndef __SWRB_TEST_DRIVER_H
#define __SWRB_TEST_DRIVER_H

#include "sweeprobot_testing_conf.h"

#define STEER_MOTOR_5V_CTRL_GPIO_PERIPH_ID              RCC_AHB1Periph_GPIOG
#define STEER_MOTOR_5V_CTRL_GPIO                        GPIOG
#define STEER_MOTOR_5V_CTRL_PIN                         GPIO_Pin_15

#define STEER_MOTOR_5V_ON()     GPIO_WriteBit(STEER_MOTOR_5V_CTRL_GPIO, STEER_MOTOR_5V_CTRL_PIN, Bit_RESET)
#define STEER_MOTOR_5V_OFF()     GPIO_WriteBit(STEER_MOTOR_5V_CTRL_GPIO, STEER_MOTOR_5V_CTRL_PIN, Bit_SET)

enum STEER_MOTOR_POS{
    STEER_MOTOR_IDLE_POS,
    STEER_MOTOR_FRONT_POS,
    STEER_MOTOR_SIDE_POS,
    STEER_MOTOR_UP_POS,
    STEER_MOTOR_DOWN_POS,
};

void SweepRobotTest_SteerMotor5VCtrlGPIOInit(void);
void SweepRobotTest_SteerMotor5VCtrlOn(void);
void SweepRobotTest_SteerMotor5VCtrlOff(void);

void SweepRobot_CollisionTestGPIOInit(void);
void SweepRobot_CollisionRelayCtrlOn(enum COLLISION_CHAN chan);
void SweepRobot_CollisionRelayCtrlOff(enum COLLISION_CHAN chan);
void SweepRobot_CollisionCtrlSteerMotorStateSet(FunctionalState state);
void SweepRobot_CollisionCtrlLeftSteerMotorPosMove(enum STEER_MOTOR_POS pos);
void SweepRobot_CollisionCtrlLeftSteerMotorPosSet(int DutyCycle);
void SweepRobot_CollisionCtrlRightSteerMotorPosMove(enum STEER_MOTOR_POS pos);
void SweepRobot_CollisionCtrlRightSteerMotorPosSet(int DutyCycle);

void SweepRobot_WheelFloatTestGPIOInit(void);
void SweepRobot_WheelFloatCtrlSteerMotorPosMove(enum STEER_MOTOR_POS pos);
void SweepRobot_WheelFloatCtrlSteerMotorPosSet(int DutyCycle);
void SweepRobot_WheelFloatCtrlShutDown(void);
void SweepRobot_WheelFloatCtrlMoveToIdlePos(void);
void SweepRobot_WheelFloatCtrlMoveToUpPos(void);
void SweepRobot_WheelFloatCtrlMoveToDownPos(void);

void SweepRobot_AshTrayTestGPIOInit(void);
void SweepRobot_AshTrayTestInsCtrlMoveToTestPos(void);
void SweepRobot_AshTrayTestInsCtrlMoveToIdlePos(void);
void SweepRobot_AshTrayTestInsCtrlShutDown(void);

void SweepRobot_KeyTestGPIOInit(void);
void SweepRobot_KeyTestElectroMagnetCtrlPushPos(void);
void SweepRobot_KeyTestElectroMagnetCtrlReleasePos(void);

void SweepRobot_IrDATestGPIOInit(void);
void SweepRobot_IrDATestGPIOPINSet(void);
void SweepRobot_IrDATestGPIOPINReset(void);
void SweepRobot_IrDACodeTxProc(u8 code);
void SweepRobot_IrDATestTxSendCmd(u8 code);

void SweepRobot_ChargeTestGPIOInit(void);
void SweepRobot_Charge24VOn(void);
void SweepRobot_Charge24VOff(void);

#endif
