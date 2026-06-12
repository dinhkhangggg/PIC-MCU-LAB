#include <18F4550.h>
#use delay(clock=8MHz)
#fuses HS
#define EXP_OUT_ENABLE_32leddon    PIN_D4   //RCK_32LED
#define EXP_OUT_ENABLE_4l7D   PIN_D6   //RCK_4L7D
#define EXP_OUT_ENABLE_LCD     PIN_D2   //RCK_LCD20x4
#define EXP_OUT_ENABLE_MTL   PIN_D7   //RCK_MATRANLED
#define EXP_OUT_ENABLE_8L7D        PIN_D5   // RCK_8L7D
#define EXP_OUT_ENABLE_RLB         PIN_D3   //RCK_RLB
#define EXP_OUT_CLOCK              PIN_E0   //SSCK
#define EXP_OUT_DO                 PIN_E2   //SSDO
#define G_ALL                      PIN_D0   //G_ALL

#define NUMBER_OF_74595_32leddon 4
#define NUMBER_OF_74595_4l7D  4
#define NUMBER_OF_74595_RLB  1
#define NUMBER_OF_74595_8L7D 2
#define NUMBER_OF_74595_LCD20x4 4
#define NUMBER_OF_74595_MTL 8

#include <74595_MTL.c>
#include <74595_LCD.c>
#include <74595_32leddon.c>
#include <74595_4l7D.c>
#include <74595_RLB.c>
#include <74595_8L7D.c> 

#define EEPROM_MIN_ADDR     0x00
#define EEPROM_RANDOM_ADDR  0x01
#define EEPROM_MAX_ADDR     0x02

#define BUTTON                    PIN_B5
//#define CLEAR     PIN_B4
//#define BT2 PIN_B3
//#define BT3 PIN_B2



unsigned int8 i, run=1;
int1 tt;
//unsigned int16 varCount;
unsigned int32 varCode,varCode1,varCode2,varCode3,varCode4;
const unsigned int8 aryCode7Seg[17]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,0xFF};
const unsigned int8 aryRandom[10]= {6,3,4,6,2,8,9,1,5,7};

unsigned int8 MIN,RANDOM, MAX;

void hienthi()
{
  varCode4=aryCode7Seg[16];         // TAT LED
  varCode3=aryCode7Seg[MAX];        // SO LON NHATTUNG XUAT HIEN
  varCode2=aryCode7Seg[RANDOM];     // GIA TRI NGAU NHIEN

  varCode1=aryCode7Seg[MIN];        // SO BE NHAT TUNG XUAT HIEN
  varCode=varCode1|(varCode2<<8)|(varCode3<<16)|(varCode4<<24);
  write_expanded_outputs_4l7D(&varCode);
  delay_ms(10);
}

void check_button()
{
  
   if (input(BUTTON) == 0)
   {
      delay_ms(20);                   
      if (input(BUTTON) == 0 && tt == 1) 
      {
         run = !run;        
         tt = 0;       
         while (input(BUTTON) == 0) {}
      }
   }
   else
   {
      tt = 1; 
   }
}
void init_values_from_eeprom()
{
    MIN = read_eeprom(EEPROM_MIN_ADDR);
    RANDOM = read_eeprom(EEPROM_RANDOM_ADDR);
    MAX = read_eeprom(EEPROM_MAX_ADDR);

   
    if (MIN > 9) MIN = aryRandom[0]; 
    if (MAX > 9) MAX = aryRandom[0];
    if (RANDOM > 9) RANDOM = aryRandom[0];
}
void save_values_to_eeprom()
{
    write_eeprom(EEPROM_MIN_ADDR, MIN);
    write_eeprom(EEPROM_RANDOM_ADDR, RANDOM);
    write_eeprom(EEPROM_MAX_ADDR, MAX);
}




void main()
{
    output_low(G_ALL);
    output_low(EXP_OUT_ENABLE_LCD);
    int lmt = 0x0;
    write_expanded_outputs_MTL(&lmt);  
    write_expanded_outputs_32led(0x00000000);
    write_expanded_outputs_8L7D(0x11111111);

 
    init_values_from_eeprom();

    while (true)
    {
        check_button();
        if (run)
        {
            for (i = 0; i < 10; i++)
            {
                RANDOM = aryRandom[i];
                if (RANDOM < MIN)
                {
                    MIN = RANDOM;
                    save_values_to_eeprom(); 
                }
                if (RANDOM > MAX)
                {
                    MAX = RANDOM;
                    save_values_to_eeprom();
                }
                hienthi();
                delay_ms(1000);
                check_button();
                if (!run) break;
            }
        }
    }
}

