#include <18F4550.h>
#fuses intrc_io
#use delay(clock=8M)  

#define EXP_OUT_ENABLE_LCD         PIN_D2  //RCK_LCD
#define EXP_OUT_ENABLE_RLB         PIN_D3  //RCK_RLB
#define EXP_OUT_ENABLE_DCMOTOR     PIN_D3 
#define EXP_OUT_ENABLE_32led    PIN_D4  //RCK_32LED
#define EXP_OUT_ENABLE_8L7D        PIN_D5  //RCK_8L7D
#define EXP_OUT_ENABLE_4l7d   PIN_D6  //RCK_4L7D
#define EXP_OUT_ENABLE_MTL         PIN_D7  //RCK_MTL
#define EXP_OUT_CLOCK              PIN_E0  //SSCK
#define EXP_OUT_DO                 PIN_E2  //SSDO
#define G_ALL                      PIN_D0  //G_ALL

#define NUMBER_OF_74595_8L7D 2
#define NUMBER_OF_74595_32led 4
#define NUMBER_OF_74595_4l7d  4
#define NUMBER_OF_74595_RLB  1
#define NUMBER_OF_74595_MTL  1
#define NUMBER_OF_74595_LCD  1
#define NUMBER_OF_74595_DCMOTOR 1

#include <74595_8L7D.c>
#include <74595_RLB.c>
#include <74595_4l7d.c>
#include <74595_MTL.c>
#include <74595_LCD.c>
#include <74595_32leddon.c>
#include <74595_DCMOTOR.c>

const char ma7doan[17]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,0xFF};
unsigned int16 as;
unsigned int32 varCode,varCode1,varCode2,varCode3,varCode4,varCode0, varCodeRLB1, varCodeRLB2;
unsigned int8 time;

void hienthi_4led7doan(unsigned int8 time){
  varCode4=ma7doan[time];
  varCode3=ma7doan[16];
  varCode2=ma7doan[16];
  varCode1=ma7doan[16];
  varCode=varCode1|(varCode2<<8)|(varCode3<<16)|(varCode4<<24);
  write_expanded_outputs_4l7d(&varCode);
  delay_ms(1);
}

void main(){
     varCode = 0;
     output_low(G_ALL);
     write_expanded_outputs_32led(&varCode);
     write_expanded_outputs_4l7d(&varCode);
     write_expanded_outputs_MTL(&varCode); 
     write_expanded_outputs_LCD(&varCode);
     
     setup_adc(adc_clock_div_32);        
     setup_adc_ports(an0_to_an2|vss_vdd); 
     output_low(g_all); 

     
     setup_adc(adc_clock_div_2);
     setup_adc_ports(an0_to_an2|vss_vdd);
     
     time=5;
     hienthi_4led7doan(time);
     
  while(true){
        set_adc_channel(2);
        delay_ms(10);
        as = read_adc() / 2.046;
        if(as >= 50){
           for (time = 5; time >= 0; time--) {
                varCodeRLB1 = 0b11010000;  
                write_expanded_outputs_rlb(&varCodeRLB1);

                hienthi_4led7doan(time);  
                delay_ms(1000);          
        }
        
        varCodeRLB2 = 0b01010000;  
        write_expanded_outputs_rlb(&varCodeRLB2);
    }
  
    else{ 
    varCode=0b01010000; write_expanded_outputs_RLB(&varCode);
    }
  }
}
  
