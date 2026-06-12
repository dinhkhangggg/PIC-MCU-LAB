/************************************ Yeu cau ****************************************************************
 1. Do khoang cach dung cam bien sieu am hien thi LCD

********************************* Cac lenh lien quan *********************************************************
 1. De do khoang cach ta dung ham:
              ----------------------------------------------
                   result = srf04_read();
              ----------------------------------------------
     . Neu result = Ok thi da do khoang cach xong
     . Neu result = ERR thi chua do khoang cach xong 
     . Sau khi goi ham tren gia tri khoang cach duoc luu trong bien "srf04.distance" theo don vi cm
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_srf04.c> 
#include<tv_lcd.c> 
void main()
{
      system_init();  
      lcd_setup();
      lcd_data("   SRF04 EXAMPLE");
      lcd_goto(14,2);
      lcd_data("(Cm)");
      while(true)
      {     
            if(srf04_read()==OK)
            {   
                  // 1. Do va hien khoang cach
                  lcd_write_2x3_num(srf04.distance/100%10,5,1);   
                  lcd_write_2x3_num(srf04.distance/10%10,8,1);
                  lcd_write_2x3_num(srf04.distance/1%10,11,1);
             }       
      }
}

