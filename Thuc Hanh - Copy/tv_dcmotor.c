#ifndef  __TV_DCMOTOR_C__
#define  __TV_DCMOTOR_C__
#include <TV_DCMOTOR.h>
result DCMOTOR_MIDDLELEVEL_CONTROL(signed int16 duty,int1 direct)
{
     static unsigned int32 t=0;
     result tt=BUSY;
     if(nowdcmotor.duty<duty)
      {
       if(delay_stop(t)>=20)
           {
               nowdcmotor.duty+=50;
               if(nowdcmotor.duty>=duty) {nowdcmotor.duty=duty;tt= OK;}
               DCMOTOR_LOWLEVEL_CONTROL(nowdcmotor.duty,direct);
               t = delay_start();
           }
      }
     else if(delay_stop(t)>=20)
      {
               nowdcmotor.duty-=50;
               if(nowdcmotor.duty<=duty){nowdcmotor.duty=duty;tt= OK;}
               DCMOTOR_LOWLEVEL_CONTROL(nowdcmotor.duty,direct);
               t = delay_start();
      }
     return tt;
}
result dcmotor_control()
{
        static int8 step=0;
        switch (step)
        {
              case 0:
                    DCMOTOR_ENABLE();
                    DCMOTOR_MIDDLELEVEL_CONTROL(dcmotor.duty,dcmotor.direct) ;
                    step =1;
                    break;
              case 1: 
                    if(nowdcmotor.duty!= dcmotor.duty)
                       DCMOTOR_MIDDLELEVEL_CONTROL(dcmotor.duty,dcmotor.direct) ;
                    else if(nowdcmotor.direct!= dcmotor.direct) step=2;
                    break;
              case 2:                                    // Giam toc tu tu
                    if(DCMOTOR_MIDDLELEVEL_CONTROL(0,nowdcmotor.direct)==OK) step=3;
                    break;
              case 3:                                     // Tang toc tu tu
                    if(DCMOTOR_MIDDLELEVEL_CONTROL(dcmotor.duty,!nowdcmotor.direct)==OK) 
                    {
                        nowdcmotor.direct = !nowdcmotor.direct;
                        step=1;
                    }
                    break;       
        }
        return OK;
}
result dcmotor_read_speed()
{
      static int1 tt_init=0;
      static int8 step=0;
      static unsigned int32 t;
      if(tt_init==0)
        {
            setup_timer_1(t1_external_sync|t1_div_by_1);  // TIMER 1 dem xung tu encoder
            tt_init=1;
        }
      switch (step)
      {
            case 0:
                  set_timer1(0);
                  t = delay_start(); 
                  step=1;
                  break;
            case 1:
                  if(delay_stop(t)>=100)
                   {
                      dcmotor.speed = (int32)SPEEDMUL*get_timer1()/delay_stop(t);
                      step=0;
                      return OK;
                   }
                  break;
      }
      return BUSY;
}
#endif
