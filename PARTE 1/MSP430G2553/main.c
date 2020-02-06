#include "mod_libs.h"
#include "string.h"

char Buffer[50] = " Proyecto", BufferPos = 0;
char BufferRx[50] = {" "}, BufferposRx = 0;
uint16_t Time = 0;
uint16_t Adc1 = 0, Adc2 = 0, Adc1Tx = 0, Adc2Tx = 0;
uint16_t BufferLcd[50], bufferposlcd = 0;
short Flag = 0;

char DatoUart = 0;

int main(void) {
    loadBasicSetUp();

    adcInit(pinAna3 + pinAna4);

    lcdInit(); lcdClear(); delay_ms(10);

    lcdGotoxy(1, 1);
    lcdPuts("    GUIA 3  \n    2 ADC    ");

    uartSetup(baudrate_115200, 0);
    uartPuts("    GUIA 3  \n    2 ADC    ");

    delay_ms(2000);
    lcdClear(); delay_ms(10);
    //actAllInterrupts();

    while(1){

        Adc1 = adcRead(ChannelA3);delay_ms(10);
        Adc2 = adcRead(ChannelA4);delay_ms(10);

        BufferPos = putString(Buffer, 0, "{A=");
        BufferPos = putInt16(Buffer, BufferPos, Adc1, 4);
        BufferPos = putString(Buffer, BufferPos, ";B=");
        BufferPos = putInt16(Buffer, BufferPos, Adc2, 4);
        BufferPos = putString(Buffer, BufferPos, ";}\n");

        lcdGotoxy(1, 1);
        lcdPuts("POT1:      ");
        lcdGotoxy(7, 1);
        lcdPrintInt16(Adc1);

        lcdGotoxy(1, 2);
        lcdPuts("POT2:      ");
        lcdGotoxy(7, 2);
        lcdPrintInt16(Adc2);

        uartPuts(Buffer);
        delay_ms(500);
    }
}


#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR_HOOK(void){
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR_HOOK(void){
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void){
}

#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR_HOOK(void){
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR_HOOK(void){
}
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR_HOOK(void){
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMERA0_ISR_HOOK(void){
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMERA1_ISR_HOOK(void){
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR_HOOK(void){
}

