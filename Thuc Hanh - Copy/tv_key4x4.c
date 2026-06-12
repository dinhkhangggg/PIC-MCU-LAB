#ifndef  __TV_KEY4x4_C__
#define  __TV_KEY4x4_C__
#include<tv_key4x4.h>


result key4x4_read()
{      
         static unsigned int8  step=0;
         static unsigned int16 t;
         key4x4.key = KEYREAD();
         switch (step)
         {
             case 0:
                   if( key4x4.key!=0xff)
                      {     t   = delay_start();
                            step=1;
                      } 
                    break;
              case 1:
                    if(key4x4.key==0xff)step =0;
                    else if(delay_stop(t)>=BUTTON_HOLD_TIME)
                    {   step = 2;
                        t    = delay_start(); 
                        return OK;
                    }
                    break; 
              case 2:
                    if(key4x4.key==0xff)step =0;
                    else if(delay_stop(t)>=500)
                       {
                           t    = delay_start(); 
                           step = 3;
                       } 
                    break;
              case 3:
                    if(key4x4.key==0xff)step =0;
                    else if(delay_stop(t)>=100)
                      {
                         t    = delay_start(); 
                         return OK;
                      }
                     break; 
         }
         return BUSY;
}
#endif
