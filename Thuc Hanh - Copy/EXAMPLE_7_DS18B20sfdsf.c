//#define  board  D401
#include<TV_BOARDS.c>
#include<TV_LCD.c>
#include<TV_DS1307.c>
void main()
{
      system_init();
      lcd_setup();
      ds1307.hour=0x19;       // chinh thoi gian
      ds1307.minute=0x56;
      ds1307.second=0x00;
      
      ds1307_set_time(ds1307);
      while(true)
      {
            if(ds1307_read_time()==OK)
            {
                lcd_goto(0,0);
                printf(lcd_data,"%u%u:%u%u:%u%u",
                ds1307.hour/16,ds1307.hour%16,
                ds1307.minute/16,ds1307.minute%16,
                ds1307.second/16,ds1307.second%16);
            }
      }
}
