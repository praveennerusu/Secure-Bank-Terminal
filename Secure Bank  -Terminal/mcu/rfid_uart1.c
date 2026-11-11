#include <lpc21xx.h>   

typedef unsigned char  u8;
typedef unsigned int   u32;

#define DLAB_BIT           7
#define WORD_LEN_SEL_BITS  0
#define _8BIT              3

#define RFID_FRAME_LEN     8  

volatile unsigned char rfid_buffer[16];
volatile u8 rfid_index = 0;
volatile u8 rfid_ready = 0;
void UART1_ISR(void) __irq;

//uart1 initiazation
void UART1_Init(void)
{
   
    PINSEL0 &= ~0x000F0000;	  //po.8-p0.9
    PINSEL0 |=  0x00050000;     

    U1LCR = ((1 << DLAB_BIT) | (_8BIT << WORD_LEN_SEL_BITS));
    U1DLL = 97;                  // baud rate divisor
    U1DLM = 0;
    U1LCR &= ~(1 << DLAB_BIT);

    U1IER = 0x01;                // enable UART1 RX interrupt

    /* ---- VIC setup: THIS WAS MISSING BEFORE ----
       Without assigning a VIC slot and unmasking the interrupt source,
       U1IER alone will not route the interrupt to UART1_ISR. */
    VICVectAddr6  = (u32)UART1_ISR;  /* UART1 = VIC channel 7 on LPC2148 —
                                         double check against your datasheet;
                                         some boards use VICVectAddr7 */
    VICVectCntl6  = 0x20 | 7;        // 7 = UART1 interrupt source, slot enabled 
    VICIntEnable  = (1 << 7);        // unmask UART1 interrupt globally 
}


void UART1_ISR(void) __irq
{
    u8 iir = U1IIR & 0x0E;

    if (iir == 0x04 || iir == 0x0C)
    {
        u8 data = U1RBR;

        if (data == 0x02)              // start frameof Tx
        {
            rfid_index = 0;
            rfid_ready = 0;
        }
        else if (data == 0x03)         	// End frameof Tx
        {
            
            if (rfid_index == RFID_FRAME_LEN)		 //condition for card have 8 digits or not
            {
                rfid_buffer[rfid_index] = '\0';
                rfid_ready = 1;
            }
            rfid_index = 0;             //reset for nxt frame
        }
        else if (rfid_index < 12)
        {
            rfid_buffer[rfid_index++] = data;
        }
    }

    VICVectAddr = 0x00;                // acknowledge interrupt to VIC
}

