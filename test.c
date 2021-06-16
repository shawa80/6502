

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


void sys_irqEnable();

extern int com_index;
extern int com_size;
extern char com_buf[];

void com_setStartFrame() {

	//start frame
	com_buf[com_index] = 0x00;  //end frame?
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;

}

void com_setColor(char r, char g, char b) {
	com_buf[com_index] = 0xE1; //global
	com_index++;
	com_buf[com_index] = b;
	com_index++;
	com_buf[com_index] = g;
	com_index++;
	com_buf[com_index] = r;
	com_index++;
}

void com_init() {

	char t = ACR;
	t &= 0xE3;	  //111x xx11 clear bits
	t |= 0x18; //xxx1 10xx //set bits
//	t |= 0x14; //xxx1 10xx //set bits

	ACR = t;
}

void com_fillColor() {
	com_index = 0;

	com_setStartFrame();
	com_setStartFrame();

	for (int l = 0; l < 40; l++) {
		com_setColor(0xff, 0x00, 0x00);
		com_setColor(0x00, 0xff, 0x00);
		com_setColor(0x00, 0x00, 0xff);
		com_setColor(0xff, 0xff, 0xff);
		com_setColor(0xff, 0xff, 0xff);
	}

	com_setStartFrame();
	com_setStartFrame();
	com_setStartFrame();
	com_setStartFrame();
	com_setStartFrame();
	com_setStartFrame();
	com_size = com_index;
}

void com_b() {

	SR = com_buf[com_index];
	com_index++;
	SR = com_buf[com_index];
	com_index++;
	SR = com_buf[com_index];
	com_index++;
	SR = com_buf[com_index];
	com_index++;

}

void com_burst() {

	com_index = 0;

	while (com_index < com_size) {
		com_b();
	}

}


inline void com_write(char d) {

	SR = d;
}

inline char com_read() {
	return SR;
}

inline void com_enableIrq() {
	IER = (VIA_IRQ | VIA_SHIFT); //0x84;
}
inline void com_disableIrq() {
	IER = (VIA_SHIFT); //0x04;
}

/*inline void com_procIrq() {


	if (com_index < com_size)
	{
		com_index++;
		com_write(com_buf[com_index]);
	}
	else {
		com_disableIrq();
	}
}*/

/*void com_start() {
	com_procIrq();
}*/


void dis_wait() {
	dis_DDRB = 0x00;
	while (1) {
		dis_portA = dis_RW;
		dis_portA = (dis_RW|dis_E);
		if ((dis_portB & 0x80) != 0x80) break;
	}
	dis_portA = dis_RW;
	dis_DDRB = 0xff; 
}

void dis_writePortA(char val) {
	dis_portA = val;
}
void dis_writePortB(char val) {
	dis_portB = val;
}

void dis_issueIns(char instruction) {
	dis_wait();
	dis_writePortB(instruction);
	dis_writePortA(0);
	dis_writePortA(0x80);
	dis_writePortA(0);
}

void dis_clear() {
	dis_issueIns(1);
}

void dis_home() {
	dis_issueIns(2);
}

void dis_setLine2() {
	dis_issueIns(0xC0);
}

void dis_printChar(char c) {
	dis_wait();
	dis_portB = c;
	dis_portA = dis_RS;
	dis_portA = (dis_RS | dis_E);
	dis_portA = dis_RS;
}


int math_div(int num, int deno) {

	int times = 0;
	while (num >= deno) {
		num = num - deno;
		times++;
	}
	return times;
}

int math_mod(int num, int deno) {

	while (num >= deno) {
		num = num - deno;
	}
	return num;
}

char dis_int2Ascii(int value) {
	return 0x30 + value;
}

void dis_printDec(int value) {

	if (value < 10) {
		dis_printChar(dis_int2Ascii(value));
		return;
	}

	int remainder = math_mod(value, 10);
	value = math_div(value, 10);

	dis_printDec(value);
	dis_printChar(dis_int2Ascii(remainder));
}


void dis_printInit() {
	dis_DDRB = 0xFF; //set all pins to outpyut
	dis_DDRA = 0xE0; //set top 3 pins on A to output

	dis_issueIns(0x38); //set 8-bit mode; 2 line display, 5x8 font
	dis_issueIns(0x0C); //set display on; cursor off; blink off
	dis_issueIns(0x06); //increment and shift cursor

	dis_clear();
}

void dis_print(char * c) {

	int i = 0;
	while (c[i] != '\0') {
		dis_printChar(c[i]);
		i++;
	}
}

void mem_cpy(char dest[], char src[], int size) {

	for (int i = 0; i < size; i++) {
		dest[i] = src[i];
	}
}


void mem_set(short addr[], int size, char value) {

	for (int i = 0; i < size; i++) {
		addr[i] = value;
	}
}

int mem_scanMem(char * startAddress) {
	int i = 0;
	while (1) {
		startAddress[i] = (char)0x00; //set value here
		if (startAddress[i] != (char)0x00) {
			break;
		}
		startAddress[i] = (char)0xFF; //set value here
		if (startAddress[i] != (char)0xFF) {
			break;
		}
		i++;
	}
	return i+(int)startAddress;
}


void timer1_init() {

	timer1_T1C_L = 0x10;
	timer1_T1C_H = 0x27;

	timer1_T1L_L = 0x10;
	timer1_T1L_H = 0x27;

//ex
//	timer1_T2C_L = 0x10;
//	timer1_T2C_H = 0x27;


	ACR = 0x40;
}

void timer1_start() {
	IER = 0xC0;
}

void timer1_clearIrq() {
	char tmp;
	tmp = timer1_T1C_L;
}

extern char clk_jiff;
extern char clk_sec;
extern char clk_min;
extern char clk_hr;

void clk_init() {
	clk_jiff = 0;
	clk_sec = 0;
	clk_min = 0;
	clk_hr = 1;
}

void clk_print() {

	dis_clear();
	dis_home();
	dis_setLine2();

	dis_printDec(clk_hr);
	dis_print(":");
	if (clk_min < 10)
		dis_print("0");
	dis_printDec(clk_min);
	dis_print(":");
	if (clk_sec < 10)
		dis_print("0");
	dis_printDec(clk_sec);
}

void clk_tick() {
	int print = 0;

	clk_jiff++;
	if (clk_jiff >= 100)
	{
		clk_jiff = 0;
		clk_sec++;
		print = 1;
	}
	if (clk_sec >= 60)
	{
		clk_sec = 0;
		clk_min++;
	}
	if (clk_min >= 60)
	{
		clk_min = 0;
		clk_hr++;
	}
	if (clk_hr >= 13)
	{
		clk_hr = 1;
	}

	if (print == 1) {
		clk_print();
		//com_write(0xA5);
	}
}




inline void irqHandler() {

	char via = IFR;

	if (via & VIA_TIMER1) {
		timer1_clearIrq();
		clk_tick();
	}
	//if (via & VIA_SHIFT) {
	//	com_procIrq();
	//}

}


void start() {

	clk_init();
	dis_printInit();
	timer1_init();

	com_init();

	dis_clear();
	dis_home();
	dis_print("Hello From C");

	dis_setLine2();
	dis_print("Memory: ");
	int memSize = mem_scanMem((char*)2048);
	dis_printDec(memSize);

	timer1_start();

	//sys_irqEnable();

	com_fillColor();
	com_burst();
	while (1){}
}



