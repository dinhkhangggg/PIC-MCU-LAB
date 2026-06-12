#ifndef  __TV_DCMOTOR_H__
#define  __TV_DCMOTOR_H__
#define  SPEEDMUL  1333
typedef enum
{
      dcmotor_direct_forward  = 1,
      dcmotor_direct_reverse  = 0
}DCMOTOR_DIRECT;
typedef struct
{
      signed int16             duty;
      DCMOTOR_DIRECT           direct;
      unsigned int16           speed;
}
DCMOTOR_STRUCT ;
DCMOTOR_STRUCT dcmotor={0,1,0};
DCMOTOR_STRUCT nowdcmotor={0,1,0};
result dcmotor_control(void);
result dcmotor_read_speed(void);
#warning "Khi goi ham "dcmotor_read_speed" TIMER1 duoc cau hinh chuc nang COUTNER => Chu y cac ung dung truoc do da su dung TIMER1"
#endif
