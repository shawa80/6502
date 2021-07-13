/**********************************************
Game pad
**********************************************/

void gp_init() {
	via2_DDRA = 0x40; //set to input, sel as output
	via2_DDRB = 0x40; //set to input, sel as output

	via2_portB = 0x40; //set to low 
	via2_portA = 0x40; //set to low
}

char gp_getPl1High() {

	via2_portA = 0x40;
        for (int i = 0; i < 2; i++) {} //nops
        char plr1H = via2_portA;

	return plr1H;
}

char gp_getPl1Low() {

        via2_portA = 0x00;
        for (int i = 0; i < 2; i++) {} //nops
        char plr1L = via2_portA;

	return plr1L;
}

/*
//T must be defined for this to work
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
		led_setPx(1, 15, 0xff, 0xff, 0xff);
	else
		led_setPx(1, 15, 0x00, 0x00, 0x00);

	if (high & 0x04)		//down
		led_setPx(1, 13, 0xff, 0xff, 0xff);
	else
		led_setPx(1, 13, 0x00, 0x00, 0x00);

	if (high & 0x08)		//left
		led_setPx(0, 14, 0xff, 0xff, 0xff);
	else
		led_setPx(0, 14, 0x00, 0x00, 0x00);


	if (high & 0x10)		//right
		led_setPx(2, 14, 0xff, 0xff, 0xff);
	else
		led_setPx(2, 14, 0x00, 0x00, 0x00);


	if (low & 0x20)		//a
		led_setPx(6, 13, 0xff, 0xff, 0xff);
	else
		led_setPx(6, 13, 0x00, 0x00, 0x00);

	if (high & 0x20)		//b
		led_setPx(7, 13, 0xff, 0xff, 0xff);
	else
		led_setPx(7, 13, 0x00, 0x00, 0x00);

	if (high & 0x80)		//c
		led_setPx(8, 13, 0xff, 0xff, 0xff);
	else
		led_setPx(8, 13, 0x00, 0x00, 0x00);


	if (low & 0x80)		//start
		led_setPx(4, 13, 0xff, 0xff, 0xff);
	else
		led_setPx(4, 13, 0x00, 0x00, 0x00);


}


void gp_test(Board * board) {

	if (clk_jiff >= 10) {
		gp_keyAction(board);
		clk_jiff = 0;
		com_burst();
	}

}
*/
