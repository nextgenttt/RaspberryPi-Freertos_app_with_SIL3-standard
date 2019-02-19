/**
 *	Integrated Interrupt Controller for RaspberryPi.
 *	@author	James Walmsley <james@fullfat-fs.co.uk>
 **/

#include "interrupts.h"
#include "bcm2835_intc.h"

static INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];


typedef struct {
	unsigned long	IRQBasic;	// Pending 0
	unsigned long	Pending1;
	unsigned long	Pending2;
	unsigned long	FIQCtrl;
	unsigned long	Enable1;
	unsigned long	Enable2;
	unsigned long	EnableBasic;
	unsigned long	Disable1;
	unsigned long	Disable2;
	unsigned long	DisableBasic;
} BCM2835_INTC_REGS;

static volatile BCM2835_INTC_REGS * const pRegs = (BCM2835_INTC_REGS *) (BCM2835_BASE_INTC);

/**
 *	Enables all IRQ's in the CPU's CPSR register.
 **/
__attribute__((no_instrument_function))
static void irqEnable() {
	__asm volatile("cpsie i" : : : "memory");
}

__attribute__((no_instrument_function))
static void irqDisable() {
	__asm volatile("cpsid i" : : : "memory");
}

#define clz(a) \
 ({ unsigned long __value, __arg = (a); \
     asm ("clz\t%0, %1": "=r" (__value): "r" (__arg)); \
     __value; })

/**
 *	This is the global IRQ handler on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
__attribute__((no_instrument_function))
void irqHandler() {
	register unsigned long ulMaskedStatus;
	register unsigned long irqNumber;

	ulMaskedStatus = pRegs->IRQBasic;

	/* Bits 7 through 0 in IRQBasic represent interrupts 64-71 */
	if (ulMaskedStatus & 0xFF) {
		irqNumber=64 + 31;
	}

	/* Bit 8 in IRQBasic indicates interrupts in Pending1 (interrupts 31-0) */
	else if(ulMaskedStatus & 0x100) {
		ulMaskedStatus = pRegs->Pending1;
		irqNumber = 0 + 31;
	}

	/* Bit 9 in IRQBasic indicates interrupts in Pending2 (interrupts 63-32) */
	else if(ulMaskedStatus & 0x200) {
		ulMaskedStatus = pRegs->Pending2;
		irqNumber = 32 + 31;
	}

	/* Bit 11 in IRQBasic indicates interrupt 9 (USB)*/
	else if(ulMaskedStatus & 0x800) {
			g_VectorTable[9].pfnHandler(9, g_VectorTable[9].pParam);
			return;
	}

	else {
		// No interrupt available, so just return.
		return;
	}

	/* Keep only least significant bit, in case multiple interrupts have occured */
	ulMaskedStatus&=-ulMaskedStatus;
	/* Some magic to determine number of interrupt to serve */
	irqNumber=irqNumber-clz(ulMaskedStatus);
	/* Call interrupt handler */
	g_VectorTable[irqNumber].pfnHandler(irqNumber, g_VectorTable[irqNumber].pParam);
}

__attribute__((no_instrument_function))
static void stubHandler(int nIRQ, void *pParam) {
	/**
	 *	Actually if we get here, we should probably disable the IRQ,
	 *	otherwise we could lock up this system, as there is nothing to
	 *	ackknowledge the interrupt.
	 **/
}

__attribute__((no_instrument_function))
int InitInterruptController() {
	int i;
	for(i = 0; i < BCM2835_INTC_TOTAL_IRQ; i++) {
		g_VectorTable[i].pfnHandler 	= stubHandler;
		g_VectorTable[i].pParam			= (void *) 0;
	}
	return 0;
}

__attribute__((no_instrument_function))
int RegisterInterrupt(int nIRQ, FN_INTERRUPT_HANDLER pfnHandler, void *pParam) {
	if(nIRQ<0 || nIRQ>71)
		return -1;

	irqDisable();
	{
		g_VectorTable[nIRQ].pfnHandler = pfnHandler;
		g_VectorTable[nIRQ].pParam		= pParam;
	}
	irqEnable();
	return 0;
}

__attribute__((no_instrument_function))
int EnableInterrupt(int nIRQ) {
	/* Datasheet says "All other bits are unaffected", and I'm counting on that. */
	unsigned int mask=1<<(nIRQ%32);
	if(nIRQ >=0 && nIRQ <=31) {
		pRegs->Enable1 = mask;
	} else
	if(nIRQ >=32 && nIRQ <=63){
		pRegs->Enable2 = mask;
	} else
	if(nIRQ >= 64 && nIRQ <= 71) {	// Basic IRQ enables
		pRegs->EnableBasic = mask;
	} else
		return -1;
	return 0;
}

__attribute__((no_instrument_function))
int DisableInterrupt(int nIRQ) {
	/* Datasheet says "All other bits are unaffected", and I'm counting on that. */
	unsigned int mask=1<<(nIRQ%32);

	if(nIRQ >=0 && nIRQ <=31) {
		pRegs->Disable1 = mask;
	} else
	if(nIRQ >=32 && nIRQ <=63){
		pRegs->Disable2 = mask;
	} else
	if(nIRQ >= 64 && nIRQ <= 71) {
		pRegs->DisableBasic = mask;
	} else
		return -1;

	return 0;
}

__attribute__((no_instrument_function))
int EnableInterrupts() {
	irqEnable();
	return 0;
}

__attribute__((no_instrument_function))
int DisableInterrupts() {
	irqDisable();
	return 0;
}
