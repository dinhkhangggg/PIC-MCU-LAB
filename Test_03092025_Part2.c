#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE        PIN_B3   //MBI_LE0
#define EXP_OUT_CLOCK         PIN_D6   //MBI_SCK
#define EXP_OUT_DO            PIN_D4   //MBI_SDI

#define G_ALL                 PIN_C5   //MBI_OE

#define NUMBER_OF_74595       4
#include <74595.c>

unsigned int8 varCode[4];
unsigned int8 x0 =0, x1=0, x2=0, x3=0;

void main() 
{
output_low(G_ALL);

   while(TRUE)
   {
 
      for (int8 i =0; i<8;i++)
      {
         x0 = (x0 << 1) + 0x01;
         x3 = (x3 >> 1) + 0x80;
         varCode[3]= x3;
         varCode[0] = x0;
      
         write_expanded_outputs(varCode);
         delay_ms(300);
      }  

      for (int8 j = 0; j<8;j++)
      {
         x1 = (x1 << 1) + 0x01;
         x2 = (x2 >> 1) + 0x80;
         varCode[2]= x2;
         varCode[1]= x1;
         varCode[3]= x3;
         varCode[0] = x0;
      
         write_expanded_outputs(varCode);
         delay_ms(300);
      }

      for (int8 k = 0; k<8;k++)
      {
         x1 = (x1 >> 1);
         x2 = (x2 << 1);
         varCode[2]= x2;
         varCode[1]= x1;
         varCode[3]= x3;
         varCode[0] = x0;
      
         write_expanded_outputs(varCode);
         delay_ms(300);
      }

      for (int8 l =0; l<8;l++)
      {
         x0 = (x0 >> 1);
         x3 = (x3 << 1);
         varCode[3]= x3;
         varCode[0] = x0;
      
         write_expanded_outputs(varCode);
         delay_ms(300);
      }

}
}
