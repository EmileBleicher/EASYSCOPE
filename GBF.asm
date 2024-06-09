#include p18f4550.inc

CONFIG FOSC = HS  ; Oscillateur externe High-Speed (HS)

ORG 0x0000             ; Adresse de départ pour le programme
GOTO Main
org 0x0008 ; interrupt vector
goto irq_handle
    ; --- IRQ routine
irq_handle
    btfsc INTCON, INT0IF ; is it AD? 
	goto EX_interrupt ; yes
    btfsc PIR1, ADIF ; is it AD? 
	goto AD_interrupt ; yes
    retfie ; no, return f.i. 
    
EX_interrupt 
    bcf INTCON, INT0IF ; clear the flag
    btfsc mode_state,0
	goto mode_state_L
    btfss mode_state,0
	goto mode_state_H
    retfie
mode_state_L
    bcf mode_state,0
    goto init_PWM
mode_state_H
    bsf mode_state,0
    goto init_DC
AD_interrupt
    bcf PIR1, ADIF ; clear the flag
    ; A/D result goes into PORTC 
    movff ADRESH, PORTD
    retfie
MAIN_PROG CODE ; let linker place main program
Main:
    val_duty equ 0X00 ; count1 address
    memo_val_duty equ 0X10
    mode_state equ 0X01
    MOVLW 0x00 
    MOVWF mode_state
    MOVLW 0x00 
    MOVWF val_duty        ; Écrire dans la variable
    MOVLW 0x72          ; Valeur pour configurer l'oscillateur à 8 MHz
    MOVWF OSCCON        ; Écrire dans le registre OSCCON

    BCF TRISC, 2        ; Configurer RC2/Pin 17 pour la sortie (PWM)

    ; Configurer le Timer2 pour le PWM
    MOVLW 0x2          ; Prédiviseur à 16 pour Timer2
    MOVWF T2CON         ; Écrire dans le registre T2CON
    ; Charger la valeur de PR2 pour la période de PWM
    
   movlw 0x2
   movwf PR2;

    ; Charger la valeur du rapport cyclique pour 75%
    MOVLW 0x0C         ; Partie inférieure du Duty Cycle (94 en décimal pour 75%)
    MOVWF CCPR1L        ; Écrire dans CCPR1L
    MOVLW 0x0C          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF CCP1CON       ; Écrire dans CCP1CON

    ; Activer le module PWM
    BSF T2CON, TMR2ON   ; Activer Timer2
    ;CODE
    MOVLW 0x00          ;remise à zero 
    MOVWF val_duty        ; Écrire dans la variable
    MOVLW 0x72          ; Valeur pour configurer l'oscillateur à 8 MHz
    MOVWF OSCCON        ; Écrire dans le registre OSCCON
     ; Charger la valeur du rapport cyclique pour 75%
    MOVLW 0x3          ; Partie inférieure du Duty Cycle (94 en décimal pour 75%)
    MOVWF TRISD        ; Écrire dans CCPR1L
    MOVLW 0x00          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF PORTD       ; Écrire dans CCP1CON
       ;external
    MOVLW 0x00          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF INTCON2       ; Écrire dans CCP1CON
    bsf INTCON, INT0IE ; clear the flag
    bcf INTCON,INT0IF
    bsf INTCON,GIE
    movlw B'00001111'
    movwf ADCON1
    MOVLW 0xFF         ; Partie inférieure du Duty Cycle (94 en décimal pour 75%)
    MOVWF TRISB        ; Écrire dans CCPR1L
    MOVLW 0x00          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF PORTB       ; Écrire dans CCP1CON
    goto stateH


Loop:
   btfss PORTD , RD0 ; what logic level has RD0 ?
      goto pressed ; zero, not pressed, wait
   btfss PORTD,RD1
      goto pressed_bp2 
   goto decode_digit  
   goto Loop           ; Boucler indéfiniment
      
decode_digit:
    
    movf val_duty, w   ; Copie la valeur de val_duty dans le registre W
    movwf memo_val_duty ; Copie la valeur du registre W dans memo_val_duty
    btfsc STATUS, Z 
    goto dis_is_0 ; Z flag affected, it is 0 
    decfsz memo_val_duty
    goto dis_mark1 ; marks to jump 
    goto dis_is_1 ; it is 1 
dis_mark1
    decfsz memo_val_duty
    goto dis_mark2
    goto dis_is_2 ; it is 2 
dis_mark2
    decfsz memo_val_duty
    goto dis_mark3 
    goto dis_is_3 ; it is 3 
dis_mark3
    decfsz memo_val_duty
    goto dis_mark4 
    goto dis_is_4 ; it is 4 
dis_mark4
    decfsz memo_val_duty
    goto dis_mark5
    goto dis_is_5 ; it is 5 
dis_mark5
    decfsz memo_val_duty
    goto dis_mark6
    goto dis_is_6 ; it is 6 
