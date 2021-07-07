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
	Drop,
	Rotate
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

typedef enum shpT {

	L,
	LL,
	T,
	I,
	Z,
	ZZ,
	B

} ShapeType;

typedef enum dir {
	N,
	E,
	S,
	W

} ShapeDir;

typedef struct shp {

	ShapeType type;
	ShapeDir dir;

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
void t_setLumByMem(char * loc, char brighness) {

	(*loc) = brighness;
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

	//zero out the game board
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
		t_setLumByMem(offset, 0xff);
		t_setPxByMem(offset, 0x00, 0x00, 0x00);
		offset += 4;
	}

}

void t_start() {

	t_clear();
	//t_setPx(0, 0, 0x00, 0x00, 0xFF);
	//t_setPx(9, 0, 0x00, 0x00, 0xFF);
	//t_setPx(9, 19, 0x00, 0x00, 0xFF);
	//t_setPx(0, 19, 0x00, 0x00, 0xFF);
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

#include "t_shapes.c"


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

//#define t_leftBtn 0x01
//#define t_centerBtn 0x02
//#define t_rightBtn 0x04

//when select is high
#define t_cBtn     0x80
#define t_bBtn     0x20
#define t_rightBtn 0x10
#define t_leftBtn  0x08
#define t_downBtn  0x04
#define t_upBtn    0x02

//when select is low
#define t_startBtn 0x80
#define t_aBtn     0x20


Action blockLogic(Action last, Board * board, Shape * block) {

	Shape bak;
	t_copyBlock(&bak, block);


	t_clearShape(block);

	via2_portA = 0x40;
	for (int i = 0; i < 2; i++) {} //nops
	char plr1H = via2_portA;

	via2_portA = 0x00;
	for (int i = 0; i < 2; i++) {} //nops
	char plr1L = via2_portA;

	//right left logic
	if (~plr1H & t_leftBtn) {
		t_moveLeft(block);
	}
	if (~plr1H & t_rightBtn) {
		t_moveRight(block);
	}

	if (~plr1L & t_aBtn) {
		t_rotate(block);
	}

	if (t_isWallHit(block)
	|| t_isCollided(board, block)) {
		t_copyBlock(block, &bak);	//rollback move
	} else {
		t_copyBlock(&bak, block);	//commit to new rollback location
	}


	if (last == Drop) 
	{
		t_setShape(block);
		return Rotate;
	}

	//down logic
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

	if (board->nextShape == L) {
		t_makeBlockL(block);
	} else if (board->nextShape == I) {
		t_makeBlockI(block);
	} else if (board->nextShape == T) {
		t_makeBlockT(block);
	} else if (board->nextShape == Z) {
		t_makeBlockZ(block);
	} else if (board->nextShape == ZZ) {
		t_makeBlockZZ(block);
	} else if (board->nextShape == LL) {
		t_makeBlockLL(block);
	} else {
		t_makeBlockB(block);
	}
	board->nextShape++;
	if (board->nextShape > B)
		board->nextShape = 0;

}



Action t_process(Action last, Board * board, Shape * block) {

	if (clk_jiff >= 10) {
		last = blockLogic(last, board, block);
		clk_jiff = 0;
		com_burst();
	}

	if (last == NewBlock) {
		t_makeBlock(board, block);
	}

	return last;
}

void t_main() {

	Board board;
	Shape block;
	t_clearBoard(&board);
	t_makeBlock(&board, &block);

	Action last = Drop;
	while (1) {
		last = t_process(last, &board, &block);
	}
}

