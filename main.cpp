/*
*hとgでボールがバーを反射する角度が変えられます
*/
#include "DxLib.h"
#define mybox_Ly 430
#define mybox_Ry 440

void ball(void);
void mybox(void);
void atari(void);
void mykey_R(void);
void mykey_L(void);
void speedkey_R(void);
void speedkey_L(void);
void brock(int x);
void Hbrock(int x);
int top(int x, int y);

int all_get_key(void);

int in[24];
int Hin[24];
int boxLx[24] = { 0, 81, 161, 241, 321, 401, 0, 81, 161, 241, 321, 401, 0, 81, 161, 241, 321, 401 , 0, 81, 161, 241, 321, 401 };
int boxLy[24] = { 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 11, 41, 41, 41, 41, 41, 41, 51, 51, 51, 51, 51, 51 };
int boxRx[24] = { 80, 160, 240, 320, 400, 480, 80, 160, 240, 320, 400, 480 , 80, 160, 240, 320, 400, 480, 80, 160, 240, 320, 400, 480 };
int boxRy[24] = { 10, 10, 10 ,10 ,10 ,10 ,20 ,20 ,20 ,20 ,20 ,20, 50, 50, 50, 50, 50, 50, 60, 60, 60, 60, 60, 60 };
int HboxLy[24] = { 21, 21, 21, 21, 21, 21, 31, 31, 31, 31, 31, 31, 61, 61, 61, 61, 61, 61, 71, 71, 71, 71, 71, 71 };
int HboxRy[24] = { 30, 30, 30, 30, 30, 30, 40, 40, 40, 40, 40, 40, 70, 70, 70, 70, 70, 70, 80, 80, 80, 80, 80, 80 };
int key[256];
int gamecount;
double ball_x = 0;
double ball_y = 430;
int x_houkou = 0;
int y_houkou = 0;
int mybox_Lx = 0;
int mybox_Rx = 60;
int move;
int a = 0;
double ballspeed = 3;
double kakudo = 0;
int x_boxhoukou = 0;
int y_boxhoukou = 0;
int total = 0;
int life = 3;
int first = 0;

int Color_White = GetColor(255, 255, 255);


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	for (int i = 0; i < 1000; i++) {

		while (ScreenFlip() == 0 && ProcessMessage() == 0
			&& ClearDrawScreen() == 0 && all_get_key() == 0 && CheckHitKey(KEY_INPUT_I) == 0) {

			double ball_x = 0;
			double ball_y = 430;

			life = 3;
			ballspeed = 3;

			if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {
				life = 0;
			}

			for (int y = 0; y < 24; y++) {
				in[y] = 0;
				Hin[y] = 0;
			}


			for (int i = 0; i < 24; i++) { brock(i); }
			for (int j = 0; j < 24; j++) { Hbrock(j); }
			//DrawBox(0, 120, 480, 350, GetColor(255, 255, 255), TRUE);
			mybox();
			mykey_R();
			speedkey_R();
			mykey_L();
			speedkey_L();
			DrawLine(480, 0, 480, 480, Color_White, TRUE);
			SetFontSize(15);
			SetFontThickness(10);
			ChangeFont("MS 明朝");
			DrawFormatString(490, 10, Color_White, "あなたのスコアは");
			DrawFormatString(490, 30, GetColor(255, 0, 0), "%d　です。", total);

			DrawFormatString(490, 60, Color_White, "あなたのライフは");
			DrawFormatString(490, 80, GetColor(255, 0, 0), "%d　です。", life);

			DrawFormatString(510, 120, GetColor(255, 255, 255), " TopPlayer");
			DrawFormatString(490, 140, GetColor(255, 255, 255), "       %d", first); 

		}

		while (ScreenFlip() == 0 && ProcessMessage() == 0
			&& ClearDrawScreen() == 0 && all_get_key() == 0 && life > 0) {

			if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {
				life = 0;
			}

			ballspeed += 0.001;

			total = 0;

			for (int y = 0; y < 24; y++) {
				total += in[y] + Hin[y];
			}

			if (ballspeed >= 15) {
				ballspeed -= 0.001;
			}

			mybox();
			mykey_R();
			speedkey_R();
			mykey_L();
			speedkey_L();
			for (int i = 0; i < 24; i++) { brock(i); }
			for (int j = 0; j < 24; j++) { Hbrock(j); }
			
			DrawLine(480, 0, 480, 480, Color_White, TRUE);
			atari();
			ball();
			//DrawBox(0, 120, 480, 350, GetColor(255, 255, 255), TRUE);

			DrawFormatString(490, 10, Color_White, "あなたのスコアは");
			DrawFormatString(490, 30, GetColor(255, 0, 0), "%d　です。", total);

			DrawFormatString(490, 60, Color_White, "あなたのライフは");
			DrawFormatString(490, 80, GetColor(255, 0, 0), "%d　です。", life);

			DrawFormatString(510, 120, GetColor(255, 255, 255), " TopPlayer");
			DrawFormatString(490, 140, GetColor(255, 255, 255), "       %d", first);
			
			if (ball_y >= 480) {
				life--;
			}
			
			if (life == 0) {
			
				SetFontSize(64);
				SetFontThickness(8);
				ChangeFont("MS 明朝");
				DrawString(100, 200, "ゲームオーバー", GetColor(255, 0, 0));
				
			}		
		}
		first = top(total, first);
	}

	WaitKey();
	DxLib_End();
	return 0;
}

