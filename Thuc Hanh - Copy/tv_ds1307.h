#ifndef  __TV_DS1307_H__
#define  __TV_DS1307_H__
typedef struct
{
   unsigned int8 second; 
   unsigned int8 minute;    
   unsigned int8 hour;    
   unsigned int8 day_of_week;    
   unsigned int8 date;    
   unsigned int8 month;    
   unsigned int8 year;       
}DS1307_STRUCT;
DS1307_STRUCT ds1307={0,0,0,0,0,0,0};
result ds1307_read_time(void);
result ds1307_set_time(DS1307_STRUCT t);
result ds1307_battery_check();
#endif
