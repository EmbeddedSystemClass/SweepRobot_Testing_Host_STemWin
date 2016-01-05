#include "swrbTestDriver.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "delay.h"

#define COLLISION_SIDE_TEST_CTRL_RCC        RCC_AHB1Periph_GPIOD
#define COLLISION_SIDE_TEST_CTRL_GPIO       GPIOD
#define COLLISION_SIDE_TEST_CTRL_L_PIN      GPIO_Pin_6
#define COLLISION_SIDE_TEST_CTRL_R_PIN      GPIO_Pin_7

#define COLLISION_FRONT_TEST_CTRL_RCC       RCC_AHB1Periph_GPIOE
#define COLLISION_FRONT_TEST_CTRL_GPIO      GPIOE
#define COLLISION_FRONT_TEST_CTRL_L_PIN     GPIO_Pin_5
#define COLLISION_FRONT_TEST_CTRL_R_PIN     GPIO_Pin_6

#define WHEEL_FLOAT_TEST_CTRL_GPIO_RCC      RCC_AHB1Periph_GPIOB
#define WHEEL_FLOAT_TEST_CTRL_GPIO          GPIOB
#define WHEEL_FLOAT_TEST_CTRL_L_PIN         GPIO_Pin_10
#define WHEEL_FLOAT_TEST_CTRL_R_PIN         GPIO_Pin_11
#define WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE  GPIO_PinSource10
#define WHEEL_FLOAT_TSET_CTRL_R_PIN_SOURCE  GPIO_PinSource11
#define WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP   GPIO_AF_TIM2
#define WHEEL_FLOAT_TEST_CTRL_TIM_RCC       RCC_APB1Periph_TIM2
#define WHEEL_FLOAT_TEST_CTRL_TIM           TIM2

#define WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS   500
#define WHEEL_FLOAT_TEST_STEERING_ENGINE_TEST_POS   1000
#define WHEEL_FLOAT_TEST_STEERING_ENGINE_STOP_WAIT_TIME 100

#define ASH_TRAY_TEST_CTRL_RCC              RCC_AHB1Periph_GPIOB
#define ASH_TRAY_TEST_CTRL_GPIO             GPIOB
#define ASH_TRAY_TEST_CTRL_PIN              GPIO_Pin_6
#define ASH_TRAY_TEST_CTRL_PIN_SOURCE       GPIO_PinSource6
#define ASH_TRAY_TEST_CTRL_GPIO_AF_PPP      GPIO_AF_TIM4
#define ASH_TRAY_TEST_CTRL_TIM              TIM4
#define ASH_TRAY_TEST_CTRL_TIM_RCC          RCC_APB1Periph_TIM4

#define KEY_TEST_CTRL_RCC                   RCC_AHB1Periph_GPIOB
#define KEY_TEST_CTRL_GPIO                  GPIOB
#define KEY_TEST_CTRL_PIN                   GPIO_Pin_7
#define KEY_TEST_CTRL_PIN_SOURCE            GPIO_PinSource7
#define KEY_TEST_CTRL_GPIO_AF_PPP           GPIO_AF_TIM4
#define KEY_TEST_CTRL_TIM_RCC               RCC_APB1Periph_TIM4
#define KEY_TEST_CTRL_TIM                   TIM4

#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_RCC             RCC_AHB1Periph_GPIOC
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_GPIO            GPIOC
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_PIN             GPIO_Pin_7
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_PIN_SOURCE      GPIO_PinSource7
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_GPIO_AF_PPP     GPIO_AF_TIM3
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM_RCC         RCC_APB1Periph_TIM3
#define IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM             TIM3

#define IRDA_TEST_TX_TIM_RCC                RCC_APB1Periph_TIM7
#define IRDA_TEST_TX_TIM                    TIM7
#define IRDA_TEST_TX_RCC                    RCC_AHB1Periph_GPIOA
#define IRDA_TEST_TX_GPIO                   GPIOA
#define IRDA_TEST_TX_L_PIN                  GPIO_Pin_4
#define IRDA_TEST_TX_R_PIN                  GPIO_Pin_5
#define IRDA_TEST_TX_M_PIN                  GPIO_Pin_6

#define IRDA_TEST_TX_PIN_SET(pin)           GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_SET)
#define IRDA_TEST_TX_PIN_RESET(pin)         GPIO_WriteBit(IRDA_TEST_TX_GPIO, pin, Bit_RESET)

static u8 gIrDACodeTxSeqNum = 0;
static u8 gIrDACodeTxCnt = 0;
static u32 gIrDACodeTxSeqTime = 0;


