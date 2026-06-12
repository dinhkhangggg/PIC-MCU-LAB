#ifndef  __TV_GLCD_C__
#define  __TV_GLCD_C__
#include<tv_glcd.h>
#if  BOARD == MOPHONG
//************************************** Board Mo Phong ****************************************************
result GLCD_UPDATE_COORDINATE(unsigned int8 x1,unsigned int8 y1, unsigned int8 x2, unsigned int8 y2)
{
      if(GLCD.tt_update==0)
      {
            GLCD.x1 = x1;
            GLCD.x2 = x2;
            GLCD.y1 = y1;
            GLCD.y2 = y2;   
      }
      else
      {    
            if(x1<GLCD.x1) GLCD.x1=x1;
            if(x2>GLCD.x2) GLCD.x2=x2;
            if(y1<GLCD.y1) GLCD.y1=y1;
            if(y2>GLCD.y2) GLCD.y2=y2;   
      } 
      GLCD.tt_update=1;
      return OK;
}
result glcd_clear(int1 color)
{
   unsigned int8 i, j,cl;
   if(color) cl =0xff;
   else      cl =0;
   for(i = 0; i < 8; ++i)
    for(j = 0; j < 192; ++j)
     GLCD.displayData[i][j]= cl; 
   GLCD.x1 = 0;
   GLCD.x2 = 191;
   GLCD.y1 = 0;
   GLCD.y2 = 63;  
   GLCD.tt_update=1;
   return OK;
}
result glcd_pixel(unsigned int8 x, unsigned int8 y, int1 color)
{
   if(color)
      bit_set(GLCD.displayData[y/8][x], y%8);
   else
      bit_clear(GLCD.displayData[y/8][x], y%8);
   GLCD_UPDATE_COORDINATE(x,y, x, y);
   return OK;
}

result GLCD_PIXEL_TEMP(unsigned int8 x, unsigned int8 y, int1 color)
{
   if(color)
      bit_set(GLCD.displayData[y/8][x], y%8);
   else
      bit_clear(GLCD.displayData[y/8][x], y%8);
   return OK;
}

result GLCD_VERTICAL_LINE(unsigned int8 x, unsigned int8 y1,unsigned int8 y2, int1 color)
{
   unsigned int8 i,top,bot;
   top=y1/8; bot = y2/8;
   if (top==bot) 
   {
          if(color)     
               GLCD.displayData[top][x]= GLCD.displayData[top][x] | ((0xff<<(y1%8)) & (0xff>>(7-(y2%8))));   
          else 
               GLCD.displayData[top][x]= GLCD.displayData[top][x] &(~((0xff<<(y1%8)) & (0xff>>(7-(y2%8)))));
   }
   else 
   {
          if(color)
          {    
               GLCD.displayData[top][x]= GLCD.displayData[top][x] | (0xff<<(y1%8));
               GLCD.displayData[bot][x]= GLCD.displayData[bot][x] | (0xff>>(7-(y2%8)));
               for(i = top + 1; i < bot; i++) GLCD.displayData[i][x]=0xff;
          }
          else 
          {    
               GLCD.displayData[top][x]= GLCD.displayData[top][x] & (~(0xff<<(y1%8)));
               GLCD.displayData[bot][x]= GLCD.displayData[bot][x] & (~(0xff>>(7-(y2%8))));
               for(i = top + 1; i < bot; i++)GLCD.displayData[i][x]=0;
          } 
   }
   return OK;
}
result glcd_update()
{
    unsigned int8 i, j;
    GLCD.y1=GLCD.y1/8;
    GLCD.y2=GLCD.y2/8;
    for(i = GLCD.y1; i <=GLCD.y2; i++)
    {  
         output_high(GLCD_CSA);
         output_high(GLCD_CSB);
         output_high(GLCD_CSC);
         if(GLCD.x1<64)
          {    output_low(GLCD_CSA);
               glcd_command(0x40 | GLCD.x1);
          }
         else if(GLCD.x1<128)
          {    output_low(GLCD_CSB);
               glcd_command(0x40 | (GLCD.x1-64));
          }
         else 
          {    output_low(GLCD_CSC);
               glcd_command(0x40 | (GLCD.x1-128));
          }  
         glcd_command(0xB8 | i);
         for (j=GLCD.x1;j<= GLCD.x2;j++)
         {
               if(j==64)      {output_high(GLCD_CSA);output_low(GLCD_CSB);glcd_command(0x40);glcd_command(0xB8 | i); }
               else if(j==128){output_high(GLCD_CSB);output_low(GLCD_CSC);glcd_command(0x40);glcd_command(0xB8 | i); }
               glcd_data(GLCD.displayData[i][j]);  
         }
    }
    GLCD.tt_update=0;
    return OK;
}
result glcd_text_setup(unsigned int16 x, unsigned int16 y,int8 size, int1 color,int1 overlay)
{
      GLCD.mau = color;
      GLCD.size = size;
      GLCD.x    = x;
      GLCD.y    =y;
      GLCD.tt_text=0;
      GLCD.overlay=overlay;
      return OK;
}

