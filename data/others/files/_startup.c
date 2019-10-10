//
// _startup.c -- Reset initialization
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

#include "freedom.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

void _reset_init(void)    __attribute__((naked, aligned(8)));
extern void _start(void);                   // newlib C lib initialization

// ----------------------------------------------------------------------------------
// Flash configuration field (loaded into flash memory at 0x400)
//
//      Note:  RESET_PIN_CFG is set to enable external RESET signal
//
__attribute__ ((section (".cfmconfig"))) const uint8_t _cfm[0x10] = {
    0xFF,  /* NV_BACKKEY3: KEY=0xFF */
    0xFF,  /* NV_BACKKEY2: KEY=0xFF */
    0xFF,  /* NV_BACKKEY1: KEY=0xFF */
    0xFF,  /* NV_BACKKEY0: KEY=0xFF */
    0xFF,  /* NV_BACKKEY7: KEY=0xFF */
    0xFF,  /* NV_BACKKEY6: KEY=0xFF */
    0xFF,  /* NV_BACKKEY5: KEY=0xFF */
    0xFF,  /* NV_BACKKEY4: KEY=0xFF */
    0xFF,  /* NV_FPROT3: PROT=0xFF */
    0xFF,  /* NV_FPROT2: PROT=0xFF */
    0xFF,  /* NV_FPROT1: PROT=0xFF */
    0xFF,  /* NV_FPROT0: PROT=0xFF */
    0x7E,  /* NV_FSEC: KEYEN=1,MEEN=3,FSLACC=3,SEC=2 */
    0xFF,  /* NV_FOPT: ??=1,??=1,FAST_INIT=1,LPBOOT1=1,RESET_PIN_CFG=1,
                        NMI_DIS=1,EZPORT_DIS=1,LPBOOT0=1 */
    0xFF,
    0xFF
  };

