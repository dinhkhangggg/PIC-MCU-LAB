#ifndef    __TV_BOARDS_C__
#define    __TV_BOARDS_C__
#define    D401          1
#define    D501          2
#define    D503          3
#define    MOPHONG       4
#ifndef    BOARD
#define    BOARD         MOPHONG
#endif
#if  BOARD ==  D401
#include <TV_D401_TH.C>

#elif  BOARD ==  D501
#include <TV_D501_TH.C>
 
#elif  BOARD ==  D503
#include <TV_D501_TH.C>

#else 
#include <TV_MOPHONG.C>
#warning"Day la file mo phong, SV khong duoc nap tren board that"
#endif
#if  BOARD == MOPHONG
#define   BUTTON_HOLD_TIME  40
#else 
#define   BUTTON_HOLD_TIME  30
#endif

#include <TV_LOWLEVER.C>
#ignore_warnings 202
#endif
