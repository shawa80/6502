

extern char dis_portA;
extern char dis_portB;
extern char dis_DDRB;
extern char dis_DDRA;

extern char timer1_T1C_L;
extern char timer1_T1C_H;
extern char timer1_T1L_L;
extern char timer1_T1L_H;

extern char timer1_T2C_L;
extern char timer1_T2C_H;

extern char SR;
extern char ACR;
extern char PCR;
extern char IFR;
extern char IER;
extern char ORA;

//via2
extern char via2_portA;
extern char via2_portB;
extern char via2_DDRB;
extern char via2_DDRA;

extern char via2_timer1_T1C_L;
extern char via2_timer1_T1C_H;
extern char via2_timer1_T1L_L;
extern char via2_timer1_T1L_H;

extern char via2_timer1_T2C_L;
extern char via2_timer1_T2C_H;

extern char via2_SR;
extern char via2_ACR;
extern char via2_PCR;
extern char via2_IFR;
extern char via2_IER;
extern char via2_ORA;


#define dis_E 0x80
#define dis_RW 0x40
#define dis_RS 0x20


#define VIA_IRQ    ((char)0x80)
#define VIA_TIMER1 ((char)0x40)
#define VIA_TIMER2 ((char)0x20)
#define VIA_CB1    ((char)0x10)
#define VIA_CB2    ((char)0x80)
#define VIA_SHIFT  ((char)0x04)
#define VIA_CA1    ((char)0x02)
#define VIA_CA2    ((char)0x01)

#define TRUE 1
#define FALSE 0


void sys_irqEnable();
void sys_irqDisable();

/************************
yes I know this is wrong... I don't have the linker setup yet...
************************/
#include "com.c"
#include "math.c"
#include "dis.c"
#include "mem.c"
#include "timer.c"
#include "clk.c"
#include "led.c"
#include "t.c"

/*****************************
irq
*****************************/

inline void irqHandler() {

	char via = IFR;

	if (via & VIA_TIMER1) {
		clk_tick();
		timer1_clearIrq();
	}
	//if (via & VIA_SHIFT) {
	//	com_procIrq();
	//}

}

#include "gp.c"


void start() {

	clk_init();
	dis_printInit();
	timer1_init();

	com_init();

	led_init();
	gp_init();

	dis_clear();
	dis_home();
	dis_print("Hello From C");

	dis_setLine2();
	//int memSize = mem_scanMem((char*)2048);
	//dis_printDec(memSize);

	//dis_print("Memory: ");

	timer1_start();

	sys_irqEnable();

	com_fillColor();
	com_burst();

	t_start();
	com_burst();

	clk_sec = 0;

	t_main();

	//Board board;
        //t_clearBoard(&board);
	//while (1) { gp_test(&board);
	//}
}



