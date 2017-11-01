#include "DxLib.h"

#define RIGHT_WALL 400
#define FIELD_HEIGHT 480
#define BALL_SIZE 4
#define BALL_SPEED 5
#define BAR_SPEED 5
#define BAR_HEIGHT 10
#define BAR_WIDTH 50
#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 10
#define MAX_BLOCK 48
#define BLOCK_MAP_INDEX_X 8
#define BLOCK_MAP_INDEX_Y 6
#define COLLISION_SIZE 3

void variable(void);
void ball_collision(void);
void ball_move(void);
void ball_draw(void);
void line_draw(void);
void bar_draw(void);
void bar_move(void);
void gameover(void);
void draw_block(void);
void blocks_variable(void);
void scorebox(void);
void op(void);
void ed(void);
void count_block(void);

struct BLOCKS {
	int x;
	int y;
	int flag;
	int color;
};

struct BLOCKS blocks[MAX_BLOCK];

int block_map[BLOCK_MAP_INDEX_Y][BLOCK_MAP_INDEX_X] = {
	{ 1,2,3,1,2,3,1,2 },
	{ 1,2,3,1,2,3,1,2 },
	{ 1,2,3,1,2,3,1,2 },
	{ 1,2,3,1,2,3,1,2 },
	{ 1,2,3,1,2,3,1,2 },
	{ 1,2,3,1,2,3,1,2 }
};


int my_get_key(void);
int key[256];

int game;
int score;
int bar_x;
int bar_y;
int ball_x;
int ball_y;
int x_speed;
int y_speed;
int Color_White;
int Color_Black;
int Color_Red;
int Color_Green;
int Color_Blue;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	variable();
	blocks_variable();

	while (ScreenFlip() == 0 && ProcessMessage() == 0
		&& ClearDrawScreen() == 0 && my_get_key() == 0) {

		switch (game) {
		case 0:
			line_draw();
			draw_block();
			ball_draw();
			bar_draw();
			op();
			if (key[KEY_INPUT_L] == 1) {
				game = 5;
			}
			break;
		case 5:
			ball_collision();
			ball_move();
			ball_draw();
			scorebox();
			line_draw();
			bar_draw();
			draw_block();
			bar_move();
			count_block();
			break;
		case 10:
			line_draw();
			bar_draw();
			draw_block();
			scorebox();
			gameover();
			if (key[KEY_INPUT_L] == 1) {
				game = 0;
				variable();
				blocks_variable();
			}
			break;
		case 15:
			line_draw();
			draw_block();
			ball_draw();
			bar_draw();
			scorebox();
			ed();
			if (key[KEY_INPUT_L] == 1) {
				game = 0;
				variable();
				blocks_variable();
			}
			break;
		default:
			break;
		}
		
	}
	DxLib_End();
	return 0;
}

void variable(void) {
	game = 0;
	score = 0;
	bar_x = 0;
	bar_y = FIELD_HEIGHT - BAR_HEIGHT;
	ball_x = BAR_WIDTH / 2;
	ball_y = (FIELD_HEIGHT - BAR_HEIGHT) - BALL_SIZE;
	x_speed = BALL_SPEED;
	y_speed = BALL_SPEED + 2;
	Color_White = GetColor(255, 255, 255);
	Color_Black = GetColor(0, 0, 0);
	Color_Red = GetColor(255, 0, 0);
	Color_Green = GetColor(0, 255, 0);
	Color_Blue = GetColor(0, 0, 255);
}

void blocks_variable() {
	int i, j;
	int count = 0;
	for (i = 0; i < BLOCK_MAP_INDEX_Y; i++) {
		for (j = 0; j < BLOCK_MAP_INDEX_X; j++) {
			if (block_map[i][j] != 0) {
				blocks[count].x = (BLOCK_WIDTH * j);
				blocks[count].y = (BLOCK_HEIGHT * i);
				blocks[count].flag = 1;
				switch (block_map[i][j]) {
				case 1:
					blocks[count].color = Color_Red;
					break;
				case 2:
					blocks[count].color = Color_Green;
					break;
				case 3:
					blocks[count].color = Color_Blue;
					break;
				default:
					break;
				}
			}
			count++;
		}
	}
}

