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

