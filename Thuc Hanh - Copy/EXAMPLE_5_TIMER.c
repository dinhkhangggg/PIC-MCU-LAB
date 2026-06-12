/************************************ Yeu cau *****************************************************************
 1. Dem giay chinh xac hien thi LED 7 doan 
 
********************************* Cac kien thuc lien quan ****************************************************
 1. Tinh toan thoi gian dinh thoi - dung TIMER1 hoac TIMER3
             -----------------------------------
             |                      t*Fosc     |
             |  nap =  65536 -    ---------    |
             |                    4000*bc*n    |
             -----------------------------------                    
     . t   : thoi gian can dinh thoi (ms)
     . Fosc: tan so bo giao dong thach anh ( tren board dang dung 20Mhz)
     . bc  : bo chi truoc cua timer ( duoc chon 1,2,4,8)
     . n   : so lan timer dem tran (duoc chon la 1 so nguyen duong)
         . Vi du de dinh thoi 1s ta the t =1000 vao cong thuc tren ta tinh duoc:
                        bc =8, n =10, nap =3036
 2. Sau khi tinh toan xong ta the cac gia tri moi tinh duoc vao bai mau sau:
 
      #define   BOARD     D501
      #include<tv_boards.c> 
      unsigned int8 bdn=0;
      void main()
      {
            system_init();   
            setup_timer_1(T1_INTERNAL|T1_DIV_BY_BC);  // thay gia tri "bc" vao day
            set_timer1(nap);                          // thay gia tri "nap" vao day
            enable_interrupts(int_timer1);            // cho phep ngat t1
            enable_interrupts(global);                // cho phep ngat toan cuc
            while(true)
            {    
                  if(bdn>=n)                          // thay gia tri "n" vao day  
                  {
                        bdn=bdn-n;
                       // xu ly khi dinh thoi xong o day                            
                  }
            }
      }
      #int_timer1                                    // chuong trinh xu ly ngat t1
      void ngatt1()                                  // khong viet gi vao giua 2 hang nay 
      {
            set_timer1(nap);                         // thay gia tri "nap" vao day
            bdn++;
      }              
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D401
#include<tv_boards.c> 
unsigned int8 bdn=0,giay=0;
void main()
{
      system_init();      
      setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);   // bc  =8 nhu tinh toan o tren
      set_timer1(3036);                         // nap =3036 
      enable_interrupts(int_timer1);            // cho phep ngat t1
      enable_interrupts(global);                // cho phep ngat toan cuc
      while(true)
      {    
            if(bdn>=10)                         // n =10 la du 1 giay
            {
                  bdn=bdn-10;
                  giay++; giay%=60;
                  d7seg.led[1]= m7d[giay/10%10];
                  d7seg.led[0]= m7d[giay/1%10];
                  d7seg_display();
            }
      }
}
#int_timer1                                    // chuong trinh xu ly ngat t1
void ngatt1()                                  // khong viet gi vao giua 2 hang nay 
{
      set_timer1(3036);
      bdn++;
}              
