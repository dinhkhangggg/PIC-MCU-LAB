#ifndef  __TV_STEPMOTOR_C__
#define  __TV_STEPMOTOR_C__
#include <TV_STEPMOTOR.h>
result stepmotor_control()   
{
       static int8 step=0;
       static signed int8 n=1;
       static unsigned int32 t; 
       switch (step)
       {
         case 0:
            if(stepmotor.direct==stepmotor_direct_forward) n=1;
            else                                           n=7;
            STEPMOTOR_ENABLE();
            step =1;
            break;
         case 1:
            t        = delay_start();
            step     = 2;
            break;
         case 2:
            if(delay_stop(t)>=stepmotor.time_per_step)
            {
                  STEPMOTOR_LOWLEVEL_CONTROL(stepmotor.mode,n);
                  if(stepmotor.direct) n++;
                  else                 n--; 
                  if(n==8)             n =0;
                  else if(n==-1)       n =7;
                  step = 1; 
                  stepmotor.step++;
                  return OK;
            }
            break;
       }
      return BUSY;
}
#endif
