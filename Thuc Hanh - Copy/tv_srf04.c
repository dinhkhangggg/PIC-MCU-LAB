#ifndef  __TV_SRF04_C__
#define  __TV_SRF04_C__
#include<TV_SRF04.h>
result srf04_read()
{
    unsigned int8 tam;
    switch (SRF04_VAR.step)
      {
          case 0:
                 enable_interrupts(int_CCP2);
                 enable_interrupts(global);
                 SRF04_VAR.step=1;
                 break;
          case 1:
                 output_high(SRF04_TRIGER);
                 SRF04_VAR.t = delay_start();
                 SRF04_VAR.step=2;
                 break;
          case 2:
                 if(delay_stop(SRF04_VAR.t)>=1)
                 {
                    output_low(SRF04_TRIGER); 
                    setup_ccp2(CCP_CAPTURE_RE); 
                    SRF04_VAR.t = delay_start();
                    SRF04_VAR.step=3;
                 }
                 break;
          case 3:
                 if(delay_stop(SRF04_VAR.t)>=100)
                 {
                    SRF04_VAR.step=1;
                    return ERROR;
                 }
                 break;       
          case 5:  
                 srf04.distance=SRF04_VAR.t/36-1;
                 SRF04_VAR.t = delay_start();
                 SRF04_VAR.step=6;
                 return OK;   
                 break;
          case 6:  
                 if(delay_stop(SRF04_VAR.t)>=100)
                 SRF04_VAR.step=1;
                 break;  
      }
      return BUSY;    
}

#int_ccp2
void interrupt_ccp2()
{
      if(SRF04_VAR.step==3)
      {
           setup_ccp2(CCP_CAPTURE_FE);
           delay.timeline += get_timer3() ;set_timer3(0);
           if(TIM3IF) delay.timeline +=65536;TIM3IF=0;
           SRF04_VAR.step=4;
      }
      else if(SRF04_VAR.step==4)
      {
           SRF04_VAR.t = get_timer3();
           SRF04_VAR.step=5;
      }    
}
#if      BOARD != MOPHONG
#error   "Cam bien sieu am tren board thuc hanh hu nhieu nen chi mo phong ma khong thuc hanh module nay!"
#endif
#endif