// ----------------------------------------------------------------------------------
//
// Initialize the system clocks to a 48 Mhz core clock speed
// Mode progression:  FEI (reset) -> FBE -> PBE -> PEE
//
// Note:  Generated by Processor Expert, cleaned up by hand. 
//        For detailed information on clock modes, see the 
//        "KL25 Sub-Family Reference Manual" section 24.5.3.1
//
static void init_clocks(void)
{   
    // Enable clock gate to Port A module to enable pin routing (PORTA=1)
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    // Divide-by-2 for clock 1 and clock 4 (OUTDIV1=1, OUTDIV4=1)   
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0x01) | SIM_CLKDIV1_OUTDIV4(0x01);

    // System oscillator drives 32 kHz clock for various peripherals (OSC32KSEL=0)
    SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL(0x03);

    // Select PLL as a clock source for various peripherals (PLLFLLSEL=1)
    // Clock source for TPM counter clock is MCGFLLCLK or MCGPLLCLK/2
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
    SIM_SOPT2 = (SIM_SOPT2 & ~(SIM_SOPT2_TPMSRC(0x02))) | SIM_SOPT2_TPMSRC(0x01);
                  
    /* PORTA_PCR18: ISF=0,MUX=0 */
    /* PORTA_PCR19: ISF=0,MUX=0 */            
    PORTA_PCR18 &= ~((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
    PORTA_PCR19 &= ~((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));                                                   
    /* Switch to FBE Mode */
    
    /* OSC0_CR: ERCLKEN=0,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
    OSC0_CR = 0;                                                   
    /* MCG_C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
    MCG_C2 = (MCG_C2_RANGE0(0x02) | MCG_C2_EREFS0_MASK);
    /* MCG_C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
    MCG_C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x03));
    /* MCG_C4: DMX32=0,DRST_DRS=0 */
    MCG_C4 &= ~((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
    /* MCG_C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=1 */
    MCG_C5 = MCG_C5_PRDIV0(0x01);                                                   
    /* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
    MCG_C6 = 0;
    
    // Check that the source of the FLL reference clock is 
    // the external reference clock.
    while((MCG_S & MCG_S_IREFST_MASK) != 0)
        ;

    while((MCG_S & MCG_S_CLKST_MASK) != 8)      // Wait until external reference
        ;
    
    // Switch to PBE mode
    //   Select PLL as MCG source (PLLS=1)
    MCG_C6 = MCG_C6_PLLS_MASK;
    while((MCG_S & MCG_S_LOCK0_MASK) == 0)      // Wait until PLL locked
        ;
    
    // Switch to PEE mode
    //    Select PLL output (CLKS=0)
    //    FLL external reference divider (FRDIV=3)
    //    External reference clock for FLL (IREFS=0)
    MCG_C1 = MCG_C1_FRDIV(0x03);
    while((MCG_S & MCG_S_CLKST_MASK) != 0x0CU)  // Wait until PLL output
        ;
}

// Blink an LED based on a pattern bitmask
void fault(uint32_t pattern)
{
    for(;;) {
        RGB_LED(pattern & 1 ? 100 : 0, 0, 0);           // Set RED led based on LSB
        pattern = (pattern >> 1) | (pattern << 31);     // Rotate
        delay(100);
    }
}

// ----------------------------------------------------------------------------------
//  Default interrupt handler
void __attribute__((interrupt("IRQ"))) Default_Handler()
{
    fault(0b11111110);          // Blink LED and halt
}

// The register frame pushed onto the stack during exceptions
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    void *pc;
    uint32_t psr;
} hw_stackframe_t; 

// Handle hard faults:  print debugging information and halt
static void __attribute__((naked)) HardFault_Handler(void) 
{
    // Set up arguments and invoke _HardFault_Handler()
    asm("mov    r0, lr\n"                   // Arg 0
        "mrs  r1, psp\n"                    // Arg 1
        "mrs  r2, msp\n"                    // Arg 2
        "b  _HardFault_Handler\n");
}

void __attribute__((naked)) _HardFault_Handler(uint32_t lr, void *psp, void *msp)
{
    hw_stackframe_t *frame;
    
    // Find the active stack pointer (MSP or PSP)
    if(lr & 0x4)
        frame = psp;
    else
        frame = msp;
        
    fiprintf(stderr, "** HARD FAULT **\r\n   pc=%p\r\n  msp=%p\r\n  psp=%p\r\n", 
                    frame->pc, msp, psp);
                    
    fault(0b1111111000);            // Blink LED and halt
}


/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler()          __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler()          __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler()       __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler()      __attribute__ ((weak, alias("Default_Handler")));
void DMA0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void DMA1_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void DMA2_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void DMA3_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void MCM_IRQHandler()       __attribute__ ((weak, alias("Default_Handler")));
void FTFL_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void PMC_IRQHandler()       __attribute__ ((weak, alias("Default_Handler")));
void LLW_IRQHandler()       __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void SPI0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void UART0_IRQHandler()     __attribute__ ((weak, alias("Default_Handler")));
void UART1_IRQHandler()     __attribute__ ((weak, alias("Default_Handler")));
void UART2_IRQHandler()     __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void CMP0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void FTM0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void FTM1_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void FTM2_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void RTC_Seconds_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PIT_IRQHandler()       __attribute__ ((weak, alias("Default_Handler")));
void USBOTG_IRQHandler()    __attribute__ ((weak, alias("Default_Handler")));
void DAC0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void TSI0_IRQHandler()      __attribute__ ((weak, alias("Default_Handler")));
void MCG_IRQHandler()       __attribute__ ((weak, alias("Default_Handler")));
void LPTimer_IRQHandler()   __attribute__ ((weak, alias("Default_Handler")));
void PORTA_IRQHandler()     __attribute__ ((weak, alias("Default_Handler")));
void PORTD_IRQHandler()     __attribute__ ((weak, alias("Default_Handler")));

// ----------------------------------------------------------------------------------
// Interrupt vector table (loaded into flash memory at 0x000)
//
void (* const InterruptVector[])() __attribute__ ((section(".isr_vector"))) = {
    (void(*)(void)) __StackTop,                     // Initial stack pointer
    _reset_init,                                    // Reset handler
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SysTick_Handler,

    /* Interrupts */
    DMA0_IRQHandler, /* DMA Channel 0 Transfer Complete and Error */
    DMA1_IRQHandler, /* DMA Channel 1 Transfer Complete and Error */
    DMA2_IRQHandler, /* DMA Channel 2 Transfer Complete and Error */
    DMA3_IRQHandler, /* DMA Channel 3 Transfer Complete and Error */
    MCM_IRQHandler, /* Normal Interrupt */
    FTFL_IRQHandler, /* FTFL Interrupt */
    PMC_IRQHandler, /* PMC Interrupt */
    LLW_IRQHandler, /* Low Leakage Wake-up */
    I2C0_IRQHandler, /* I2C0 interrupt */
    I2C1_IRQHandler, /* I2C1 interrupt */
    SPI0_IRQHandler, /* SPI0 Interrupt */
    SPI1_IRQHandler, /* SPI1 Interrupt */
    UART0_IRQHandler, /* UART0 Status and Error interrupt */
    UART1_IRQHandler, /* UART1 Status and Error interrupt */
    UART2_IRQHandler, /* UART2 Status and Error interrupt */
    ADC0_IRQHandler, /* ADC0 interrupt */
    CMP0_IRQHandler, /* CMP0 interrupt */
    FTM0_IRQHandler, /* FTM0 fault, overflow and channels interrupt */
    FTM1_IRQHandler, /* FTM1 fault, overflow and channels interrupt */
    FTM2_IRQHandler, /* FTM2 fault, overflow and channels interrupt */
    RTC_Alarm_IRQHandler, /* RTC Alarm interrupt */
    RTC_Seconds_IRQHandler, /* RTC Seconds interrupt */
    PIT_IRQHandler, /* PIT timer all channels interrupt */
    Default_Handler, /* Reserved interrupt 39/23 */
    USBOTG_IRQHandler, /* USB interrupt */
    DAC0_IRQHandler, /* DAC0 interrupt */
    TSI0_IRQHandler, /* TSI0 Interrupt */
    MCG_IRQHandler, /* MCG Interrupt */
    LPTimer_IRQHandler, /* LPTimer interrupt */
    Default_Handler, /* Reserved interrupt 45/29 */
    PORTA_IRQHandler, /* Port A interrupt */
    PORTD_IRQHandler /* Port D interrupt */
};

// ----------------------------------------------------------------------------------
//
// init_led_io() -- Initialize I/O pins for on-board RGB LED (PWM)
//
static void init_led_io(void)
{
    // Turn on clock gating to PortB module (red and green LEDs) and 
    // PortD module (blue LED)  
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

    PORTB_PCR18 = PORT_PCR_MUX(3);  // TPM2_CH0 enable on PTB18 (red)
    PORTB_PCR19 = PORT_PCR_MUX(3);  // TPM2_CH1 enable on PTB19 (green)
    PORTD_PCR1  = PORT_PCR_MUX(4);  // TPM0_CH1 enable on PTD1  (blue)

    RGB_LED(0,0,0);                 // Off
    
    TPM0_MOD  = 99;
    TPM0_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2_MOD  = 99;
    TPM2_C0SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

    TPM2_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
    TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}

// ----------------------------------------------------------------------------------
//
// _reset_init() -- Reset entry point.  
//
//      The CPU reset vector points here.  Initialize the CPU, and jump
//      to the C runtime start, which will eventually invoke main()
//
void _reset_init(void)
{
    SIM_COPC = 0;                       // Disable the watchdog timer   
    SCB_VTOR = (uint32_t)InterruptVector;

    // Copy values to initialize data segment
    uint32_t *fr = __etext;
    uint32_t *to = __data_start__;
    unsigned int len = __data_end__ - __data_start__;
    while(len--)
        *to++ = *fr++;

    init_clocks();
    init_led_io();
    _start();                           // Goto C lib startup
    fault(FAULT_FAST_BLINK);            // ...should never get here.
}