result glcd_text( char t)
{      
      static unsigned int8 x1,x2,y1,y2;
      unsigned int8 j, k, l;                      
      unsigned int8 pixelData[5]; 
      if(GLCD.overlay)
      {
            glcd_rect(GLCD.x,GLCD.y,GLCD.x+6*GLCD.size,GLCD.y+7*GLCD.size,1,!GLCD.mau);
      }
      if(GLCD.tt_text==0)
      {
          x1=GLCD.x;
          y1=GLCD.y;
          GLCD.tt_text=1;
      }
      if(t < 'S')
         memcpy(pixelData, GLCD_FONT[t - ' '], 5);
      else if(t <= '~') 
         memcpy(pixelData, GLCD_FONT2[t - 'S'], 5);
      else
         memcpy(pixelData, GLCD_FONT[0], 5);   
      if(GLCD.x+5*GLCD.size >= GLCD_WIDTH)         
      {
         GLCD.x = 0;                           
         GLCD.y += 7*GLCD.size + 1;                 
      }
      for(j=0; j<5; ++j, GLCD.x+=GLCD.size)         
      {
         for(k=0; k < 7; ++k)             
         {
            if(bit_test(pixelData[j], k)) 
            {
               for(l=0; l < GLCD.size; ++l)   
               {   
                  GLCD_VERTICAL_LINE(GLCD.x+l, GLCD.y+k*GLCD.size,GLCD.y+(k+1)*GLCD.size-1, GLCD.mau);
               }
            }
         }
      }
      x2 = GLCD.x;
      GLCD.x+=GLCD.size;
      y2= GLCD.y + 7*GLCD.size;
      GLCD_UPDATE_COORDINATE(x1,y1, x2,y2);
      return OK;
 }
 

result glcd_spkt_logo_raw(int x) 
{ 
   unsigned int8 i=0, j=0, k=0,y=0; 
   unsigned int16 count=0; 
   GLCD_UPDATE_COORDINATE(x,0, x+64,63);
   for(j=0;j<64;j++) 
      {    
         for(;i<64;) 
         { 
            for(k=8;k>0;k--)
            {
               GLCD_PIXEL_TEMP(i+x,j+y,bit_test(GLCD_Logo_DHSPKTTP[count],(k-1))); 
               i++; 
            } 
            count++; 
         } 
      i=0; 
      } 
     return OK;
} 

