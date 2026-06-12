#define   BOARD     D501
#include<tv_boards.c> 
result saobang(int8 dau, int8 dai, int8 ghphai,int8 ghtrai, int1 chieu)
{
      unsigned int32 tam,phai,trai;
      unsigned int8 a,dl;
      tam  = (int32)1<<dau;
      phai = (int32)1<<ghphai;
      trai = (int32)1<<ghtrai;
      for(a=1;a<=dai;a++)
      {     
            led32.ledx32|=tam;
            for(dl=0;dl<=(dai-a);dl++)led32_display();
            if(tam==phai){tam=phai<<1; chieu=-!chieu;}
            else if(tam==trai){tam=trai>>1; chieu=-!chieu;}
            else
            {
               if(chieu) tam>>=1;
               else      tam<<=1;  
            }    
      }
      led32.ledx32=0; led32_display();
      return OK;
}
result ziczac_saobang()
{
      unsigned int8 i=0,j=0,k,n,m,h;
      for(n=i;n<(32-j);n++)
      {
           for(k=0;k<2;k++)
           {
               for(m=n;m<(31-j);m++)
               {
                     for(h=0;h<25;h++)
                     {
                       led32.ledx32 = (0xffffffff<<(32-i))|(0xffffffff>>(32-j));
                       if(k==0)saobang(m   , 8, j,32-i,1);
                       else    saobang(31-m, 8, j,32-i,0);
                     }     
               }
               if(k==0) i++;
               else     j++;
              }
      }
      return OK;
}
void main()
{
      system_init(); 
      while(true)
      {     
         ziczac_saobang();
      }
}
