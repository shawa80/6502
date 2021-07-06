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



