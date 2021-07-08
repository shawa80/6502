

void xy(Shape * block, int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy) {
	block->a.x = ax;
	block->a.y = ay;

	block->b.x = bx;
	block->b.y = by;

	block->c.x = cx;
	block->c.y = cy;

	block->d.x = dx;
	block->d.y = dy;
}


void t_moveBlockAbs(Shape * block, int x, int y)
{
	block->a.x = block->a.x + x;
	block->a.y = block->a.y + y;

	block->b.x = block->b.x + x;
	block->b.y = block->b.y + y;

	block->c.x = block->c.x + x;
	block->c.y = block->c.y + y;

	block->d.x = block->d.x + x;
	block->d.y = block->d.y + y;
}


void t_makeBlockIr(Shape * block, ShapeDir dir) {

	if (dir == N || dir == S)
		xy(block, 0, 1, 0, 0, 0, 2, 0, -1);
	if (dir == E || dir == W)
		xy(block, -1, 0, 0, 0, 1, 0, 2, 0);
}



void t_makeBlockBr(Shape * block, ShapeDir dir) {

	xy(block, 0, 1, 0, 0, 1, 1, 1, 0); 
}

void t_makeBlockZZr(Shape * block, ShapeDir dir) {

	if (dir == N)
		xy(block, 0, 1, 0, 0, 1, 1, -1, 0); //ZZ
	if (dir == E)
		xy(block, 0, 1, 0, 0, 1, 0, 1, -1);
	if (dir == S)
		xy(block, 1, 0, 0, 0, -1, -1, 0, -1);
	if (dir == W)
		xy(block, -1, 1, 0, 0, -1, 0, 0, -1);
}

void t_makeBlockZr(Shape * block, ShapeDir dir) {

	if (dir == N)
		xy(block, -1, 1, 0, 0, 0, 1, 1, 0); //Z
	if (dir == E)
		xy(block, 1, 1, 0, 0, 1, 0, 0, -1);
	if (dir == S)
		xy(block, -1, 0, 0, 0, 0, -1, 1, -1);
	if (dir == W)
		xy(block, 0, 1, 0, 0, -1, 0, -1, -1);
}

void t_makeBlockTr(Shape * block, ShapeDir dir) {

	if (dir == N)
		xy(block, -1, 0, 0, 0, 0, 1, 1, 0); //T
	if (dir == E)
		xy(block, 0, 1, 0, 0, 1, 0, 0, -1);
	if (dir == S)
		xy(block, -1, 0, 0, 0, 1, 0, 0, -1);
	if (dir == W)
		xy(block, 0, 1, 0, 0, -1, 0, 0, -1);
}

void t_makeBlockLLr(Shape * block, ShapeDir dir) {

	if (dir == N)
		xy(block, -1, 1, 0, 0, -1, 0, 1, 0); //LL
	if (dir == E)
		xy(block, 0, 1, 0, 0, 1, 1, 0, -1);
	if (dir == S)
		xy(block, -1, 0, 0, 0, 1, 0, 1, -1);
	if (dir == W)
		xy(block, 0, 1, 0, 0, 0, -1, -1, -1);

}
void t_makeBlockLr(Shape * block, ShapeDir dir) {

	if (dir == N)
		xy(block, -1, 0, 0, 0, 1, 0, 1, 1); //L
	if (dir == E)
		xy(block, 0, 1, 0, 0, 0, -1, 1, -1);
	if (dir == S)
		xy(block, -1, 0, 0, 0, 1, 0, -1, -1);
	if (dir == W)
		xy(block, -1, 1, 0, 0, 0, 1, 0, -1);

}

void t_makeBlockI(Shape * block) {

        block->type = I;
        block->dir = N;

	t_makeBlockIr(block, block->dir);
	t_moveBlockAbs(block, 5, 17);
}
void t_makeBlockLL(Shape * block) {

        block->type = LL;
        block->dir = N;

	t_makeBlockLLr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}
void t_makeBlockL(Shape * block) {

        block->type = L;
        block->dir = N;

	t_makeBlockLr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}
void t_makeBlockZZ(Shape * block) {

        block->type = ZZ;
        block->dir = N;

	t_makeBlockZZr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}
void t_makeBlockT(Shape * block) {

        block->type = T;
        block->dir = N;

	t_makeBlockTr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}
void t_makeBlockZ(Shape * block) {

        block->type = Z;
        block->dir = N;

	t_makeBlockZr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}
void t_makeBlockB(Shape * block) {

        block->type = B;
        block->dir = N;

	t_makeBlockBr(block, block->dir);
	t_moveBlockAbs(block, 5, 18);
}

void t_rotate(Shape * block) {

	int x = block->b.x;
	int y = block->b.y;

	block->dir++;
	if (block->dir > W)
		block->dir = N;

	if (block->type == I)
		t_makeBlockIr(block, block->dir);
	if (block->type == L)
		t_makeBlockLr(block, block->dir);
	if (block->type == Z)
		t_makeBlockZr(block, block->dir);
	if (block->type == T)
		t_makeBlockTr(block, block->dir);
	if (block->type == B)
		t_makeBlockBr(block, block->dir);
	if (block->type == ZZ)
		t_makeBlockZZr(block, block->dir);
	if (block->type == LL)
		t_makeBlockLLr(block, block->dir);


	t_moveBlockAbs(block, x, y);
}
