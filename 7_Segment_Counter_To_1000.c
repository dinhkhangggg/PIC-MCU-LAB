

#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

// --- Các d?nh nghia chân (gi? nguyên nhu c?a b?n) ---
//#define EXP_OUT_ENABLE_B        PIN_F4      //MBI_LE0
#define EXP_OUT_CLOCK           PIN_D6      //MBI_SCK
#define EXP_OUT_DO              PIN_D4      //MBI_SDI
#define G_ALL                   PIN_C5      //MBI_OE
//#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN        PIN_F6  

#define NUMBER_OF_74595_MA7DOAN 8
#include <74595_MA7DOAN.c>

// --- M?ng mã cho LED 7 do?n Cathode chung ---
const unsigned char array[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// --- Khai báo bi?n ---
unsigned int8 varCode[8];       // Buffer d? li?u cho 8 LED 7 do?n
unsigned int16 i;               // Bi?n d?m, dùng int16 d? ch?a du?c giá tr? > 255
unsigned int8 donvi, chuc, tram, nghin; // Các bi?n d? tách s?

void main() 
{   
    // --- Kh?i t?o ban d?u ---
    output_low(G_ALL); // Cho phép IC ch?t ho?t d?ng (OE m?c th?p)
    output_low(PIN_F4);
    output_low(PIN_B3);
    output_high(PIN_F6);
    
    while(TRUE)
    {
        // --- Vòng l?p d?m chính t? 0 d?n 1000 ---
        for(i = 0; i <= 1000; i++) // S?a di?u ki?n l?p thành <= 1000
        {  
            // --- 1. Tách s? ? m?i hàng ---
            // Phép chia l?y ph?n nguyên và chia l?y du d? tách t?ng ch? s?
            nghin = i / 1000;
            tram  = (i / 100) % 10;
            chuc  = (i / 10) % 10;
            donvi = i % 10;

            // --- 2. Gán mã hi?n th? và xóa s? 0 vô nghia ---
            // T?t các LED không s? d?ng b?ng cách g?i mã 0x00
            
            // Hàng don v? (varCode[0]): Luôn luôn hi?n th?
            varCode[0] = array[donvi];

            // Hàng ch?c (varCode[1]): Ch? hi?n th? khi s? d?m >= 10
            if (i < 10) varCode[1] = 0x00; // T?t LED
            else        varCode[1] = array[chuc];

            // Hàng tram (varCode[2]): Ch? hi?n th? khi s? d?m >= 100
            if (i < 100) varCode[2] = 0x00; // T?t LED
            else         varCode[2] = array[tram];

            // Hàng nghìn (varCode[3]): Ch? hi?n th? khi s? d?m >= 1000
            if (i < 1000) varCode[3] = 0x00; // T?t LED
            else          varCode[3] = array[nghin];
            
            // T?t 4 LED 7 do?n còn l?i (r?t quan tr?ng!)
            // N?u không, chúng s? hi?n th? giá tr? rác trong b? nh?
            varCode[4] = 0x00;
            varCode[5] = 0x00;
            varCode[6] = 0x00;
            varCode[7] = 0x00;

            // --- 3. G?i d? li?u ra module LED 7 do?n ---
            write_expanded_outputs_MA7DOAN(varCode);
            
            // --- 4. T?o tr? d? quan sát ---
            delay_ms(100); // T?c d? d?m, có th? thay d?i
        }
    }
}
