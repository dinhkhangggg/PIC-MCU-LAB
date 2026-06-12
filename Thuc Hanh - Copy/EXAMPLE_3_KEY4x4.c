/************************************ Yeu cau *******************************************************************
 1. Nhan cac phim tu 0 den 12 de bat hoac tat cac LED tu 0-12
 2. Nhan phim 15 de cho phep 16 LED tan cung ben trai sang dan lien tuc
 3. Nhan giu phim 13 de tang bien dem  va hien thi LED 7 doan
 4. Nhan giu phim 14 de giam bien dem  va hien thi LED 7 doan
 
********************************* Cac lenh lien quan ************************************************************
1.Khi tac vu xu ly LAP DI LAP LAI LIEN TUC (vd:16 LED sang dan lien tuc nhu yeu cau 2)      
   . Ta thuc hien theo mau:
              ----------------------------------------------
                       int1 tt=0;
                       if(key4x4_read()==OK)
                        {
                              if(key4x4.key== ?) tt=1;
                        }
                       if(tt)
                        {
                            thuc thi tac vu o day
                        }
              ----------------------------------------------                                    
2.Khi tac vu xu ly KHONG LAP DI LAP LAI LIEN TUC(vd:Nhan cac phim tu 0 den 12 de bat hoac tat cac LED tu 0-12 nhu yeu cau 1)      
   . Ta thuc hien theo mau: 
              ----------------------------------------------      
                        if(key4x4_read()==OK)
                        {
                              if(key4x4.key== ?) 
                              {
                                    thuc thi tac vu o day
                              }
                        }
              ----------------------------------------------
3.Doi voi tac vu nhan giu ta thuc hien theo mau: 
              ---------------------------------------------- 
                      if(key4x4_read()==OK)
                        {
                              if(key4x4.key== ?) 
                              {
                                    thuc thi tac vu o day
                              }
                        }    
              ---------------------------------------------- 
4.Khi ban phim dap ung khong tot (nhan giu lau moi duoc)
   .Ta goi chuong trinh quet phim va xu ly phim nhan trong cac lenh vong lap 
   .Bo cac ham delay mat nhieu thoi gian va thay vao do la goi ham quet phim va xu ly nhieu lan 
      
*********************************** Chuong trinh ************************************************************/
//!#define   BOARD     D501
//!#include<tv_boards.c> 
//!#include<tv_key4x4.c>
//!signed int8 i;
//!unsigned int8 dem=0;
//!unsigned int1 tt=0;
//!void sangdan16led()                
//!{
//!     for(i=16;i>=0;i--)
//!      {
//!            led32.ledx16[1]=0xffff>>i;
//!            led32_display(); delay_ms(200);
//!      }
//!}
//!void main()
//!{
//!      system_init();  
//!      d7seg.led[2] = m7d[dem/100%10];      
//!      d7seg.led[1] = m7d[dem/10%10];        
//!      d7seg.led[0] = m7d[dem/1%10];       
//!      d7seg_display();
//!      while(true)
//!      {     
//!            if(key4x4_read()==OK)
//!            {     //1. Nhan cac phim tu 0 den 12 de bat hoac tat cac LED tu 0-12
//!                  if(key4x4.key<13) led32.ledx16[0]^=((int16)0x01<<key4x4.key);
//!                  led32_display();
//!                  
//!                  //2. Nhan phim 15 de cho phep 16 LED tan cung ben trai sang dan lien tuc
//!                  if(key4x4.key==15)tt=1;
//!                  
//!                  //3. Nhan giu phim 13 de tang bien dem  va hien thi LED 7 doan
//!                  if(key4x4.key==13)dem++;   
//!                  
//!                  //4. Nhan giu phim 14 de giam bien dem  va hien thi LED 7 doan
//!                  if(key4x4.key==14)dem--; 
//!
//!                  // hien thi bien dem tren LED 7 doan->tham khao example_d7seg
//!                  d7seg.led[2] = m7d[dem/100%10];      
//!                  d7seg.led[1] = m7d[dem/10%10];        
//!                  d7seg.led[0] = m7d[dem/1%10];       
//!                  d7seg_display();
//!            }
//!            
//!            //2. Nhan phim 15 de cho phep 16 LED tan cung ben trai sang dan lien tuc
//!            if(tt==1)   
//!            {
//!               sangdan16led() ;
//!            }
//!      }
//!}

/*Nhan thay neu ta viet chuong trinh nhu tren thi khi 16 LED don dang sang dan se gay ra tri hoan 
nhieu 200*17 = 3.7s. Trong 3.7s nay neu ta nhan nut se khong tac dung nen ta se khac phuc theo 
huong dan (4) o ben tren nhu sau:  
********************************** Chuong trinh hoan chinh****************************************************/
//#define   BOARD     D503
#include<tv_boards.c>  
#include<tv_key4x4.c> 
signed int8 i;
unsigned int16 n;
unsigned int8 dem=0;
unsigned int1 tt=0;
void quetphim_xuly()
{
      if(key4x4_read()==OK)
      {     //1. Nhan cac phim tu 0 den 12 de bat hoac tat cac LED tu 0-12
            if(key4x4.key<13) led32.ledx16[0]^=((int16)0x01<<key4x4.key);
            led32_display();
            
            //2. Nhan phim 15 de cho phep 16 LED tan cung ben trai sang dan lien tuc
            if(key4x4.key==15)tt=1;
            
            //3. Nhan giu phim 13 de tang bien dem  va hien thi LED 7 doan
            if(key4x4.key==13)dem++;   
            
            //4. Nhan giu phim 14 de giam bien dem  va hien thi LED 7 doan
            if(key4x4.key==14)dem--;
            d7seg.led[2] = m7d[dem/100%10];      
            d7seg.led[1] = m7d[dem/10%10];        
            d7seg.led[0] = m7d[dem/1%10];       
            d7seg_display();
      }
}
void sangdan16led()                
{    
     for(i=16;i>=0;i--)
      {
            led32.ledx16[1]=0xffff>>i;
            led32_display(); 
            // delay_ms(200);        // bo delay lau thay bang chuong trinh quet va xu ly phim nhieu lan
            for(n=0;n<1000;n++) quetphim_xuly();
      }
}
void main()
{
      system_init();  
      d7seg.led[2] = m7d[dem/100%10];      
      d7seg.led[1] = m7d[dem/10%10];        
      d7seg.led[0] = m7d[dem/1%10];       
      d7seg_display();
      
      while(true)
      {     
            quetphim_xuly();
            //2. Nhan phim 15 de cho phep 16 LED tan cung ben trai sang dan lien tuc
            if(tt==1)   
            {
               sangdan16led() ;
            }
      }
}
