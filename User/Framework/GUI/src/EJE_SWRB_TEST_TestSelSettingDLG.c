#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define IS_TESTSEL_BUTTON_ID(id)   (  (id == ID_TESTSEL_BUTTON_WHEEL)       ||\
                                      (id == ID_TESTSEL_BUTTON_BRUSH)       ||\
                                      (id == ID_TESTSEL_BUTTON_FAN)         ||\
                                      (id == ID_TESTSEL_BUTTON_IFRD)        ||\
                                      (id == ID_TESTSEL_BUTTON_COLLISION)   ||\
                                      (id == ID_TESTSEL_BUTTON_WHEEL_FLOAT) ||\
                                      (id == ID_TESTSEL_BUTTON_ASH_TRAY)    ||\
                                      (id == ID_TESTSEL_BUTTON_UNIWHEEL)    ||\
                                      (id == ID_TESTSEL_BUTTON_KEY)         ||\
                                      (id == ID_TESTSEL_BUTTON_IRDA)        ||\
                                      (id == ID_TESTSEL_BUTTON_BUZZER)      ||\
                                      (id == ID_TESTSEL_BUTTON_RGB_LED)     ||\
                                      (id == ID_TESTSEL_BUTTON_CHARGE)      \
                                    )
//                                      (id == ID_TESTSEL_BUTTON_TEST1)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST2)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST3)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST4)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST5)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST6)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST7)       \
//                                    )

#define SWRB_TEST_TASK_NUM          (SWRB_TEST_TASK_PRIO_END_BOUND - (SWRB_TEST_TASK_PRIO_START_BOUND+1))
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static u8 gSwrbTest_TaskSelState[SWRB_TEST_TASK_NUM] = { 0 };

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void Button_Init(WM_HWIN hItem)
{
    BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
    BUTTON_SetSkinClassic(hItem);
    BUTTON_SetFocussable(hItem, DISABLE);
    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
}

static void Button_ConfirmProc(WM_HWIN hWin)
{
    SWRB_ValidTestTaskCntGet();

    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_PCBTEST);
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;
}

static void Button_SelAllProc(void)
{
    int id;

    SWRB_TestCheckboxStateSet(1);

    for(id=ID_TESTSEL_BUTTON_WHEEL;id<=ID_TESTSEL_BUTTON_CHARGE;id++){
        Button_Set_BkColor(hWin_SWRB_TESTSEL, id, GUI_LIGHTBLUE);
    }
}

static void Button_SelSingleProc(int id)
{
    WM_HWIN hItem;

    hItem = WM_GetDialogItem(hWin_SWRB_PCBTEST, id-ID_TESTSEL_BUTTON_WHEEL+ID_PCBTEST_CHECKBOX_WHEEL);

    if(CHECKBOX_GetState(hItem)){
        CHECKBOX_SetState(hItem, 0);
        Button_Set_BkColor(hWin_SWRB_TESTSEL, id, GUI_LIGHTGRAY);
    }else{
        CHECKBOX_SetState(hItem, 1);
        Button_Set_BkColor(hWin_SWRB_TESTSEL, id, GUI_LIGHTBLUE);
    }
}

static void Button_SelNoneProc(void)
{
    int id;

    SWRB_TestCheckboxStateSet(0);

    for(id=ID_TESTSEL_BUTTON_WHEEL;id<=ID_TESTSEL_BUTTON_CHARGE;id++){
        Button_Set_BkColor(hWin_SWRB_TESTSEL, id, GUI_LIGHTGRAY);
    }
}

static void Button_CancelResetCheckBoxStateProc(void)
{
    u8 i;

    for(i=0;i<=SWRB_TEST_TASK_NUM;i++){
        Checkbox_Set_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL,gSwrbTest_TaskSelState[i]);
        if(gSwrbTest_TaskSelState[i] == 1){
            Button_Set_BkColor(hWin_SWRB_TESTSEL, ID_TESTSEL_BUTTON_WHEEL+i, GUI_LIGHTBLUE);
        }else{
            Button_Set_BkColor(hWin_SWRB_TESTSEL, ID_TESTSEL_BUTTON_WHEEL+i, GUI_LIGHTGRAY);
        }
    }
}

static void Button_CancelProc(WM_HWIN hWin)
{
    Button_CancelResetCheckBoxStateProc();
    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_PCBTEST);
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;
}



