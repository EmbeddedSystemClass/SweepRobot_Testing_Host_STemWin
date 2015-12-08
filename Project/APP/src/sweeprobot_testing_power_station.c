#include "sweeprobot_testing_power_station.h"

#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

enum PS_RX_POS{
    SWRB_TEST_PS_RX_LL_POS,
    SWRB_TEST_PS_RX_LS_POS,
    SWRB_TEST_PS_RX_L_POS,
    SWRB_TEST_PS_RX_M_POS,
    SWRB_TEST_PS_RX_R_POS,
    SWRB_TEST_PS_RX_RS_POS,
    SWRB_TEST_PS_RX_RL_POS,
    SWRB_TEST_PS_24V_STATE_POS,
};

#define SWRB_TEST_PS_TX_SIG_NUM        7

static PowerStation_TestTypeDef powerStation[SWRB_TEST_PS_TX_SIG_NUM];
static PowerStation_TestTypeDef powerStation24V;

static u8 gIrDATmpCode;
static u8 gIndicateFlag = 0;

static void SweepRobot_PowerStationDrawGraph(void);

static void SweepRobot_PowerStationTestInit(void)
{
    int i;
    char *str;
    
    gSwrbTestRuningTaskPrio = SWRB_POWER_STATION_TEST_TASK_PRIO;
    
    str = "\r\n>>>POWER STATION TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

    printf("IRDA->OFF\r\n");
    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_INIT_WAIT_TIME_MS);
    
    for(i=0;i<SWRB_TEST_PS_TX_SIG_NUM;i++){
        powerStation[i].code = 0;
        powerStation[i].validCnt = 0;
        powerStation[i].validFlag = 0;
    }
    
    powerStation24V.state = 0;
    powerStation24V.validCnt = 0;
    powerStation24V.validFlag = 0;
    
    for(i=ID_PS_CHECKBOX_LL;i<=ID_PS_CHECKBOX_24V;i++){
        Checkbox_Set_State(hWin_SWRB_POWER_STATION, i, 0);
        Checkbox_Set_Box_Back_Color(hWin_SWRB_POWER_STATION, i, GUI_LIGHTGRAY, CHECKBOX_CI_DISABLED);
    }
}

static void SweepRobot_PowerStationTestSigGet(u8 codePos, u8 code)
{
    static u8 codeRecCnt = 0;
    char *str;
    
    
    if(!powerStation[codePos].validFlag){
        if(gIrDATmpCode == code){
            gSwrbTestStateMap &= ~(1<<(SWRB_TEST_PS_RX_LL_POS+codePos));
            powerStation[0].validFlag = 1;
            Checkbox_Set_State(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_LL+codePos, 1);
            Checkbox_Set_Box_Back_Color(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_LL+codePos, GUI_GREEN, CHECKBOX_CI_DISABLED);
            
            str = mymalloc(SRAMIN, sizeof(char)*30);
            codeRecCnt++;
            sprintf(str, "CODE[%d]->%x\r\n", codeRecCnt, code);
            MultiEdit_Add_Text(hWin_SWRB_POWER_STATION, ID_PS_MULTIEDIT_MAIN, str);
            myfree(SRAMIN, str);
        }else{
            gSwrbTestStateMap |= (1<<(SWRB_TEST_PS_RX_LL_POS+codePos));
        }
    }
}

static void SweepRobot_PowerStationTest24VStateGet(void)
{
    u8 i;
    
    if(!powerStation24V.validFlag){
        for(i=0;i<SWRB_TEST_USART_READ_TIMES;i++){
            printf("CHARGE->READ=2\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                powerStation24V.state = usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                USART_RX_STA = 0;
                break;
            }else{
                powerStation24V.state = 0;
                continue;
            }
        }

        if(powerStation24V.state == 1){
            gSwrbTestStateMap &= ~(1<<SWRB_TEST_PS_24V_STATE_POS);
            Checkbox_Set_State(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_24V, 1);
            Checkbox_Set_Box_Back_Color(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_24V, GUI_GREEN, CHECKBOX_CI_DISABLED);
            powerStation24V.validFlag = 1;
        }else{
            gSwrbTestStateMap |= (1<<SWRB_TEST_PS_24V_STATE_POS);
        }
    }
}

