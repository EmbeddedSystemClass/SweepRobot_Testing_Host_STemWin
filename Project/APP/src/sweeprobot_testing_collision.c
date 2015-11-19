#include "sweepRobot_testing_collision.h"

#include "usart.h"
#include "includes.h"

#define COLLISION_TEST_CTRL_RCC         RCC_AHB1Periph_GPIOB
#define COLLISION_TEST_CTRL_GPIO        GPIOB
#define COLLISION_TEST_CTRL_F_L_PIN     GPIO_Pin_6
#define COLLISION_TEST_CTRL_F_R_PIN     GPIO_Pin_7
#define COLLISION_TEST_CTRL_S_L_PIN     GPIO_Pin_10
#define COLLISION_TEST_CTRL_S_R_PIN     GPIO_Pin_11

#define COLLISION_CHAN_NUM  4

static COLLISION_TestTypeDef collision[COLLISION_CHAN_NUM];

void SweepRobot_CollisionTestGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(COLLISION_TEST_CTRL_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = COLLISION_TEST_CTRL_F_L_PIN |\
                                  COLLISION_TEST_CTRL_F_R_PIN |\
                                  COLLISION_TEST_CTRL_S_L_PIN |\
                                  COLLISION_TEST_CTRL_S_R_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COLLISION_TEST_CTRL_GPIO, &GPIO_InitStructure);
}

void SweepRobot_Collision_Chan_On(void)
{
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_L_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_R_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_L_PIN);
    GPIO_SetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_R_PIN);
}

void SweepRobot_Collision_Chan_Off(void)
{
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_L_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_F_R_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_L_PIN);
    GPIO_ResetBits(COLLISION_TEST_CTRL_GPIO, COLLISION_TEST_CTRL_S_R_PIN);
}

static void SweepRobot_CollisionTestInit(void)
{
    u8 i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_COLLISION_TEST_TASK_PRIO;
    
    str = "\r\n>>>COLLISION TEST<<<";
    SWRB_TestDataFileWriteString(str);
    
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(str);

    OSTimeDlyHMSM(0,0,1,0);
    
    SweepRobot_Collision_Chan_On();
    
    for(i=0;i<COLLISION_CHAN_NUM;i++){
        collision[i].value = 0;
        collision[i].validCnt = 0;
        collision[i].validFlag = 0;
    }
}

static void SweepRobot_CollisionTestProc(void)
{
    u8 i,j;
    
    for(i=0;i<COLLISION_CHAN_NUM;i++){
        if(!collision[i].validFlag){
            for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
                printf("COLLISION->READ=%d\r\n",i);
                OSTimeDlyHMSM(0,0,0,6);
                if(usartRxFlag){
                    Edit_Set_Value(ID_EDIT_U1+i, usartRxNum);
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
                collision[i].validCnt = 0;
            }

            if(collision[i].validCnt > 5){
                collision[i].validFlag = 1;
            }
        }
    }
    
    if(collision[0].validFlag && collision[1].validFlag && collision[2].validFlag && collision[3].validFlag){
        gSwrbTestTaskRunCnt = 0;
        SweepRobot_Collision_Chan_Off();

        for(i=0;i<COLLISION_CHAN_NUM;i++){
            gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_VALUE_POS+i] = collision[i].value;
        }
        SWRB_TestDataSaveToFile(Collision_TestDataSave);
        
        MultiEdit_Add_Text("COLLISION OK\r\n");
        Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_BLUE);
        Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION OK");
        Progbar_Set_Percent(SWRB_TEST_STATE_COLLISION);

        SWRB_NextTestTaskResume(SWRB_COLLISION_TEST_TASK_PRIO);
    }
}

static void SweepRobot_CollisionTestOverTimeProc(void)
{
    u8 i;
    
    gSwrbTestTaskRunCnt = 0;
    SweepRobot_Collision_Chan_Off();

    for(i=0;i<COLLISION_CHAN_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_COLLISION_L_VALUE_POS+i] = collision[i].value;
    }
    SWRB_TestDataSaveToFile(Collision_TestDataSave);
    
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_L_MASK)
        MultiEdit_Add_Text("ERROR->COLLISION_L\r\n");
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_L_MASK)
        MultiEdit_Add_Text("ERROR->COLLISION_F_L\r\n");
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_F_R_MASK)
        MultiEdit_Add_Text("ERROR->COLLISION_F_R\r\n");
    if(gSwrbTestStateMap & SWRB_TEST_FAULT_COLLISION_R_MASK)
        MultiEdit_Add_Text("ERROR->COLLISION_R\r\n");
    Checkbox_Set_Text_Color(ID_CHECKBOX_COLLISION, GUI_RED);
    Checkbox_Set_Text(ID_CHECKBOX_COLLISION, "COLLISION ERROR");
    Progbar_Set_Percent(SWRB_TEST_STATE_COLLISION);

    SWRB_NextTestTaskResume(SWRB_COLLISION_TEST_TASK_PRIO);
}

void SweepRobot_CollisionTestTask(void *pdata)
{
    
    SweepRobot_CollisionTestGPIOInit();

    while(1){
        
        if(!Checkbox_Get_State(ID_CHECKBOX_COLLISION)){
            SWRB_NextTestTaskResume(SWRB_COLLISION_TEST_TASK_PRIO);
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
    SWRB_TestDataFileWriteData("COLLISION->L_Value=", collision[0].value);
    SWRB_TestDataFileWriteData("COLLISION->FL_Value=", collision[1].value);
    SWRB_TestDataFileWriteData("COLLISION->FR_Value=", collision[2].value);
    SWRB_TestDataFileWriteData("COLLISION->R_Value=", collision[3].value);
}
