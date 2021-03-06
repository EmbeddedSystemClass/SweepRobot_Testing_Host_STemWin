#include "GUI.h"
#include "stm32f4xx.h"

static GUI_CONST_STORAGE GUI_COLOR _aColorsCHNText[] = {
  0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_COLOR _aColorsWhiteCHNText[] = {
  0xFFFFFF, 0xFFFFFF
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalCHNText = {
  2,    /* number of entries */
  1,     /* No transparency */
  &_aColorsCHNText[0]
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalWhiteCHNText = {
  2,    /* number of entries */
  1,     /* No transparency */
  &_aColorsWhiteCHNText[0]
};

static GUI_CONST_STORAGE unsigned char acSerialNumStr[] = {
  ________,________,________,      ________,________,________,      ________,________,________,
  ________,___X____,________,      ________,________,________,      ________,________,________,
  ________,____XX__,________,      ________,________,____XX__,      ______X_,________,_X______,
  ________,_____X__,____X___,      ________,____XX__,____X___,      ______XX,XXXXXXXX,XX______,
  ____X___,_____X__,___XXX__,      ___XXXXX,XXXXXXX_,____X___,      ______X_,________,_X______,
  ____XXXX,XXXXX_XX,XXX_____,      _______X,________,____X___,      ______X_,________,_X______,
  ____X___,________,________,      _______X,_______X,X___X___,      ______X_,________,_X______,
  ____X__X,XXXXXXXX,XXXX____,      ______XX,_______X,X___X___,      ______X_,________,_X______,
  ____X___,________,_XX_____,      ______X_,___XX__X,X___X___,      ______XX,XXXXXXXX,XX______,
  ____X___,___X___X,X_______,      _____XXX,XXXXX__X,X___X___,      ______X_,________,________,
  ____X___,____XXX_,________,      _____X__,___X___X,X___X___,      ________,________,____XX__,
  ____X___,_____XX_,________,      ____XXX_,__XX___X,X___X___,      __XXXXXX,XXXXXXXX,XXXX____,
  ____X___,______X_,____XX__,      ____X_XX,__XX___X,X___X___,      ________,XX______,________,
  ____X_XX,XXXXXXXX,XXXXXXX_,      ___X___X,__X____X,X___X___,      ________,X_______,________,
  ____X___,_____XX_,___X____,      __X____X,_XX____X,X___X___,      _______X,X_______,_XX_____,
  ___XX___,_____XX_,________,      _______X,_X_____X,X___X___,      _______X,_XXXXXXX,XXX_____,
  ___X____,_____XX_,________,      ________,XX_____X,X___X___,      ________,________,_X______,
  ___X____,_____XX_,________,      ________,X______X,X___X___,      ________,________,_X______,
  ___X____,_____XX_,________,      _______X,_______X,____X___,      ________,________,_X______,
  __X_____,_____XX_,________,      ______X_,________,____X___,      ________,________,XX______,
  __X_____,_____XX_,________,      _____X__,________,____X___,      ________,________,XX______,
  _X______,__XXXX__,________,      ___XX___,________,XXXXX___,      ________,___XXXXX,X_______,
  _X______,____XX__,________,      __X_____,________,___X____,      ________,_____XXX,________,
  ________,________,________,      ________,________,________,      ________,________,________
};

GUI_CONST_STORAGE GUI_BITMAP _bmSerialNumCHN = {
  72,                   /* XSize */
  24,                   /* YSize */
  9,                    /* BytesPerLine */
  1,                    /* BitsPerPixel */
  acSerialNumStr,       /* Pointer to picture data (indices) */
  &_PalCHNText          /* Pointer to palette */
};

static GUI_CONST_STORAGE unsigned char acTimeStr[] = {
  ________,________,________,      ________,________,________,
  ________,________,________,      ________,________,________,
  ________,________,_XX_____,      _____X__,________,________,
  ________,________,_X______,      ______X_,________,____X___,
  __X____X,X_______,_X______,      ______XX,__XXXXXX,XXXXX___,
  __XXXXXX,X_______,_X______,      ___XX_X_,________,____X___,
  __X____X,X_______,_X______,      ___X____,________,____X___,
  __X____X,XXXXXXXX,XXXXXX__,      ___X____,X_______,____X___,
  __X____X,X_______,_X______,      ___X____,XXXXXXXX,____X___,
  __X____X,X_______,_X______,      ___X____,X_____X_,____X___,
  __X____X,X_______,_X______,      ___X____,X_____X_,____X___,
  __XXXXXX,X__X____,_X______,      ___X____,X_____X_,____X___,
  __X____X,X___X___,_X______,      ___X____,XXXXXXX_,____X___,
  __X____X,X___XX__,_X______,      ___X____,X_____X_,____X___,
  __X____X,X___XX__,_X______,      ___X____,X_____X_,____X___,
  __X____X,X_______,_X______,      ___X____,X_____XX,____X___,
  __X____X,X_______,_X______,      ___X____,X_____XX,____X___,
  __X____X,X_______,_X______,      ___X____,XXXXXXXX,____X___,
  __XXXXXX,X_______,_X______,      ___X____,X_____X_,____X___,
  __X____X,________,_X______,      ___X____,________,____X___,
  __X_____,________,_X______,      ___X____,________,____X___,
  ________,______XX,XX______,      ___X____,________,_XXXX___,
  ________,________,X_______,      ___X____,________,__XX____,
  ________,________,________,      ________,________,________
};

GUI_CONST_STORAGE GUI_BITMAP _bmTimeCHN = {
  48,              /* XSize */
  24,               /* YSize */
  6,                /* BytesPerLine */
  1,                /* BitsPerPixel */
  acTimeStr,     /* Pointer to picture data (indices) */
  &_PalCHNText      /* Pointer to palette */
};

GUI_CONST_STORAGE GUI_BITMAP _bmWhiteTimeCHN = {
  48,              /* XSize */
  24,               /* YSize */
  6,                /* BytesPerLine */
  1,                /* BitsPerPixel */
  acTimeStr,     /* Pointer to picture data (indices) */
  &_PalWhiteCHNText      /* Pointer to palette */
};

static GUI_CONST_STORAGE unsigned char acTestSelStr[] = {
  ________,________,________,      ________,________,________,      ________,________,________,
  ________,________,________,      ________,________,________,      ________,________,________,
  ___X___X,________,____XX__,      ________,_______X,X_X_____,      ________,________,____XX__,
  ____X__X,XXXXXXX_,____X___,      ___X____,_______X,XX_XX___,      ________,__XXXXXX,XXXX____,
  ____XX_X,X____XX_,____X___,      ____X___,________,X___X___,      ________,X______X,________,
  _____X_X,X____XX_,_X__X___,      ____XX__,________,X___X___,      __XXXXXX,XX____X_,________,
  _______X,X_X__XX_,_X__X___,      _____X__,________,X____X__,      _____X__,______X_,____X___,
  _X_____X,X_XX_XX_,_X__X___,      ________,XXXXXXXX,XXXXXXX_,      _____X__,___XXXXX,XXXXX___,
  __XX__XX,X_XX_XX_,_X__X___,      ________,________,X_______,      _____X__,___X____,____X___,
  ___X__XX,X_XX_XX_,_X__X___,      ________,________,X_______,      _____X__,___X___X,____X___,
  ___X_X_X,X_XX_XX_,_X__X___,      _____X__,________,X_______,      _____X__,___X___X,X___X___,
  _____X_X,X_XX_XX_,_X__X___,      __XXXX__,______X_,X_______,      _____X__,___X___X,X___X___,
  _____X_X,X_XX_XX_,_X__X___,      _____X__,_XXXXX_X,XX______,      _____X__,___X___X,X___X___,
  _____X_X,X_XX_XX_,_X__X___,      _____X__,____X___,XX______,      _____X__,___X___X,X___X___,
  ____X__X,X_X__XX_,_X__X___,      _____X__,____X___,_X______,      _____X__,___X___X,____X___,
  ____X__X,X_X__XX_,_X__X___,      _____X__,____X___,_X______,      _____X_X,X__X___X,____X___,
  __XXX__X,X_X__X__,_X__X___,      _____X__,_X__X___,_XX_____,      ____XXX_,___X___X,____X___,
  ___XX___,__X_____,_X__X___,      _____X__,X___X___,__X___X_,      _XXXX___,___X___X,X_______,
  ___XX___,_X_XX___,____X___,      _____X_X,____XXXX,__XX_X__,      __X_____,______X_,_XX_____,
  ___XX___,_X___X__,____X___,      _____XXX,__XXX___,___X_X__,      ________,_____XX_,___XX___,
  ___XX___,X____XX_,____X___,      _____XX_,XXX_____,___XXX__,      ________,____XX__,____XX__,
  ___XX__X,______X_,__XXX___,      _____X__,________,____XXX_,      ________,___XX___,____XX__,
  _____XX_,________,___XX___,      ________,________,_____XX_,      ________,_XX_____,_____X__,
  ________,________,________,      ________,________,________,      ________,________,________,
};

GUI_CONST_STORAGE GUI_BITMAP _bmTestSelCHN = {
  72,               /* XSize */
  24,               /* YSize */
  9,                /* BytesPerLine */
  1,                /* BitsPerPixel */
  acTestSelStr,     /* Pointer to picture data (indices) */
  &_PalCHNText      /* Pointer to palette */
};
