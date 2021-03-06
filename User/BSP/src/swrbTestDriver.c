#include "swrbTestDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "delay.h"

/* Collision Relay Control GPIO */
#define COLLISION_SIDE_TEST_CTRL_GPIO_PERIPH_ID         RCC_AHB1Periph_GPIOD
#define COLLISION_SIDE_TEST_CTRL_GPIO                   GPIOD
#define COLLISION_SIDE_TEST_CTRL_L_PIN                  GPIO_Pin_6
#define COLLISION_SIDE_TEST_CTRL_R_PIN                  GPIO_Pin_7

#define COLLISION_FRONT_TEST_CTRL_GPIO_PERIPH_ID        RCC_AHB1Periph_GPIOE
#define COLLISION_FRONT_TEST_CTRL_GPIO                  GPIOE
#define COLLISION_FRONT_TEST_CTRL_L_PIN                 GPIO_Pin_3
#define COLLISION_FRONT_TEST_CTRL_R_PIN                 GPIO_Pin_4

/* Collision Steering Motor Control GPIO */
#define COLLISION_TEST_STEER_MOTOR_CTRL_GPIO_PERIPH_ID  RCC_AHB1Periph_GPIOE
#define COLLISION_TEST_STEER_MOTOR_CTRL_GPIO            GPIOE
#define COLLISION_TEST_STEER_MOTOR_CTRL_TIM_PERIPH_ID   RCC_APB2Periph_TIM9
#define COLLISION_TEST_STEER_MOTOR_CTRL_TIM             TIM9
#define COLLISION_TEST_STEER_MOTOR_CTRL_GPIO_AF_PPP     GPIO_AF_TIM9
#define COLLISION_TEST_LEFT_STEER_MOTOR_CTRL_PIN        GPIO_Pin_5              //TIM9_CH1
#define COLLISION_TEST_RIGHT_STEER_MOTOR_CTRL_PIN       GPIO_Pin_6              //TIM9_CH2
#define COLLISION_TEST_LEFT_STEER_MOTOR_CTRL_PIN_SOURCE GPIO_PinSource5
#define COLLISION_TEST_RIGHT_STEER_MOTOR_CTRL_PIN_SOURCE GPIO_PinSource6

#define COLLISION_TEST_LEFT_STEERING_MOTOR_IDLE_POS     1120
#define COLLISION_TEST_LEFT_STEERING_MOTOR_FRONT_POS    630
#define COLLISION_TEST_LEFT_STEERING_MOTOR_SIDE_POS     1650

#define COLLISION_TEST_RIGHT_STEERING_MOTOR_IDLE_POS    970
#define COLLISION_TEST_RIGHT_STEERING_MOTOR_FRONT_POS   1500
#define COLLISION_TEST_RIGHT_STEERING_MOTOR_SIDE_POS    480

/* Wheel Float Steering Motor Control GPIO */
#define WHEEL_FLOAT_TEST_CTRL_GPIO_PERIPH_ID            RCC_AHB1Periph_GPIOB
#define WHEEL_FLOAT_TEST_CTRL_GPIO                      GPIOB
#define WHEEL_FLOAT_TEST_STEER_MOTOR_CTRL_PIN           GPIO_Pin_6             //TIM4_CH1
#define WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE              GPIO_PinSource6
#define WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP               GPIO_AF_TIM4
#define WHEEL_FLOAT_TEST_CTRL_TIM_PERIPH_ID             RCC_APB1Periph_TIM4
#define WHEEL_FLOAT_TEST_CTRL_TIM                       TIM4

#define WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS       280
#define WHEEL_FLOAT_TEST_STEERING_ENGINE_UP_POS         750
#define WHEEL_FLOAT_TEST_STEERING_ENGINE_DOWN_POS       280
#define WHEEL_FLOAT_TEST_STEERING_ENGINE_STOP_WAIT_TIME 100

