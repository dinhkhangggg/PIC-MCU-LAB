#ifndef  __TV_STEPMOTOR_H__
#define  __TV_STEPMOTOR_H__
typedef enum 
{
      stepmotor_mode_fullstep  = 1,
      stepmotor_mode_halfstep  = 0
}STEPMOTOR_MODE;
typedef enum 
{
      stepmotor_direct_forward  = 1,
      stepmotor_direct_reverse  = 0
}STEPMOTOR_DIRECT;
typedef struct
{
      STEPMOTOR_DIRECT   direct;
      STEPMOTOR_MODE     mode;
      unsigned int8      time_per_step;
      unsigned int32     step;  
}STEPMOTOR_STRUCT;
STEPMOTOR_STRUCT  stepmotor={1,1,10,0};
result stepmotor_control(void);
#endif

