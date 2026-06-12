#include <18F4550.h>
#use delay(clock=8MHz)
#fuses HS

#define EXP_OUT_ENABLE_4l7d  PIN_D6  //RCK_4L7D
#define EXP_OUT_CLOCK             PIN_E0   //SSCK
#define EXP_OUT_DO                PIN_E2   //SSDO
#define G_ALL                     PIN_D0   //G_ALL

#define SET       PIN_B5
#define CLEAR     PIN_B4

#define NUMBER_OF_74595_4l7d  4
#include <74595_4l7d.c>

#define EEPROM_PV_ADDR  0x00  // Dia chi de luu PV
#define EEPROM_CV_ADDR  0x01  // Dia chi de luu CV
#define EEPROM_BOX_ADDR 0x02  // Dia chi de luu BOX

unsigned int32 varCode, varCode1, varCode2, varCode3, varCode4;
const unsigned int8 aryCode7Seg[17] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xFF};

unsigned int8 PV = 0, CV, BOX = 0;



void hienthi() {
    varCode4 = aryCode7Seg[BOX % 10]; // SO THUNG: HANG DON VI 
    varCode3 = aryCode7Seg[BOX / 10]; // SO THUNG: HANG CHUC
    varCode2 = aryCode7Seg[CV];       // GIA TRI DEM 0-9
    varCode1 = aryCode7Seg[PV];       // GIA TRI DAT 0-9
    varCode = varCode1 | (varCode2 << 8) | (varCode3 << 16) | (varCode4 << 24);
    write_expanded_outputs_4l7d(&varCode);
    delay_ms(10);
}

void kiemtrabutton() {
    if (input(SET) == 0) {
        delay_ms(5);
        if (input(SET) == 0) {
            while (input(SET) == 0) {}
            PV++;
            write_eeprom(EEPROM_PV_ADDR, PV); 
        }
    }
    if (input(CLEAR) == 0) {
        delay_ms(5);
        if (input(CLEAR) == 0) {
            while (input(CLEAR) == 0) {}
            PV = 0;
            CV = 0;
            BOX = 0;
            write_eeprom(EEPROM_PV_ADDR, PV);   
            write_eeprom(EEPROM_BOX_ADDR, BOX); 
        }
    }
}

void main() {
    output_low(G_ALL);

   
    PV = read_eeprom(EEPROM_PV_ADDR);
    BOX = read_eeprom(EEPROM_BOX_ADDR);

    setup_timer_0(t0_ext_l_to_h | t0_div_1 | t0_8_bit);
    set_timer0(0);

    while (true) {
        kiemtrabutton();
        if (PV > 9) PV = 0;
        CV = get_timer0();
        hienthi();
        if (CV > PV) {
            set_timer0(0);
            BOX++;
            write_eeprom(EEPROM_BOX_ADDR, BOX); 
        }
    }
}