int top(int x, int y) {

	if (x > y) {
		return x;
	}
	else {
		return y;
	}
}

void brock(int x) {

	if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y <= boxRy[x] && ball_y >= boxLy[x] && y_houkou == 0 && in[x] == 0) {
		y_houkou = 1;
		DrawBox(boxLx[x], boxLy[x], boxRx[x], boxRy[x], GetColor(255, 0, 0), TRUE);
		in[x]++;
	}
	else if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y == boxRy[x] && ball_y >= 0 && y_houkou == 1 && in[x] == 0) {
		y_houkou = 0;
		DrawBox(boxLx[x], boxLy[x], boxRx[x], boxRy[x], GetColor(255, 0, 0), TRUE);
		in[x]++;
	}
	else if (in[x] == 0) {
		DrawBox(boxLx[x], boxLy[x], boxRx[x], boxRy[x], GetColor(255, 0, 0), TRUE);
	}
	else {
		DrawBox(boxLx[x], boxLy[x], boxRx[x], boxRy[x], GetColor(0, 0, 0), TRUE);
	}
}

void mybox(void) {

	DrawBox(mybox_Lx + move, mybox_Ly, mybox_Rx + move, mybox_Ry, GetColor(0, 255, 0), TRUE);

}

void ball(void) {

	DrawCircle(ball_x, ball_y, 3, GetColor(255, 0, 0));

	if (x_houkou == 0) {
		ball_x = ball_x + ballspeed + kakudo;
	}
	else {
		ball_x = ball_x - ballspeed - kakudo;
	}

	if (y_houkou == 0) {
		ball_y = ball_y - ballspeed;
	}

	else {
		ball_y = ball_y + ballspeed;
	}

	if (ball_x >= 480) {
		x_houkou = 1;
	}

	if (ball_x <= 0) {
		x_houkou = 0;
	}

	if (ball_y >= 480) {
		y_houkou = 0;
	}

	if (ball_y <= 0) {
		y_houkou = 1;
	}
}