/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "TestSelSettingDLG", ID_TESTSEL_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL", ID_TESTSEL_BUTTON_WHEEL, 0, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BRUSH", ID_TESTSEL_BUTTON_BRUSH, 140, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "FAN", ID_TESTSEL_BUTTON_FAN, 280, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IFRD", ID_TESTSEL_BUTTON_IFRD, 420, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "COLLISION", ID_TESTSEL_BUTTON_COLLISION, 560, 0, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL FLOAT", ID_TESTSEL_BUTTON_WHEEL_FLOAT, 0, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ASH TRAY", ID_TESTSEL_BUTTON_ASH_TRAY, 140, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UNIWHEEL", ID_TESTSEL_BUTTON_UNIWHEEL, 280, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY", ID_TESTSEL_BUTTON_KEY, 420, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IRDA", ID_TESTSEL_BUTTON_IRDA, 560, 105, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUZZER", ID_TESTSEL_BUTTON_BUZZER, 0, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RGB LED", ID_TESTSEL_BUTTON_RGB_LED, 140, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CHARGE", ID_TESTSEL_BUTTON_CHARGE, 280, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST1", ID_TESTSEL_BUTTON_TEST1, 420, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST2", ID_TESTSEL_BUTTON_TEST2, 560, 210, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST3", ID_TESTSEL_BUTTON_TEST3, 0, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST4", ID_TESTSEL_BUTTON_TEST4, 140, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST5", ID_TESTSEL_BUTTON_TEST5, 280, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST6", ID_TESTSEL_BUTTON_TEST6, 420, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST7", ID_TESTSEL_BUTTON_TEST7, 560, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Confirm", ID_TESTSEL_BUTTON_CONFIRM, 700, 0, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelAll", ID_TESTSEL_BUTTON_SELALL, 700, 120, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelNone", ID_TESTSEL_BUTTON_SELNONE, 700, 240, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_TESTSEL_BUTTON_CANCEL, 700, 360, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SnSet", ID_TESTSEL_BUTTON_SNSET, 0, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimeSet", ID_TESTSEL_BUTTON_TIMESET, 100, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TestSel", ID_TESTSEL_BUTTON_TESTSELSET, 200, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv2", ID_TESTSEL_BUTTON_RESERVE1, 300, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv3", ID_TESTSEL_BUTTON_RESERVE2, 400, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv4", ID_TESTSEL_BUTTON_RESERVE3, 500, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv5", ID_TESTSEL_BUTTON_RESERVE4, 600, 420, 100, 60, 0, 0x0, 0 },
};

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     i;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    for(i=ID_TESTSEL_BUTTON_0;i<ID_TESTSEL_BUTTON_BOUND;i++){
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        Button_Init(hItem);
        if( (ID_TESTSEL_BUTTON_WHEEL <= i) && (ID_TESTSEL_BUTTON_CHARGE >= i) ){
            Button_Set_BkColor(pMsg->hWin, i, GUI_LIGHTBLUE);
        }else if(i<=ID_TESTSEL_BUTTON_RESERVE4){
            if(i%2){

            }else{
                BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_LIGHTGRAY);
            }
        }else if(ID_TESTSEL_BUTTON_TEST1 <= i && ID_TESTSEL_BUTTON_TEST7 >= i){
            WM_DisableWindow(hItem);
        }else{

        }
    }

    WM_HideWin(pMsg->hWin);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    if(IS_TESTSEL_BUTTON_ID(Id)){
        switch(NCode) {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                Button_SelSingleProc(Id);
                break;
        }
        break;
    }else{
        switch(Id) {
        case ID_TESTSEL_BUTTON_CONFIRM: // Notifications sent by 'btnConfirm'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            Button_ConfirmProc(pMsg->hWin);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_SELALL: // Notifications sent by 'btnSelAll'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            Button_SelAllProc();
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_SELNONE: // Notifications sent by 'btnSelNone'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            Button_SelNoneProc();
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_CANCEL: // Notifications sent by 'btnCancel'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            Button_CancelProc(pMsg->hWin);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_SNSET: // Notifications sent by 'btnSnSet'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            hItem = WM_GetDialogItem(hWin_SWRB_SNSETTING, ID_SNSET_BUTTON_SNSET);
            BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
            BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
            BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
            gSwrbTestSetState = SWRB_TEST_SET_STATE_SN;
            WM_HideWin(pMsg->hWin);
            WM_ShowWin(hWin_SWRB_SNSETTING);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_TIMESET: // Notifications sent by 'btnTimeSet'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            hItem = WM_GetDialogItem(hWin_SWRB_TIMESETTING, ID_TIMESET_BUTTON_TIMESET);
            BUTTON_SetBkColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
            BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLACK);
            BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
            gSwrbTestSetState = SWRB_TEST_SET_STATE_TIME;
            WM_HideWin(pMsg->hWin);
            WM_ShowWin(hWin_SWRB_TIMESETTING);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_TESTSELSET: // Notifications sent by 'btnTestSelSet'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_RESERVE1: // Notifications sent by 'btnRvs1'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_RESERVE2: // Notifications sent by 'btnRvs2'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            break;
          }
          break;
        }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

WM_HWIN hWin_SWRB_TESTSEL;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateTestSelSettingDLG
*/

WM_HWIN CreateTestSelSettingDLG(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

void SWRB_TestSelLastCheckBoxStateSave(void)
{
    u8 i;

    for(i=0;i<=SWRB_TEST_TASK_NUM;i++){
        gSwrbTest_TaskSelState[i] = Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL+i);
    }
}

/*************************** End of file ****************************/

