#include "sweeprobot_testing_rgb_led.h"
#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

#include "usart.h"
#include "includes.h"

extern GUI_CONST_STORAGE GUI_BITMAP _bmREDLEDCHECKCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmGREENLEDCHECKCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmBLUELEDCHECKCHN;

static RGB_LED_TestTypeDef rgb_led;

static void SweepRobot_RGBLEDTestOKProc(void);
static void SweepRobot_RGBLEDTestErrProc(void);

static void SweepRobot_RGBLEDTestInit(void)
{
    char *str;

    gSwrbTestRuningTaskPrio = SWRB_RGB_LED_TEST_TASK_PRIO;

    str = "\r\n>>>RGB LED TEST<<<\r\n";
    SWRB_TestDataFileWriteString(str);

#ifdef _SHOW_TEST_TITLE
    MultiEdit_Set_Text_Color(GUI_BLACK);
    MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
#endif

    SWRB_TestInitCommonAct(gSwrbTestRuningTaskPrio);

    mymemset(&rgb_led, 0, sizeof(rgb_led));

    OSTimeDlyHMSM(0,0,0,SWRB_TEST_TASK_INIT_WAIT_TIME_MS);

    hWin_SWRB_RGB_LED = CreateRGB_LED_TestDLG();
    WM_BringToTop(hWin_SWRB_RGB_LED);
}

static void SweepRobot_RGBLEDTestSingleProc(int *ledState, GUI_COLOR color, char *strText, char *strMultiEdit1, char *strMultiEdit2)
{
    Text_Set_Color(hWin_SWRB_RGB_LED, ID_PCBTEST_TEXT_RGB_LED, color);
    Text_Set_Text(hWin_SWRB_RGB_LED, ID_PCBTEST_TEXT_RGB_LED, strText);
    OSTaskSuspend(gSwrbTestRuningTaskPrio);
    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        if(*ledState){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  strMultiEdit1);
            SWRB_TestDataFileWriteString(strMultiEdit1);
        }else{
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  strMultiEdit2);
            SWRB_TestDataFileWriteString(strMultiEdit2);
        }
    }
}

static void SweepRobot_RGBLEDTestSingleProcWithCHN(int *ledState, const GUI_BITMAP *bitmap, char *strMultiEdit1, char *strMultiEdit2)
{
    BUTTON_Set_Bitmap_Ex(hWin_SWRB_RGB_LED, ID_PCBTEST_BUTTON_RGB_LED_TEXT, bitmap, 90, 15);
    OSTaskSuspend(gSwrbTestRuningTaskPrio);
    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        if(*ledState){
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  strMultiEdit1);
            SWRB_TestDataFileWriteString(strMultiEdit1);
        }else{
            MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  strMultiEdit2);
            SWRB_TestDataFileWriteString(strMultiEdit2);
        }
    }
}

static void SweepRobot_RGBLEDManulTestSingleProc(int *ledState, enum SWRB_MANUL_TEST_DATA_RGB_LED_POS pos, GUI_COLOR okColor, GUI_COLOR errColor)
{
    if (gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_MANUL){
        if(*ledState){
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                                gSwrbManulTestListviewDispDataRGBLEDCoord[pos][0],\
                                                                gSwrbManulTestListviewDispDataRGBLEDCoord[pos][1],\
                                                                okColor);
        }else{
            Listview_Set_Item_BkColor(hWin_SWRB_MANUL, ID_MANUL_LISTVIEW_MAIN,\
                                                                gSwrbManulTestListviewDispDataRGBLEDCoord[pos][0],\
                                                                gSwrbManulTestListviewDispDataRGBLEDCoord[pos][1],\
                                                                errColor);
        }
    }
}