void atari(void) {

	if (ball_x <= mybox_Rx + move && ball_x >= mybox_Lx + move && ball_y >= mybox_Ly && ball_y <= mybox_Ry && CheckHitKey(KEY_INPUT_G) > 0 && x_houkou == 0) {
		y_houkou = 0;
		kakudo -= 0.7;
	}
	else if (ball_x <= mybox_Rx + move && ball_x >= mybox_Lx + move && ball_y >= mybox_Ly && ball_y <= mybox_Ry && CheckHitKey(KEY_INPUT_G) > 0 && x_houkou == 1) {
		y_houkou = 0;
		kakudo += 0.7;
	}
	else if (ball_x <= mybox_Rx + move && ball_x >= mybox_Lx + move && ball_y >= mybox_Ly && ball_y <= mybox_Ry && CheckHitKey(KEY_INPUT_H) > 0 && x_houkou == 0) {
		y_houkou = 0;
		kakudo += 0.7;
	}
	else if (ball_x <= mybox_Rx + move && ball_x >= mybox_Lx + move && ball_y >= mybox_Ly && ball_y <= mybox_Ry && CheckHitKey(KEY_INPUT_H) > 0 && x_houkou == 1) {
		y_houkou = 0;
		kakudo -= 0.7;
	}
	else if (ball_x <= mybox_Rx + move && ball_x >= mybox_Lx + move && ball_y >= mybox_Ly && ball_y <= mybox_Ry) {
		y_houkou = 0;
	}
}

void mykey_R(void) {

	if ((mybox_Rx + move) <= 475) {
		if (CheckHitKey(KEY_INPUT_RIGHT) > 0) {
			//RIGTHを押されたら右に動く
			move += 5;
		}
	}
}

void speedkey_R(void) {

	if ((mybox_Rx + move) <= 475) {
		if (CheckHitKey(KEY_INPUT_RIGHT) > 0 && CheckHitKey(KEY_INPUT_A) > 0) {
			//Aを押されたら右に動く
			move += 10;
		}
	}
}

void mykey_L(void) {

	if ((mybox_Lx + move) >= 0) {
		if (CheckHitKey(KEY_INPUT_LEFT) > 0) {
			//LEFTを押されたら左に動く
			move -= 5;
		}
	}
}

void speedkey_L(void) {

	if ((mybox_Lx + move) >= 0) {
		if (CheckHitKey(KEY_INPUT_LEFT) > 0 && CheckHitKey(KEY_INPUT_A) > 0) {
			//Aを押されたら左に動く
			move -= 10;
		}
	}
}

void Hbrock(int x) {

	if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y <= HboxRy[x] && ball_y >= HboxLy[x] && y_houkou == 0 && Hin[x] == 0) {
		y_houkou = 1;
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(0, 255, 0), TRUE);
		Hin[x]++;
	}
	else if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y <= HboxRy[x] && ball_y >= HboxLy[x] && y_houkou == 1 && Hin[x] == 0) {
		y_houkou = 0;
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(0, 255, 0), TRUE);
		Hin[x]++;
	}
	else if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y <= HboxRy[x] && ball_y >= HboxLy[x] && y_houkou == 0 && Hin[x] == 1) {
		y_houkou = 1;
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(255, 0, 0), TRUE);
		Hin[x]++;
	}
	else if (ball_x >= boxLx[x] && ball_x <= boxRx[x] && ball_y <= HboxRy[x] && ball_y >= HboxLy[x] && y_houkou == 1 && Hin[x] == 1) {
		y_houkou = 0;
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(255, 0, 0), TRUE);
		Hin[x]++;
	}
	else if (Hin[x] == 0) {
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(0, 255, 0), TRUE);
	}
	else if (Hin[x] == 1) {
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(255, 0, 0), TRUE);
	}
	else if (Hin[x] == 2) {
		DrawBox(boxLx[x], HboxLy[x], boxRx[x], HboxRy[x], GetColor(0, 0, 0), TRUE);
		for (int y = 0; y < 24; y++) {
		}
	}
}

int all_get_key() {

	char keys[256];
	int i;
	GetHitKeyStateAll(keys);

	for (i = 0; i < 256; i++) {
		if (keys[i] != 0) {
			key[i]++;
		}
		else {
			key[i] = 0;
		}
	}
	return 0;
}
