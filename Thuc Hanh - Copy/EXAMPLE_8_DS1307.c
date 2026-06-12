/************************************ Yeu cau ****************************************************************
 1. Doc thoi gian gio:phut:giay va hien thi tren GLCD
 2. Chinh thoi gian dung 3 nut UP, DW, MOD
 3. Neu he thong bi loi ket noi thi hien thi thong bao
 
********************************* Cac lenh lien quan *********************************************************
 1. De doc thoi gian ta dung ham:
              ----------------------------------------------
                   result = ds1307_read_time();
              ----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . Sau khi goi ham tren cac thong so thoi gian duoc doc ve va lu trong cac bien:
         . ds1307.second  => gia tri giay  
         . ds1307.minute  => gia tri phut  
         . ds1307.hour    => gia tri gio
         . ds1307.day_of_week   => gia tri thu
         . ds1307.date    => gia tri ngay
         . ds1307.month   => gia tri thang
         . ds1307.year    => gia tri nam
     . Chu y: Cac gia tri nay thuoc dang BCD (vi 36 giay thi se la 0x36) vi vay khi tach so ta phai chia 16
 2. De cai dat thoi gian ta dung ham:
              -----------------------------------------------
                result= ds1307_set_time(DS1307_STRUCT t);
              -----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . DS1307_STRUCT t la bien luu thoi gian can cai dat
     
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_glcd.c>
#include<tv_ds1307.c>
unsigned int8 mode=0;    
unsigned int8 hieuchinhBCD(unsigned int8 x)   // 2.truong hop tang tu 0x09 len 1 thanh 0x0A nen phai +6 de thanh 0x10
{                                             // 2.truong hop giam tu 0x10 xuong 1 thanh 0x0f nen phai -6 de thanh 0x09
      if((x&0x0f)==0x0a) x+=6;
      if((x&0x0f)==0x0f) x-=6;
      return x;
}
void hieuchinhBCDvaNapthoigian()             // 2.Nap thoi gian vao ds1307
{
      ds1307.second= hieuchinhbcd(ds1307.second);
      ds1307.minute= hieuchinhbcd(ds1307.minute);
      ds1307.hour  = hieuchinhbcd(ds1307.hour);
      ds1307_set_time(ds1307);
}
void mode_display()                          // 2.hien thi giao dien cai dat
{ 
      glcd_rect(0,17,122,52,1,0);
      if(mode!=0)glcd_circle(30*mode,32,15,1,1); 
      glcd_text_setup(43,29,1,1,0); glcd_text(":");
      glcd_text_setup(74,29,1,1,0); glcd_text(":");    
}

void main()
{
      system_init();  
      glcd_setup();
      if(ds1307_battery_check()==ERROR)                           // kiem tra pin
      {                                                           // do het pin nen thoi gian chay sai => can phai chinh lai
             ds1307.second=0x00;                                  // thay bang thoi gian hien tai
             ds1307.minute=0x00; 
             ds1307.hour  =0x00; 
             ds1307_set_time(ds1307);
             glcd_text_setup(0,29,1,1,0); glcd_text("Empty Battery");
             glcd_update(); delay_ms(3000);
             glcd_rect(0,29,119,36,1,0); glcd_update();    
      }
      mode_display();
      while(true)
      {     
             if(inputcd(MOD)==0){mode++; mode%=4;mode_display();}   // 2.mode =0: khong chinh, mode=1:chinh gio, mode=2:phut, mode=3:chinh giay
             if(inputcd(UP)==0)                                     // 2.neu nhan UP thi tang thoi gian
             {
                  if((mode==3)&&(ds1307.second<0x59)) ds1307.second++;
                  if((mode==2)&&(ds1307.minute<0x59)) ds1307.minute++;
                  if((mode==1)&&(ds1307.hour  <0x23)) ds1307.hour++;
                  hieuchinhBCDvaNapthoigian();
             }
             if(inputcd(DW)==0)                                     // 2.neu nhan DW thi giam thoi gian
             {
                  if((mode==3)&&(ds1307.second>0)) ds1307.second--;
                  if((mode==2)&&(ds1307.minute>0)) ds1307.minute--;
                  if((mode==1)&&(ds1307.hour  >0)) ds1307.hour--;
                  hieuchinhBCDvaNapthoigian();
             }
            if(ds1307_read_time()==OK)                             // 1.neu doc thoi gian tu DS1307 thanh cong thi tien hanh hien thi
            {    
                  glcd_text_setup(20,25,2,!(mode==1),1); 
                  printf(glcd_text,"%d%d",ds1307.hour/16,ds1307.hour%16);
                  glcd_text_setup(50,25,2,!(mode==2),1); 
                  printf(glcd_text,"%d%d",ds1307.minute/16,ds1307.minute%16);
                  glcd_text_setup(80,25,2,!(mode==3),1); 
                  printf(glcd_text,"%d%d",ds1307.second/16,ds1307.second%16);
                  glcd_update();
            }
            else                                                   //3. neu khong giao tiep duoc DS1307 thi hien thi loi 
            { 
                  glcd_rect(0,0,127,63,1,0);
                  glcd_text_setup(0,15,2,1,0);
                  glcd_text("Connection");
                  glcd_text_setup(0,35,2,1,0);
                  glcd_text(" Erorr...!");
                  glcd_update();delay_ms(500);
                  glcd_rect(0,0,127,63,1,0);
                  glcd_update(); delay_ms(500);
            }
      }
}

