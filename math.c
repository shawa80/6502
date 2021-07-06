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

