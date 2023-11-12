/*
 * File:   newmain.c
 * Author: emile
 *
 * Created on 25 octobre 2023, 21:36
 */
#include <xc.h>
#include "main.h"
#include <stdbool.h>
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config LVP = OFF 		//Disable low voltage programmig
#pragma config DEBUG = OFF		//Debug ON



#define maSortie PORTAbits.RA6
const unsigned char digitDisplay[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111 // 9
};

void displayVoltage(unsigned int adcValue) {
// Conversion de la valeur ADC en une séquence de chiffres
unsigned int voltage_mV = (unsigned int) ((double) adcValue * 5000 / 255);
unsigned char digits[4];
digits[3] = voltage_mV / 1000;
digits[2] = (voltage_mV / 100) % 10;
digits[1] = (voltage_mV / 10) % 10;
digits[0] = voltage_mV % 10;
// Boucle pour afficher les chiffres sur les afficheurs 7 segments
for (int i = 0; i < 4; i++) {
PORTD = digitDisplay[digits[i]]; // Afficher le chiffre sur PORTD
if(i == 3){
PORTDbits.RD7 = 1;
}
PORTA = (1 << i); // Sélectionner l'afficheur actif sur PORTA
__delay_ms(5); // Attendez un court instant pour afficher
PORTA = 0; // Désactivez l'afficheur
}
}
int val=18;
float Amp=1;
char valH;
bool mode=false;
void toggle_mode(){
   mode= (mode==false)?true:false;
}
void __interrupt() ISR() {
    char static affichage[30];
    static char valL;
    static float Te;
        if (PIR1bits.ADIF && CHS0==0 && CHS1==0) {
            if(mode==false){
            ADC_Interupt(Amp);}
            else if (mode==true){             
                displayVoltage(ADRESH);
                CHS2=1;
                CHS1=0;
                CHS0=1;
                GO_DONE=1;
                valL=ADRESL;
                valH=ADRESH;
  
                
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                sprintf(affichage,"H:%d",valH);
                Lcd_Write_String(affichage);
                Lcd_Set_Cursor(1,10);
                sprintf(affichage,"L:%d",valL);
                Lcd_Write_String(affichage);
                Lcd_Set_Cursor(2,1);
                Te=(val/2)+1;
                sprintf(affichage,"TE:%.1f",Te);
                Lcd_Write_String(affichage);
                PIR1bits.ADIF=0;
            }
        }
        if (PIR1bits.ADIF && CHS0==1 && CHS1==0) {
            Frequence_Echantillonage(&val);}
        if (PIR1bits.ADIF && CHS0==0 && CHS1==1){
          Amp= Amplitude_Echantillonage();}//A revoir notament en modiffiant l'équation
        if(INTCONbits.TMR0IF){			//Est-ce que Timer0 a causé l'interruption?
            T0_Interupt(val);}      	//Si oui, exécuter T0 ISR (interrupt service routine)
        if(RBIF==1){
            if((PORTBbits.RB6==0)&&(PORTBbits.RB7==0)){
                toggle_mode();
                glcd_FillScreen(0);
                glcd_text_write("Changement mode", 20,1, 1,1);
                __delay_ms(1000);
                glcd_FillScreen(0);
                if(mode==false){
                    TRISA=255;
                    CHS2=0;
                    CHS1=0;
                    CHS0=0;
                    glcd_FillScreen(0);
                    print_oscylocope();
                }
                else if(mode==true){
                    TRISA=240;
                    CHS2=1;
                    CHS1=0;
                    CHS0=1;
                    Lcd_Init();
                    Lcd_Clear();
                    Lcd_Set_Cursor(1,1);
                    Lcd_Write_String("LANCEMENT DE L ECRAN LCD");
                }
            }
            else {
                external_interupt(Amp);
            }  
        }
        }



void main(void)
{
//    glcd_Init(GLCD_ON);
//    glcd_Image();
//    __delay_ms(1000);
//    glcd_FillScreen(0);
//    print_oscylocope();
//    init_ADC();
//    InitTimer0(0x03, 0xE8);
//    init_external_interupt();
//    while(1){
//    }
    loop_oscilloscope();
    while(1){
    while(mode==false){
    }
    while (mode==true){
        GO_DONE=1;
         displayVoltage(valH);
    } }
    return;
}
