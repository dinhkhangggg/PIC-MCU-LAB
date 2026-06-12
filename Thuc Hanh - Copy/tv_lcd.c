#ifndef  __TV_LCD_C__
#define  __TV_LCD_C__
#include<TV_LCD.h>
result  lcd_command(int8 command)
{    
      write_lcd20x4(0x01,command);     
      write_lcd20x4(0x00,command); 
      delay_us(200);
      return OK;
}
result lcd_data(int8 data)
{
      write_lcd20x4(0x05,data);     
      write_lcd20x4(0x04,data); 
      delay_us(200);
      return OK;
}
result lcd_setup()
{
      int8 n;
      lcd_command(0x3c);  delay_ms(5);
      lcd_command(0x3c);  delay_ms(5);
      lcd_command(0x0c);    
      lcd_command(0x06); 
      lcd_command(0x40);                             // di chuyen den dia chi dau vung nho CGRAM
      for(n=0;n<64;n++) lcd_data(LCD_MA_8DOAN[n]);   // ghi ma cua ky tu moi tao vao CGRAM
      lcd_command(lcd_clear_display); delay_ms(2);
      return OK;
}
result lcd_goto( int8 colum_0_19, int8 row_0_3) 
{
      const unsigned int8 dc[]={0x80,0xc0,0x94,0xd4};
       lcd_command(dc[row_0_3]+colum_0_19);
       return OK;
}
result lcd_write_2x3_num(signed int8 num_0_9, int8 colum_0_17, int8 row_0_2)
{     
      lcd_goto(colum_0_17,row_0_2);
      for (int8 i=0;i<6;i++)     
      {                  
        if (i==3)   lcd_goto(colum_0_17,row_0_2+1);             
        lcd_data(lcd_so_x[num_0_9][i]);     
      } 
      return OK;
}

result lcd_write_4x3_num(signed int8 num_0_9, int8 colum_0_17)
{
      for(int8 n=0;n<12;n++)
      {
            if(n%3==0)LCD_GOTO(colum_0_17,n/3); 
            LCD_DATA(LCD_SO_X1[num_0_9][n]);
      }
      return OK;
}
#endif
