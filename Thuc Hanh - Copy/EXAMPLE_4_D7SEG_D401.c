/************************************ Yeu cau *****************************************************************
 1. Hien thi chu --HELLO- tren 8 LED 7 doan
 2. Sau 3 giay chuyen qua hien thi gia tri 2 bien XX va YY theo mau XX.YY tren 4 LED chinh giua
 3. Sau 3 giay chuyen ve hien thi chu --HELLO- va lap lai tu dau
 
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
                            8      . 128
                            
    Vi du: hien thi chu E gom cac canh 1,32,64,16,8
    . Doi voi LED 7 doan anode chung thi ma chu E se la  : 0XFF-1-32-64-16-8
    . Doi voi LED 7 doan cathode chung thi ma chu E se la: 1+32+64+16+8
2.Cach dieu khien module 8 LED 7 doan truc tiep 
    . Giai ma vao cac phan tu "d7seg.led[X]" tuong ung voi 8 LED 7 doan
                    
                  d7seg.led[6]        d7seg.led[4]        d7seg.led[2]        d7seg.led[0]
           ______    ______    ______    ______    ______    ______    ______    ______
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      |
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      | 
          |______|  |______|  |______|  |______|  |______|  |______|  |______|  |______|
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      |
          |      |  |      |  |      |  |      |  |      |  |      |  |      |  |      | 
          |______|  |______|  |______|  |______|  |______|  |______|  |______|  |______| 
        d7seg.led[7]        d7seg.led[5]         d7seg.led[3]       d7seg.led[1]       
        
    . Goi ham " d7seg_display(); " de hien thi ra LED 7 doan
  
*********************************** Chuong trinh **************************************************************/
//#define   BOARD     D401
#include<tv_boards.c> 
unsigned int8 XX = 38, YY = 59;
void main()
{
      system_init(); 
      while(true)
      {                                           // 1. Hien thi chu "--HELLO-"
             d7seg.led[7] = 0xff-64;              //  - 
             d7seg.led[6] = 0xff-64;              //  - 
             d7seg.led[5] = 0XFF-16-32-64-2-4;    //  H 
             d7seg.led[4] = 0XFF-1-32-64-16-8;    //  E  
             d7seg.led[3] = 0xff-8-16-32;         //  L 
             d7seg.led[2] = 0xff-8-16-32;         //  L 
             d7seg.led[1] = 0XFF-1-2-4-8-16-32;   //  O 
             d7seg.led[0] = 0xff-64;              //  -
             d7seg_display();
             delay_ms(3000);
                                                 //  2. Sau 3 giay chuyen qua hien thi gia tri bien
             d7seg.led[7] = 0xff;                //  tat LED 
             d7seg.led[6] = 0xff;                //  tat LED 
             d7seg.led[5] = m7d[XX/10%10];       //  Hang chuc bien XX 
             d7seg.led[4] = m7d[XX/1%10]-128;    //  Hang don vi bien XX , -128 de xuat hien dau .
             d7seg.led[3] = m7d[YY/10%10];       //  Hang chuc bien YY 
             d7seg.led[2] = m7d[YY/1%10];        //  Hang don vi bien YY 
             d7seg.led[1] = 0xff;                //  tat LED 
             d7seg.led[0] = 0xff;                //  tat LED 
             d7seg_display();
             delay_ms(3000);          
      }
}
  