result glcd_line(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 color)
{
   unsigned int16        dy, dx;
   signed int8  addx=1, addy=1;
   signed int16 P, diff;
   unsigned int8 i=0,xt1,xt2,yt1,yt2;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   xt1=x1; xt2=x2;
   yt1=y1, yt2=y2;
   if(x1 > x2)
   {
      addx = -1;
      xt2=x1;
      xt1=x2;
   }
   if(y1 > y2)
   {
      addy = -1;
      yt2=y1;
      yt1=y2;
   }
   GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         GLCD_PIXEL_TEMP(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         GLCD_PIXEL_TEMP(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   return OK;
}
result glcd_rect(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 fill, int1 color)
{
      unsigned int8  xmin, xmax, ymin, ymax,i;
      xmin = x1;
      xmax = x2;
      ymin = y1;
      ymax = y2;
      if(x1 > x2)                            
      {
         xmin = x2;
         xmax = x1;
      }
      if(y1 > y2)                           
      {
         ymin = y2;
         ymax = y1;
      }
      if(fill)
      {
         for(i=xmin; i <= xmax; i++)
            GLCD_VERTICAL_LINE(i, ymin,ymax,color);
      }
      else
      {
         GLCD_VERTICAL_LINE(xmin, ymin,ymax, color);
         GLCD_VERTICAL_LINE(xmax, ymin,ymax, color);
         glcd_line(x1, y1, x2, y1, color);      // Draw the 4 sides
         glcd_line(x1, y2, x2, y2, color);
      }
      GLCD_UPDATE_COORDINATE(xmin,ymin, xmax,ymax);
      return OK;
}

result glcd_bar(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, unsigned int8 width, int1 color)
{
   unsigned int8         half_width;
   signed int16 dy, dx;
   signed int8  addx=1, addy=1, j;
   signed int16 P, diff, c1, c2;
   unsigned int8 i=0,xt1,yt1,xt2,yt2;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   half_width = width/2;
   c1 = -(dx*x1 + dy*y1);
   c2 = -(dx*x2 + dy*y2);  
   xt1=x1-half_width;
   xt2=x2+half_width;
   yt1=y1-half_width;
   yt2=y2+half_width;
   if(x1 > x2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addx = -1;
      xt1=x2-half_width;
      xt2=x1+half_width;  
   }
   if(y1 > y2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addy = -1;
      yt1=y2-half_width;
      yt2=y1+half_width;
   }
   if(xt1>191)  xt1=0;
   if(yt1>191)  yt1=0;
   if(xt2>191)  xt2=191;
   if(yt2>63)   yt2=63;
   GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
   if(dx >= dy)
   {
      P = 2*dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               GLCD_PIXEL_TEMP(x1, y1+j, color);
         }
         if(P < 0)
         {
            P  += 2*dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P  += 2*dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               GLCD_PIXEL_TEMP(x1+j, y1, color);
         }
      }
   }
   return OK;
}

