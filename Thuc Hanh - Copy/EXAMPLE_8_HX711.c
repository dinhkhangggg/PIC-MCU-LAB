/************************************ Yeu cau ****************************************************************
 1. Do khoi luong theo don vi Kg va hien thi LCD

********************************* Cac lenh lien quan *********************************************************
 1. De do khoi luong ta dung ham:
              ----------------------------------------------
                      result = hx711_read();
              ----------------------------------------------
     . Neu result = Ok thi da do duoc can nang
     . Neu result = ERR thi ta mo thu vien chinh lai cac he so tinh toan can nang (dong code 25) 
     . Sau khi goi ham tren gia tri can nang duoc luu trong bien "hx711.weight" theo don vi Kg
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D401
#include<tv_boards.c> 
#include<tv_hx711.c> 
#include<tv_lcd.c>
void main()
{
      system_init();  
      lcd_setup();
      lcd_data("  LOADCELL EXAMPLE");
      lcd_goto(15,2);
      lcd_data("(Kg)");
      while(true)
      {     
            if(hx711_read()==OK)
            {   
                  // 1. Do va hien thi khoi luong 
                  lcd_write_2x3_num(hx711.weight/100%10,6,1);   
                  lcd_write_2x3_num(hx711.weight/10%10,9,1);
                  lcd_write_2x3_num(hx711.weight/1%10,12,1);
            }
      }
}

