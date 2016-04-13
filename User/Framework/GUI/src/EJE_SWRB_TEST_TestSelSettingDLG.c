#include "EJE_SWRB_TEST_DLG_Conf.h"
#include "sweeprobot_testing.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

extern GUI_CONST_STORAGE GUI_BITMAP _bmConfirmCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmSelectAllCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmSelectNoneCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmCancelCHN;


extern GUI_CONST_STORAGE GUI_BITMAP _bmWheelCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmBrushCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmFanCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmIFRDCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmCollisionCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmWheelFloatCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmAshTrayCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmUniWheelCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmKeyCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmIrDACHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmBuzzerCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmRGBLEDCHN;
extern GUI_CONST_STORAGE GUI_BITMAP _bmChargeCHN;

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
                                      (id == ID_TESTSEL_BUTTON_CHARGE)      ||\
                                      (id == ID_TESTSEL_BUTTON_FRONT_IFRD)  \
                                    )
//                                      (id == ID_TESTSEL_BUTTON_TEST1)       ||\
//                                      (id == ID_TESTSEL_BUTTON_TEST2)       ||\
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

static u8 aSwrbTest_TaskLastState[SWRB_TEST_TASK_NUM] = { 0 };

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
    WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
}

static void Button_ConfirmProc(WM_HWIN hWin)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    SWRB_ValidTestTaskCntGet();

    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_START);
}

static void Button_SelAllProc(void)
{
    int id;
    int i;

    SWRB_TestCheckboxStateSet(1);

    for(id=ID_TESTSEL_BUTTON_WHEEL,i=0;id<=ID_TESTSEL_BUTTON_KEY;id++,i++){
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

    for(id=ID_TESTSEL_BUTTON_WHEEL;id<=ID_TESTSEL_BUTTON_KEY;id++){
        Button_Set_BkColor(hWin_SWRB_TESTSEL, id, GUI_LIGHTGRAY);
    }
}

static void Button_CancelResetCheckBoxStateProc(void)
{
    u8 i;

    for(i=0;i<=SWRB_TEST_TASK_NUM;i++){
        Checkbox_Set_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL+i,aSwrbTest_TaskLastState[i]);
        if(aSwrbTest_TaskLastState[i] == 1){
            Button_Set_BkColor(hWin_SWRB_TESTSEL, ID_TESTSEL_BUTTON_WHEEL+i, GUI_LIGHTBLUE);
        }else{
            Button_Set_BkColor(hWin_SWRB_TESTSEL, ID_TESTSEL_BUTTON_WHEEL+i, GUI_LIGHTGRAY);
        }
    }
}

