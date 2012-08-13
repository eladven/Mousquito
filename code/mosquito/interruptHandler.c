#include "main.h"
#include "uart.h"


ISR(RESET_vect){ LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT0_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT1_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT2_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT3_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT4_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT5_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(INT6_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(INT7_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(PCINT0_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(USB_General_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(USB_Endpoint_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(WDT_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER2_COMPA_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER2_COMPB_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER2_OVF_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER1_CAPT_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
//ISR(TIMER1_COMPA_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(TIMER1_COMPB_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(TIMER1_COMPC_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(TIMER1_OVF_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(TIMER0_COMPA_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER0_COMPB_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER0_OVF_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(SPI_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(USART1_RX_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(USART1_UDRE_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(USART1_TX_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(ANALOG_COMP_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(ADC_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
ISR(EE_READY_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER3_CAPT_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();} 
//ISR(TIMER3_COMPA_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(TIMER3_COMPB_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER3_COMPC_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(TIMER3_OVF_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
//ISR(TWI_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}
ISR(SPM_READY_vect){LED_ON(4); LED_ON(5); LED_ON(6); _delay_ms(500); PrintString("very seris problem"); PrintEndl();}