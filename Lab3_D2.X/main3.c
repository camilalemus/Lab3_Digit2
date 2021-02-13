/*
 * File:   main3.c
 * Author: Camila Lemus Lone
 *
 * Created on February 8, 2021, 10:43 AM
 */

//******************************************************************************
//                          Configuration Bits
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//******************************************************************************
//                          Variables
//******************************************************************************

#include <xc.h>
#include "ADC.h"
#include <stdint.h>
#include "lcd.h"

#define _XTAL_FREQ (8000000)

uint8_t ADC_cflag;
uint8_t ADC_analogvalue;

//******************************************************************************
//                          Instanciar Funciones
//******************************************************************************

void setup (void);
void screen (void);

//******************************************************************************
//                          Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    while (1){
        screen();
        PORTC = ADC_analogvalue;
        if (ADC_cflag == 1) { // When the value is copied on my display
            __delay_us(500); // Wait the required acquisition time
            ADC_cflag = 0; // Turn off the adc_c flag
            ADCON0bits.GO = 1; // Start ADC Convertion
        }
    }
    return;
}

//******************************************************************************
//                                Setup
//******************************************************************************

void setup (void) {
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1; //Analogic because of pot
    ANSELbits.ANS1 = 1;

    TRISA = 0;
    TRISAbits.TRISA0 = 1; // Inputs because of pot
    TRISAbits.TRISA1 = 1; 
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    INTCONbits.GIE = 1; //Global Interrupts Enable
    INTCONbits.PEIE = 1; //Periferal Interrupts Enable
    
}

void screen(void){
        Lcd_Set_Cursor(1,1);        //Set cursor on file 1, position 1
        Lcd_Write_String ("S1: ");  //Write the text
        Lcd_Set_Cursor(1, 7);
        Lcd_Write_String ("S2: ");
        Lcd_Set_Cursor(1, 14);
        Lcd_Write_String ("S3: ");
}

void __interrupt()isr(void){
    if (PIR1bits.ADIF == 1){
        ADC_analogvalue = ADRESH;
        ADC_cflag = 1; 
        PIR1bits.ADIF = 0;
    }
}
