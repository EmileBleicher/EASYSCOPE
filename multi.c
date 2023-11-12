/* 
 * File:   multi.c
 * Author: emile
 *
 * Created on 10 novembre 2023, 08:59
 */

#include <stdio.h>
#include <stdlib.h>
#include "multi.h"
#include <xc.h>
#define _XTAL_FREQ 800000
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

//void displayVoltage(unsigned int adcValue) {
//    // Conversion de la valeur ADC en une séquence de chiffres
//    unsigned int voltage_mV = (unsigned int) ((double) adcValue * 5000 / 255);
//    unsigned char digits[4];
//    digits[3] = voltage_mV / 1000;
//    digits[2] = (voltage_mV / 100) % 10;
//    digits[1] = (voltage_mV / 10) % 10;
//    digits[0] = voltage_mV % 10;
//    // Boucle pour afficher les chiffres sur les afficheurs 7 segments
//    for (int i = 0; i < 4; i++) {
//        PORTD = digitDisplay[digits[i]]; // Afficher le chiffre sur PORTD
//    if(i == 3){
//        PORTDbits.RD7 = 1;
//    }
//        PORTA = (1 << i); // Sélectionner l'afficheur actif sur PORTA
//        __delay_ms(1); // Attendez un court instant pour afficher
//        PORTA = 0; // Désactivez l'afficheur
//    }
//}

//LCD CODE
// source : ECE
#define RS LATBbits.LATB4
#define EN LATBbits.LATB5
#define D4 LATBbits.LATB0
#define D5 LATBbits.LATB1
#define D6 LATBbits.LATB2
#define D7 LATBbits.LATB3
void Lcd_Port(char a)
{
	if(a & 1) D4 = 1;
	else D4 = 0;

	if(a & 2) D5 = 1;
	else D5 = 0;

	if(a & 4) D6 = 1;
	else D6 = 0;

	if(a & 8) D7 = 1;
	else D7 = 0;
}

void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
    __delay_ms(4);
    EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
  TRISB = 0b11000000;
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1; // => RS = 1
   Lcd_Port(y>>4); //Data transfer
   EN = 1;
   __delay_us(4);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(4);
   EN = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}

void Lcd_start()
{
        unsigned int a;
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("EMILE EST TROP FORT !");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("ET TROP BEAU !");
    
        __delay_ms(2000);

        for(a=0;a<15;a++)
        {
            __delay_ms(300);
            Lcd_Shift_Left();
        }

        for(a=0;a<15;a++)
        {
            __delay_ms(300);
            Lcd_Shift_Right();
        }
}

