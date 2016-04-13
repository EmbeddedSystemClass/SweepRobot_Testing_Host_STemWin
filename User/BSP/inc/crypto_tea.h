#ifndef __CRYPTO_H
#define __CRYPTO_H

#include "stm32f4xx_conf.h"

void SWRB_ByteEncrypt(char * buffer);
void SWRB_ByteDecrypt(char * buffer);
void TEA_Encypt(uint32_t* v,const uint32_t* key);
void TEA_Decypt(uint32_t* v,const uint32_t* key);


#endif