/* Ash Tray Steering Motor Control GPIO */
#ifdef USE_USART1
#define ASH_TRAY_TEST_CTRL_GPIO_PERIPH_ID               RCC_AHB1Periph_GPIOB
#define ASH_TRAY_TEST_CTRL_GPIO                         GPIOB
#define ASH_TRAY_TEST_CTRL_PIN                          GPIO_Pin_10
#elif USE_USART2
#define ASH_TRAY_TEST_CTRL_GPIO_PERIPH_ID               RCC_AHB1Periph_GPIOB
#define ASH_TRAY_TEST_CTRL_GPIO                         GPIOB
#define ASH_TRAY_TEST_CTRL_PIN                          GPIO_Pin_10
#elif USE_USART3
#define ASH_TRAY_TEST_CTRL_GPIO_PERIPH_ID               RCC_AHB1Periph_GPIOA
#define ASH_TRAY_TEST_CTRL_GPIO                         GPIOA
#define ASH_TRAY_TEST_CTRL_PIN                          GPIO_Pin_9
#endif

/* Key ElectroMagnet Control GPIO */
#ifdef USE_USART1
#define KEY_TEST_CTRL_GPIO_PERIPH_ID                    RCC_AHB1Periph_GPIOB
#define KEY_TEST_CTRL_GPIO                              GPIOB
#define KEY_TEST_CTRL_PIN                               GPIO_Pin_11
#elif USE_USART2
#define KEY_TEST_CTRL_GPIO_PERIPH_ID                    RCC_AHB1Periph_GPIOB
#define KEY_TEST_CTRL_GPIO                              GPIOB
#define KEY_TEST_CTRL_PIN                               GPIO_Pin_11
#elif USE_USART3
#define KEY_TEST_CTRL_GPIO_PERIPH_ID                    RCC_AHB1Periph_GPIOA
#define KEY_TEST_CTRL_GPIO                              GPIOA
#define KEY_TEST_CTRL_PIN                               GPIO_Pin_10
#endif

/* IrDA Tx GPIO */
#define IRDA_TEST_TX_TIM_PERIPH_ID                      RCC_APB1Periph_TIM7
#define IRDA_TEST_TX_TIM                                TIM7
#define IRDA_TEST_TX_GPIO_PERIPH_ID                     RCC_AHB1Periph_GPIOG
#define IRDA_TEST_TX_GPIO                               GPIOG
#define IRDA_TEST_TX_L_PIN                              GPIO_Pin_9
#define IRDA_TEST_TX_R_PIN                              GPIO_Pin_6
#define IRDA_TEST_TX_M_PIN                              GPIO_Pin_7

#define IRDA_TEST_TX_RELAY_CTRL_GPIO_PERIPH_ID          RCC_AHB1Periph_GPIOG
#define IRDA_TEST_TX_RELAY_CTRL_GPIO                    GPIOF
#define IRDA_TEST_TX_RELAY_CTRL_PIN                     GPIO_Pin_6

#define IRDA_TEST_TX_PIN_SET(pin)                       GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_SET)
#define IRDA_TEST_TX_PIN_RESET(pin)                     GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_RESET)

#define IRDA_TEST_TX_RELAY_ON()                         GPIO_WriteBit(IRDA_TEST_TX_RELAY_CTRL_GPIO, IRDA_TEST_TX_RELAY_CTRL_PIN, Bit_RESET)
#define IRDA_TEST_TX_RELAY_OFF()                        GPIO_WriteBit(IRDA_TEST_TX_RELAY_CTRL_GPIO, IRDA_TEST_TX_RELAY_CTRL_PIN, Bit_SET)

/* Charge 24V Relay Ctrl GPIO */
#define CHARGE_TEST_24V_CTRL_GPIO_PERIPH_ID             RCC_AHB1Periph_GPIOC
#define CHARGE_TEST_24V_CTRL_GPIO                       GPIOC
#define CHARGE_TEST_24V_CTRL_PIN                        GPIO_Pin_7

static u8 gIrDACodeTxSeqNum = 0;
static u8 gIrDACodeTxCnt = 0;
static u32 gIrDACodeTxSeqTime = 0;

void SweepRobotTest_SteerMotor5VCtrlGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = STEER_MOTOR_5V_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(STEER_MOTOR_5V_CTRL_GPIO, &GPIO_InitStructure);
    
    SweepRobotTest_SteerMotor5VCtrlOn();
}

void SweepRobotTest_SteerMotor5VCtrlOn(void)
{
    STEER_MOTOR_5V_ON();
}

void SweepRobotTest_SteerMotor5VCtrlOff(void)
{
    STEER_MOTOR_5V_OFF();
}

