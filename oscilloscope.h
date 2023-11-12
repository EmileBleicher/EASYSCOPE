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
    void init_external_interupt();
    void print_oscylocope();
    void T0_Interupt( int val);
    void ADC_Interupt(float k);
    void external_interupt(float k);
    void Frequence_Echantillonage(int *valeur);
    float Amplitude_Echantillonage();
    int ADC_8to10();
    void debug (int n,int val);
    void ADC_Recording(int print_point);
    void Stay_Value(int ADC_Value,float Vtriger,int print_ADC);
    void print_Trigger(float value,float k);
    void print_Vmax(float A);
    void print_Techantillonage(int time);
    void loop_oscilloscope();
#endif	/* OSCILLOSCOPE_H */

