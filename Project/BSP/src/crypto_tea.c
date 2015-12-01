#include "crypto_tea.h"

#include "malloc.h"

static const uint32_t DELTA=0x9e3779b9;
static const uint32_t TEAKey[4] = {0x95a8882d, 0x9d2cc114, 0x815aa0ce, 0xa1c489f8};

void SWRB_StrEncrypt(char * buffer)
{
    uint32_t datablock[2];

    datablock[0] = (buffer[0] << 24) | (buffer[1] << 16)  | (buffer[2] << 8) | (buffer[3]);
    datablock[1] = (buffer[4] << 24) | (buffer[5] << 16)  | (buffer[6] << 8) | (buffer[7]);

    TEA_Encypt(datablock, TEAKey);
    
    buffer[0] = (char) ((datablock[0] >> 24) & 0xFF);
    buffer[1] = (char) ((datablock[0] >> 16) & 0xFF);
    buffer[2] = (char) ((datablock[0] >> 8) & 0xFF);
    buffer[3] = (char) ((datablock[0]) & 0xFF);
    buffer[4] = (char) ((datablock[1] >> 24) & 0xFF);
    buffer[5] = (char) ((datablock[1] >> 16) & 0xFF);
    buffer[6] = (char) ((datablock[1] >> 8) & 0xFF);
    buffer[7] = (char) ((datablock[1]) & 0xFF);
}

void SWRB_StrDecrypt(char * buffer)
{
    uint32_t datablock[2];

    datablock[0] = (buffer[0] << 24) | (buffer[1] << 16)  | (buffer[2] << 8) | (buffer[3]);
    datablock[1] = (buffer[4] << 24) | (buffer[5] << 16)  | (buffer[6] << 8) | (buffer[7]);

    TEA_Decypt(datablock, TEAKey);

    buffer[0] = (char) ((datablock[0] >> 24) & 0xFF);
    buffer[1] = (char) ((datablock[0] >> 16) & 0xFF);
    buffer[2] = (char) ((datablock[0] >> 8) & 0xFF);
    buffer[3] = (char) ((datablock[0]) & 0xFF);
    buffer[4] = (char) ((datablock[1] >> 24) & 0xFF);
    buffer[5] = (char) ((datablock[1] >> 16) & 0xFF);
    buffer[6] = (char) ((datablock[1] >> 8) & 0xFF);
    buffer[7] = (char) ((datablock[1]) & 0xFF);
}

void TEA_Encypt(uint32_t* v,const uint32_t* key)
{
    unsigned long y=v[0], z=v[1], sum=0;
    u8 n;
    uint32_t k1,k2,k3,k4;
    
    k1 = key[0];
    k2 = key[1];
    k3 = key[2];
    k4 = key[3];
    n=32;
    
    while (n-->0){
        sum += DELTA ;
        y += ((z<<4)+k1) ^ (z+sum) ^ ((z>>5)+k2) ;
        z += ((y<<4)+k3) ^ (y+sum) ^ ((y>>5)+k4) ;
    }
    
    v[0]=y ; v[1]=z ;
}

void TEA_Decypt(uint32_t* v,const uint32_t* key)
{
    unsigned long sum, y=v[0], z=v[1];
    u8 n;
    uint32_t k1,k2,k3,k4;
    
    k1 = key[0];
    k2 = key[1];
    k3 = key[2];
    k4 = key[3];
    n=32;
    
    sum=DELTA<<5; 
    while (n-->0) {
        z-= ((y<<4)+k3) ^ (y+sum) ^ ((y>>5)+k4) ;
        y-= ((z<<4)+k1) ^ (z+sum) ^ ((z>>5)+k2) ;
        sum-=DELTA ;
    }

    v[0]=y ; v[1]=z ;
}
