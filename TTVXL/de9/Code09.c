// T=2ms =>PR2=249 => maxduty = 1000 moi cap = 100
#include <18F4550.h>
#fuses intrc_io
#use delay(clock=8M)  

#define EXP_OUT_ENABLE_LCD         PIN_D2  //RCK_LCD
#define EXP_OUT_ENABLE_RLB         PIN_D3  //RCK_RLB
#define EXP_OUT_ENABLE_DCMOTOR     PIN_D3 
#define EXP_OUT_ENABLE_32led       PIN_D4  //RCK_32LED
#define EXP_OUT_ENABLE_8L7D        PIN_D5  //RCK_8L7D
#define EXP_OUT_ENABLE_4l7d        PIN_D6  //RCK_4L7D
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

#define ffrev pin_b5
#define stop   pin_b4
#define level  pin_b3

const char ma7doan[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
unsigned int16 speed = 0;
int1 tt = 0;
unsigned int32 varCode, varSeg, varSeg1, varSeg2, varSeg3, varSeg4;

void display_speed() {
    varSeg1 = ma7doan[speed % 10];
    varSeg2 = 0xff;
    varSeg3 = 0xff;
    varSeg4 = 0xff;
    varSeg = varSeg1|varSeg2<<8|varSeg3<<16|varSeg4<<24;
    write_expanded_outputs_4l7d(&varSeg);
}

void phim_level() {
    if (!input(level)) {
        delay_ms(20);
        if (!input(level)) {
            while (!input(level));
            speed++;
            if (speed > 9) speed = 0;
            display_speed();
            //speed = read_eeprom(0x02);
        }
    }
}

void phim_ff_rev() {
    phim_level();

    if (!input(ffrev)) {
        delay_ms(20);
        if (!input(ffrev)) {
            while (!input(ffrev));
            tt = ~tt;
        }
    }

    if (tt == 1) {
        setup_ccp1(CCP_PWM);
        set_pwm1_duty((int16)speed * 100);
        setup_ccp2(CCP_OFF);
        output_low(PIN_C2);
        varCode = 0x01;
        write_expanded_outputs_32led(&varCode);
    } else {
        setup_ccp2(CCP_PWM);
        set_pwm2_duty((int16)speed * 100);
        setup_ccp1(CCP_OFF);
        output_low(PIN_C1);
        varCode = 0x02;
        write_expanded_outputs_32led(&varCode);
    }
}

void phim_stop() {
    if (!input(stop)) {
        delay_ms(20);
        if (!input(stop)) {
            while (!input(stop));
            setup_ccp1(CCP_OFF);
            output_low(PIN_C1);
            setup_ccp2(CCP_OFF);
            output_low(PIN_C2);
            varCode = 0x00; 
            write_expanded_outputs_32led(&varCode);
            varSeg = 0xFFFFFFFF; 
            write_expanded_outputs_4l7d(&varSeg);
            speed = 0; 
            display_speed();
        }
    }
}

void main() {
    varCode = 0;
    output_low(G_ALL);
    write_expanded_outputs_32led(&varCode);
    write_expanded_outputs_4l7d(&varCode);
    write_expanded_outputs_MTL(&varCode); 
    write_expanded_outputs_LCD(&varCode);
    
    varCode= 0b11111111;
    write_expanded_outputs_DCMOTOR(&varCode);
    
    varSeg1 = 0xff;
    varSeg2 = 0xff;
    varSeg3 = 0xff;
    varSeg4 = 0xff;
    varSeg = varSeg1|varSeg2<<8|varSeg3<<16|varSeg4<<24;
    write_expanded_outputs_4l7d(&varSeg);

    set_tris_b(0x3C);
    port_b_pullups(0xFF);
    output_c(0);

    setup_timer_2(T2_DIV_BY_16, 249, 1);
    set_timer2(0);

    setup_ccp1(CCP_OFF);
    output_low(PIN_C2);
    setup_ccp2(CCP_OFF);
    output_low(PIN_C1);
    
    varSeg1 = read_eeprom(0x02);

    while (true) {
        phim_ff_rev();
        phim_stop();
        write_eeprom(0x02, varSeg1);
    }
}
