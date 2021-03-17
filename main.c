#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#define KEY_LVL     150                     // Defines threshold for a key press

/* Definitions for use with the WDT settings*/
#define DIV_ACLK_32768  (WDT_ADLY_1000)     // ACLK/32768
#define DIV_ACLK_8192   (WDT_ADLY_250)      // ACLK/8192
#define DIV_ACLK_512    (WDT_ADLY_16)       // ACLK/512
#define DIV_ACLK_64     (WDT_ADLY_1_9)      // ACLK/64
#define DIV_SMCLK_32768 (WDT_MDLY_32)       // SMCLK/32768
#define DIV_SMCLK_8192  (WDT_MDLY_8)        // SMCLK/8192
#define DIV_SMCLK_512   (WDT_MDLY_0_5)      // SMCLK/512
#define DIV_SMCLK_64    (WDT_MDLY_0_064)    // SMCLK/64
#define DIV_ACLK_512    (WDT_ADLY_16)       // ACLK/512
#define DIV_SMCLK_512   (WDT_MDLY_0_5)      // SMCLK/512

#define WDT_meas_setting (DIV_SMCLK_8192)
//#define WDT_meas_setting (DIV_SMCLK_512)
#define WDT_delay_setting (DIV_SMCLK_512)
//#define WDT_delay_setting (WDT_ADLY

unsigned int base_cntA, base_cntB, meas_cntA, meas_cntB;
int delta_cntA;
int delta_cntB;
int intensity;
int intensity_count;
int cycles;
void measure_count(void);            // Measures each capacitive sensor

int main(void)
{
  unsigned volatile int i,j, k;

  WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
  DCOCTL = 0;                         // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;              // Set DCO to 1MHz
  DCOCTL =  CALDCO_1MHZ;
  BCSCTL3 |= LFXT1S_2;                // LFXT1 = VLO
  IE1 |= WDTIE;                       // enable WDT interrupt
  P2SEL = 0x00;                       // No XTAL
  P1DIR = 0x41;                       // P1.0 & P1.6 = LEDs
  P1OUT = 0x01;

  TA0CTL = TASSEL_3+MC_2;             // TACLK, cont mode
  TA0CCTL1 = CM_3+CCIS_2+CAP;         // Pos&Neg,GND,Cap

  P1DIR &= ~ BIT4;                    // P1.4 is the capacitive sensor input used here
  P1SEL &= ~ BIT4;
  P1SEL2 |= BIT4;

  P1SEL  = BIT1 + BIT2;               // Select UART as the pin function
  P1SEL2 = BIT1 + BIT2;
  UCA0CTL1 |= UCSWRST;                // Disable UART module for configuration
  UCA0CTL0 = 0x00;                    // No parity, LSB first, 8-bit data, 1 stop bit, UART, Asynchronous
  UCA0CTL1 = UCSSEL_2 + UCSWRST;      // SMCLK source, keep in reset state
  UCA0BR0 = 104;                      // 9600 Baud rate   - Assumes 1 MHz clock
  UCA0BR1 = 0;                        // 9600 Baud rate   - Assumes 1 MHz clock
  UCA0MCTL = 0x02;                    // 2nd Stage modulation = 1, Oversampling off
  UCA0CTL1 &= ~UCSWRST;               // Enable UART module
  j = 0;

  __bis_SR_register(GIE);             // Enable interrupts

  while (1)
  {

        measure_count();              // Measure pin
        serial_string("             \r");
        serial_number(meas_cntA);

        volatile unsigned int i;

      if(meas_cntA > 14000){
          P1OUT |= 0x01;
      }
      else if((meas_cntA >= 13300) && (meas_cntA <= 13999)){
         // __delay_cycles(2000);
         // for(i>90000; i==0; i--){}
          base_cntA = 90000;

      }
      else if((meas_cntA >= 12700) && (meas_cntA <= 13299)){
          //__delay_cycles(1000);
         // for(i>70000; i==0; i--){}
          base_cntA = 70000;
      }
      else if((meas_cntA >= 12300) && (meas_cntA <= 12699)){
          //__delay_cycles(500);
         // for(i>50000; i==0; i--){}
          base_cntA = 50000;
      }
      else if((meas_cntA >= 11700) && (meas_cntA <= 12299)){
          //__delay_cycles(250);
         // for(i>40000; i==0; i--){}
          base_cntA = 40000;
      }
      else if((meas_cntA >= 10000) && (meas_cntA <= 11699)){
          //__delay_cycles(120);
         // for(i>30000; i==0; i--){}
          base_cntA = 30000;
      }
      else if( (meas_cntA < 9999)){
         // __delay_cycles(60);
        //  for(i>20000; i==0; i--){}
          base_cntA = 20000;
      }
      else{}

      WDTCTL = WDT_delay_setting;       // WDT, ACLK, interval timer

      P1OUT |= 0x40;

      for(i=base_cntA; i > 0 ; i-- );
      P1OUT &= ~0x40;

      for(j=base_cntA; j > 0 ; j-- );

    __bis_SR_register(LPM0_bits);
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
  TA0CCTL1 ^= CCIS0;                        // Create SW capture of CCR1
  __bic_SR_register_on_exit(LPM3_bits);     // Exit LPM3 on reti
}


void measure_count(void)
{
 //   P1DIR &= ~ BIT4;                    // P1.4 is the capacitive sensor input used here
 //   P1SEL &= ~ BIT4;
    P1SEL2 |= BIT4;
    WDTCTL = WDT_meas_setting;          // WDT, ACLK, interval timer
    TA0CTL |= TACLR;                    // Clear Timer_A TAR
    __bis_SR_register(LPM0_bits+GIE);   // Wait for WDT interrupt
    TA0CCTL1 ^= CCIS0;                  // Create SW capture of CCR1
    meas_cntA = TACCR1;                 // Save result
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    P1SEL2 &= ~BIT4;

}


void serial_string(char string[])
{
    int i;
    for(i = 0; string[i] != '\0'; i++)      // Send characters until end-of-string
    {
        while( !(IFG2 & UCA0TXIFG) );       // Wait until the transmit buffer is empty
        UCA0TXBUF = string[i];                      // Send the character through the Xmit buffer
    }
}

void serial_number(int value)
    {
        char string[10];
        sprintf(string, "%d", value);       // Convert an integer to a character string
        serial_string(string);
    }
