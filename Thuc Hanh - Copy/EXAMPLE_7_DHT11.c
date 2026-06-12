/************************************ Yeu cau ****************************************************************
 1. Do nhiet do hien thi LCD lay 1 so thap phan
 2. Do do am hien thi LCD lay 1 so thap phan
 
********************************* Cac lenh lien quan *********************************************************
 1. De doc nhiet do va do am ta dung ham:
              ----------------------------------------------
                   result = dht11_read();
              ----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . Sau khi goi ham tren cac thong nhiet do va do am duoc luu trong cac bien:
         . dht11.temperature_integral  => phan nguyen cua nhiet do 
         . dht11.temperature_decimal   => phan thap phan cua nhiet do
         . dht11.humidity_integral     => phan nguyen cua do am
         . dht11.humidity_decimal      => phan thap phan cua do am      
2. Chu y khong nen goi ham "dht11_read();" nhieu vi ham nay thuc thi ton nhieu thoi gian  
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_dht11.c> 
#include<tv_lcd.c>
void main()
{
      system_init();  
      lcd_setup();
      lcd_data("    DHT11 EXAMPLE");
      lcd_goto(0,3);
      lcd_data(" TEMP         HUMI");
      while(true)
      {     
            if(dht11_read()==OK)
            {
                  // 1. Do va hien thi nhiet do lay 1 so thap phan
                  lcd_write_2x3_num(dht11.temperature_integral/10,0,1);   
                  lcd_write_2x3_num(dht11.temperature_integral%10,3,1);
                  printf(lcd_data,".%01d",dht11.temperature_decimal);
                  //2. Do va hien thi do am lay 1 so thap phan
                  lcd_write_2x3_num(dht11.humidity_integral/10,12,1);
                  lcd_write_2x3_num(dht11.humidity_integral%10,15,1);
                  printf(lcd_data,".%01d",dht11.humidity_decimal);
            }
      }
}

