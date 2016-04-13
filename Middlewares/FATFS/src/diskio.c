/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"        /* FatFs lower layer API */
#include "sdio_sdcard.h"
#include "w25qxx.h"
#include "malloc.h"
#include "usbh_usr.h"

#define USB_DISK    2
#define EX_FLASH    1
#define SD_CARD     0

#define FLASH_SECTOR_SIZE       4*1024      //512

static const unsigned short FLASH_SECTOR_COUNT=4096;   //=256*16       //2048*12
static const unsigned short FLASH_BLOCK_SIZE=65535;       //=4*1024*16;   //8

DSTATUS disk_initialize (
    BYTE pdrv                /* Physical drive nmuber (0..) */
)
{
    u8 res=0;
    switch(pdrv)
    {
#ifdef USE_SD_CARD
        case SD_CARD:
            res=SD_Init();
              break;
#endif
        case EX_FLASH:
             W25QXX_Init();
//             FLASH_SECTOR_COUNT=2048*12;
             break;
        case USB_DISK:
              if(USBH_UDISK_Status())return 0;
            else return 1;
        default:
            res=1;
    }
    if(res)return  STA_NOINIT;
    else return 0;
}

DSTATUS disk_status (
    BYTE pdrv        /* Physical drive nmuber (0..) */
)
{
    return 0;
}

DRESULT disk_read (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    UINT count        /* Number of sectors to read (1..128) */
)
{
    u8 res=0;
    if (!count)return RES_PARERR;
    switch(pdrv)
    {
#ifdef USE_SD_CARD
        case SD_CARD:
            res=SD_ReadDisk(buff,sector,count);
            while(res)
            {
                SD_Init();
                res=SD_ReadDisk(buff,sector,count);
                //printf("sd rd error:%d\r\n",res);
            }
            break;
#endif
        case EX_FLASH:
            for(;count>0;count--)
            {
                W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
                sector++;
                buff+=FLASH_SECTOR_SIZE;
            }
            res=0;
            break;
        case USB_DISK:
            res=USBH_UDISK_Read(buff,sector,count);
            break;
        default:
            res=1;
    }
    if(res==0x00)return RES_OK;
    else return RES_ERROR;
}

#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,            /* Physical drive nmuber (0..) */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    UINT count            /* Number of sectors to write (1..128) */
)
{
    u8 res=0;
    if (!count)return RES_PARERR;
    switch(pdrv)
    {
#ifdef USE_SD_CARD
        case SD_CARD:
            res=SD_WriteDisk((u8*)buff,sector,count);
            while(res)
            {
                SD_Init();
                res=SD_WriteDisk((u8*)buff,sector,count);
                //printf("sd wr error:%d\r\n",res);
            }
            break;
#endif
        case EX_FLASH:
            for(;count>0;count--)
            {
                W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
                sector++;
                buff+=FLASH_SECTOR_SIZE;
            }
            res=0;
            break;
        case USB_DISK:
            res=USBH_UDISK_Write((u8*)buff,sector,count);
            break;
        default:
            res=1;
    }
    if(res == 0x00)return RES_OK;
    else return RES_ERROR;
}
#endif

#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,         /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
    switch(pdrv){
#ifdef USE_SD_CARD
        case SD_CARD:
            switch(cmd){
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                case GET_SECTOR_SIZE:
                    *(DWORD*)buff = 512;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(WORD*)buff = SDCardInfo.CardBlockSize;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff = SDCardInfo.CardCapacity/512;
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
            break;
#endif
        case EX_FLASH:
            switch(cmd){
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                case GET_SECTOR_SIZE:
                    *(WORD*)buff = FLASH_SECTOR_SIZE;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(WORD*)buff = FLASH_BLOCK_SIZE;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff = FLASH_SECTOR_COUNT;
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
            break;
        case USB_DISK:
            switch(cmd){
                case CTRL_SYNC:
                    res = RES_OK;
                    break;
                case GET_SECTOR_SIZE:
                    *(WORD*)buff=4096;
                    res = RES_OK;
                    break;
                case GET_BLOCK_SIZE:
                    *(WORD*)buff=4096;
                    res = RES_OK;
                    break;
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff=USBH_MSC_Param.MSCapacity;
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
            break;
        default:
            res=RES_ERROR;
            break;
    }

    return res;
}
#endif

DWORD get_fattime (void)
{
    return 0;
}

void *ff_memalloc (UINT size)
{
    return (void*)mymalloc(SRAMIN,size);
}

void ff_memfree (void* mf)
{
    myfree(SRAMIN,mf);
}
