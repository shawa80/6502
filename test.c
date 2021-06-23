

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

#define TRUE 1
#define FALSE 0


void sys_irqEnable();
void sys_irqDisable();

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

	//sys_irqDisable();

	com_index = 0;

	while (com_index < com_size) {
		com_b();
	}

	//sys_irqEnable();
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

inline void timer1_clearIrq() {
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
		com_burst();
		//com_fillGreen();
	}/*
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
	}*/
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


typedef enum act {
	NewBlock,
	Drop

} Action;

typedef struct loc {
	char x;
	char y;
} Location;

typedef struct clr {
	char r;
	char g;
	char b;
} Color;

typedef struct shp {

	Color color;

	Location a;
	Location b;
	Location c;
	Location d;
} Shape;

typedef struct brd {
	int nextColor;
	int nextShape;
	char grid[10][20];
} Board;

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
		clk_tick();
		timer1_clearIrq();
	}
	//if (via & VIA_SHIFT) {
	//	com_procIrq();
	//}

}


void t_setShape(Shape * block) {
	t_setPx(block->a.x, block->a.y, block->color.r, block->color.g, block->color.b);
	t_setPx(block->b.x, block->b.y, block->color.r, block->color.g, block->color.b);
	t_setPx(block->c.x, block->c.y, block->color.r, block->color.g, block->color.b);
	t_setPx(block->d.x, block->d.y, block->color.r, block->color.g, block->color.b);
}
void t_clearShape(Shape * block) {
	t_setPx(block->a.x, block->a.y, 0x00, 0x00, 0x00);
	t_setPx(block->b.x, block->b.y, 0x00, 0x00, 0x00);
	t_setPx(block->c.x, block->c.y, 0x00, 0x00, 0x00);
	t_setPx(block->d.x, block->d.y, 0x00, 0x00, 0x00);
}

void t_moveDown(Shape * block) {
	block->a.y--;
	block->b.y--;
	block->c.y--;
	block->d.y--;
}
void t_moveLeft(Shape * block) {
	block->a.x--;
	block->b.x--;
	block->c.x--;
	block->d.x--;
}
void t_moveRight(Shape * block) {
	block->a.x++;
	block->b.x++;
	block->c.x++;
	block->d.x++;
}


void t_copyBlock(Shape * dest, Shape * src) {

	dest->color.r = src->color.r;
	dest->color.g = src->color.g;
	dest->color.b = src->color.b;

	dest->a.x = src->a.x;
	dest->a.y = src->a.y;

	dest->b.x = src->b.x;
	dest->b.y = src->b.y;

	dest->c.x = src->c.x;
	dest->c.y = src->c.y;

	dest->d.x = src->d.x;
	dest->d.y = src->d.y;

}

int t_isWallHit(Shape * block) {
	if (block->a.x < 0)
		return TRUE;
	if (block->b.x < 0)
		return TRUE;
	if (block->c.x < 0)
		return TRUE;
	if (block->d.x < 0)
		return TRUE;

	if (block->a.x > 9)
		return TRUE;
	if (block->b.x > 9)
		return TRUE;
	if (block->c.x > 9)
		return TRUE;
	if (block->d.x > 9)
		return TRUE;

	return FALSE;
}

int t_isFloorHit(Shape * block) {
	if (block->a.y < 0)
		return TRUE;
	if (block->b.y < 0)
		return TRUE;
	if (block->c.y < 0)
		return TRUE;
	if (block->d.y < 0)
		return TRUE;

	return FALSE;
}

int t_isCollided(Board * board, Shape * block) {

	if (board->grid[block->a.x][block->a.y] == 1)
		return TRUE;
	if (board->grid[block->b.x][block->b.y] == 1)
		return TRUE;
	if (board->grid[block->c.x][block->c.y] == 1)
		return TRUE;
	if (board->grid[block->d.x][block->d.y] == 1)
		return TRUE;

	return FALSE;
}

void t_stamp(Board * board, Shape * block) {
	board->grid[block->a.x][block->a.y] = 1;
	board->grid[block->b.x][block->b.y] = 1;
	board->grid[block->c.x][block->c.y] = 1;
	board->grid[block->d.x][block->d.y] = 1;
}

#define t_leftBtn 0x01
#define t_centerBtn 0x02
#define t_rightBtn 0x04


