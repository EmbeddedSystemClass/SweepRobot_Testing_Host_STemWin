#include "crypto.h"

void SWRB_CRYPInit(void)
{
    CRYP_InitTypeDef CRYP_InitStructure;
    
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_CRYP, ENABLE);
    
    CRYP_DeInit();
    
    CRYP_InitStructure.CRYP_AlgoDir = CRYP_AlgoDir_Encrypt;
    CRYP_InitStructure.CRYP_AlgoMode = CRYP_AlgoMode_AES_CBC;
    CRYP_InitStructure.CRYP_DataType = CRYP_DataType_16b;
    CRYP_InitStructure.CRYP_KeySize = CRYP_KeySize_128b;
    CRYP_Init(&CRYP_InitStructure);
    
    CRYP_Cmd(ENABLE);
}
