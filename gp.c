/**********************************************
Game pad
**********************************************/

void gp_init() {
	via2_DDRA = 0x00; //set to input
	via2_DDRB = 0x00; //set to input
	//via2_portB = 0xAA; //set to 
	//via2_portA = 0xAA; //set to high
}

void gp_keyAction(Board * board) {

	if (via2_portA & 0x01)
		t_setPx(1, 1, 0xff, 0xff, 0xff);
	else
		t_setPx(1, 1, 0x00, 0x00, 0x00);

	if (via2_portA & 0x02)
		t_setPx(2, 1, 0xff, 0xff, 0xff);
	else
		t_setPx(2, 1, 0x00, 0x00, 0x00);

	if (via2_portA & 0x04)
		t_setPx(3, 1, 0xff, 0xff, 0xff);
	else
		t_setPx(3, 1, 0x00, 0x00, 0x00);

	if (via2_portA & 0x08)
		t_setPx(4, 1, 0xff, 0xff, 0xff);
	else
		t_setPx(4, 1, 0x00, 0x00, 0x00);

}


void gp_test(Board * board) {

	if (clk_jiff >= 10) {
		gp_keyAction(board);
		clk_jiff = 0;
		com_burst();
	}

}
