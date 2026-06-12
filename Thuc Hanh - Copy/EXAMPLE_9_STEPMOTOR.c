/************************************ Yeu cau ***************************************************************
 1. Nhan cac phim 0-9 de dieu khien dong co buoc quay tu 0-9 vong
 2. Nhan phim E de thay doi chieu quay cua dong co (thuan-nghich)
 3. Nhan phim F de thay doi che do hoat dong cua dong co ( fullstep - halfstep)
 4. Hien thi thong tin ve chieu quay, che do hoat dong va so vong quay cua dong co len GLCD
 
********************************* Cac lenh lien quan ********************************************************
1. De dao chieu quay cua dong co ta viet lenh:
               ----------------------------------------------
                         stepmotor.direct = dr;
                         stepmotor_control();
               ----------------------------------------------
   . dr = stepmotor_direct_forward hoac 1 => de dong co quay thuan    
   . dr = stepmotor_direct_reverse hoac 0 => de dong co quay nghich   
2.De thay doi che do hoat dong cua dong co ta viet lenh:
               ----------------------------------------------
                         stepmotor.mode  = md;
                         stepmotor_control();
               ----------------------------------------------
   . md = stepmotor_mode_fullstep hoac 1 => de dong co hoat dong o che do full step
   . md = stepmotor_mode_halfstep hoac 0 => de dong co hoat dong o che do half step
3.De thay doi toc do quay cua dong co ta viet lenh:
               ----------------------------------------------
                         stepmotor.time_per_step = t;
                         stepmotor_control();
               ----------------------------------------------
   . t la thoi gian cho moi buoc (don vi ms)  
4. Khi dong co quay thi so buoc da quay duoc luu trong bien:
               ----------------------------------------------
                         stepmotor.step
               ----------------------------------------------
               
*********************************** Chuong trinh ************************************************************/
//#define   BOARD     D401
#include<tv_boards.c> 
#include<tv_stepmotor.c>
#include<tv_glcd.c>
#include<tv_key4x4.c>
unsigned int16 maxstep;
unsigned int8 cycle=3;

void cycle_display()// chuong trinh hien thi so vong con lai
{
      glcd_text_setup(24,21,3,1,1);  
      printf(glcd_text,"%d",cycle);
      glcd_update();  
}
void direct_display()      // chuong trinh con hien thi chieu quay ra GLCD theo dang do hoa
{      
     glcd_circle(31,31,25,1,1);
     glcd_circle(31,31,20,1,0);
     glcd_rect(25,0,37,62,1,0);
     if(stepmotor.direct==stepmotor_direct_forward)
     {
         glcd_triangle(25,0,34,9,25,17,1,1);
         glcd_triangle(29,53,38,45,38,62,1,1);
     }
     else 
     {
         glcd_triangle(25,45,34,53,25,62,1,1);
         glcd_triangle(29,9,38,0,38,17,1,1);
     }
     glcd_update(); 
     cycle_display(); 
}
void mode_display()  // chuong trinh con hien thi che do hoat dong cua dong co ra GLCD
{  
      glcd_text_setup(65,15,2,1,1);
      if(stepmotor.mode==stepmotor_mode_fullstep)glcd_text("FULL");
      else                                       glcd_text("HALF");
      glcd_update();
      glcd_text_setup(65,38,2,1,0);
      glcd_text("STEP");
      glcd_update();
}
void main()
{ 
      system_init();  
      glcd_setup();
      direct_display();
      mode_display();
      cycle_display();
      stepmotor.time_per_step =5;                    // moi buoc cach nhau 5ms
      while(true)
      {    
            if(key4x4_read()==OK)   
            {
                  // 1. Neu nhan cac phim tu 0-9 thi cap nhat so vong bang phim duoc nhan
                  if(key4x4.key<10) { cycle= key4x4.key;                                 cycle_display();}
                  // 2. Neu nhan phim E thi dao chieu quay 
                  if(key4x4.key==14){ stepmotor.direct=!stepmotor.direct;                direct_display();}
                  // 3. Neu nhan phim F thi doi che do hoat dong cua dong co 
                  if(key4x4.key==15){ stepmotor.mode=!stepmotor.mode;                    mode_display();}
                  
            }
            if(cycle>0)
            {     // Moi lan "stepmotor_control()==OK" thi dong co quay xong 1 buoc
              stepmotor_control();
                  // Che do fullstep thi 200 buoc = 1 vong, halfstep thi 400 buoc = 1 vong
              if(stepmotor.mode==stepmotor_mode_fullstep) maxstep =200;
              else                                        maxstep =400;
                  // Neu quay du 1 vong roi thi giam so vong xuong, va reset step ve 0 cho chu ky tiep theo
              if(stepmotor.step>=maxstep)
                {
                     stepmotor.step=0;
                     cycle--;
                     cycle_display();   
                }
            }    
      } 
}
