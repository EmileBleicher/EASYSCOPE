/*
 * File:   newmain.c
 * Author: emile
 *
 * Created on 25 octobre 2023, 21:36
 */
#include <xc.h>
#include "main.h"
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config LVP = OFF 		//Disable low voltage programmig
#pragma config DEBUG = OFF		//Debug ON
#define maSortie PORTAbits.RA4
int val=18;
int Amp=5;

void __interrupt() ISR() {
    if (PIR1bits.ADIF && CHS0==0 && CHS1==0) {
        ADC_Interupt(Amp);}
    if (PIR1bits.ADIF && CHS0==1) {
        Frequence_Echantillonage(&val);}
    if (PIR1bits.ADIF && CHS1==1){
       Amp=Amplitude_Echantillonage();}
    if(INTCONbits.TMR0IF ){			//Est-ce que Timer0 a causé l'interruption?
		T0_Interupt(val);  }      	//Si oui, exécuter T0 ISR (interrupt service routine)
    if(RBIF==1){
        glcd_text_write("Interupt", 20, 1, 1);
        RBIF=0;
    }
    
    
}

void main(void) {
    glcd_Init(GLCD_ON);
    init_ADC();
    InitTimer0(0x03, 0xE8);
    init_external_interupt();
    while(1){}
    
    return;
}
