#include "usbh_usr.h"
#include "led.h"
#include "ff.h"
#include "usart.h"

static u8 AppState;

extern USBH_HOST        USB_Host;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

void OTG_FS_IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

USBH_Usr_cb_TypeDef USR_Callbacks=
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_MSC_Application,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
};

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- USB HOST Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]     = "> Device Attached \n";
const uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "> Enumeration completed \n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]        = "> Device fault \n";

const uint8_t MSG_MSC_CLASS[]        = "> Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]        = "> HID device connected\n";
const uint8_t MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \n";
const uint8_t MSG_LUN[]              = "> LUN Available in the device:\n";
const uint8_t MSG_ROOT_CONT[]        = "> Exploring disk flash ...\n";
const uint8_t MSG_WR_PROTECT[]       = "> The disk is write protected\n";
const uint8_t MSG_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE\n";

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
static uint8_t  Explore_Disk (char* path , uint8_t recu_level);
static uint8_t  Image_Browser (char* path);
static void     Show_Image(void);
static void     Toggle_Leds(void);
/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/


/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
    printf("USB OTG HS MSC Host\r\n");
    printf("> USB Host library started.\r\n");
    printf("  USB Host Library v2.1.0\r\n\r\n");
}

void USBH_USR_DeviceAttached(void)
{
    LED1=1;
    printf("Device Attached r\n");
}

void USBH_USR_DeviceDisconnected(void)
{
    LED1=0;
    printf("Device Disconnected\r\n");
}

void USBH_USR_ResetDevice(void)
{
    printf("Device reset\r\n");
}

void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED){
        printf("High speed device detected\r\n");
    }else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED){
        printf("Full speed device detected\r\n");
    }else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED){
        printf("Low speed device detected\r\n");
    }else{
        printf("Device fault\r\n");
    }
}

void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
    USBH_DevDesc_TypeDef *hs;
    hs=DeviceDesc;
    printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor);
    printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct);
}

void USBH_USR_DeviceAddressAssigned(void)
{
    printf("Device address assign success\r\n");
}

void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
    USBH_InterfaceDesc_TypeDef *id;
    id = itfDesc;
    if((*id).bInterfaceClass==0x08){
        printf("Mass storage device connected\r\n");
    }else if((*id).bInterfaceClass==0x03){
        printf("HID device connected\r\n");
    }
}

void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}

void USBH_USR_Product_String(void *ProductString)
{
    printf("Product: %s\r\n",(char *)ProductString);
}

void USBH_USR_SerialNum_String(void *SerialNumString)
{
    printf("Serial Number: %s\r\n",(char *)SerialNumString);
}

void USBH_USR_EnumerationDone(void)
{
    printf("Enumeration completed\r\n\r\n");
}

void USBH_USR_DeviceNotSupported(void)
{
    printf("No registered class for this device.\r\n");
}

USBH_USR_Status USBH_USR_UserInput(void)
{
    printf("Skip user input\r\n");
    return USBH_USR_RESP_OK;
}

void USBH_USR_OverCurrentDetected (void)
{
    printf("Overcurrent detected.\r\n");
}

extern u8 USB_HOST_User_App(void);

int USBH_USR_MSC_Application(void)
{
    u8 res=0;

    switch(AppState){
        case USH_USR_FS_INIT:
            printf("start executing user program\r\n");
            AppState=USH_USR_FS_TEST;
            break;
        case USH_USR_FS_TEST:
            res=USB_HOST_User_App();
            res=0;
            if(res)AppState=USH_USR_FS_INIT;
            break;
        default:break;
    }
    return res;
}

void USBH_USR_DeInit(void)
{
    AppState=USH_USR_FS_INIT;
}

void USBH_USR_UnrecoveredError (void)
{
    printf("UnrecoveredError occured\r\n");
}

u8 USBH_UDISK_Status(void)
{
    return HCD_IsDeviceConnected(&USB_OTG_Core);
}

u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
    u8 res=1;

    if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST){
        do{
            res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
            if(!HCD_IsDeviceConnected(&USB_OTG_Core)){
                res=1;
                break;
            };
        }while(res==USBH_MSC_BUSY);
    }else
        res=1;

    if(res==USBH_MSC_OK)
        res=0;

    return res;
}

u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
    u8 res=1;

    if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST){
        do{
            res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
            if(!HCD_IsDeviceConnected(&USB_OTG_Core)){
                res=1;
                break;
            }
        }while(res==USBH_MSC_BUSY);
    }else
        res=1;

    if(res==USBH_MSC_OK)
        res=0;

    return res;
}