result glcd_circle(unsigned int8 x, unsigned int8 y, unsigned int8 radius, int1 fill, int1 color)
{
   unsigned int8 xt1,xt2, yt1, yt2;
   signed int8  a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;
   do
   {
      if(fill)
      {
          GLCD_VERTICAL_LINE(x+b, y-a,y+a, color);
          GLCD_VERTICAL_LINE(x-b, y-a,y+a, color);
          GLCD_VERTICAL_LINE(x+a, y-b,y+b, color);
          GLCD_VERTICAL_LINE(x-a, y-b,y+b, color);
      }
      else
      {
         GLCD_PIXEL_TEMP(a+x, b+y, color);
         GLCD_PIXEL_TEMP(b+x, a+y, color);
         GLCD_PIXEL_TEMP(x-a, b+y, color);
         GLCD_PIXEL_TEMP(x-b, a+y, color);
         GLCD_PIXEL_TEMP(b+x, y-a, color);
         GLCD_PIXEL_TEMP(a+x, y-b, color);
         GLCD_PIXEL_TEMP(x-a, y-b, color);
         GLCD_PIXEL_TEMP(x-b, y-a, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
    xt1 = x-radius;
    xt2 = x+radius;
    yt1 = y-radius;
    yt2 = y+radius;
    if(xt1>191)   xt1=0;
    if(yt1>191)   yt1=0;
    if(xt2>191)   xt2=191;
    if(yt2>63)    yt2=63;
    GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
    return OK;
}
result glcd_triangle(signed int16 x1,signed int16 y1, signed int16 x2, signed int16 y2, signed int16 x3, signed int16 y3,int1 fill,int1 color)
{
    unsigned int8 xt1, xt2, yt11,yt22;
    signed int16 x[3],y[3],n,yt1,yt2,yt,dx1,dx2,dx3,dy1,dy2,dy3; 
    signed int16 sh1,sh2,sh3;
    x[0]=x1; x[1]=x2; x[2]=x3;y[0]=y1; y[1]=y2; y[2]=y3;
    if(x[0]>x[1]){yt=x[0];x[0]=x[1];x[1]=yt;yt=y[0];y[0]=y[1];y[1]=yt;}
    if(x[0]>x[2]){yt=x[0];x[0]=x[2];x[2]=yt;yt=y[0];y[0]=y[2];y[2]=yt;}
    if(x[1]>x[2]){yt=x[1];x[1]=x[2];x[2]=yt;yt=y[1];y[1]=y[2];y[2]=yt;}
    dx1= x[2]-x[0]; dx2= x[1]-x[0];dx3=x[2]-x[1];
    dy1= y[2]-y[0]; dy2= y[1]-y[0];dy3=y[2]-y[1];
    sh1=x[2]*y[0] - x[0]*y[2]; sh2=x[1]*y[0] - x[0]*y[1]; sh3=x[2]*y[1] - x[1]*y[2];
    if(fill)
    {
        for(n=x[0] ; n<x[1]; n++)
          {
              yt1= (dy1*n + sh1)/dx1;
              yt2= (dy2*n + sh2)/dx2;
              if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
              GLCD_VERTICAL_LINE(n,yt1,yt2,color);
          }
          for(n=x[1] ; n<x[2]; n++)
          {
              yt1= (dy1*n + sh1)/dx1;
              yt2= (dy3*n + sh3)/dx3;
              if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
              GLCD_VERTICAL_LINE(n,yt1,yt2,color);
          }  
    }
    else 
    {
         glcd_line(x1,y1,x2,y2,color);
         glcd_line(x1,y1,x3,y3,color);
         glcd_line(x2,y2,x3,y3,color);
    }
    xt1 = x[0];
    xt2 = x[2];
    if(y[0]>y[1])yt22= y[0]; else yt22 = y[1];
    if(yt22<y[2]) yt22 = y[2]; 
    if(y[0]<y[1]) yt11= y[0]; else yt11 = y[1];
    if(yt11>y[2])yt11 = y[2];
    GLCD_UPDATE_COORDINATE(xt1,yt11, xt2,yt22); 
    return OK;
}
result glcd_data(int8 data)
{
   output_high(glcd_rs);
   output_glcd(data);             
   output_high(glcd_e);     
   output_low(glcd_e);  
   delay_cycles(4);
   return OK;
}
result glcd_command(int8 data)
{
   output_low(glcd_rs);
   output_glcd(data);             
   output_high(glcd_e);     
   output_low(glcd_e);  
   delay_us(4);
   return OK;
}
result glcd_setup()
{
   output_low(GLCD_CSA);
   output_low(GLCD_CSB);
   output_low(GLCD_CSC);
   glcd_command(0x3F);
   glcd_clear(0);
   glcd_update();
   output_high(GLCD_CSA);
   output_high(GLCD_CSB);
   output_high(GLCD_CSC);
   GLCD.tt_text=1;
   GLCD.tt_update=0;
   return OK;
}
/**********************************************Board Thuc  Hanh******************************************/
#else
result GLCD_UPDATE_COORDINATE(unsigned int8 x1,unsigned int8 y1, unsigned int8 x2, unsigned int8 y2)
{
      if(GLCD.tt_update==0)
      {
            GLCD.x1 = x1;
            GLCD.x2 = x2;
            GLCD.y1 = y1;
            GLCD.y2 = y2;   
      }
      else
      {    
            if(x1<GLCD.x1) GLCD.x1=x1;
            if(x2>GLCD.x2) GLCD.x2=x2;
            if(y1<GLCD.y1) GLCD.y1=y1;
            if(y2>GLCD.y2) GLCD.y2=y2;   
      } 
      GLCD.tt_update=1;
      return OK;
}
result glcd_clear(int1 color)
{  
  int16 d,doc,ngang; 
  if(color)  d=0xffff; else d=0;
  for (DOC=0; DOC < GLCD_DOC; DOC++) 
  { 
    for (NGANG=0; NGANG < GLCD_NGANG; NGANG++) 
    { 
      GLCD.PIXEL[DOC][NGANG].word = d; 
    } 
  } 
   GLCD.x1 = 0;
   GLCD.x2 = 127;
   GLCD.y1 = 0;
   GLCD.y2 = 63;  
   GLCD.tt_update=1;
   return OK;
} 

result glcd_pixel(int8 x, int8 y, int1 color) 
{ 
  int8  b,doc,ngang; 
  if(y>31){x += 128; y-= 32;}; 
  DOC = y; 
  NGANG = x/16; 
  b = 15 - (x % 16); 
  
  if (color == 1) bit_set (GLCD.PIXEL[DOC][NGANG].word, b); 
  else bit_clear (GLCD.PIXEL[DOC][NGANG].word, b); 
  GLCD_UPDATE_COORDINATE(x,y, x, y);
  return OK;
} 
result GLCD_PIXEL_TEMP(int8 x, int8 y, int1 color) 
{ 
  int8  b,doc,ngang; 
  if(y>31){x += 128; y-= 32;}; 
  DOC = y; 
  NGANG = x/16; 
  b = 15 - (x % 16); 
  
  if (color == 1) bit_set (GLCD.PIXEL[DOC][NGANG].word, b); 
  else bit_clear (GLCD.PIXEL[DOC][NGANG].word, b); 
  return OK;
} 

result glcd_update ()
{
  int8 hesox,hesox2,hesoy,doc,ngang; 
 // glcd_rect(GLCD.x1,GLCD.y1, GLCD.x2, GLCD.y2, 0,1);  
  if(GLCD.x1>GLCD.x2) {hesox=GLCD.x1; GLCD.x1=GLCD.x2, GLCD.x2= hesox;}
  if(GLCD.y1>GLCD.y2) {hesoy=GLCD.y1; GLCD.y1=GLCD.y2, GLCD.y2= hesoy;}
  if (GLCD.tt_update) 
  { 
      GLCD.x1 = GLCD.x1/16; GLCD.x2=GLCD.x2/16;
      for (DOC = GLCD.y1; DOC<=GLCD.y2; DOC++) 
      { 
         if(DOC>31)   {hesox = 8+GLCD.x1;hesox2= GLCD.x2+8; hesoy = DOC-32;}
         else         {hesox = GLCD.x1  ;hesox2 = GLCD.x2 ; hesoy = DOC;   }
         GLCD_COMMAND( 0x80 |hesoy);   
         GLCD_COMMAND( 0x80 |hesox);  
      
         for (NGANG=hesox; NGANG <=hesox2; NGANG++) 
         { 
            GLCD_DATA( GLCD.PIXEL[hesoy][NGANG].nbyte[1]);   // Write High Byte. 
            GLCD_DATA( GLCD.PIXEL[hesoy][NGANG].nbyte[0]);   // Write Low Byte. 
         } 
       } 
    GLCD.tt_update = FALSE; 
  }  
  return OK;
}

result GLCD_VERTICAL_LINE(unsigned int8 x, unsigned int8 y1,unsigned int8 y2, int1 color)
{
   unsigned int8 i;
   for(i=y1;i<=y2;i++) glcd_pixel_temp(x, i, color) ;
   return OK;
}

result GLCD_HORIZONE_LINE(unsigned int8 x1, unsigned int8 x2,unsigned int8 y, int1 color)
{
  int8 doc;
  unsigned int8 i,right,left;
  if(y>31){x1 += 128;x2 += 128; y-= 32;}; 
  left=x1/16; right = x2/16;DOC = y; 
   if (left==right) 
   {
          if(color)
               GLCD.PIXEL[DOC][left].word= GLCD.PIXEL[DOC][left].word | (0xffff<<(15-(x2%16))) & (0xffff>>(x1%16));           
          else 
               GLCD.PIXEL[DOC][left].word= GLCD.PIXEL[DOC][left].word & (~((0xffff<<(15-(x2%16))) & (0xffff>>(x1%16))));   
   }
   else 
   {
          if(color)
          {    GLCD.PIXEL[DOC][right].word = GLCD.PIXEL[DOC][right].word | (0xffff<<(15-(x2%16)));
               GLCD.PIXEL[DOC][left].word = GLCD.PIXEL[DOC][left].word|(0xffff>>(x1%16));
               for(i = left + 1; i < right; i++) GLCD.PIXEL[DOC][i].word=0xffff;
          }
          else 
          {    
               GLCD.PIXEL[DOC][right].word = GLCD.PIXEL[DOC][right].word & (~(0xffff<<(15-(x2%16))));
               GLCD.PIXEL[DOC][left].word = GLCD.PIXEL[DOC][left].word& (~(0xffff>>(x1%16)));
               for(i = left + 1; i < right; i++) GLCD.PIXEL[DOC][i].word=0;
          } 
   }
   return OK;
}

result glcd_text_setup(unsigned int16 x, unsigned int16 y,int8 size, int8 color,int1 overlay)
{
      GLCD.mau = color;
      GLCD.size = size;
      GLCD.x    = x;
      GLCD.y    =y;
      GLCD.tt_text=0;
      GLCD.overlay=overlay;
      return OK;
}

result glcd_text( char t)
{
      static unsigned int8 x1,x2,y1,y2;
      unsigned int8 j, k, l, m;                      
      unsigned int8 pixelData[5];
      if(GLCD.overlay)
      {
            glcd_rect(GLCD.x,GLCD.y,GLCD.x+6*GLCD.size,GLCD.y+7*GLCD.size,1,!GLCD.mau);
      }
      if(GLCD.tt_text==0)
      {
          x1=GLCD.x;
          y1=GLCD.y;
          GLCD.tt_text=1;
      }
      if(t < 'S')
         memcpy(pixelData, GLCD_FONT[t - ' '], 5);
      else if(t <= '~') 
         memcpy(pixelData, GLCD_FONT2[t - 'S'], 5);
      else
         memcpy(pixelData, GLCD_FONT[0], 5);   
      if(GLCD.x+5*GLCD.size >= GLCD_WIDTH)         
      {
         GLCD.x = 0;                           
         GLCD.y += 7*GLCD.size + 1;                 
      }
      for(j=0; j<5; ++j, GLCD.x+=GLCD.size)         
      {
         for(k=0; k < 7; ++k)             
         {
            if(bit_test(pixelData[j], k)) 
            {
               for(l=0; l < GLCD.size; ++l)       
                    GLCD_horizone_line(GLCD.x, GLCD.x+GLCD.size-1,GLCD.y+k*GLCD.size+l,GLCD.mau);   
            }
         }
      }
      x2 = GLCD.x;
      GLCD.x+=GLCD.size;
      y2= GLCD.y + 7*GLCD.size;
      GLCD_UPDATE_COORDINATE(x1,y1, x2,y2);
      return OK;
 }
 
result glcd_spkt_logo_raw(int x) 
{ 
   unsigned int8 i=0, j=0, k=0,y=0; 
   unsigned int16 count=0; 
   GLCD_UPDATE_COORDINATE(x,0, x+64,63);
   for(j=0;j<64;j++) 
      {    
         for(;i<64;) 
         { 
            for(k=8;k>0;k--)
            {
               glcd_pixel_temp(i+x,j+y,bit_test(GLCD_Logo_DHSPKTTP[count],(k-1))); 
               i++; 
            } 
            count++; 
         } 
      i=0; 
      } 
      return OK;
} 

result glcd_line(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 color)
{
   unsigned int16        dy, dx;
   signed int8  addx=1, addy=1;
   signed int16 P, diff;
   unsigned int8 i=0,xt1,xt2,yt1,yt2;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   xt1=x1; xt2=x2;
   yt1=y1, yt2=y2;
   if(x1 > x2)
   {
      addx = -1;
      xt2=x1;
      xt1=x2;
   }
   if(y1 > y2)
   {
      addy = -1;
      yt2=y1;
      yt1=y2;
   }
   GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         glcd_pixel_temp(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         glcd_pixel_temp(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   return OK;
}

result glcd_rect(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, int1 fill, int1 color)
{
      unsigned int8  xmin, xmax, ymin, ymax,i;
      xmin = x1;
      xmax = x2;
      ymin = y1;
      ymax = y2;
      if(x1 > x2)                            
      {
         xmin = x2;
         xmax = x1;
      }
      if(y1 > y2)                           
      {
         ymin = y2;
         ymax = y1;
      }
      if(fill)
      {
         for(i=ymin; i <= ymax; i++)
            glcd_horizone_line(xmin, xmax,i, color);
      }
      else
      {
         glcd_vertical_line(xmin, ymin,ymax, color);
         glcd_vertical_line(xmax, ymin,ymax, color);
         glcd_horizone_line(x1, x2, y1, color);      // Draw the 4 sides
         glcd_horizone_line(x1, x2, y2, color);
      }
      GLCD_UPDATE_COORDINATE(xmin,ymin, xmax,ymax);
      return OK;
}

result glcd_bar(unsigned int8 x1, unsigned int8 y1, unsigned int8 x2, unsigned int8 y2, unsigned int8 width, int1 color)
{
   unsigned int8         half_width;
   signed int16 dy, dx;
   signed int8  addx=1, addy=1, j;
   signed int16 P, diff, c1, c2;
   unsigned int8 i=0,xt1,yt1,xt2,yt2;
   dx = abs((signed int8)(x2 - x1));
   dy = abs((signed int8)(y2 - y1));
   half_width = width/2;
   c1 = -(dx*x1 + dy*y1);
   c2 = -(dx*x2 + dy*y2);  
   xt1=x1-half_width;
   xt2=x2+half_width;
   yt1=y1-half_width;
   yt2=y2+half_width;
   if(x1 > x2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addx = -1;
      xt1=x2-half_width;
      xt2=x1+half_width;  
   }
   if(y1 > y2)
   {
      signed int16 temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addy = -1;
      yt1=y2-half_width;
      yt2=y1+half_width;
   }
   if(xt1>127)   xt1=0;
   if(yt1>127)   yt1=0;
   if(xt2>127)   xt2=127;
   if(yt2>63)    yt2=63;
   GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
   if(dx >= dy)
   {
      P = 2*dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               glcd_pixel_temp(x1, y1+j, color);
         }
         if(P < 0)
         {
            P  += 2*dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P  += 2*dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               glcd_pixel_temp(x1+j, y1, color);
         }
      }
   }
   return OK;
}

result glcd_circle(unsigned int8 x, unsigned int8 y, unsigned int8 radius, int1 fill, int1 color)
{
   unsigned int8 xt1,xt2, yt1, yt2;
   signed int8  a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;
   do
   {
      if(fill)
      {
         glcd_horizone_line(x-a, x+a, y+b, color);
         glcd_horizone_line(x-a, x+a, y-b, color);
         glcd_horizone_line(x-b, x+b, y+a, color);
         glcd_horizone_line(x-b, x+b, y-a, color);  
      }
      else
      {
         glcd_pixel_temp(a+x, b+y, color);
         glcd_pixel_temp(b+x, a+y, color);
         glcd_pixel_temp(x-a, b+y, color);
         glcd_pixel_temp(x-b, a+y, color);
         glcd_pixel_temp(b+x, y-a, color);
         glcd_pixel_temp(a+x, y-b, color);
         glcd_pixel_temp(x-a, y-b, color);
         glcd_pixel_temp(x-b, y-a, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
    xt1 = x-radius;
    xt2 = x+radius;
    yt1 = y-radius;
    yt2 = y+radius;
    if(xt1>127)   xt1=0;
    if(yt1>127)   yt1=0;
    if(xt2>127)   xt2=127;
    if(yt2>63)    yt2=63;
    GLCD_UPDATE_COORDINATE(xt1,yt1, xt2,yt2);
    return OK;
}
result glcd_triangle(signed int16 x1,signed int16 y1, signed int16 x2, signed int16 y2, signed int16 x3, signed int16 y3,int1 fill,int1 color)
{
    unsigned int8 xt1, xt2, yt11,yt22;
    signed int16 x[3],y[3],n,yt1,yt2,yt,dx1,dx2,dx3,dy1,dy2,dy3; 
    signed int16 sh1,sh2,sh3;
    x[0]=x1; x[1]=x2; x[2]=x3;y[0]=y1; y[1]=y2; y[2]=y3;
    if(x[0]>x[1]){yt=x[0];x[0]=x[1];x[1]=yt;yt=y[0];y[0]=y[1];y[1]=yt;}
    if(x[0]>x[2]){yt=x[0];x[0]=x[2];x[2]=yt;yt=y[0];y[0]=y[2];y[2]=yt;}
    if(x[1]>x[2]){yt=x[1];x[1]=x[2];x[2]=yt;yt=y[1];y[1]=y[2];y[2]=yt;}
    dx1= x[2]-x[0]; dx2= x[1]-x[0];dx3=x[2]-x[1];
    dy1= y[2]-y[0]; dy2= y[1]-y[0];dy3=y[2]-y[1];
    sh1=x[2]*y[0] - x[0]*y[2]; sh2=x[1]*y[0] - x[0]*y[1]; sh3=x[2]*y[1] - x[1]*y[2];
    if(fill)
    {
        for(n=x[0] ; n<x[1]; n++)
          {
              yt1= (dy1*n + sh1)/dx1;
              yt2= (dy2*n + sh2)/dx2;
              if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
              glcd_vertical_line(n,yt1,yt2,color);
          }
          for(n=x[1] ; n<x[2]; n++)
          {
              yt1= (dy1*n + sh1)/dx1;
              yt2= (dy3*n + sh3)/dx3;
              if(yt2<yt1){yt= yt1; yt1=yt2; yt2=yt;}
              glcd_vertical_line(n,yt1,yt2,color);
          }  
    }
    else 
    {
         glcd_line(x1,y1,x2,y2,color);
         glcd_line(x1,y1,x3,y3,color);
         glcd_line(x2,y2,x3,y3,color);
    }
    xt1 = x[0];
    xt2 = x[2];
    if(y[0]>y[1])yt22= y[0]; else yt22 = y[1];
    if(yt22<y[2]) yt22 = y[2]; 
    if(y[0]<y[1]) yt11= y[0]; else yt11 = y[1];
    if(yt11>y[2])yt11 = y[2];
    GLCD_UPDATE_COORDINATE(xt1,yt11, xt2,yt22);   
    return OK;
}
result glcd_data(int8 data)
{
      write_glcd128x64(0x17,data);
      write_glcd128x64(0x13,data);
      delay_us(9);
      return OK;
}
result glcd_command(int8 data)
{
      write_glcd128x64(0x07,data);
      write_glcd128x64(0x03,data);
      delay_us(2);
      return OK;
}
result glcd_setup()
{
      GLCD_COMMAND(0x30);    DELAY_mS(1);   
      GLCD_COMMAND(0x01);    DELAY_MS(10);
      GLCD_COMMAND(0x36);     
      glcd_clear(0);  
      glcd_update();
      GLCD.tt_update=0;
      GLCD.tt_text=0;
      return OK;
}
#endif
#endif