static void SweepRobot_RGBLEDTestProc(void)
{
    rgb_led.validCnt = 1;
    printf("RGB->ON=%d\r\n", RGB_LED_RED);
//    SweepRobot_RGBLEDTestSingleProc(&rgb_led.r_state, GUI_DARKRED, "IS RED LED OK?", ""/*"RED LED OK\r\n"*/, "ERROR->RED LED\r\n");
    SweepRobot_RGBLEDTestSingleProcWithCHN(&rgb_led.r_state, &_bmREDLEDCHECKCHN, ""/*"RED LED OK\r\n"*/, "ERROR->RED LED\r\n");
    SweepRobot_RGBLEDManulTestSingleProc(&rgb_led.r_state, SWRB_MANUL_TEST_DATA_RGB_LED_RED_POS, GUI_LIGHTBLUE, GUI_LIGHTRED);
    rgb_led.validCnt = 2;
    printf("RGB->ON=%d\r\n", RGB_LED_GREEN);
//    SweepRobot_RGBLEDTestSingleProc(&rgb_led.g_state, GUI_DARKGREEN, "IS GREEN LED OK?", ""/*"GREEN LED OK\r\n"*/, "ERROR->GREEN LED\r\n");
    SweepRobot_RGBLEDTestSingleProcWithCHN(&rgb_led.g_state, &_bmGREENLEDCHECKCHN, ""/*"GREEN LED OK\r\n"*/, "ERROR->GREEN LED\r\n");
    SweepRobot_RGBLEDManulTestSingleProc(&rgb_led.g_state, SWRB_MANUL_TEST_DATA_RGB_LED_GREEN_POS, GUI_LIGHTBLUE, GUI_LIGHTRED);
    rgb_led.validCnt = 3;
    printf("RGB->ON=%d\r\n", RGB_LED_BLUE);
//    SweepRobot_RGBLEDTestSingleProc(&rgb_led.b_state, GUI_DARKBLUE, "IS BLUE LED OK?", ""/*"BLUE LED OK\r\n"*/, "ERROR->BLUE LED\r\n");
    SweepRobot_RGBLEDTestSingleProcWithCHN(&rgb_led.b_state, &_bmBLUELEDCHECKCHN, ""/*"BLUE LED OK\r\n"*/, "ERROR->BLUE LED\r\n");
    SweepRobot_RGBLEDManulTestSingleProc(&rgb_led.b_state, SWRB_MANUL_TEST_DATA_RGB_LED_BLUE_POS, GUI_LIGHTBLUE, GUI_LIGHTRED);

    if(rgb_led.r_state && rgb_led.g_state && rgb_led.b_state){
        SweepRobot_RGBLEDTestOKProc();
    }else{
        SweepRobot_RGBLEDTestErrProc();
    }
}

static void SweepRobot_RGBLEDTestOKProc(void)
{
    char *str;

    gSwrbTestTaskRunCnt = 0;
    printf("RGB->ON=2\r\n");
    GUI_EndDialog(hWin_SWRB_RGB_LED, 0);

    SWRB_TestDataSaveToFile(RGB_LED_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        str = "RGB LED OK\r\n";
        SWRB_TestDataFileWriteString(str);

    //        MultiEdit_Add_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_MULTIEDIT_MAIN,  str);
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_RGB_LED, GUI_BLUE);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_RGB_LED, "RGB LED OK");
        Edit_Clear();
    }

    SWRB_NextTestTaskResumePostAct(SWRB_RGB_LED_TEST_TASK_PRIO);
}

static void SweepRobot_RGBLEDTestErrProc(void)
{
    gSwrbTestTaskRunCnt = 0;
    printf("RGB->ON=2\r\n");
    GUI_EndDialog(hWin_SWRB_RGB_LED, 0);

    SWRB_TestDataSaveToFile(RGB_LED_TestDataSave);

    if(gSwrbDialogSelectFlag == SWRB_DIALOG_SELECT_PCB){
        Checkbox_Set_Text_Color(ID_PCBTEST_CHECKBOX_RGB_LED, GUI_RED);
        Checkbox_Set_Text(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_RGB_LED, "RGB LED ERROR");
        Edit_Clear();
    }

#ifdef _TASK_WAIT_WHEN_ERROR
    SWRB_TestTaskErrorAct();
#else
    SWRB_NextTestTaskResumePostAct(SWRB_RGB_LED_TEST_TASK_PRIO);
#endif
}

void SweepRobot_RGBLEDTestTask(void *pdata)
{
    while(1){

        if(!Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_RGB_LED)){
            SWRB_NextTestTaskResumePreAct(SWRB_RGB_LED_TEST_TASK_PRIO);
        }else{
            gSwrbTestTaskRunCnt++;

            if(gSwrbTestTaskRunCnt ==1){
                SweepRobot_RGBLEDTestInit();
            }

            if(gSwrbTestTaskRunCnt >1){
                SweepRobot_RGBLEDTestProc();
            }

            OSTimeDlyHMSM(0,0,0,SWRB_TEST_TEST_TASK_OSTIMEDLY_TIME_MS);
        }
    }
}

void RGB_LED_TestDataSave(void)
{
    SWRB_TestDataFileWriteData("RGB_LED->R_State=", rgb_led.r_state, 1);
    SWRB_TestDataFileWriteData("RGB_LED->G_State=", rgb_led.g_state, 1);
    SWRB_TestDataFileWriteData("RGB_LED->B_State=", rgb_led.b_state, 1);
}

u8 RGB_LED_TestValidCntGet(void)
{
    return rgb_led.validCnt;
}

void RGB_LED_TestRgbStateSet(int state, int value)
{
    switch(state){
        case 1:
            rgb_led.r_state = value;
            break;
        case 2:
            rgb_led.g_state = value;
            break;
        case 3:
            rgb_led.b_state = value;
            break;
        default:break;
    }
}
