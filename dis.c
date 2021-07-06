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