/* COLLISION TEST GPIO INIT */
void SweepRobot_CollisionTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(COLLISION_FRONT_TEST_CTRL_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(COLLISION_SIDE_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = COLLISION_SIDE_TEST_CTRL_L_PIN |\
                                  COLLISION_SIDE_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_SIDE_TEST_CTRL_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = COLLISION_FRONT_TEST_CTRL_L_PIN |\
                                  COLLISION_FRONT_TEST_CTRL_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_FRONT_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_CollisionCtrlOn(enum CollisionChan chan)
{
    switch(chan){
        case COLLISION_CHAN_L:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FL:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_R:
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FR:
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_ALL:
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
    }
}

void SweepRobot_CollisionCtrlOff(enum CollisionChan chan)
{
    switch(chan){
        case COLLISION_CHAN_L:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FL:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_R:
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            break;
        case COLLISION_CHAN_FR:
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
        case COLLISION_CHAN_ALL:
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
            GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
            GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
            break;
    }
}

/* WHEEL FLOAT TEST GPIO INIT */
void SweepRobot_WheelFloatTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(WHEEL_FLOAT_TEST_CTRL_GPIO_RCC, ENABLE);
    
    GPIO_PinAFConfig(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE, WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP);
    GPIO_PinAFConfig(WHEEL_FLOAT_TEST_CTRL_GPIO, WHEEL_FLOAT_TSET_CTRL_L_PIN_SOURCE, WHEEL_FLOAT_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = WHEEL_FLOAT_TEST_CTRL_L_PIN | WHEEL_FLOAT_TEST_CTRL_R_PIN;
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
    TIM_OC3Init(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_OCInitStructure);
    TIM_OC4Init(WHEEL_FLOAT_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC3PreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
    
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlMoveToTestPos(void)
{
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_TEST_POS);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_TEST_POS);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlMoveToIdlePos(void)
{
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_WheelFloatCtrlShutDown(void)
{
    TIM_SetCompare3(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    TIM_SetCompare4(WHEEL_FLOAT_TEST_CTRL_TIM, WHEEL_FLOAT_TEST_STEERING_ENGINE_IDLE_POS);
    
    OSTimeDlyHMSM(0,0,0,WHEEL_FLOAT_TEST_STEERING_ENGINE_STOP_WAIT_TIME);
    
    TIM_Cmd(WHEEL_FLOAT_TEST_CTRL_TIM, DISABLE);
}

/* ASH TRAY TEST GPIO Init */
void SweepRobot_AshTrayTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(ASH_TRAY_TEST_CTRL_RCC, ENABLE);
    
    GPIO_PinAFConfig(ASH_TRAY_TEST_CTRL_GPIO, ASH_TRAY_TEST_CTRL_PIN_SOURCE, ASH_TRAY_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = ASH_TRAY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(ASH_TRAY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(ASH_TRAY_TEST_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ASH_TRAY_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(ASH_TRAY_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(ASH_TRAY_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
    
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlMoveToTestPos(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 1000);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlMoveToIdlePos(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_AshTrayTestInsCtrlShutDown(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    OSTimeDlyHMSM(0,0,0,50);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, DISABLE);
}

/* KEY TEST GPIO INIT */
void SweepRobot_KeyTestGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(KEY_TEST_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(KEY_TEST_CTRL_RCC, ENABLE);
    
    GPIO_PinAFConfig(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN_SOURCE, KEY_TEST_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = KEY_TEST_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(KEY_TEST_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(KEY_TEST_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(KEY_TEST_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(KEY_TEST_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(KEY_TEST_CTRL_TIM, ENABLE);
    
    TIM_SetCompare2(KEY_TEST_CTRL_TIM, 300);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
}

void SweepRobot_KeyTestCtrlTestPos(void)
{
    TIM_SetCompare1(KEY_TEST_CTRL_TIM, 1000);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
    
//    GPIO_SetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

void SweepRobot_KeyTestCtrlIdlePos(void)
{
    TIM_SetCompare1(KEY_TEST_CTRL_TIM, 300);
    
//    OSTimeDlyHMSM(0,0,1,0);
    
    TIM_Cmd(KEY_TEST_CTRL_TIM, ENABLE);
    
//    GPIO_ResetBits(KEY_TEST_CTRL_GPIO, KEY_TEST_CTRL_PIN);
}

void SweepRobot_KeyTestCtrlShutDown(void)
{
    TIM_SetCompare1(ASH_TRAY_TEST_CTRL_TIM, 300);
    
    OSTimeDlyHMSM(0,0,0,50);
    
    TIM_Cmd(ASH_TRAY_TEST_CTRL_TIM, DISABLE);
}

/* IFRD FRONT TEST GPIO INIT */
void SweepRobot_IFRDFrontTestCtrlGPIOInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_RCC, ENABLE);
    
    GPIO_PinAFConfig(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_GPIO, IFRD_FRONT_TEST_STEP_MOTOR_CTRL_PIN_SOURCE, IFRD_FRONT_TEST_STEP_MOTOR_CTRL_GPIO_AF_PPP);

    GPIO_InitStructure.GPIO_Pin = IFRD_FRONT_TEST_STEP_MOTOR_CTRL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(KEY_TEST_CTRL_GPIO, &GPIO_InitStructure);
    
    TIM_DeInit(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM);
    
    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, &TIM_TimeBaseInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    /* SHOULD WORK IN PULSE MODE */
    TIM_OCInitStructure.TIM_Pulse = 2000;
    TIM_OC2Init(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, &TIM_OCInitStructure);
    
    TIM_OC2PreloadConfig(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, ENABLE);
    
    TIM_SetCompare2(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, 300);
    
    TIM_Cmd(IFRD_FRONT_TEST_STEP_MOTOR_CTRL_TIM, ENABLE);
}

/* IrDA TEST GPIO Init */
void SweepRobot_IrDATestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(IRDA_TEST_TX_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = IRDA_TEST_TX_L_PIN |\
                                  IRDA_TEST_TX_R_PIN |\
                                  IRDA_TEST_TX_M_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(IRDA_TEST_TX_GPIO, &GPIO_InitStructure);
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

