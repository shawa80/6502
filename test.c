

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


/********************************************
Serial port IO AKA led control
********************************************/

extern int com_index;
extern int com_size;
extern char com_buf[];
extern char com_buf_px[];

void com_init() {

	char t = ACR;
	t &= 0xE3;	//111x xx11 //clear bits
	t |= 0x18; 	//xxx1 10xx //output using main clock

	ACR = t;
}

void com_setStartFrame() {

	//start frame, we only need clock pulse, data should be zero
	com_buf[com_index] = 0x00;
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;
	com_buf[com_index] = 0x00;
	com_index++;

}

void com_setEndFrame() {
	//Calculate how many end frames we need
	com_setStartFrame();
}

void com_setColor(char r, char g, char b) {
	com_buf[com_index] = 0xE1; //111x xxxx, brightness
	com_index++;
	com_buf[com_index] = b;
	com_index++;
	com_buf[com_index] = g;
	com_index++;
	com_buf[com_index] = r;
	com_index++;
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

	//clean up, calculate how many we need
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_size = com_index;
}

void com_fillGreen() {

	com_index = 0;

	com_setStartFrame();
	com_setStartFrame();

	for (int l = 0; l < 40; l++) {
		com_setColor(0x00, 0xFF, 0x00);
		com_setColor(0x00, 0xff, 0x00);
		com_setColor(0x00, 0xFF, 0x00);
		com_setColor(0x00, 0xff, 0x00);
		com_setColor(0x00, 0xff, 0x00);
	}

	//clean up, calculate how many we need
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_setEndFrame();
	com_size = com_index;
}

inline void com_b() {

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


/****************************************
  Serial irq handling
*****************************************/
/*inline void com_write(char d) {

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

inline void com_procIrq() {


	if (com_index < com_size)
	{
		com_index++;
		com_write(com_buf[com_index]);
	}
	else {
		com_disableIrq();
	}
}

void com_start() {
	com_procIrq();
}*/

/*****************************************
LibMath
*****************************************/

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

//so much wrong here
int __mulint16(int a, int b) {
	int r = 0;
	for (int i = 0; i < a; i++)
		r += b;
	return r;
}

/**************************************************
two line lcd display
***************************************************/

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


char dis_int2Ascii(int value) {
	return 0x30 + value;
}

void dis_printDec(int value) {

	if (value < 0) {
		dis_printChar('!');
		return;
	}

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

/*************************************************
LibMem
*************************************************/

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
		char orig = startAddress[i];
		startAddress[i] = (char)0x00; //set value here
		if (startAddress[i] != (char)0x00) {
			break;
		}
		startAddress[i] = (char)0xFF; //set value here
		if (startAddress[i] != (char)0xFF) {
			break;
		}
		startAddress[i] = orig;
		i++;
	}
	return i+(int)startAddress;
}

/**********************************
Timer
***********************************/



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


/******************************
Clock
******************************/
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
		//com_burst();
		//com_fillGreen();
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
		//clk_print();
	}
}


/****************************
tetris
****************************/

/***************************
20 40
19 39
18 38
.  .
3  23
2  22
1  21
***************************/

#define t_w 20
#define t_h 10


extern char* t_px[10][20];


void t_setLum(int x, int y, char brighness) {

	(*t_px[x][y]) = brighness;
}

void t_setPx(int x, int y, char r, char g, char b) {

	char* loc = t_px[x][y];
	loc++;
	(*loc) = b;
	loc++;
	(*loc) = g;
	loc++;
	(*loc) = r;

}
void t_setPxByMem(char * loc, char r, char g, char b) {

	loc++;
	(*loc) = b;

	loc++;
	(*loc) = g;

	loc++;
	(*loc) = r;
}


void t_init() {

	int o = 0;
	int y;

	y = 0;
	while(y < 20) {
		t_px[0][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		t_px[1][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		t_px[2][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		t_px[3][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		t_px[4][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		t_px[5][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		t_px[6][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		t_px[7][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		t_px[8][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		t_px[9][y] = &com_buf_px[o];
		y--;
		o+=4;
	}

}

void t_clear() {

	char * offset = com_buf_px;
	for (int i = 0; i < 200; i++) {

		t_setPxByMem(offset, 0x00, 0x00, 0x00);
		offset += 4;
	}

}

void t_start() {

	t_clear();
	t_setPx(0, 0, 0x00, 0x00, 0xFF);
	t_setPx(9, 0, 0x00, 0x00, 0xFF);
	t_setPx(9, 19, 0x00, 0x00, 0xFF);
	t_setPx(0, 19, 0x00, 0x00, 0xFF);
}



/*****************************
irq
*****************************/

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

	t_init();

	dis_clear();
	dis_home();
	dis_print("Hello From C");

	dis_setLine2();
	//int memSize = mem_scanMem((char*)2048);
	//dis_printDec(memSize);
	

	//dis_print("Memory: ");

	timer1_start();

	//sys_irqEnable();

	com_fillColor();
	com_burst();

	t_start();
	com_burst();


	while (1){}
}



