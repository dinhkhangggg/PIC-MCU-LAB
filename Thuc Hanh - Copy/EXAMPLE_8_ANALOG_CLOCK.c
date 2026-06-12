#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_glcd.c>  
#include<tv_ds1307.c>   
float gio,phut,giay;
void main()
{
      system_init();  
      glcd_setup();  
      glcd_circle(63,31,31,0,1);
      while(true)
      {     
            if(ds1307_read_time()==OK)
            {
               giay= (ds1307.second/16)*10 + ds1307.second%16;
               phut= (ds1307.minute/16)*10 + ds1307.minute%16;
               gio=  (ds1307.hour/16)*10   + ds1307.hour%16;
               
               glcd_circle(63+29*cos(pi*(giay-15)/30),31+29*sin(pi*(giay-15)/30),2,1,1);
               if(giay==0)
               {    glcd_rect(31,0,96,63,1,0);
                    glcd_circle(63,31,31,0,1);
               }
               else glcd_circle(63,31,27,1,0);
               glcd_text_setup(57,5,1,1,0);glcd_text("12");
               glcd_text_setup(61,51,1,1,0); glcd_text("6");
               glcd_text_setup(37,28,1,1,0);glcd_text("9");
               glcd_text_setup(85,28,1,1,0); glcd_text("3");
               glcd_circle(63+23*cos(pi*(5-15)/30),31+23*sin(pi*(5-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(10-15)/30),31+23*sin(pi*(10-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(20-15)/30),31+23*sin(pi*(20-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(25-15)/30),31+23*sin(pi*(25-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(35-15)/30),31+23*sin(pi*(35-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(40-15)/30),31+23*sin(pi*(40-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(50-15)/30),31+23*sin(pi*(50-15)/30),1,1,1);
               glcd_circle(63+23*cos(pi*(55-15)/30),31+23*sin(pi*(55-15)/30),1,1,1);
                
               glcd_triangle(63,31,63+17*cos(pi*((gio+phut/60)-3)/6), 31+17*sin(pi*((gio+phut/60)-3)/6),63+13*cos(pi*((gio+phut/60)-3.3)/6), 31+13*sin(pi*((gio+phut/60)-3.3)/6),1,1);          
               glcd_triangle(63,31,63+17*cos(pi*((gio+phut/60)-3)/6), 31+17*sin(pi*((gio+phut/60)-3)/6),63+13*cos(pi*((gio+phut/60)-2.7)/6), 31+13*sin(pi*((gio+phut/60)-2.7)/6),1,1);   
               glcd_bar(63, 31,  63+19*cos(pi*((phut+giay/60)-15)/30), 31+19*sin(pi*((phut+giay/60)-15)/30),2 ,1);
               glcd_line(63, 31, 63+25*cos(pi*(giay-15)/30), 31+25*sin(pi*(giay-15)/30),1);
               glcd_circle(63,31,3,1,1);
               glcd_circle(63,31,1,1,0);
               glcd_update();
            }
            else 
            {    
                glcd_rect(31,0,96,63,1,0);
                glcd_text_setup(34,28,2,1,1);glcd_text("ERROR");
                glcd_update();
            }
      }
}
