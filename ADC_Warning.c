#include <18F6722.h> 
#device adc = 10
#fuses HS,NOWDT,NOPROTECT,NOLVP,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER  PIN_F4
#define EXP_OUT_CLOCK          PIN_D6
#define EXP_OUT_DO             PIN_D4
#define G_ALL                  PIN_C5
#define EXP_OUT_ENABLE_32LED   PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN PIN_F6

#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs  4
#define NUMBER_OF_74595_MA7DOAN 8

#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

// M? LED 7 o?n cho cathode chung (09)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};

#define CHAR_C   0x39   // k? t? C
#define CHAR_DEG 0x63   // k? t? 
#define CHAR_A   0x77   // k? t? A
#define CHAR_L   0x38   // k? t? L

#define CHAR_r   0x50   // r
#define CHAR_m   0x37   // m (gi? l?p g?n ng)

unsigned int8 varCode[8];
unsigned int8 varCode0[2];

#define TEMP_LIMIT 34   // Ng?ng c?nh bo C

void main()
{
   int16 adc_value;
   int16 tempC;
   int8 tens, unit;
   int blink = 0;

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   output_low(G_ALL);
   varCode0[0]=0;
   varCode0[1]=0;

   while(TRUE)
   {
      delay_ms(10);
      adc_value = read_adc();                 
      tempC = (adc_value) / 2.046;            // ?i sang C (Vref=5V)

      // Xo ton b? LED tr?c
      for(int i=0;i<8;i++)
         varCode[i]=0x00;

      if(tempC >= TEMP_LIMIT) 
      {
         // ==== Qu nhi?t: LED + buzzer nh?p nhy cng nhau ====
         if(blink==0) 
         {
            //varCode[0] = CHAR_m;   // LED0: M
            //varCode[1] = CHAR_r;   // LED1: r
            //varCode[2] = CHAR_A;   // LED2: A
            varCode[2] = Char_L  ; // LED3: S
            //VarCode[3] = 0x5c;     //O
            varCode[4] = Char_A ;
            
            
            // LED4: A
            varCode0[0] = 0xFF;    // buzzer b?t
            varCode0[1] = 0xFF;
         } 
         else 
         {
            varCode[2] = 0x00;     // LED t?t
            varCode[3] = 0x00;
            varCode0[0] = 0x00;    // buzzer t?t
            varCode0[1] = 0x00;
         }
         blink ^= 1;  // ?i tr?ng thi nh?p nhy
      }
      else 
      {
         // ==== B?nh th?ng: hi?n th? nhi?t ? ====
         varCode0[0]=0;      // buzzer off
         varCode0[1]=0;

         tens = tempC / 10;
         unit = tempC % 10;

         varCode[2] = array[unit];  // LED0: n v?
         varCode[3] = array[tens];  // LED1: ch?c
         varCode[0] = CHAR_C;       // LED2: C
         varCode[1] = CHAR_DEG;     // LED3: 
      }

      // Xu?t ra LED + buzzer
      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);
      //if (tempc>36)
      //{
      delay_ms(50); 
      //}
      //else
      //delay_ms(300);// t?c ? nh?p nhy/ku
   }
}

