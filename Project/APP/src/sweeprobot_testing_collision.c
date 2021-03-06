#include "sweeprobot_testing_collision.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

#define COLLISION_SIDE_TEST_CTRL_RCC         RCC_AHB1Periph_GPIOD
#define COLLISION_SIDE_TEST_CTRL_GPIO        GPIOD
#define COLLISION_SIDE_TEST_CTRL_L_PIN       GPIO_Pin_6
#define COLLISION_SIDE_TEST_CTRL_R_PIN       GPIO_Pin_7

#define COLLISION_FRONT_TEST_CTRL_RCC         RCC_AHB1Periph_GPIOE
#define COLLISION_FRONT_TEST_CTRL_GPIO        GPIOE
#define COLLISION_FRONT_TEST_CTRL_L_PIN       GPIO_Pin_5
#define COLLISION_FRONT_TEST_CTRL_R_PIN       GPIO_Pin_6
    
#define COLLISION_CHAN_NUM  4

enum CollisionChan{
    COLLISION_CHAN_L,
    COLLISION_CHAN_FL,
    COLLISION_CHAN_R,
    COLLISION_CHAN_FR,
};

static COLLISION_TestTypeDef collision[COLLISION_CHAN_NUM];

static void SweepRobot_CollisionTestGPIOInit(void)
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

void SweepRobot_CollisionCtrlOn(void)
{
    GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
    GPIO_SetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
    GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
    GPIO_SetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
}

void SweepRobot_CollisionCtrlOff(void)
{
    GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_L_PIN);
    GPIO_ResetBits(COLLISION_FRONT_TEST_CTRL_GPIO, COLLISION_FRONT_TEST_CTRL_R_PIN);
    GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_L_PIN);
    GPIO_ResetBits(COLLISION_SIDE_TEST_CTRL_GPIO, COLLISION_SIDE_TEST_CTRL_R_PIN);
}

static void SweepRobot_CollisionTestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
    
    str = "\r\n>>>COLLISION TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    
    SweepRobot_CollisionCtrlOn();

    OSTimeDlyHMSM(0,0,1,0);

    for(i=0;i<COLLISION_CHAN_NUM;i++){
        collision[i].value = 0;
        collision[i].validCnt = 0;
        collision[i].validFlag = 0;
    }
}

static void SweepRobot_CollisionTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<COLLISION_CHAN_NUM;i++){
        if(!collision[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("COLLISION->READ=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    Edit_Set_Value(ID_MAIN_EDIT_U1+i, usartRxNum);
                    collision[i].value = usartRxNum;
                    usartRxNum = 0;
                    usartRxFlag = 0;
                    break;
                }else{
                    continue;
                }
            }

            if(!collision[i].value){
                gSwrbTestStateMap &= ~(1<<(SWRB_TEST_COLLISION_L_POS+i));
                collision[i].validCnt++;
            }else{
                gSwrbTestStateMap |= (1<<(SWRB_TEST_COLLISION_L_POS+i));
            }

            if(collision[i].validCnt > SWRB_TEST_VALID_COMP_TIMES){
                collision[i].validFlag = 1;
            }
        }
    }
    
    if(collision[COLLISION_CHAN_L].validFlag && collision[COLLISION_CHAN_FL].validFlag && collision[COLLISION_CHAN_R].validFlag && collision[COLLISION_CHAN_FR].validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_CollisionCtrlOff();

        SWRB_TestDataSaveToFile(Collision_TestDataSave);
        
        str = "COLLISION OK\r\n";
        SWRB_TestDataFileWriteString(str);
        
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_COLLISION, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_COLLISION, "COLLISION OK");
        Edit_Clear();

        SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
    }
}

static void SweepRobot_CollisionTestOverTimeProc(void)
{
    char *str;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_CollisionCtrlOff();

    SWRB_TestDataSaveToFile(Collision_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK){
        str = "ERROR->COLLISION_L\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FL_MASK){
        str = "ERROR->COLLISION_FL\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK){
        str = "ERROR->COLLISION_R\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_FR_MASK){
        str = "ERROR->COLLISION_FR\r\n";
        SWRB_TestDataFileWriteString(str);
        MultiEdit_Add_Text(hWin_SWRB_MAIN, ID_MAIN_MULTIEDIT_MAIN,  str);
    }
    
    Checkbox_Set_Text_Color(ID_MAIN_CHECKBOX_COLLISION, GUI_RED);
    Checkbox_Set_Text(hWin_SWRB_MAIN, ID_MAIN_CHECKBOX_COLLISION, "COLLISION ERROR");
    Edit_Clear();

    SWRB_NextTestTaskResumePostAct(SWRB_COLLISION_TEST_TASK_PRIO);
}

void SweepRobot_CollisionTestTask(void *pdata)
{
    
    SweepRobot_CollisionTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_MAIN_CHECKBOX_COLLISION)){
            SWRB_NextTestTaskResumePreAct(SWRB_COLLISION_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt == 1){
                SweepRobot_CollisionTestInit();
            }

            if(gSwrbTestTaskRunCnt > 1){
                SweepRobot_CollisionTestProc();
            }

            if(gSwrbTestTaskRunCnt > 20){
                SweepRobot_CollisionTestOverTimeProc();
            }
            OSTimeDlyHMSM(0,0,0,50);
        }
    }
}

void Collision_TestDataSave(void)
{
    u8 i;
    
    for(i=0;i<COLLISION_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_VALUE_POS+i] = collision[i].value;
    }
    
    SWRB_TestDataFileWriteData("COLLISION->L_Value=", collision[COLLISION_CHAN_L].value, 1);
    SWRB_TestDataFileWriteData("COLLISION->FL_Value=", collision[COLLISION_CHAN_FL].value, 1);
    SWRB_TestDataFileWriteData("COLLISION->R_Value=", collision[COLLISION_CHAN_R].value, 1);
    SWRB_TestDataFileWriteData("COLLISION->FR_Value=", collision[COLLISION_CHAN_FR].value, 1);
}
