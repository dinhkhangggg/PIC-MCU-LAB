#include <18F4550.h>
#device adc=10
#fuses HS
#use delay(clock=8MHz)
#define EXP_OUT_ENABLE_4l7d      PIN_D6  
#define EXP_OUT_ENABLE_RLB       PIN_D3
#define EXP_OUT_ENABLE_MTL       PIN_D7
#define EXP_OUT_ENABLE_LCD       PIN_D2
#define EXP_OUT_ENABLE_32led     PIN_D4
#define EXP_OUT_ENABLE_8L7D  PIN_D5
#define EXP_OUT_CLOCK   PIN_E0   //SSCK
#define EXP_OUT_DO      PIN_E2   //SSDO

#define SET   PIN_B5 //SET PV [0..99]
#define CLEAR PIN_B4//CLEAR PV VE 0

#define G_ALL           PIN_D0   //G_ALL

#define NUMBER_OF_74595_4l7d      4
#define NUMBER_OF_74595_32led     4
#define NUMBER_OF_74595_RLB       1
#define NUMBER_OF_74595_MTL       2
#define NUMBER_OF_74595_LCD       2
#define NUMBER_OF_74595_8L7D      2
#include <74595_4l7d.c>
#include <74595_RLB.c>
#include <74595_MTL.c>
#include <74595_LCD.c>
#include <74595_32leddon.c>
#include <74595_8L7D.c>

const char ma7doan[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E, 0x7f, 0x1c};
unsigned int32 varSeg, varSeg1, varSeg2, varSeg3, varSeg4,varCodeMTL,varCodeLCD,varCode32leddon;
unsigned int32 varCodeRLB1,varCodeRLB2;
unsigned int8 CV, PV;

void ALARM(){  
   varCodeRLB1=0b10000000;
   write_expanded_outputs_RLB(&varCodeRLB1);
   delay_ms(50);
   varCodeRLB2=0b00000000;
   write_expanded_outputs_RLB(&varCodeRLB2);
}


void hienthi_PV_CV(){
      varSeg1 = ma7doan[PV / 10];//SET TEMP
      varSeg2 = ma7doan[PV % 10];
      varSeg3 = ma7doan[CV / 10];//CHI THI NHIET DO
      varSeg4 = ma7doan[CV % 10];
      varSeg = varSeg1|varSeg2<<8|varSeg3<<16|varSeg4<<24;
      write_expanded_outputs_4l7d(&varSeg);
      delay_ms(100);
}

void SET_BUTTON(){
     if(!input(SET)){
       delay_ms(20);
       if(!input(SET)){
          while(!input(SET));
          if(PV==99) PV=00;
          else PV++;          
     }
     }
     }
     
void CLEAR_BUTTON(){
     if(!input(CLEAR)){
      while(!input(CLEAR));
      PV=0;//SET GIA TRI DAT NHIET DO BANG 0
     }
     }
     

void main(){
   varCode32leddon=0;
      output_low(G_ALL);
      write_expanded_outputs_32led(&varCode32leddon);

      output_low(G_ALL);
      write_expanded_outputs_8L7D(&varCode32leddon);

   varCodeMTL=0;
      output_low(G_ALL);
      write_expanded_outputs_MTL(&varCodeMTL);

   varCodeLCD=0;
      output_low(G_ALL);
      write_expanded_outputs_LCD(&varCodeLCD);

   varCodeRLB2=0b00000000;
   write_expanded_outputs_rlb(&varCodeRLB2);
     output_low(G_ALL);
     
     
     setup_adc(adc_clock_div_2);
     setup_adc_ports(an0 | vss_vdd);   
  PV = read_eeprom(0x02);
     
     while(true){
      set_adc_channel(0);
      delay_ms(10);
      CV = read_adc() / 2.046;
          hienthi_PV_CV();
          if(CV > PV) ALARM();
          if(CV==PV) ALARM();
          SET_BUTTON();
          CLEAR_BUTTON();
        write_eeprom(0x02, PV);
     }
}
