#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"
#include <stdio.h>

void main(void) {
    
     int adc = 0;
     int adc1=0;
    
     float ref,vt1,vt2;
     unsigned char buffer1[16];
     
    
    //OSCILADOR
    OSCFRQbits.HFFRQ = 0b110;
    
    //PUERTOS
    
    ANSELD=0;//PUERTO D COMO DIGITAL
    ANSELE=0;// PUERTO E COMO DIGITAL
   
    
    ANSELAbits.ANSA5 = 1; 
    TRISAbits.TRISA5 = 1;//A5  POTENCIOMETRO
    ANSELAbits.ANSA3 = 1;//
    TRISAbits.TRISA3 = 1;//A3 LM35
    ANSELBbits.ANSB1 = 0; //Analogica y salida
    TRISBbits.TRISB1 = 0;//FOCO
     
    
    //Convertidor Analogico-Digital
    
    ADCON0bits.ADON=1;
    ADCON0bits.ADCS=1;
    ADREFbits.ADNREF=0;
    ADREFbits.ADPREF=0b00;
    ADCON0bits.ADFRM=1;
  // __delay_ms(200);
    
   
    
    //Init LCD
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR);
    Lcd_Cmd(LCD_CURSOR_OFF);
    __delay_ms(100);
    
    //ciclo infinito
    while(1){
        
        Repetir:
        ADPCH=0b000011; //Canal A3(Sensor)
        //ADPCH=0b000101; // Canal A5(Potenciometro)
      __delay_ms(20);
      ADCON0bits.GO_nDONE=1;//Inicia la converción
      while(ADCON0bits.GO_nDONE);
      adc1 = ADRESH;
      adc1 = adc1 << 8;
      adc1 = adc1 + ADRESL;
      
     vt1=(5.0*adc1)/1023.0;//voltaje
     sensor = vt1*100.0;
      sprintf(buffer1, "Sen= %02.2f " , sensor);
      Lcd_Out2(1,1, buffer1);
      __delay_ms(2);
      Lcd_Out2(1,10," -C ");
      __delay_ms(300);
      
      
      //ADPCH=0b000011; //Canal A3(Sensor)
      ADPCH=0b000101; // Canal A5(Potenciometro)
      __delay_ms(20);
      ADCON0bits.GO_nDONE=1;//Inicia la converción
      
      
      
      while(ADCON0bits.GO_nDONE);
      adc = ADRESH;
      adc = adc<<8;
      adc = adc+ADRESL;
      vt2= 5.0*adc/1023.0;
      
      ref= (vt2*100.0);
      sprintf(buffer1, "Ref= %02.2f " , ref);
      Lcd_Out2(2,2,buffer1);
      __delay_ms(300);     
      
      a = (5.0*6.14740/1023.0)*100.0;
     
      if (sensor > (ref + a)){
          LATBbits.LATB1 = 1; // El foco se activa  
      }
      
      __delay_ms(300);
      
      if (sensor < (ref - a)){ // El rele se desactiva
          
          LATBbits.LATB1 = 0;   
      }
      
    }
    return;
}