void ball_collision() {
	int count = 0;

	if (ball_x >= RIGHT_WALL) {
		ball_x = RIGHT_WALL - BALL_SIZE;
		x_speed *= -1;
	}
	if (ball_x <= 0) {
		ball_x = BALL_SIZE;
		x_speed *= -1;
	}
	if (ball_y <= 0) {
		ball_y = BALL_SIZE;
		y_speed *= -1;
	}
	if (ball_y >= FIELD_HEIGHT) {
		game = 10;
	}
	if (ball_x > bar_x && ball_x < (bar_x + BAR_WIDTH) && ball_y >= bar_y) {
		ball_y = bar_y - BALL_SIZE;
		y_speed *= -1;
	}
	for (count = 0; count < MAX_BLOCK; count++) {
		if (blocks[count].flag == 1) {
			if (ball_x >= blocks[count].x && ball_x <= blocks[count].x + BLOCK_WIDTH) {
				if (ball_y >= blocks[count].y && ball_y <= blocks[count].y + COLLISION_SIZE) {
					ball_y = blocks[count].y;
					y_speed *= -1;
					blocks[count].flag = 0;
					score++;
					break;
				}
				else if (ball_y <= blocks[count].y + BLOCK_HEIGHT && ball_y >= blocks[count].y + BLOCK_HEIGHT - COLLISION_SIZE) {
					ball_y = blocks[count].y + BLOCK_HEIGHT;
					y_speed *= -1;
					blocks[count].flag = 0;
					score++;
					break;
				}
			}

			if (ball_y >= blocks[count].y && ball_y <= blocks[count].y + BLOCK_HEIGHT) {
				if (ball_x >= blocks[count].x && ball_x <= blocks[count].x + COLLISION_SIZE) {
					ball_x = blocks[count].x;
					x_speed *= -1;
					blocks[count].flag = 0;
					score++;
					break;
				}
				else if (ball_x <= blocks[count].x + BLOCK_WIDTH && ball_x >= blocks[count].x + BLOCK_WIDTH - COLLISION_SIZE) {
					ball_x = blocks[count].x + BLOCK_WIDTH;
					x_speed *= -1;
					blocks[count].flag = 0;
					score++;
					break;
				}
			}
		}
	}
}

void ball_move() {
	ball_x += x_speed;
	ball_y += y_speed;
}

void ball_draw() {
	DrawCircle(ball_x, ball_y, BALL_SIZE, Color_White, false);
}

void line_draw() {
	DrawLine(RIGHT_WALL + BALL_SIZE, 0, RIGHT_WALL + BALL_SIZE, 480, Color_White);
}

void bar_draw() {
	DrawBox(bar_x, bar_y, bar_x + BAR_WIDTH, bar_y + BAR_HEIGHT, Color_Red, true);
}

void bar_move() {
	if (key[KEY_INPUT_RIGHT] > 0 && (bar_x + BAR_WIDTH) < RIGHT_WALL) {
		bar_x += BAR_SPEED;
	}
	if (key[KEY_INPUT_LEFT] > 0 && bar_x > 0) {
		bar_x -= BAR_SPEED;
	}
}

void gameover() {
	DrawFormatString(170, 240, Color_White, "GAME OVER");
}

void draw_block() {
	int count = 0;

	for (count = 0; count < MAX_BLOCK; count++) {
		if (blocks[count].flag == 1) {
			DrawBox(blocks[count].x, blocks[count].y, blocks[count].x + BLOCK_WIDTH,
				blocks[count].y + BLOCK_HEIGHT, blocks[count].color, true);
			DrawBox(blocks[count].x, blocks[count].y, blocks[count].x + BLOCK_WIDTH,
				blocks[count].y + BLOCK_HEIGHT, Color_Black, false);
		}
	}
}

void draw_field() {
	DrawBox(0, 0, RIGHT_WALL, FIELD_HEIGHT, Color_White, false);
}

void scorebox() {
	DrawFormatString(460, 20, Color_White, "■BREAK BLOCK■");
	DrawFormatString(495, 60, Color_White, "STAGE");
	DrawFormatString(515, 80, Color_White, "1");
	DrawFormatString(495, 100, Color_White, "SCORE");
	DrawFormatString(510, 120, Color_White, "%d", score);
}

void op() {
	DrawFormatString(150, 210, Color_White, "THE BREAK BLOCK");
	DrawFormatString(155, 230, Color_White, "PLEASE PUSH L");
}

void ed() {
	DrawFormatString(145, 210, Color_White, "Congratulations");
}

void count_block() {
	int count;

	for (count = 0; count < MAX_BLOCK; count++) {
		if (blocks[count].flag == 1) {
			break;
		}
	}
	if (count == MAX_BLOCK) {
		game = 15;
	}
}

int my_get_key() {
	char keys[256];
	GetHitKeyStateAll(keys);
	for (int i = 0; i < 256; i++) {
		if (keys[i] != 0) {
			key[i]++;
		}
		else {
			key[i] = 0;
		}
	}
	return 0;
}
