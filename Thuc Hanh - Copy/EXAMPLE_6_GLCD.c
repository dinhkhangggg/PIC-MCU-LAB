/************************************ Yeu cau *******************************************************************
1. HT LOGO SPKT tai toa do x = 0
2. HT hinh tron (circle) mau trang(1) ban kinh R=6 tai tao do xy=(70,7)
3. HT hinh chu nhat (rect) mau trang(1) tao boi 2 diem x1x1 = (85,1), x2y2= (97,13)
4. HT hinh tam giac (triangle) mau trang(1) tao boi 3 diem x1x1 = (103,13), x2y2= (110,1) , x3x3 = (117,13)
5. HT doan thang (line) mau trang(1) di qua 2 diem x1x1 = (64,15), x2y2= (118,15)
6. HT thanh (bar) mau trang(1) co do day =3 va di qua 2 diem x1x1 = (64,53), x2y2= (118,53)
7. HT chuoi (text) noi dung la "SPKT" mau trang(1) tai toa do xy=(80,57), font 5x7
8. HT gia tri bien "dem" tu 00-99 mau trang (1) tai toa do xy=(70,20), font 20x28

**************************** Cac kien thuc lien quan *********************************************************
1. De ve LOGO SPKT ta dung ham:      glcd_spkt_logo_raw(int x) ;               
2. De ve hinh tron ta dung ham:      glcd_circle(x, y, radius,  fill, color);  
3. De ve hinh chu nhat ta dung ham:  glcd_rect(x1,y1, x2, y2,  fill, color);
4. De ve hinh tam gia ta dung ham:   glcd_triangle(x1,y1, x2,  y2, x3, y3, fill,color);
5. De ve doan thang ta dung ham:     glcd_line(x1, y1, x2,  y2,  color);
6. De ve thanh ta dung ham:          glcd_bar(x1, y1, x2, y2, width, color);
7. De ghi chuoi ky tu ta dung ham:   glcd_text_setup( x, y,size, color, overlay) ;
                                     glcd_text( "chuoi can hien thi");
8. De ghi gia tri cua bien ta dung:  glcd_text_setup( x, y,size, color, overlay) ;
                                     printf(glcd_text,"%??",bien);  // xem lai ham printf trong EXAMPLE_LCD.c                                     
Trong do:                                   
         . x,y, x1,y1, x2, y2, x3,y3  : la cac toa do
         . radius                     : la ban kinh hinh tron
         . width                      : la do day cua thanh
         . fill                       : la co to kin ben trong hinh can ve khong? (1=co , 0=khong)
         . color                      : la mau can ve ( 1: trang , 0 : xanh)
         . size                       : la kich thuoc chu, vi du size = 3 co kich thuoc la (3*5) x (3*7) = 15x21
         . overlay                    : co ghi de len cac noi dung truoc do khong ? (1=co , 0=khong)
9. De hien thi cac noi dung da ve ra man hinh ta dung ham :  glcd_update();
Chu y:   . Thieu ham nay GLCD se khong hien thi
         . Neu cac net ve khong nam de len nhau thi sau khi ve noi net ra goi ham "glcd_update();" ngay sau do
         . Neu cac net ve nam de len nhau thi ta ve xong het cac net do roi moi goi ham "glcd_update();"
         
*********************************** Chuong trinh ***************************************************************/
//#define   BOARD     D501  
#include<tv_boards.c>  
#include<tv_glcd.c>  
unsigned int8 dem=0;
void main()
{
      system_init(); 
      
      glcd_setup(); // cau hinh GLCD - thieu ham nay GLCD khong chay
      glcd_spkt_logo_raw(0);                  // 1. Ve logo SPKT o toa do x=0
      glcd_update();                          // cap nhat logo SPKT hien thi ra man hinh
      glcd_circle(70, 7, 6,  1, 1);           // 2. ve hinh tron co tam tai (70,7) , ban kinh 6, to kin, mau trang
      glcd_update();
      glcd_rect(85,1, 97, 13, 1,1);           // 3. ve hinh chu nhat tai (81,1), (97,13), to kin , mau trang
      glcd_update();
      glcd_triangle(103,13,110,1,117,13,1,1); // 4. ve hinh tam giac
      glcd_update();
      glcd_line(64, 15, 118,  15,  1);        // 5. ve doan thang
      glcd_update();
      glcd_bar(64, 53, 118, 53,3,1);          // 6. ve thanh co do day =3
      glcd_update();                          
      glcd_text_setup(80,56,1,1,0);           // 7. cau hinh hien thi chuoi ky tu tai (80,57),font1 (5x7), mau trang, khong de len cac net truoc do
      glcd_text("SPKT");                      // 7. xuat chuoi can hien thi 
      glcd_update();                         
      while(true)
      {    
            glcd_text_setup(70,20,4,1,1);    // 8. cau hinh hien thi chuoi ky tu tai (70,20),font4 4x(5x7)=20x28, mau trang, ghi de len so truoc do
            printf(glcd_text,"%02d",dem);    // 8. hien thi gia tri cua bien dem
            glcd_update();                  
            dem++; dem%=100;
            delay_ms(200);
      }
}

