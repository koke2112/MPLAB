/* 
 * File:   newmain.c
 * Author: jorgesm
 *
 * Created on 27 de marzo de 2014, 14:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic10f320.h>

#include "TimeDelay.h"


// PIC10F320 Configuration Bit Settings

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#pragma config LPBOR = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

/*
 * 
 */
int main(int argc, char** argv) {
    //OSCON
    //set_tris_a(0x00);
    //TRISAbits.TRISA0= 0;
    TRISA= 0x000;
    PORTAbits.RA2=0;
    for(int i=0; i< 0xFFFF; i++);
    PORTAbits.RA2=1;
    for(int i=0; i< 0xFFFF; i++);
    
   
}