/* COLLISION TEST GPIO INIT */
void SweepRobot_CollisionTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(COLLISION_FRONT_TEST_CTRL_GPIO_PERIPH_ID, ENABLE);
    RCC_AHB1PeriphClockCmd(COLLISION_SIDE_TEST_CTRL_GPIO_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(COLLISION_TEST_STEER_MOTOR_CTRL_TIM_PERIPH_ID, ENABLE);

    GPIO_InitStructure.GPIO_Pin = COLLISION_SIDE_TEST_CTRL_L_PIN |\
                                  COLLISION_SIDE_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_SIDE_TEST_CTRL_GPIO, &GPIO_InitStructure);
    GPIO_WriteBit(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN, Bit_SET);
    GPIO_WriteBit(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = COLLISION_FRONT_TEST_CTRL_L_PIN |\
                                  COLLISION_FRONT_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_FRONT_TEST_CTRL_GPIO, &GPIO_InitStructure);
    GPIO_WriteBit(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN, Bit_SET);
    GPIO_WriteBit(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN, Bit_SET);

    GPIO_InitStructure.GPIO_Pin = COLLISION_TEST_LEFT_STEER_MOTOR_CTRL_PIN |\
                                  COLLISION_TEST_RIGHT_STEER_MOTOR_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_TEST_STEER_MOTOR_CTRL_GPIO, &GPIO_InitStructure);

    GPIO_PinAFConfig(COLLISION_TEST_STEER_MOTOR_CTRL_GPIO, COLLISION_TEST_LEFT_STEER_MOTOR_CTRL_PIN_SOURCE, COLLISION_TEST_STEER_MOTOR_CTRL_GPIO_AF_PPP);
    GPIO_PinAFConfig(COLLISION_TEST_STEER_MOTOR_CTRL_GPIO, COLLISION_TEST_RIGHT_STEER_MOTOR_CTRL_PIN_SOURCE, COLLISION_TEST_STEER_MOTOR_CTRL_GPIO_AF_PPP);

    TIM_DeInit(COLLISION_TEST_STEER_MOTOR_CTRL_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, &TIM_OCInitStructure);
    TIM_OC2Init(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, ENABLE);

    TIM_SetCompare1(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_LEFT_STEERING_MOTOR_IDLE_POS);
    TIM_SetCompare2(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_RIGHT_STEERING_MOTOR_IDLE_POS);

    TIM_Cmd(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, ENABLE);
}

void SweepRobot_CollisionRelayCtrlOn(enum COLLISION_CHAN chan)
{
    switch(chan){
        case COLLISION_CHAN_L:
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FL:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_R:
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_FR:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_ALL:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
    }
}

void SweepRobot_CollisionRelayCtrlOff(enum COLLISION_CHAN chan)
{
    switch(chan){
        case COLLISION_CHAN_L:
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FL:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_R:
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_FR:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_ALL:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
    }
}

void SweepRobot_CollisionCtrlSteerMotorStateSet(FunctionalState state)
{
    switch(state){
        case ENABLE:
            TIM_Cmd(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, ENABLE);
            break;
        case DISABLE:
            TIM_Cmd(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, DISABLE);
            break;
        default:break;
    }
}

void SweepRobot_CollisionCtrlLeftSteerMotorPosMove(enum STEER_MOTOR_POS pos)
{
    switch(pos){
        case STEER_MOTOR_IDLE_POS:
            TIM_SetCompare1(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_LEFT_STEERING_MOTOR_IDLE_POS);
            break;
        case STEER_MOTOR_FRONT_POS:
            TIM_SetCompare1(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_LEFT_STEERING_MOTOR_FRONT_POS);
            break;
        case STEER_MOTOR_SIDE_POS:
            TIM_SetCompare1(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_LEFT_STEERING_MOTOR_SIDE_POS);
            break;
        default:break;
    }
}

void SweepRobot_CollisionCtrlLeftSteerMotorPosSet(int DutyCycle)
{
    TIM_SetCompare1(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, DutyCycle);
}

