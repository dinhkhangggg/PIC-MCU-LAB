/************************************ Yeu cau **********************************************************************
 1. Nhan BT0 de dieu khien 16 LED tan cung ben phai sang hoac tat
 2. Nhan BT1 de cho phep 16 LED tan cung ben trai sang dan lien tuc
 3. Nhan giu BT2 de tang bien dem  va hien thi LED 7 doan
 4. Nhan giu BT3 de giam bien dem  va hien thi LED 7 doan

********************************* Cac lenh lien quan ***************************************************************
1.Khi tac vu xu ly LAP DI LAP LAI LIEN TUC (vd:16 LED sang dan lien tuc nhu yeu cau 2)      
   . Ta thuc hien theo mau:
              ----------------------------------------------   
                       int1 tt=0;
                       ........
                       if(inputcd(BT?) ==0)  tt=1;
                       if(tt)
                        {
                            thuc thi tac vu o day
                        }
              ----------------------------------------------
2.Khi tac vu xu ly KHONG LAP DI LAP LAI LIEN TUC(vd:nhan BT0 de 16 LED sang nhu yeu cau 1)      
   . Ta thuc hien theo mau: 
              ----------------------------------------------
                       if(inputcd(BT?) ==0)  
                       {
                            thuc thi tac vu o day
                       }
              ----------------------------------------------
3.Doi voi tac vu nhan giu ta thuc hien theo mau:   
              ----------------------------------------------
                      if(inputcd(BT?) ==0) 
                       {
                            thuc thi tac vu o day
                       }
              ----------------------------------------------                                   
4.Khi nut nhan dap ung khong tot (nhan giu lau moi duoc):
   .Ta goi chuong trinh kiem tra va xu ly nut nhan trong cac lenh vong lap 
   .Bo cac ham delay mat nhieu thoi gian va thay vao do la goi ham kiem tra va xu ly nhieu lan 
      
*********************************** Chuong trinh ************************************************************/
//!#define   BOARD     D501
//!#include<tv_boards.c> 
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
//!      while(true)
//!      {     
//!         //1. Nhan BT0 de dieu khien 16 LED tan cung ben phai sang<->tat   
//!            if(inputcd(BT0)==0)                  // tac vu xu ly khong lap di lap lai lien tuc
//!            {
//!               led32.ledx16[0]=~led32.ledx16[0];
//!               led32_display();
//!            }
//!
//!          //2. Nhan BT1 de cho phep 16 LED tan cung ben trai sang dan lien tuc
//!            if(inputcd(BT1)==0)tt=1;              // tac vu xu ly lap di lap lai lien tuc
//!            if(tt==1)   
//!            {
//!               sangdan16led() ;
//!            }
//!
//!         //3. Nhan giu BT2 de tang bien dem  va hien thi LED 7 doan
//!            if(inputcd(BT2)==0)dem++;            // tac vu nhan giu 
//!            if(inputcd(BT3)==0)dem--;            // tac vu nhan giu
//!            
//!            d7seg.led[2] = m7d[dem/100%10];      // hien thi bien dem tren LED 7 doan
//!            d7seg.led[1] = m7d[dem/10%10];       // tham khao example_d7seg
//!            d7seg.led[0] = m7d[dem/1%10];
//!            d7seg_display();   
//!      }
//!}

/*Nhan thay neu ta viet chuong trinh nhu tren thi khi 16 LED don dang sang dan se gay ra tri hoan 
nhieu 200*17 = 3.7s. Trong 3.7s nay neu ta nhan nut se khong tac dung nen ta se khac phuc theo 
huong dan (4) o ben tren nhu sau:  

************************************* Chuong trinh hoan chinh**************************************************/
//#define   BOARD     D503 
#include<tv_boards.c>
signed int8 i;
unsigned int8 dem=0; 
unsigned int1 tt=0;
void kiemtra_xuly_nutnhan()                 // tac vu nao dap ung khong tot thi bo vao day
{        
      if(inputcd(BT0)==0)                   // 1. Nhan BT0 de dieu khien 16 LED tan cung ben trai sang hoac tat         
      {
         led32.ledx16[0]=~led32.ledx16[0];  
         led32_display();
      }
      if(input(BT1)==0)tt=1; // 2. Nhan BT1 de bat qua trinh sang dan lien tuc
      if(inputcd(BT2)==0)dem++;             // 3. Nhan giu BT2 de tang bien dem  
      if(inputcd(BT3)==0)dem--;             // 4. Nhan giu BT3 de giam bien dem 
      d7seg.led[2] = m7d[dem/100%10];       // Hien thi bien dem
      d7seg.led[1] = m7d[dem/10%10];       
      d7seg.led[0] = m7d[dem/1%10];
      d7seg_display();   
}
void sangdan16led()                
{
     unsigned int16 n;
     for(i=16;i>=0;i--)
      {
            led32.ledx16[1]=0xffff>>i;
            led32_display();
           // delay_ms(200);// bo delay mat nhieu thoi gian
           for(n=0;n<100;n++)              // thay bang viec kiem tra va xu ly nut nhan nhieu lan
           kiemtra_xuly_nutnhan();     // goi chuong trinh xu ly nut nhan trong cac lenh vong lap 
      }
}

void main()
{
      system_init();  
      while(true)
      {     
            if(tt==1) sangdan16led() ;
            kiemtra_xuly_nutnhan()  ; 
      } 
}
  

