#include <18F4550.h>
#device  ADC = 10
#fuses  HS
#use    delay(clock=8MHz)
#define EXP_OUT_ENABLE_8L7D   PIN_D5       //RCK_8L7D
#define EXP_OUT_ENABLE_RLB    PIN_D3      //RCK_RLB
#define EXP_OUT_ENABLE_32led     PIN_D4     //RCK_32led
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

#define SET PIN_B5   // BTN0
#define CLEAR PIN_B4 // BTN1

const char M7D[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
                            0x88,0x83,0xC6,0xA1,0x86,0x8E};
const char OFFL7D[] = {0XFF};
unsigned int32 varCode_8L7D,varCode1_8L7D,varCode2_8L7D;
unsigned int32 varCode1_4L7D,varCode2_4L7D,varCode3_4L7D,varCode4_4L7D,varCode_4L7D;
unsigned int32 varCode_MTL;
unsigned int32 varCode_32;
unsigned int16 varCode_LCD;
unsigned int32 varCodeRLB1,varCodeRLB2;

unsigned int8 CV, PV;



void Tat_8L7D(){
varCode1_8L7D=(~OFFL7D[0])&0x00FF;  
varCode2_8L7D=~0b0000000011111110; 
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;   //h
write_expanded_outputs_8L7D(&varCode_8L7D);


varCode1_8L7D=(~OFFL7D[0])&0x00FF; 
varCode2_8L7D=~0b00000000111111101; //g
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011111011; //f
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011110111; // e
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000011101111; // d
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF;  
varCode2_8L7D=~0b0000000011011111; // c
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF; 
varCode2_8L7D=~0b0000000010111111; // b
varCode_8L7D=varCode2_8L7D<<8|varCode1_8L7D;
write_expanded_outputs_8L7D(&varCode_8L7D);

varCode1_8L7D=(~OFFL7D[0])&0x00FF;
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




void ALARM(){  
   varCodeRLB1=0b10000000;
   write_expanded_outputs_RLB(&varCodeRLB1);
   delay_ms(50);
   varCodeRLB2=0b00000000;
   write_expanded_outputs_RLB(&varCodeRLB2);
}
void XUAT_PV_CV(){
      varCode1_4L7D = M7D[PV / 10];//SET CANHBAO
      varCode2_4L7D = M7D[PV % 10];
      varCode3_4L7D = M7D[CV / 10];//CHI THI NHIET DO
      varCode4_4L7D = M7D[CV % 10];
      varCode_4L7D = varCode1_4L7D|varCode2_4L7D<<8|varCode3_4L7D<<16|varCode4_4L7D<<24;
      write_expanded_outputs_4L7D(&varCode_4L7D);   
      delay_ms(100);  
}
void SET_BUTTON(){
     if(!input(SET)){
       delay_ms(5);
       if(!input(SET)){
          while(!input(SET));
          if(PV==99) PV=00;
          else PV++;  }   }     
}    
void CLEAR_BUTTON(){
     if(!input(CLEAR)){
      while(!input(CLEAR));
      PV=0;   //SET GIA TRI CANHBAO BANG 0
     }
     }
     
void main(){  
output_low(G_ALL);
Tat_8L7D();  
Tat_LCD();  
Tat_32();
Tat_MTL();

setup_adc(adc_clock_div_2);
setup_adc_ports(an0 | vss_vdd);   
PV = read_eeprom(0xFF);
     
while(true){
set_adc_channel(0);
delay_ms(10);
CV = read_adc() / 2.046;
XUAT_PV_CV();

if(PV < CV) ALARM();
if(PV==CV) ALARM();
SET_BUTTON();
CLEAR_BUTTON();
write_eeprom(0xFF, PV);
}
}