static void Button_CancelProc(WM_HWIN hWin)
{
    gSwrbDialogSelectFlag = SWRB_DIALOG_SELECT_NONE;
    gSwrbTestMode = SWRB_TEST_MODE_IDLE;

    Button_CancelResetCheckBoxStateProc();
    WM_HideWin(hWin);
    WM_ShowWin(hWin_SWRB_START);
}

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "TestSelSettingDLG", ID_TESTSEL_WINDOW_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL", ID_TESTSEL_BUTTON_WHEEL, 0, 0, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BRUSH", ID_TESTSEL_BUTTON_BRUSH, 140, 0, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "FAN", ID_TESTSEL_BUTTON_FAN, 280, 0, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IFRD", ID_TESTSEL_BUTTON_IFRD, 420, 0, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "COLLISION", ID_TESTSEL_BUTTON_COLLISION, 560, 0, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "WHEEL FLOAT", ID_TESTSEL_BUTTON_WHEEL_FLOAT, 0, 140, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ASH TRAY", ID_TESTSEL_BUTTON_ASH_TRAY, 140, 140, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "UNIWHEEL", ID_TESTSEL_BUTTON_UNIWHEEL, 280, 140, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "IRDA", ID_TESTSEL_BUTTON_IRDA, 420, 140, 140, 140, 0, 0x0, 0},
  { BUTTON_CreateIndirect, "CHARGE", ID_TESTSEL_BUTTON_CHARGE, 560, 140, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUZZER", ID_TESTSEL_BUTTON_BUZZER, 0, 280, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RGB LED", ID_TESTSEL_BUTTON_RGB_LED, 140, 280, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY", ID_TESTSEL_BUTTON_KEY, 280, 280, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST1", ID_TESTSEL_BUTTON_TEST1, 420, 280, 140, 140, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TEST2", ID_TESTSEL_BUTTON_TEST2, 560, 280, 140, 140, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "FRONT IFRD", ID_TESTSEL_BUTTON_FRONT_IFRD, 0, 315, 140, 105, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "TEST4", ID_TESTSEL_BUTTON_TEST4, 140, 315, 140, 105, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "TEST5", ID_TESTSEL_BUTTON_TEST5, 280, 315, 140, 105, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "TEST6", ID_TESTSEL_BUTTON_TEST6, 420, 315, 140, 105, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "TEST7", ID_TESTSEL_BUTTON_TEST7, 560, 315, 140, 105, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Confirm", ID_TESTSEL_BUTTON_CONFIRM, 700, 0, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelAll", ID_TESTSEL_BUTTON_SELALL, 700, 120, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SelNone", ID_TESTSEL_BUTTON_SELNONE, 700, 240, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_TESTSEL_BUTTON_CANCEL, 700, 360, 100, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SnSet", ID_TESTSEL_BUTTON_SNSET, 0, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TestSel", ID_TESTSEL_BUTTON_TESTSEL, 100, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv2", ID_TESTSEL_BUTTON_RESERVE1, 200, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv3", ID_TESTSEL_BUTTON_RESERVE2, 300, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv4", ID_TESTSEL_BUTTON_RESERVE3, 400, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Rsrv5", ID_TESTSEL_BUTTON_RESERVE4, 500, 420, 100, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TimeSet", ID_TESTSEL_BUTTON_TIMESET, 600, 420, 100, 60, 0, 0x0, 0 },
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
        if( (ID_TESTSEL_BUTTON_WHEEL <= i) && (ID_TESTSEL_BUTTON_KEY >= i) ){
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

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_CONFIRM, "");
    BUTTON_DispConfirmCHNStr(pMsg->hWin, ID_TESTSEL_BUTTON_CONFIRM, 18, 43);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_SELALL, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_SELALL, &_bmSelectAllCHN, 18, 43);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_SELNONE, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_SELNONE, &_bmSelectNoneCHN, 2, 43);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_CANCEL, "");
    BUTTON_DispCancelCHNStr(pMsg->hWin, ID_TESTSEL_BUTTON_CANCEL, 18, 43);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_SNSET, "");
    BUTTON_DispSerialNumCHNStr(pMsg->hWin, ID_TESTSEL_BUTTON_SNSET, 14, 18);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_TIMESET, "");
    BUTTON_DispTimeCHNStr(pMsg->hWin, ID_TESTSEL_BUTTON_TIMESET, 26, 18);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_TESTSEL, "");
    BUTTON_DispTestSelCHNStr(pMsg->hWin, ID_TESTSEL_BUTTON_TESTSEL, 14, 18);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_WHEEL, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_WHEEL, &_bmWheelCHN, 34, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_BRUSH, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_BRUSH, &_bmBrushCHN, 46, 40);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_FAN, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_FAN, &_bmFanCHN, 46, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_IFRD, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_IFRD, &_bmIFRDCHN, 46, 40);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_COLLISION, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_COLLISION, &_bmCollisionCHN, 46, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_WHEEL_FLOAT, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_WHEEL_FLOAT, &_bmWheelFloatCHN, 34, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_ASH_TRAY, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_ASH_TRAY, &_bmAshTrayCHN, 46, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_UNIWHEEL, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_UNIWHEEL, &_bmUniWheelCHN, 34, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_KEY, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_KEY, &_bmKeyCHN, 46, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_IRDA, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_IRDA, &_bmIrDACHN, 46, 40);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_BUZZER, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_BUZZER, &_bmBuzzerCHN, 34, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_RGB_LED, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_RGB_LED, &_bmRGBLEDCHN, 34, 55);

    Button_Set_Text(pMsg->hWin, ID_TESTSEL_BUTTON_CHARGE, "");
    BUTTON_Set_Bitmap_Ex(pMsg->hWin, ID_TESTSEL_BUTTON_CHARGE, &_bmChargeCHN, 46, 55);

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
            Button_Set_BkColor(hWin_SWRB_SNSET, ID_SNSET_BUTTON_SNSET, GUI_LIGHTRED);
            gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_SN;
            WM_HideWin(pMsg->hWin);
            WM_ShowWin(hWin_SWRB_SNSET);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_TIMESET: // Notifications sent by 'btnTimeSet'
          switch(NCode) {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            Button_Set_BkColor(hWin_SWRB_TIMESET, ID_TIMESET_BUTTON_TIMESET, GUI_LIGHTRED);
            gSwrbTestSetSelectFlag = SWRB_TEST_SET_SELECT_TIME;
            WM_HideWin(pMsg->hWin);
            WM_ShowWin(hWin_SWRB_TIMESET);
            break;
          }
          break;
        case ID_TESTSEL_BUTTON_TESTSEL: // Notifications sent by 'btnTestSelSet'
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

void SWRB_TestTaskCheckBoxLastStateSave(void)
{
    u8 i;

    for(i=0;i<SWRB_TEST_TASK_NUM;i++){
        aSwrbTest_TaskLastState[i] = Checkbox_Get_State(hWin_SWRB_PCBTEST, ID_PCBTEST_CHECKBOX_WHEEL+i);
    }
}

/*************************** End of file ****************************/