dis_mark6
    decfsz memo_val_duty
    goto dis_mark7 
    goto dis_is_7 ; it is 7 
dis_mark7
    decfsz memo_val_duty
    goto dis_error
    goto dis_is_8 ; it is 8
dis_error ; should never arrive here
    goto dis_is_error
; --- apply mask and continue 
dis_is_0: 
    btfss LATD,2
	MOVLW 0x0C ; mask for 0     
    btfsc LATD,2
	MOVLW 0x1
    movwf CCPR1L
    goto Loop
dis_is_1: 
    btfss LATD,2
	MOVLW 0x19 ; mask for 1 
    btfsc LATD,2
	MOVLW 0x2
    movwf CCPR1L
    goto Loop
dis_is_2:
    btfss LATD,2
	MOVLW 0x26 ; mask for 2 
    btfsc LATD,2
	MOVLW 0x3
    movwf CCPR1L
    goto Loop
dis_is_3:
    btfss LATD,2
	MOVLW 0x33 ; mask for 3
    btfsc LATD,2
	MOVLW 0x4
    movwf CCPR1L
    goto Loop
dis_is_4:
    btfss LATD,2
	MOVLW 0x40 ; mask for 4
    btfsc LATD,2
	MOVLW 0x5
    movwf CCPR1L
    goto Loop
dis_is_5:
    btfss LATD,2
	MOVLW 0x4D ; mask for 5
    btfsc LATD,2
	MOVLW 0x6
    movwf CCPR1L
    goto Loop
dis_is_6:
    btfss LATD,2
	MOVLW 0x5A ; mask for 6
    btfsc LATB,0
	MOVLW 0x7
    movwf CCPR1L
    goto Loop
dis_is_7: 
    btfss LATD,2
	MOVLW 0x66 ; mask for 7 
    btfsc LATD,2
	MOVLW 0x8
   movwf CCPR1L
    goto Loop
dis_is_8:
    btfss LATD,2
	MOVLW 0x73 ; mask for 8
    btfsc LATD,2
	MOVLW 0x9
   movwf CCPR1L
   goto Loop
dis_is_error:
    MOVLW 0x00 ; mask for E
    movwf val_duty
    goto Loop
; gère le bouton 
    pressed ; now wait for the release.
       btfss PORTD , RD0 ; what logic level has RD0 ?
	 goto pressed ; one, wait for the release
       goto not_pressed ; zero, not pressed, wait
    not_pressed
      incf val_duty
      goto Loop
    pressed_bp2 ; now wait for the release.
       btfss PORTD , RD1 ; what logic level has RD0 ?
	 goto pressed_bp2 ; one, wait for the release
       goto not_pressed_bp2 ; zero, not pressed, wait
    not_pressed_bp2
	btfss LATD,2
	    goto stateH
	btfsc LATD,2
	    goto stateL
stateH 
    bsf LATD,2
    movlw D'11'
    movwf PR2
     MOVLW 0x2
    movwf CCPR1L
    goto Loop
stateL 
    bcf LATD,2
    movlw 0X7C
    movwf PR2;
    MOVLW 0x19
    movwf CCPR1L
    MOVLW 0x0
    movwf CCPR1H
    goto Loop
init_PWM
    BsF T2CON, TMR2ON   ; désactive Timer2
    bcf PIE1, ADIE  
    bcf INTCON, PEIE ; periph. Int.
    bsf INTCON, GIE ; interrupts
    movlw B'00001111'
    movwf ADCON1
    MOVLW 0xFF         ; Partie inférieure du Duty Cycle (94 en décimal pour 75%)
    MOVWF TRISB        ; Écrire dans CCPR1L
    MOVLW 0x00          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF PORTB       ; Écrire dans CCP1CON
    goto Loop
init_DC
    MOVLW 0xFF         ; Partie inférieure du Duty Cycle (94 en décimal pour 75%)
    MOVWF TRISA        ; Écrire dans CCPR1L
    MOVLW 0x00          ; Configuration pour le CCP1CON, duty cycle bits 5:4 à 0b00
    MOVWF PORTA       ; Écrire dans CCP1CON
    BcF T2CON, TMR2ON   ; désactive Timer2
    clrf TRISD ; PORTC is an output
    clrf PORTD ; clear PORTC 
     ;select channel 0 -> AN0
    movlw B'00000001'
    movwf ADCON0 
    ;configure AN1 comme une entrée analogique (autrement dit une entrée de notre CAN)
    movlw B'01101110' 
    movwf ADCON1
       ; --- enable AD interrupts
    bsf PIE1, ADIE  
    bsf INTCON, PEIE ; periph. Int.
    bsf INTCON, GIE ; interrupts
    goto DC_loop
    
    
    DC_loop
	bsf ADCON0, GO_DONE 
	goto DC_loop
end
