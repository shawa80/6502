


extern char* led_px[10][20];



void led_setLum(int x, int y, char brighness) {

	(*led_px[x][y]) = brighness;
}
void led_setLumByMem(char * loc, char brighness) {

	(*loc) = brighness;
}

void led_setPx(int x, int y, char r, char g, char b) {

	char* loc = led_px[x][y];
	loc++;
	(*loc) = b;
	loc++;
	(*loc) = g;
	loc++;
	(*loc) = r;

}
void led_setPxByMem(char * loc, char r, char g, char b) {

	loc++;
	(*loc) = b;

	loc++;
	(*loc) = g;

	loc++;
	(*loc) = r;
}

void led_cpy(int dx, int dy, int sx, int sy) {

	char* loc = led_px[sx][sy];
	loc++;
	char b = (*loc);
	loc++;
	char g = (*loc);
	loc++;
	char r = (*loc);

	led_setPx(dx, dy, r, g, b);
}


void led_clear() {

	char * offset = com_buf_px;
	for (int i = 0; i < 200; i++) {
		led_setLumByMem(offset, 0xff);
		led_setPxByMem(offset, 0x00, 0x00, 0x00);
		offset += 4;
	}

}

void led_init() {

	int o = 0;
	int y;

	y = 0;
	while(y < 20) {
		led_px[0][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		led_px[1][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		led_px[2][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		led_px[3][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		led_px[4][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		led_px[5][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		led_px[6][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		led_px[7][y] = &com_buf_px[o];
		y--;
		o+=4;
	}
	y = 0;
	while(y < 20) {
		led_px[8][y] = &com_buf_px[o];
		y++;
		o+=4;
	}
	y = 19;
	while(y >= 0) {
		led_px[9][y] = &com_buf_px[o];
		y--;
		o+=4;
	}

}

