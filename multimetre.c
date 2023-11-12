// Tableau de correspondance des chiffres à afficher sur les segments
#include <stdbool.h>
#include <xc.h>
#include "main.h"
#include "multimetre.h"
#define maSortie PORTAbits.RA4
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
void multi_init() {
TRISD = 0x00; // Configurez PORTD comme sortie pour les données des afficheurs 7 segments
TRISA = 0b00100000; // Configurez PORTA comme sortie pour la sélection d'afficheur
TRISC = 0x00;
PORTD=0;
PORTA=0;
PORTC=0;
// Configuration de l'ADC pour le canal analogique que vous utilisez (AN4 - RA5)
ADCON1 = 0; // Configurez AN4 (RA5) comme entrée analogique
ADCON0 = 0x11; // Configuration de l'ADC : AN4 comme entrée, FRC
PORTD=63;
PORTA=1;
PORTD=63;
PORTA=2;
}
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
void loop_multi(){
    multi_init();
    maSortie=1;
    init_external_interupt();
    while (1) {
        GO_DONE=1;
    }
}

