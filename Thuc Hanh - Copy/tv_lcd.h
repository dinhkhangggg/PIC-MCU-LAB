#ifndef __tv_lcd_h__
#define __tv_lcd_h__
typedef enum 
{
      lcd_clear_display  = 0x01,
      lcd_shift_left     = 0x18,
      lcd_shift_right    = 0x1c,
}LCD_COMMANDS;
result lcd_command(int8 command);
result lcd_data(int8 data);
result lcd_setup(void);
result lcd_goto( int8 colum_0_19, int8 row_0_3) ; 
result lcd_write_2x3_num(signed int8 num_0_9, int8 colum_0_17, int8 row_0_2);
result lcd_write_4x3_num(signed int8 num_0_9, int8 colum_0_17);
//xxxxxxxxxxxxxxxxxxxxxxxx Font xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
const unsigned int8 LCD_SO_X[11][6] ={       // Font 3x2
                  0,1,2,5,3,4,               // SO 0
                  1,2,32,3,7,3,              // SO 1
                  6,6,2,5,3,3,               // SO 2
                  6,6,2,3,3,4,               // SO 3
                  5,3,7,32,32,7,             // SO 4
                  7,6,6,3,3,4,               // SO 5
                  0,6,6,5,3,4,               // SO 6
                  1,1,7,32,32,7,             // SO 7
                  0,6,2,5,3,4,               // SO 8
                  0,6,2,3,3,4,               // SO 9
                  32,32,32,32,32,32          // XOA
                  };             
const unsigned int8 LCD_SO_X1[11][12] ={           //Font 3x4
                  0,1,2,7,32,7,7,32,7,5,3,4,       //0   
                  1,2,32,32,7,32,32,7,32,3,7,3,    //1  
                  1,1,2,3,3,7,7,32,32,5,3,3,       //2           
                  0,1,2,32,3,4,32,32,2,5,3,4,      //3
                  7,32,7,5,3,7,32,32,7,32,32,7,    //4             
                  7,1,1,7,3,3,32,32,7,5,3,4,       //5           
                  0,1,2,7,3,3,7,32,7,5,3,4,        //6             
                  1,1,7,32,32,4,32,0,32,32,7,32,   //7             
                  0,1,2,5,3,4,0,32,2,5,3,4,        //8     
                  0,1,2,5,3,7,32,32,7,5,3,4,       //9 
                  32,32,32,32,32,32,32,32,32,32,32,32}; //XOA                  
                  
const unsigned int8 LCD_MA_8DOAN[] = {
                  0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,  
                  0x1F,0x1F,0x1F,0X00,0X00,0X00,0X00,0X00,  
                  0x1C,0x1E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F, 
                  0X00,0X00,0X00,0X00,0X00,0x1F,0x1F,0x1F,  
                  0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C,  
                  0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07, 
                  0x1F,0x1F,0x1F,0X00,0X00,0X00,0x1F,0x1F,  
                  0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}; 
   
const unsigned char LCD_MA_TRAITAO[]={
               0,0,0,0x0c,0x1f,0x1f,0x1f,0x1f,
               0x0e,0x1c,0x10,0x06,0x1f,0x1f,0x1f,0x1f
               ,0x0f,0x07,0x03,0,0,0,0,0,
               0x1e,0x1c,0x18,0,0,0,0,0};
#endif
