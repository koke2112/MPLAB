/*
 * File:   main.c
 * Author: jorgesm
 *
 * Created on 3 de abril de 2014, 19:23
 */

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)



#define FREQ 500000L


//// CONFIG1
//#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
//#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
//#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
//#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
//#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
//#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
//#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
//
//// CONFIG2
//#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
//#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
//#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
//#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
//#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#include <stdio.h>
#include <stdlib.h>
#include <pic16lf1503.h>
#include <xc.h>
//modificables por usuario
//int DEBUG = 1;
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.





#define DEBUG
int rpm=42; //revolucuiones del motor 210-008
int paso=1; //longitud del paso en mm
//no modificables (fijas)
int va=0; //guarda la posicion donde tiene q ir el sintonizador.
int x=0; //offset a la posicion incial del sinto

/*
 * Metodo de delay en microsegundos si el reloj es el de 16MHz
 * Averiguar maxima cuenta
 */
 void delayUs(int us){
    //float ciclo_esp= us*(160);
     for(int i=0; i< us*4; i++);
//   while(ciclo_esp>0){
//        ciclo_esp--;
//    }
}

/*
 * Metodo de delay en ms si el reloj es el de 16MHz
 * Ajustado con osciloscopio
 * Averiguar maxima cuenta
 */
 void delayMs(int ms){
    //float ciclo_esp= ms*1000;
    for(int i=0; i< ms*71; i++);
//   while(ciclo_esp>0){
       //delayUs(1);
//        ciclo_esp--;
//    }
}

 /*
 * Metodo de delay en segundos si el reloj es el de 16MHz
 * Averiguar maxima cuenta
 */
 void delayS(int s){
    
    for(int i=0; i< s*1000; i++){
         delayMs(1);
 }
}


/*
 * Metodo para preparar y fijar el voltaje de referencia
 */

void vRef(){
    /*resgistro FVRCON 0b10000100
     * 1    FVR ON
     * 0    Para saber si esta preparado
     * 0    para temperatura
     * 0    para temperatura
     * 01    Ganancia 1 para el comparador
     * 00    Sin uso para el adc
     */
    FVRCON=0b10000100;


}
/*
 *Metodo para medir con el comparador
 * Devuelve 1 (invertido) si la entrada baja del nivel de referencia
 * Se usa el comparador 2 que esta conectado a los Registros RC
 */
void medida_COMP(){

    TRISCbits.TRISC0=1;     //habilita el RC0 como entrada
    ANSELCbits.ANSC0=1;     //habilita el RCO como analogico


    /*resgistro CM2CON0 0b10010000 Este registro esta conectado a los pines RC
     * 0    Comparador OFF  (activarlo al final)
     * 0    De donde se lee la salida
     * 0    Uso interno (no se va a sacar la salida por otro pin)
     * 1    Salida invertida
     * 0    No se usa
     * 0    bajo consumo/baja velocidad
     * 0    sin histeresis
     * 0    asincrono con el timer 1
     */
    CM2CON0=0b00010000; //comparador 2 registro de config0

    /*resgistro CM2CON1 0b10000100 Este registro esta conectado a los pines RC
     * 0    sin Interrupcion cuando la salida pasa a 1 (mover el sinto)
     * 0    Sin interrupcion cuando la salida pasa a 0
     * 00   Entrada positiva conectada al puerto analogico RC0
     * 0    No se usa
     * 100    Entrada negativa del voltage de referencia
     */
    CM2CON1=0b10000100; //comparador 2 registro de config1


}
/*
 * Metodo que mueve el motor en funcion de va
 *
 */
void mover (int v){
#if defined DEBUG
    /* se usan RC3 y RC5 para mover el motor
         * RC3 hacia la bobina y RC5 hacia el motor
         */


    if(v>0){    // mover hacia adelante RC3
         /*calcular el tiempo de excitacion necesario para mover una distancia.
          * La distancia van a ser multiplos del paso.
          */
        LATCbits.LATC3=1;
        delayS(paso*(60/rpm)*v);
        //delayMs(1);
        LATCbits.LATC3=0;
    }else{
        LATCbits.LATC5=1;
        //delayMs(1);
        delayS(paso*(60/rpm)*v*(-1));
        LATCbits.LATC5=0;
    }
#else
        //se usan RA0 y RA1
    if(v>0){    // mover hacia adelante RC0
         /* Calcular el tiempo de excitacion necesario para mover una distancia.
          * La distancia van a ser multiplos del paso.
          */
        LATAbits.LATA0=1;
        delayS(paso*(60/rpm)*v);
        LATAbits.LATA0=0;
    }else{
        LATAbits.LATA1=1;
        delayS(paso*(60/rpm)*v*(-1));
        LATAbits.LATA1=0;
    }
#endif


}
/*
 * Aqui va implementado el algoritmo1 (alternante)
 */
void algoritmoAlternante(){

       va=va+x;
       if(x>0){
           x++;
       }
       else if(x<0){
           x--;
       }
       x=x*(-1);
       mover(va);

}

/*
 * Metodo donde va lo que se ejecuta solo la primera vez
 */
void setup(){
    vRef();     //se fija el voltage de referencia
    medida_COMP();      //se configura el comparador
    CM2CON0bits.C2ON=1; //encender el comparador
    #if defined DEBUG
    /* se usan RC3 y RC5 para mover el motor
         * RC3 hacia la bobina y RC5 hacia el motor
         */
    TRISCbits.TRISC3=0;
    TRISCbits.TRISC5=0;

    #else
        //se usan RA0 y RA1
    TRISAbits.TRISA0=0;
    TRISAbits.TRISA1=0;
    #endif
    delayS(1);   //para dejar que se estabilice todo


}
/*
 * Metodo que se ejcuta en bucle
 */
void loop(){
    va=0;//posicion de la anterior sintonizacion
    x=1;
    while(CM2CON0bits.C2OUT==1){
        algoritmoAlternante();
    }

}
/*
 * Metodo para la inicialización del reloj de 16MHz
 */
void clockInit(){
    OSCCON=0b01111000;
    while(OSCSTATbits.HFIOFR!=1 && OSCSTATbits.HFIOFS!=1);


}
/*
 *
 */
int main(int argc, char** argv) {
    clockInit();
    setup();
    while(1){
       loop();
    }

}

