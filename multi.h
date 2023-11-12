/* 
 * File:   multi.h
 * Author: emile
 *
 * Created on 10 novembre 2023, 08:58
 */
#ifndef MULTI_H
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_start();
void displayVoltage(unsigned int adcValue);
#define	MULTI_H

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* MULTI_H */

