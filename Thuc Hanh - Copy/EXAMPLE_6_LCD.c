/************************************ Yeu cau ****************************************************************
1. Hien thi chuoi "VXL SPKT" tai cot 9, hang 0
2. Hien thi dau Ohm tai cot 18, hang 2
3. Hien thi gia tri bien dem theo font 1x1 tu 0 -99 tai cot 10, hang 3           
4. Hien thi gia tri bien dem theo font 2x3 tai cot 10, hang1
5. Hien thi gia tri bien dem theo font 4x3 tai cot 4

**************************** Cac kien thuc lien quan **********************************************************
1. De di chuyen con tro ta dung ham:
              ----------------------------------------------
                             lcd_goto(cot,hang); 
              ----------------------------------------------
     . cot la cot can hien thi tu 0 den 19 
     . hang la hang can hien thi tu 0 den 3
2. De hien thi chuoi ky tu ta dung ham:
              ----------------------------------------------
                         lcd_data("Chuoi can hien thi"); 
              ----------------------------------------------            
3. De hien thi ky tu khong danh may duoc ta tra ma ky tu do trong bang ma ASCII trong sach. 
     .Vi du ta tra duoc ma ky hieu Ohm la 0xf4. Ta dung ham "lcd_data(0xf4);" de hien thi ky tu Ohm
4. De hien thi gia tri cua bien ta dung ham:
              ----------------------------------------------
                         printf(lcd_data,"%??", bien);
              ---------------------------------------------- 
     .Voi %?? xac dinh nhu sau:
        %d : bien thuoc kieu signed int8
        %u : bien thuoc kieu unsigned int8
        %ld: bien thuoc kieu signed int16 hoac signed int32
        %lu: bien thuoc kieu unsigned int16 hoac unsigned int32
     .Co the hien thi 1 lan nhieu bien nhu vi du sau:
            printf(lcd_data,"%02u:%02u:%02u", gio,phut,giay);      
            (noi dung hien thi se co dang:  09:36:53)
5. De hien thi duoc gia tri bien theo font 2x3 ta dung ham:
              ----------------------------------------------
                      lcd_write_2x3_num(so,cot,hang);
              ---------------------------------------------- 
     . so la gia tri so can hien thi tu 0 den 9
     . cot la cot can hien thi tu 0 den 19 
     . hang la hang can hien thi tu 0 den 3
6. De hien thi duoc gia tri bien theo font 4x3 ta dung ham:
              ----------------------------------------------
                      lcd_write_4x3_num(so,cot);
              ---------------------------------------------- 
     . so la gia tri so can hien thi tu 0 den 9
     . cot la cot can hien thi tu 0 den 19 
7. Chu y sau khi thuc hien lenh xoa man hinh "lcd_command(lcd_clear_display); hoac lcd_command(0x01);"
   ta phai delay_ms(2); de tranh LCD bo qua tac vu tiep theo     
   
*********************************** Chuong trinh **************************************/
#define   BOARD     D501 
#include<tv_boards.c> 
#include<tv_lcd.c>
unsigned int8 dem=0;
void main() 
{
      system_init();  
      lcd_setup();               // cau hinh LCD - thieu lenh nay lcd khong chay
      lcd_goto(9,0);             // 1.di chuyen con tro den hang 0 cot 9
      lcd_data("VXL SPKT");      // 1.hien thi chuoi VXL SPKT tai hang 0 cot 9
      lcd_goto(18,2);            // 2.di chuyen con tro den hang 2 cot 18
      lcd_data(0xf4);            // 2.hien thi ky tu Ohm tra trong bang ma ASCII o hang 2 cot 18
      while(true)
      {    
           lcd_goto(10,3);                   //3. di chuyen con tro den hang 3 cot 10
           printf(lcd_data,"%u  ",dem);       //3. hien thi bien dem font 1x1
           lcd_write_2x3_num(dem/10%10,10,1);//4. hien thi hang chuc bien dem font 2x3
           lcd_write_2x3_num(dem%10,13,1);   //4. hien thi hang don vi bien dem font 2x3
           lcd_write_4x3_num(dem/10%10,0);   //5. hien thi hang chuc bien dem font 4x3
           lcd_write_4x3_num(dem%10,4);      //5. hien thi hang don vi bien dem font 4x3

           dem++; dem%=100; delay_ms(200);
      }
}