static void SweepRobot_PowerStationTestChargeVolGet(void)
{
    
}

static void SweepRobot_PowerStationTestChargeCurGet(void)
{
    
}

/* TODO: add charge voltage and current graph display function */
static void SweepRobot_PowerStationDrawGraph(void)
{
    if(powerStation24V.state == 1){
        
    }
}


static void SweepRobot_PowerStationTestProc(void)
{
    u8 i,j;
    char *str;
    
    for(i=0;i<SWRB_TEST_PS_TX_SIG_NUM;i++){
        for(j=0;j<SWRB_TEST_USART_READ_TIMES;j++){
            printf("POWER_STATION->READ\r\n");
            OSTimeDlyHMSM(0,0,0,6);
            if(usartRxFlag){
                gIrDATmpCode = usartRxNum;
                usartRxNum = 0;
                usartRxFlag = 0;
                USART_RX_STA = 0;
                break;
            }else{
                gIrDATmpCode = 0;
                continue;
            }
        }
        
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_LL_POS, 0x41);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_LS_POS, 0x44);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_L_POS, 0x46);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_M_POS, 0x40);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_R_POS, 0x45);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_RS_POS, 0x42);
        SweepRobot_PowerStationTestSigGet(SWRB_TEST_PS_RX_RL_POS, 0x43);
        SweepRobot_PowerStationTest24VStateGet();

        if( powerStation[0].validFlag && powerStation[1].validFlag && powerStation[2].validFlag && powerStation[3].validFlag &&\
            powerStation[4].validFlag && powerStation[5].validFlag && powerStation[6].validFlag && powerStation24V.validFlag){
               
            gSwrbTestTaskRunCnt = 0;

            SWRB_TestDataSaveToFile(PowerStation_TestDataSave);

            str = "POWER STATION OK\r\n";
            SWRB_TestDataFileWriteString(str);
                
            Checkbox_Set_State(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_INDICATE, 1);
            Checkbox_Set_Text(hWin_SWRB_POWER_STATION, ID_PS_CHECKBOX_INDICATE, "PowerStation OK");
        }
    }
}

void SweepRobot_PowerStationTestTask(void *pdata)
{
    while(1){
        gSwrbTestTaskRunCnt++;

        if(gSwrbTestTaskRunCnt == 1){
            SweepRobot_PowerStationTestInit();
        }

        if(gSwrbTestTaskRunCnt > 1){
            SweepRobot_PowerStationTestProc();
        }

        OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
    }
}

void PowerStation_TestDataSave(void)
{
    u8 i;
    
    for(i=0;i<SWRB_TEST_PS_TX_SIG_NUM;i++){
        gSwrbTestAcquiredData[SWRB_TEST_DATA_POWER_STATION_SIG_LL_POS+i] = powerStation[i].code;
    }
    gSwrbTestAcquiredData[SWRB_TEST_DATA_POWER_STATION_24V_STATE_POS] = powerStation24V.state;
    
    SWRB_TestDataFileWriteData("POWER_STATION->LL_Code=", powerStation[0].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->LS_Code=", powerStation[1].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->L_Code=", powerStation[2].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->M_Code=", powerStation[3].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->R_Code=", powerStation[4].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->RS_Code=", powerStation[5].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->RL_Code=", powerStation[6].code, 1);
    SWRB_TestDataFileWriteData("POWER_STATION->24V_State=", powerStation24V.state, 1);
}

void SweepRobot_PowerStationIndicateBtnToggle(void)
{
    if(++gIndicateFlag%2){
        Button_Set_unPressedBkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_INDICATE, GUI_DARKGREEN);
    }else{
        Button_Set_unPressedBkColor(hWin_SWRB_POWER_STATION, ID_PS_BUTTON_INDICATE, GUI_LIGHTGRAY);
    }
}
