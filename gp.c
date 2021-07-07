/**********************************************
Game pad
**********************************************/

void gp_init() {
	via2_DDRA = 0x40; //set to input, sel as output
	via2_DDRB = 0x40; //set to input, sel as output

	via2_portB = 0x40; //set to low 
	via2_portA = 0x40; //set to low
}

void gp_keyAction(Board * board) {

	via2_portA = 0x40;
	for(int i = 0; i<2;i++)
	{}
	char high = via2_portA;

	via2_portA = 0x00;
	for(int i = 0; i<2;i++)
	{}
	char low  = via2_portA;



	if (high & 0x02)		//up
		t_setPx(1, 15, 0xff, 0xff, 0xff);
	else
		t_setPx(1, 15, 0x00, 0x00, 0x00);

	if (high & 0x04)		//down
		t_setPx(1, 13, 0xff, 0xff, 0xff);
	else
		t_setPx(1, 13, 0x00, 0x00, 0x00);

	if (high & 0x08)		//left
		t_setPx(0, 14, 0xff, 0xff, 0xff);
	else
		t_setPx(0, 14, 0x00, 0x00, 0x00);


	if (high & 0x10)		//right
		t_setPx(2, 14, 0xff, 0xff, 0xff);
	else
		t_setPx(2, 14, 0x00, 0x00, 0x00);


	if (low & 0x20)		//a
		t_setPx(6, 13, 0xff, 0xff, 0xff);
	else
		t_setPx(6, 13, 0x00, 0x00, 0x00);

	if (high & 0x20)		//b
		t_setPx(7, 13, 0xff, 0xff, 0xff);
	else
		t_setPx(7, 13, 0x00, 0x00, 0x00);

	if (high & 0x80)		//c
		t_setPx(8, 13, 0xff, 0xff, 0xff);
	else
		t_setPx(8, 13, 0x00, 0x00, 0x00);


	if (low & 0x80)		//start
		t_setPx(4, 13, 0xff, 0xff, 0xff);
	else
		t_setPx(4, 13, 0x00, 0x00, 0x00);


}


void gp_test(Board * board) {

	if (clk_jiff >= 10) {
		gp_keyAction(board);
		clk_jiff = 0;
		com_burst();
	}

}