void SweepRobot_CollisionCtrlRightSteerMotorPosMove(enum STEER_MOTOR_POS pos)
{
    switch(pos){
        case STEER_MOTOR_IDLE_POS:
            TIM_SetCompare2(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_RIGHT_STEERING_MOTOR_IDLE_POS);
            break;
        case STEER_MOTOR_FRONT_POS:
            TIM_SetCompare2(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_RIGHT_STEERING_MOTOR_FRONT_POS);
            break;
        case STEER_MOTOR_SIDE_POS:
            TIM_SetCompare2(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, COLLISION_TEST_RIGHT_STEERING_MOTOR_SIDE_POS);
            break;
        default:break;
    }
}

void SweepRobot_CollisionCtrlRightSteerMotorPosSet(int DutyCycle)
{
    TIM_SetCompare2(COLLISION_TEST_STEER_MOTOR_CTRL_TIM, DutyCycle);
}

/* WHEEL FLOAT TEST GPIO INIT */
void SweepRobot_WheelFloatTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_TIM_PERIPH_ID, ENABLE);
    RCC_AHB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_GPIO_PERIPH_ID, ENABLE);

    GPIO_PinAFConfig(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE, WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP);
    GPIO_PinAFConfig(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE, WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = WHEEL_FLOAT_TEST_STEER_MOTOR_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(WHEEL_FLOAT_TEST_CTRL_GPIO, &GPIO_InitStructure);

    TIM_DeInit(WHEEL_FLOAT_TEST_CTRL_TIM);

    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);

    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_DOWN_POS);

    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlSteerMotorPosMove(enum STEER_MOTOR_POS pos)
{
    switch(pos){
        case STEER_MOTOR_IDLE_POS:
            TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
            break;
        case STEER_MOTOR_UP_POS:
            TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_UP_POS);
            break;
        case STEER_MOTOR_DOWN_POS:
            TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_DOWN_POS);
            break;
        default:break;
    }
}

void SweepRobot_WheelFloatCtrlSteerMotorPosSet(int DutyCycle)
{
    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, DutyCycle);
}

void SweepRobot_WheelFloatCtrlMoveToIdlePos(void)
{
    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);

    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlMoveToUpPos(void)
{
    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_UP_POS);

    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlMoveToDownPos(void)
{
    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_DOWN_POS);

    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlShutDown(void)
{
    TIM_SetCompare1(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);

    OSTimeDlyHMSM(0,0,0,WHEEL_FLOAT_TEST_STEERING_ENGINE_STOP_WAIT_TIME);

    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, DISABLE);
}

/* ASH TRAY TEST GPIO Init */
void SweepRobot_AshTrayTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_GPIO_PERIPH_ID, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ASH_TRAY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ASH_TRAY_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

/* KEY TEST GPIO INIT */
void SweepRobot_KeyTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(KEY_TEST_CTRL_GPIO_PERIPH_ID, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    GPIO_WriteBit(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN, Bit_SET);
}

void SweepRobot_KeyTestElectroMagnetCtrlPushPos(void)
{
    GPIO_WriteBit(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN, Bit_RESET);
}

void SweepRobot_KeyTestElectroMagnetCtrlReleasePos(void)
{
    GPIO_WriteBit(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN, Bit_SET);
}

/* IrDA TEST GPIO Init */
void SweepRobot_IrDATestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(IRDA_TEST_TX_GPIO_PERIPH_ID, ENABLE);
    RCC_AHB1PeriphClockCmd(IRDA_TEST_TX_RELAY_CTRL_GPIO_PERIPH_ID, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IRDA_TEST_TX_L_PIN |\
                                  IRDA_TEST_TX_R_PIN |\
                                  IRDA_TEST_TX_M_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(IRDA_TEST_TX_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = IRDA_TEST_TX_RELAY_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(IRDA_TEST_TX_RELAY_CTRL_GPIO, &GPIO_InitStructure);
    
    SweepRobot_IrDATestTxRelayOff();
}

void SweepRobot_IrDATestTxRelayOn(void)
{
    IRDA_TEST_TX_RELAY_ON();
}

void SweepRobot_IrDATestTxRelayOff(void)
{
    IRDA_TEST_TX_RELAY_OFF();
}

void SweepRobot_IrDATestGPIOPINSet(void)
{
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_SET(IRDA_TEST_TX_M_PIN);
}

void SweepRobot_IrDATestGPIOPINReset(void)
{
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_L_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_R_PIN);
    IRDA_TEST_TX_PIN_RESET(IRDA_TEST_TX_M_PIN);
}

