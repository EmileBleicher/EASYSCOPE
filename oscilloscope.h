/* 
 * File:   oscilloscope.h
 * Author: emile
 *
 * Created on 27 octobre 2023, 08:59
 */

#ifndef OSCILLOSCOPE_H
#define	OSCILLOSCOPE_H
    void init_ADC();
    void InitTimer0(unsigned char Hb, unsigned char Lb);
    void T0_Interupt( int val);
    void ADC_Interupt(int k);
    void Frequence_Echantillonage(int *valeur);
    int ADC_8to10();
    void debug (int n,int val);
    int Amplitude_Echantillonage();
    void init_external_interupt();
#endif	/* OSCILLOSCOPE_H */

