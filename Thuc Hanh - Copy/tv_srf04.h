#ifndef  __TV_SRF04_H__
#define  __TV_SRF04_H__
typedef struct 
{ 
  int8 step;
  unsigned int32 t;
} SRF04_VAR_STRUCT;    
SRF04_VAR_STRUCT SRF04_VAR={0,0};
typedef struct 
{ 
  unsigned int16 distance;
} SRF04_STRUCT;    
SRF04_STRUCT srf04;
result srf04_read(void);
#endif