Action blockLogic(Board * board, Shape * block) {

	Shape bak;
	t_copyBlock(&bak, block);


	t_clearShape(block);

	if (~dis_portA & t_leftBtn) {
		t_moveLeft(block);
	}
	if (~dis_portA & t_rightBtn) {
		t_moveRight(block);
	}

	if (t_isWallHit(block)
	|| t_isCollided(board, block)) {
		t_copyBlock(block, &bak);	//rollback move
	} else {
		t_copyBlock(&bak, block);	//commit to new rollback location
	}

	t_moveDown(block);

	Action act = Drop;

	if (t_isFloorHit(block)
	|| t_isCollided(board, block)) {
		t_copyBlock(block, &bak);
		t_stamp(board, block);
		act = NewBlock;
	}

	t_setShape(block);

	return act;

}

void t_clearBoard(Board * board) {

	board->nextColor = 0;
	board->nextShape = 0;

	for (int y = 0; y < 20; y++)
		board->grid[0][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[1][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[2][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[3][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[4][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[5][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[6][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[7][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[8][y] = 0;
	for (int y = 0; y < 20; y++)
		board->grid[9][y] = 0;
}

void t_makeBlockL(Shape * block) {
	block->a.x = 4;
	block->a.y = 19;
	block->b.x = 4;
	block->b.y = 18;
	block->c.x = 4;
	block->c.y = 17;
	block->d.x = 5;
	block->d.y = 17;
}
void t_makeBlockT(Shape * block) {
	block->a.x = 4;
	block->a.y = 19;
	block->b.x = 4;
	block->b.y = 18;
	block->c.x = 4;
	block->c.y = 17;
	block->d.x = 5;
	block->d.y = 18;

}
void t_makeBlockI(Shape * block) {
	block->a.x = 4;
	block->a.y = 19;
	block->b.x = 4;
	block->b.y = 18;
	block->c.x = 4;
	block->c.y = 17;
	block->d.x = 4;
	block->d.y = 16;
}
void t_makeBlockZ(Shape * block) {
	block->a.x = 4;
	block->a.y = 19;
	block->b.x = 5;
	block->b.y = 19;
	block->c.x = 5;
	block->c.y = 18;
	block->d.x = 6;
	block->d.y = 18;

}
void t_makeBlockZZ(Shape * block) {
	block->a.x = 5;
	block->a.y = 19;
	block->b.x = 4;
	block->b.y = 19;
	block->c.x = 4;
	block->c.y = 18;
	block->d.x = 3;
	block->d.y = 18;
}


void t_makeBlock(Board * board, Shape * block) {


	if (board->nextColor == 0) {
		block->color.r = 0x00;
		block->color.g = 0x00;
		block->color.b = 0xFF;
	} else if (board->nextColor == 1) {
		block->color.r = 0xFF;
		block->color.g = 0x00;
		block->color.b = 0x00;
	} else if (board->nextColor == 2) {
		block->color.r = 0x00;
		block->color.g = 0xFF;
		block->color.b = 0x00;
	} else if (board->nextColor == 3) {
		block->color.r = 0x00;
		block->color.g = 0xFF;
		block->color.b = 0xff;
	} else if (board->nextColor == 4) {
		block->color.r = 0xFF;
		block->color.g = 0x00;
		block->color.b = 0xFF;
	} else {
		block->color.r = 0xFF;
		block->color.g = 0xFF;
		block->color.b = 0xFF;
	}
	board->nextColor++;
	if (board->nextColor == 6)
		board->nextColor = 0;

	if (board->nextShape == 0) {
		t_makeBlockL(block);
	} else if (board->nextShape == 1) {
		t_makeBlockI(block);
	} else if (board->nextShape == 2) {
		t_makeBlockT(block);
	} else if (board->nextShape == 3) {
		t_makeBlockZ(block);
	} else {
		t_makeBlockZZ(block);
	}
	board->nextShape++;
	if (board->nextShape == 5)
		board->nextShape = 0;

}

void test(Board * board, Shape * block) {

	Action act = Drop;
	if (clk_jiff >= 10) {
		act = blockLogic(board, block);
		clk_jiff = 0;
		com_burst();
	}

	if (act == NewBlock) {
		t_makeBlock(board, block);
	}

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

	sys_irqEnable();

	com_fillColor();
	com_burst();

	t_start();
	com_burst();

	clk_sec = 0;

	Board board;
	Shape block;
	t_clearBoard(&board);
	t_makeBlock(&board, &block);
	

	while (1) {
		test(&board, &block);
	}
}



