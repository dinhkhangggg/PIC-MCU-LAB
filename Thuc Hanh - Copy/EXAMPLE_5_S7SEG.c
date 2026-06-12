/************************************ Yeu cau *****************************************************************
1. Dieu khien 32 LED don sang dan lien tuc
2. Hien thi chu ChuKy-XX tren 8 LED 7 doan, voi XX la so chu ky sang dan cua LED don tu 00 den 99 

**************************** Cac kien thuc lien quan *********************************************************
1.Cach tao ma LED 7 doan 
   . Ve hinh LED 7 doan va danh so cac canh nhu hinh duoi
                             1
                          ______
                         |      |
                       32|  64  | 2
                         |______|
                         |      |
                       16|      | 4
                         |______|  
                            8     * 128
    .Vi du: hien thi chu E gom cac canh 1,32,64,16,8
       . Doi voi LED 7 doan anode chung thi ma chu E se la  : 0XFF-1-32-64-16-8
       . Doi voi LED 7 doan cathode chung thi ma chu E se la: 1+32+64+16+8
2.Cach dieu khien module 8 LED 7 doan quet 
   . Giai ma vao cac phan tu "s7seg.led[X]" tuong ung voi 8 LED 7 doan
                    
                  s7seg.led[6]        s7seg.led[4]        s7seg.led[2]        s7seg.led[0]
           ______    ______    ______    ______    ______    ______    ______    ______
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      |
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      | 
          |______|  |______|  |______|  |______|  |______|  |______|  |______|  |______|
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      |
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      | 
          |______|  |______|  |______|  |______|  |______|  |______|  |______|  |______| 
        s7seg.led[7]        s7seg.led[5]         s7seg.led[3]       s7seg.led[1]       
        
   . Goi ham " s7seg_display(); " de hien thi ra LED 7 doan
3.Khi 8 LED quet bi mo hoac bi nhap nhay ta khac phuc nhu sau:
   . Goi chuong trinh " s7seg_display(); " trong cac lenh vong lap
   . Bo cac lenh delay lau thay bang viec thuc thi ham "s7seg_display();" nhieu lan
  
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
signed int8 n,chuky=0,m;
void main()
{
      system_init();  
      // 2. Nhung noi dung khong thay doi thi giai ma truoc while(true)
      s7seg.led[7] = 0XFF-1-8-16-32;       //  C      
      s7seg.led[6] = 0XFF-4-16-32-64;      //  h  
      s7seg.led[5] = 0xff-4-8-16     ;     //  u 
      s7seg.led[4] = 0xff-16-32-64-2-4;    //  K 
      s7seg.led[3] = 0XFF-2-4-8-32-64  ;   //  Y 
      s7seg.led[2] = 0xff-64;              //  -
      s7seg.led[1]= m7d[chuky/10%10];      // hang chuc
      s7seg.led[0]= m7d[chuky%10];         // hang don vi
      while(true)
      {     
//!             for(n=32;n>=0;n--)
//!             {
//!                    led32.ledx32=0xffffffff>>n;
//!                    led32_display();
//!                    delay_ms(100);           
//!             }
//!             chuky++; chuky%=100;
//!             s7seg.led[1]= m7d[chuky/10%10];       // cai gi thay doi thi giai ma lai
//!             s7seg.led[0]= m7d[chuky%10]; 
//!             s7seg_display();
/*************************************************************************************************************
        Doan chuong trinh tren co yeu diem la LED quet se nhap nhay lien tuc do tan so quet khong du. 
       => De khac phuc hien tuong  tren ta thuc hien theo huong dan (3) ben tren nhu sau:
*************************************************************************************************************/      
             for(n=32;n>=0;n--)                        // 1. Dieu khien 32 LED don sang dan lien tuc
             {
                    led32.ledx32=0xffffffff>>n;
                    led32_display();
                    //delay_ms(100);                   // 3*.Bo delay lau thay bang "s7seg_display();" nhieu lan
                    for(m=0;m<10;m++) s7seg_display(); // 3*.Goi "s7seg_display();" trong vong lap      
             } 
             chuky++; chuky%=100;
             s7seg.led[1]= m7d[chuky/10%10];           // 2. Cai gi thay doi thi giai ma lai
             s7seg.led[0]= m7d[chuky%10]; 
             s7seg_display();
      }
}


  