/* Improved IrDA Code Tx process */
/* FIXME: Need to Change IrDA Tx process to match 38khz receiver */
void SweepRobot_IrDACodeTxProc(u8 code)
{
    plat_int_reg_cb(STM32F4xx_INT_TIM7, SweepRobot_IrDACodeTxProc);

    switch(gIrDACodeTxSeqNum){
        case 0:
            gIrDACodeTxSeqNum++;
            gIrDACodeTxSeqTime = 3000;
            SweepRobot_IrDATestGPIOPINSet();
            break;
        case 1:
            gIrDACodeTxSeqNum++;
            gIrDACodeTxSeqTime = 1000;
            SweepRobot_IrDATestGPIOPINReset();
            break;
        case 2:
            gIrDACodeTxCnt++;
            gIrDACodeTxSeqNum++;
            gIrDACodeTxSeqTime = (code & 0x80)?800:1600;
            SweepRobot_IrDATestGPIOPINSet();
            break;
        case 3:
            if(gIrDACodeTxCnt != 8){
                gIrDACodeTxSeqNum--;
            }else{
                gIrDACodeTxSeqNum++;
            }
            gIrDACodeTxSeqTime = (code & 0x80)?1600:800;
            SweepRobot_IrDATestGPIOPINReset();
            code <<= 1;
            break;
        case 4:
            gIrDACodeTxSeqNum++;
            gIrDACodeTxSeqTime = 500;
            SweepRobot_IrDATestGPIOPINSet();
            break;
        case 5:
            gIrDACodeTxCnt = 0;
            gIrDACodeTxSeqNum = 0;
            TIM_SetCounter(IRDA_TEST_TX_TIM, 0);
            TIM_ITConfig(IRDA_TEST_TX_TIM, TIM_IT_Update, DISABLE);
            TIM_Cmd(IRDA_TEST_TX_TIM, DISABLE);
            plat_int_dereg_cb(STM32F4xx_INT_TIM7);
            SweepRobot_IrDATestGPIOPINReset();
            return;
        default:break;
    }

    TIM_SetCounter(IRDA_TEST_TX_TIM, 0);
    TIM_ITConfig(IRDA_TEST_TX_TIM, TIM_IT_Update, DISABLE);
    TIM_SetAutoreload(IRDA_TEST_TX_TIM, gIrDACodeTxSeqTime);
    TIM_ClearFlag(IRDA_TEST_TX_TIM, TIM_FLAG_Update);
    TIM_ITConfig(IRDA_TEST_TX_TIM, TIM_IT_Update, ENABLE);
    TIM_Cmd(IRDA_TEST_TX_TIM, ENABLE);
}

/* Original IrDA code Tx process */
/* FIXME: Should modulate to match 38kHz IrDA Receiver */
void SweepRobot_IrDATestTxSendCmd(u8 code)
{
    u8 i;
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL();

    SweepRobot_IrDATestGPIOPINSet();
    delay_us(3000);
    SweepRobot_IrDATestGPIOPINReset();
    delay_us(1000);

    for(i=0;i<8;i++){
        if(code & 0x80){
            SweepRobot_IrDATestGPIOPINSet();
            delay_us(800);
            SweepRobot_IrDATestGPIOPINReset();
            delay_us(1600);
        }else{
            SweepRobot_IrDATestGPIOPINSet();
            delay_us(1600);
            SweepRobot_IrDATestGPIOPINReset();
            delay_us(800);
        }
        code<<=1;
    }

    SweepRobot_IrDATestGPIOPINSet();
    delay_us(100);
    SweepRobot_IrDATestGPIOPINReset();

    OS_EXIT_CRITICAL();
}

/* Charge Test GPIO Init */
void SweepRobot_ChargeTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(CHARGE_TEST_24V_CTRL_GPIO_PERIPH_ID, ENABLE);

    GPIO_InitStructure.GPIO_Pin = CHARGE_TEST_24V_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CHARGE_TEST_24V_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Charge24VOn(void)
{
    GPIO_ResetBits(CHARGE_TEST_24V_CTRL_GPIO, CHARGE_TEST_24V_CTRL_PIN);
}

void SweepRobot_Charge24VOff(void)
{
    GPIO_SetBits(CHARGE_TEST_24V_CTRL_GPIO, CHARGE_TEST_24V_CTRL_PIN);
}

