/************************************ Yeu cau ****************************************************************
 1. Cai dat do phan giai 11 bit cho ds18b20
 2. Hien thi so luong cam bien va nhiet do do duoc cua tung cam bien tren LCD, lay 1 so tp
 3. Neu khong co cam bien nao ket noi voi he thong thi hien thi thong bao loi 
 
********************************* Cac lenh lien quan *********************************************************
 1. De do nhiet do ta dung ham:
              ----------------------------------------------
                   result = ds18b20_read_temperature();
              ----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . So luong cam bien duoc luu trong bien "ds18b20.mumber_of_ds18b20"
     . Neu he thong co 1 cam bien thi ket qua nhiet do duoc luu trong bien "ds18b20.temperatureA"
     . Neu he thong co 2 cam bien thi ket qua nhiet do duoc luu trong bien "ds18b20.temperatureA" va "ds18b20.temperatureB"
 2. De cai dat do phan giai ta dung ham:
              -----------------------------------------------
              result = ds18b20_set_resolution(int8 res_9_12);
              -----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . res_9_12 la gia tri tu 9-12 tuong ung voi cac che do tu 9 den 12 bit
     . 9 = 0.5, 10 = 0.25, 11=0.125, 12= 0.0625
     
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_ds18b20.c>   
#include<tv_lcd.c>
unsigned int16 ng,tp;
void main()
{
      system_init();  
      lcd_setup();
      ds18b20_set_resolution(11);                     // 1. Cai dat do phan giai 11 bit
      while(true)
      {     
            if(ds18b20_read_temperature()==OK)        // 2. Do nhiet do thanh cong
            {
                  lcd_goto(0,0);                      // 2. Hien thi so luong cam bien
                  printf(lcd_data,"Number of Sensor: %d",ds18b20.mumber_of_ds18b20);
                  
                  if(ds18b20.mumber_of_ds18b20==1)    // 2. Neu co 1 cam bien thi hien thi nhiet do 1 cam bien 
                  {
                        ng = ds18b20.temperatureA>>4; // 2. Tinh toan phan nguyen
                        tp = (ds18b20.temperatureA&0x0f)*10/16; // 2. Tinh toan phan thap phan, neu lay 2 so tp thi x100, 3 so thi x1000
                        lcd_write_2x3_num(ng/10%10,6,1); // 2. Hien thi LCD 
                        lcd_write_2x3_num(ng/1%10,9,1); 
                        printf(lcd_data,".%ld",tp);
                  }
                  
                  else if(ds18b20.mumber_of_ds18b20==2) //2. Neu phat hien 2 cam bin thi hien thi nhiet do ca 2 cam bien 
                  {
                        ng = ds18b20.temperatureA>>4;   //2. Tinh toan va hien thi nhiet do cam bien A
                        tp = (ds18b20.temperatureA&0x0f)*10/16;
                        lcd_write_2x3_num(ng/10%10,0,1); 
                        lcd_write_2x3_num(ng/1%10,3,1); 
                        printf(lcd_data,".%ld",tp);
                        
                        ng = ds18b20.temperatureB>>4;   // 2. Tinh toan va hien thi nhiet do cam bien B
                        tp = (ds18b20.temperatureB&0x0f)*10/16;
                        lcd_write_2x3_num(ng/10%10,12,1); 
                        lcd_write_2x3_num(ng/1%10,15,1); 
                        printf(lcd_data,".%ld",tp);
                        
                        lcd_goto(0,3);
                        lcd_data("DS18B20A    DS18B20B");
                  }
            }
            else                                        // 3. Neu phat hien loi thi hien thi thong bao loi
            {
                 lcd_command(0x01);               delay_ms(500);
                 lcd_data("Connection Error...!");delay_ms(500);
            }
            
      }
}

