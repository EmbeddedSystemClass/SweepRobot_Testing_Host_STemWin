#ifndef __SWEEPROBOT_TESTING_IRDA_H
#define __SWEEPROBOT_TESTING_IRDA_H

#include "stm32f4xx.h"

enum SWRB_IRDA_CHAN{
    SWRB_IRDA_CHAN_BACK,
    SWRB_IRDA_CHAN_LEFT,
    SWRB_IRDA_CHAN_FLEFT,
    SWRB_IRDA_CHAN_FRIGHT,
    SWRB_IRDA_CHAN_RIGHT,
    SWRB_IRDA_CHAN_BOUND,    
};

#define REMOTE_CMD_CHARGE                   0xAC
#define REMOTE_CMD_UP                       0xBE
#define REMOTE_CMD_LEFT                     0xBA
#define REMOTE_CMD_DOWN                     0xBC
#define REMOTE_CMD_RIGHT                    0xB8
#define REMOTE_CMD_RUN_STOP                 0x8A
#define REMOTE_CMD_MODE_AUTO                0xAF
#define REMOTE_CMD_MODE_1                   0xB4
#define REMOTE_CMD_MODE_2                   0xB3
#define REMOTE_CMD_MODE_3                   0xB2
#define REMOTE_CMD_MODE_4                   0xB0
#define REMOTE_CMD_SPOT                     0xB5

#define IRDA_HOMING_CODE_BOUND_UPPER        0x47
#define IRDA_HOMING_CODE_BOUND_LOWER        0x39
#define IS_IRDA_HOMING_CODE(code)           ((code<IRDA_HOMING_CODE_BOUND_UPPER) && (code>IRDA_HOMING_CODE_BOUND_LOWER))

#define IS_IRDA_CONTROL_CODE(code)  (   ( code ==  REMOTE_CMD_CHARGE)   ||\
                                        ( code ==  REMOTE_CMD_UP)       ||\
                                        ( code ==  REMOTE_CMD_LEFT)     ||\
                                        ( code ==  REMOTE_CMD_DOWN)     ||\
                                        ( code ==  REMOTE_CMD_RIGHT)    ||\
                                        ( code ==  REMOTE_CMD_RUN_STOP) ||\
                                        ( code ==  REMOTE_CMD_MODE_AUTO)||\
                                        ( code ==  REMOTE_CMD_MODE_1)   ||\
                                        ( code ==  REMOTE_CMD_MODE_2)   ||\
                                        ( code ==  REMOTE_CMD_MODE_3)   ||\
                                        ( code ==  REMOTE_CMD_MODE_4)   ||\
                                        ( code ==  REMOTE_CMD_SPOT)     \
                                    )

#define IS_IRDA_CODE(code)      (IS_IRDA_HOMING_CODE(code)|| IS_IRDA_CONTROL_CODE(code))

typedef struct{
    int code;
    u8 validCnt;
    u8 validFlag;
}IRDA_TestTypeDef;

void SweepRobot_IrDATestTask(void *pdata);
void IRDA_TestDataSave(void);

#endif
