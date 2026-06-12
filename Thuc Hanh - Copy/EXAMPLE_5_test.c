//#define   BOARD     D501
#include<tv_boards.c> 
signed int8 n,m;
void main()
{
      system_init();  
      while(true)
      {     
            for(n=0;n<10;n++)
            {
                  s7seg.led[0]= m7d[n];
                  for(m=0; m<100;m++)s7seg_display();
                 
            }
      }
}


  
