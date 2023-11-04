#include "oscilloscope.h"
#include <stdbool.h>
#include <xc.h>
#include "main.h"
#define maSortie PORTAbits.RA4
unsigned char i=0;
unsigned char enable=0;
unsigned char memo_GLCD[120];
 float Trigger=2.5;
 float memo_trigger=0;
bool Start_Single=false;
bool Oscilo_Mode=0;
void init_external_interupt(){
    TRISB=192;
   INT1IE=1;
   RBIF=0;
   INTEDG0=0;
}
void init_ADC() {
    TRISD = 0x00; // PORTD is an output
    PORTD = 0x00;
     TRISC = 0x00; // PORTD is an output
    PORTC = 0x00;
    ADCON1 = 0; // Only RA0 analog
    ADCON0 = 0;
    ADIE = 1; // Enable AD interrupts
    ADIF = 0;
    GIE = 1; // Activer l'interruption globale
    PEIE = 1;
    ADON = 1;
}
void InitTimer0(unsigned char Hb, unsigned char Lb){
  //Configuration du Timer0 et des registres SFR associés aux interruptions
    T0CON = 10;              	//Timer 0, 16-bit, sans prédiviseur
    TMR0H=252;               //Réinitialiser le timer
	TMR0L=134;            	//Initialiser Timer0 (octet du poids faible)
    TMR0 = 0;  
    INTCONbits.TMR0IF = 0; 	//RAZ du flag du Timer0
    INTCONbits.TMR0IE = 1; 	//Activer l'interruption du Timer0
    T0CONbits.TMR0ON = 1; 	//Metter Timer0 en marche
    INTCONbits.PEIE = 1; 	//Activer les interruptions de l'ensemble des périphériques
    INTCONbits.GIE = 1; 	//Activer toutes les interruptions
}
void T0_Interupt( int val){
    static int z=0;
    if(val<=z){
        GO_DONE=1;	// Active the ADC 
        z=0;
//        maSortie =~maSortie;
    }
    else{
        z++;}
	TMR0H = 252;             // preset for timer1 MSB register
    TMR0L = 134;    
	TMR0IF = 0;  //Clear flag du timer0
}
void ADC_Interupt(int k){
    int ADC_Value=ADRESH;
    int print_ADC=50-(ADC_Value/k);
    if (Oscilo_Mode){
        if(Start_Single){
               ADC_Recording(print_ADC);
        }else{
            i=0;
            Stay_Value(ADC_Value,Trigger,print_ADC);
        }
    }else{
        ADC_Recording(print_ADC);
    }
    ADIF = 0;
}

void Stay_Value(int ADC_Value,float Vtriger,int print_ADC){
    float Volt_ADC=((float)ADC_Value/255.0)*5.0;
    if(((Vtriger-0.1)<Volt_ADC)&&((Vtriger+0.1)>Volt_ADC)){//(Vtriger-0.10)<Volt_ADC)&&((Vtriger+0.10)>Volt_ADC)
         Start_Single=true;
         ADC_Recording(print_ADC);
    }
}
void ADC_Recording(int print_point){
    if((i>120)){
        i=0;
        enable=1;
        Start_Single=false;
        CHS0=1;
        CHS1=0;
        GO_DONE=1;
        
    }
    if(enable==1){
        glcd_PlotPixel(i,memo_GLCD[i] , 0);
        if((enable==1) && (memo_GLCD[i]!=25)&& (i%30!=0)&&(memo_GLCD[i]!=0)){
            glcd_PlotPixel(i,memo_GLCD[i] , 0);
        }
        
        if((i>1)&&(Oscilo_Mode==1)){
            if(memo_GLCD[0]<memo_GLCD[1]){
                Start_Single=false;
            }
        }
        memo_GLCD[i]=print_point;
        glcd_PlotPixel(i,print_point , 1);
        i++;
}
void reset_recording(){
    for (int y=0;y<120;y++){
        if((enable==1) && (memo_GLCD[i]!=25)&& (i%30!=0)&&(memo_GLCD[i]!=0)){
            glcd_PlotPixel(i,memo_GLCD[i] , 0);
        }
    }
}
void debug (int n,int val){
    char debug[10];
    sprintf(&debug,"%d=%d",n,val);
    glcd_text_write("          ", 10,1,10*n);
    glcd_text_write(debug, 10, 1, 10*n);
}
int ADC_8to10(){
    unsigned int adc_value;  // Une variable pour stocker les 10 bits de l'ADC
    adc_value = (ADRESH << 2) | ADRESL;
    return adc_value;
}
;void Frequence_Echantillonage(int *valeur){
    static int ok=0;
    ok++;
    *valeur=ADC_8to10();
    CHS0=0;
    CHS1=1;
    ADIF = 0;
    GO_DONE=1;
}
int Amplitude_Echantillonage(){
//    static int ok=0;
//    ok++;
//    debug(2,ok);
    int valeur=ADRESH/28;
//    debug(3,valeur);
    valeur++;
    ADIF = 0;
    CHS1=0;
    CHS0=0;
    return valeur;
}