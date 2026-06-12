/************************************ Yeu cau ****************************************************************
 1. Nhan phim BT0 de tang toc dong co theo 10 cap
 2. Nhan phim BT1 de giam toc dong co theo 10 cap
 3. Nhan phim BT2 de dao chieu quay dong co
 4. Nhan phim BT3 de dung dong co
 5. Do toc do dong co theo don vi vong/phut (RPM) 
 6. Hien thi cac thong tin: cap toc do, toc do dong co va chieu quay cua dong co tren GLCD
 
********************************* Cac lenh lien quan *********************************************************
1. De dieu khien toc do dong co ta viet lenh:
               ----------------------------------------------
                         dcmotor.duty = dt;
                         dcmotor_control();
               ----------------------------------------------
   .dt la gia tri tu 0(khong quay) den 1000(nhanh nhat)
2. De dao chieu quay cua dong co ta viet lenh:
               ----------------------------------------------
                         dcmotor.direct = dr;
                         dcmotor_control();
               ----------------------------------------------
   . dr = dcmotor_direct_forward hoac 1 => de dong co quay thuan    
   . dr = dcmotor_direct_reverse hoac 0 => de dong co quay nghich                 
3. De do toc do dong co ta thuc hien theo mau sau trong while(true): 
               ----------------------------------------------
                    if(dcmotor_read_speed()==OK)
                     {
                           hien thi bien "dcmotor.speed"
                     }  
               ---------------------------------------------- 
              
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501 
#include<tv_boards.c> 
#include<tv_dcmotor.c>
#include<tv_glcd.c>
void level_display()        // Hien thi cap do dong co - xem lai bai EXAMPLE_GCLD.c
{
     glcd_text_setup(9,13,3,1,1);
     printf(glcd_text,"%02lu",dcmotor.duty/100);
     glcd_update();
}
void speed_display()       // Hien thi toc do quay cua dong co 
{
     glcd_text_setup(54,13,3,1,1);
     printf(glcd_text,"%04lu",dcmotor.speed);
     glcd_update(); 
}
void status_display()       // Hien thi trang thai quay cua dong co
{
     glcd_text_setup(43,52,1,0,1);
     if(dcmotor.duty==0)                            glcd_text("  STOP ");
     else if(dcmotor.direct==dcmotor_direct_forward)glcd_text("FORWARD"); 
     else                                           glcd_text("REVERSE"); 
     glcd_update();
}
void main()
{ 
      system_init();  
      glcd_setup();
      glcd_rect(0,3,47,40,0,1);
      glcd_text_setup(5,0,1,1,1);
      glcd_text(" LEVEL");
      glcd_update();
      glcd_rect(50,3,127,40,0,1);
      glcd_text_setup(71,0,1,1,1);
      glcd_text(" SPEED");
      glcd_update();
      glcd_rect(0,45,127,63,1,1);
      glcd_update();
      dcmotor.duty=800; 
      level_display();
      
      while(true)
      {    // 1. Nhan BT0 de tang toc
           if((inputcd(BT0)==0)&& (dcmotor.duty<1000)){ dcmotor.duty+= 100;     level_display();}
           // 2. Nhan BT1 de giam toc
           if((inputcd(BT1)==0)&& (dcmotor.duty>0))   { dcmotor.duty-= 100;     level_display();}
           // 3. Nhan BT2 de dao chieu dong co
           if(inputcd(BT2)==0) dcmotor.direct=!dcmotor.direct;
           // 4. Nhan BT2 de dung dong co
           if(inputcd(BT3)==0)                        {dcmotor.duty=0;          level_display();}
           dcmotor_control();   // cap nhat cac gia tri dieu khien
           //5. Do toc do dong co
           if(dcmotor_read_speed()==OK) speed_display();
           status_display();         
      }
}
