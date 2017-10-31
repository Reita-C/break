#include "DxLib.h"

#define RIGHT_WALL 400
#define FIELD_HEIGHT 480
#define BALL_SIZE 4
#define BALL_SPEED 5
#define BAR_SPEED 5
#define BAR_HEIGHT 10
#define BAR_WIDTH 70
#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 10
#define MAX_BLOCK 36
#define BLOCK_MAP_INDEX_X 6
#define BLOCK_MAP_INDEX_Y 6

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

struct BLOCKS {
	int x;
	int y;
	int flag;
	int color;
};

struct BLOCKS block;

int my_get_key(void);
int key[256];

int game;
int bar_x;
int bar_y;
int ball_x;
int ball_y;
int x_speed;
int y_speed;
int Color_White;
int Color_Red;
int Color_Green;

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
			game = 5;
			break;
		case 5:
			ball_collision();
			ball_move();
			ball_draw();
			line_draw();
			bar_draw();
			draw_block();
			bar_move();
			break;
		case 10:
			line_draw();
			bar_draw();
			draw_block();
			gameover();
			break;
		default:
			break;
		}
		
	}
	DxLib_End();
	return 0;
}

void variable(void) {
	game;
	bar_x = 0;
	bar_y = FIELD_HEIGHT - BAR_HEIGHT;
	ball_x = BAR_WIDTH / 2;
	ball_y = (FIELD_HEIGHT - BAR_HEIGHT) - BALL_SIZE;
	x_speed = BALL_SPEED;
	y_speed = BALL_SPEED;
	Color_White = GetColor(255, 255, 255);
	Color_Red = GetColor(255, 0, 0);
	Color_Green = GetColor(0, 255, 0);
}

void blocks_variable() {
	block.x = 100;
	block.y = 30;
	block.flag = 1;
	block.color = Color_Green;
}

void ball_collision() {
	if (ball_x >= RIGHT_WALL) {
		ball_x = RIGHT_WALL - BALL_SIZE;
		x_speed *= -1;
	}
	if (ball_x <= 0) {
		ball_x = BALL_SIZE;
		x_speed *= -1;
	}
	if (ball_y >= 480) {
		game = 10;
	}
	if (ball_y <= 0) {
		ball_y = BALL_SIZE;
		y_speed *= -1;
	}
	if (ball_x > bar_x && ball_x < (bar_x + BAR_WIDTH) && ball_y >= bar_y) {
		ball_y = bar_y - BALL_SIZE;
		y_speed *= -1;
	}
	if (block.flag == 1) {
		if (ball_x >= block.x && ball_x <= block.x + BLOCK_WIDTH) {
			if (ball_y >= block.y && ball_y <= block.y + 3) {
				ball_y = block.y;
				y_speed *= -1;
				block.flag = 0;
			}
			else if (ball_y <= block.y + BLOCK_HEIGHT && ball_y >= block.y + BLOCK_HEIGHT - 3) {
				ball_y = block.y + BLOCK_HEIGHT;
				y_speed *= -1;
				block.flag = 0;
			}
		}
		else if (ball_y >= block.y && ball_y <= block.y + BLOCK_HEIGHT) {
			if (ball_x >= block.x && ball_x <= block.x + 3) {
				ball_x = block.x;
				x_speed *= -1;
				block.flag = 0;
			}
			else if (ball_x <= block.x + BLOCK_WIDTH && ball_x >= block.x + BLOCK_WIDTH - 3) {
				ball_x = block.x + BLOCK_WIDTH;
				x_speed *= -1;
				block.flag = 0;
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
	if (block.flag == 1) {
		DrawBox(block.x, block.y, block.x + BLOCK_WIDTH, block.y + BLOCK_HEIGHT, block.color, true);
	}
}

void draw_field() {
	DrawBox(0, 0, RIGHT_WALL, FIELD_HEIGHT, Color_White, false);
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
