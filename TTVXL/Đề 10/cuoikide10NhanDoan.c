#include <18F4550.h>
#device ADC = 10
#fuses HS
#use delay(clock=8MHz)
#define EXP_OUT_ENABLE_8L7D   PIN_D5       //RCK_8L7D
#define EXP_OUT_ENABLE_RLB    PIN_D3      //RCK_RLB
#define EXP_OUT_ENABLE_32led  PIN_D4     //RCK_32led
#define EXP_OUT_ENABLE_4L7D   PIN_D6    //RCK_4L7D
#define EXP_OUT_ENABLE_MTL    PIN_D7   //RCK_MA TRAN LED
#define EXP_OUT_ENABLE_LCD    PIN_D2  //RCK_LCD

#define EXP_OUT_CLOCK         PIN_E0   
#define EXP_OUT_DO            PIN_E2
#define G_ALL                 PIN_D0   //G_ALL

#define NUMBER_OF_74595_32led   4
#define NUMBER_OF_74595_8L7D 2
#define NUMBER_OF_74595_RLB  1
#define NUMBER_OF_74595_4L7D 4
#define NUMBER_OF_74595_MTL  4
#define NUMBER_OF_74595_LCD  4

#include <74595_LCD.c>          //LCD
#include <74595_MTL.c>         //MA TRAN LED
#include <74595_8L7D.c>       //8L7D
#include <74595_RLB.c>       //RLB - LOA
#include <74595_32leddon.c>       //32LEDDON
#include <74595_4L7D.c>    //4L7D


#define BTN0 PIN_B5
#define BTN1 PIN_B4
//                  '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'  '9'
const char M7D[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
                            0x88,0x83,0xC6,0xA1,0x86,0x8E};
unsigned int32 varCode_8L7D,varCode1_8L7D,varCode2_8L7D;
unsigned int32 varCode1_4L7D,varCode2_4L7D,varCode3_4L7D,varCode4_4L7D,varCode_4L7D;
unsigned int8 varCode_RLB;
unsigned int32 varCode_MTL;
unsigned int32 varCode_32;
unsigned int16 varCode_LCD;

signed int8 k,i,SP;
unsigned int8 trangthai;
unsigned int16 quangsensor;

void Tat_8L7D(){
varCode1_8L7D=(~OFFM7D[0])&0x00FF;  
varCode2_8L7D=~0b0000000011111110; 
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;   //h
write_expanded_outputs_8L7D(&varCode_8L7D);


varCode1_8L7D=(~OFFM7D[0])&0x00FF; 
varCode2_8L7D=~0b00000000111111101; //g
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011111011; //f
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011110111; // e
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011101111; // d
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF;  
varCode2_8L7D=~0b0000000011011111; // c
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000010111111; // b
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFM7D[0])&0x00FF;
varCode2_8L7D=~0b0000000001111111; // a  
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);
}
void Tat_LCD(){
varCode_LCD=0;   //TAT LCD
write_expanded_outputs_LCD(0);
}
void Tat_32(){
varCode_32= 0;    //tat 32led don
write_expanded_outputs_32led(&varCode_32);  
}
void Tat_MTL(){
varCode_MTL =0;    //TAT MATRANLED
write_expanded_outputs_MTL(&varCode_MTL);
}
void Tat_4L7D(){
varCode1_4L7D = 0xff;  //A
varCode2_4L7D = 0xff;  //B
varCode3_4L7D = 0xff;  //C
varCode4_4L7D = 0xff;  //D
varCode_4L7D = varCode1_4L7D|varCode2_4L7D<<8|varCode3_4L7D<<16|varCode4_4L7D<<24;   // D C B A THEO THU TU NHA
write_expanded_outputs_4L7D(&varCode_4L7D);
}

void Tat_RLB(){
varCode_RLB=0b01010000;
output_low(G_ALL);
write_expanded_outputs_RLB(&varCode_RLB);
delay_ms(50);
}
void Bat_RLB(){
varCode_RLB=0b01010001;
output_low(G_ALL);
write_expanded_outputs_RLB(&varCode_RLB);
delay_ms(50);
}

void main ()
{
output_low(G_ALL);

Tat_4L7D(); 
Tat_8L7D();  
Tat_LCD();  
Tat_32();
Tat_MTL();
Tat_RLB();

//****************************QUANG SENSOR
SETUP_ADC(ADC_CLOCK_internal);
SETUP_ADC_PORTS(AN0_TO_AN2 | VSS_VDD); 
set_adc_channel(2);
quangsensor=0;



//****************************COUNTER
SP=0;
SETUP_TIMER_0(T0_EXT_L_TO_H|T0_DIV_1|t0_8_bit);
SET_TIMER0(0);


while(TRUE)
{quangsensor= READ_ADC();//      QUANG SENSOR

if(quangsensor<300) trangthai=1;
else trangthai=0;

 if (trangthai == 1){    //toi
for(k=5; k>-1 ; k--){
Bat_RLB();
varCode4_4L7D =0xff; //D0-D7          nghin
varCode3_4L7D =0xff; //C0-C7   tram  
varCode2_4L7D =0xff; //B0-B7      chuc               
varCode1_4L7D =M7D[k]; //A0-A7         donvi
varCode_4L7D=varCode1_4L7D|(varCode2_4L7D<<8)|(varCode3_4L7D<<16)|(varCode4_4L7D<<24);  //varCode 4 LED7DOAN
write_expanded_outputs_4L7D(&varCode_4L7D);
delay_ms(100);
}}
  
   
   
if (trangthai == 0){   //sang
varCode4_4L7D =0xff; //D0-D7          nghin
varCode3_4L7D =0xff ; //C0-C7   tram  
varCode2_4L7D =0xff; //B0-B7      chuc               
varCode1_4L7D =0xff; //A0-A7         donvi
varCode_4L7D=varCode1_4L7D|(varCode2_4L7D<<8)|(varCode3_4L7D<<16)|(varCode4_4L7D<<24);  //varCode 4 LED7DOAN
write_expanded_outputs_4L7D(&varCode_4L7D);
Tat_RLB();
   }
   
//xuat8l7d   
for (int x=0;x<60/* <===== chinh toc do tai day nha*/;x++)  {
  Xuat8L7D();   //xuat 1 lan la 10ms
  }



//************************COUNTER0*********************  
SP=GET_TIMER0();   
varCode4_4L7D =M7D[SP/1000]; //D0-D7          nghin
varCode3_4L7D =M7D[(SP%1000)/100] ; //C0-C7   tram  
varCode2_4L7D =M7D[(SP%100)/10]; //B0-B7      chuc               
varCode1_4L7D =M7D[SP%10]; //A0-A7         donvi
varCode_4L7D=varCode4_4L7D|(varCode3_4L7D<<8)|(varCode2_4L7D<<16)|(varCode1_4L7D<<24);  //varCode 4 LED7DOAN
write_expanded_outputs_4L7D(&varCode_4L7D);
if(SP>15)  set_timer0(1);   

   
}
}



