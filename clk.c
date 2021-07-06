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


