/*****************************************************************************
*  Game8x8-打磚塊遊戲.
*
* created  by channel
*
* todo list:
*
* note:
*
****************************************************************************/
#include "Game8x8Tone.h"
#include "Game8x8JoyStick.h"
#include "Game8x8Time.h"
#include "Game8x8Graphics.h"

// 繪圖函數.
Game8x8Graphics game8x8Graphics = Game8x8Graphics();
// 搖桿函數.
Game8x8JoyStick game8x8JoyStick = Game8x8JoyStick();
// 蜂鳴器函數.
Game8x8Tone		game8x8Tone = Game8x8Tone();

// 繪圖更新時脈.
Game8x8Time TimeGraphicsUpdate = Game8x8Time();
// 移動版子更新時脈.
Game8x8Time TimeMovePaddle = Game8x8Time();
// 移動球更新時脈.
Game8x8Time TimeMoveBall = Game8x8Time();

// 磚塊.
byte brick[8][8] = {
	{ 0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
};
// 磚塊數量.
byte brickMax = 18;

// 球初始位置.
int x = 3;
int y = 6;
// 移動像素.
int dx = 1;
int dy = 1;


// 版子.
int  paddleBegin = 2;
int  paddleEnd = 4;

// 遊戲狀態.
// 0:發球中.
// 1:遊戲中.
byte gameStatus = 0;

// 音效開關.
bool soundSwitch = true;
 
//---------------------------------------------------------------------------
// 初始.
//---------------------------------------------------------------------------
void setup()
{
	Serial.begin(9600);
}

//---------------------------------------------------------------------------
// 主迴圈.
//---------------------------------------------------------------------------
void loop()
{	
	// *移動球更新時脈(60~180).
	if (TimeMoveBall.update(140) && gameStatus == 1) {
		// 判斷球碰磚塊.
		if (brick[y][x] == 1) {
			brick[y][x] = 2;
			brickMax--;
			// 播放音效.
			if (soundSwitch) {
				// 發出聲音.
				game8x8Tone.playTone(262, 33);
			}
			// 重新開始遊戲.
			if (brickMax == 0) {
				resetGame();
				return;
			}
			dy = -dy;			 // 反彈
		}

		// 處理球移動.
		if (x + dx > 7 || x + dx < 0) {
			dx = -dx;
		}
		if (y + dy < 0) {
			dy = -dy;
		}
		else if (y + dy > 7) {
			// 判斷碰到底板反彈.
			if (x >= paddleBegin && x <= paddleEnd) {
				// 播放音效.
				if (soundSwitch) {
					// 發出聲音.
					game8x8Tone.playTone(494, 33);
				}
				// 碰左角.
				if (x == paddleBegin) {
					x--;
				}
				// 碰右角.
				else if (x == paddleEnd) {
					x++;
				}
				dy = -dy;
			}
			// 沒碰到底板.
			else 
			{
				// 暫停遊戲.
				gameStatus = 0;

				// 球初始位置.
				x = 3;y = 6;
				// 版子.
				paddleBegin = 2;
				paddleEnd = 4;
				return;
			}
		}
		x += dx;
		y += dy;
	}

	// *移動版子更新時脈.
	if (TimeMovePaddle.update(80)) {
		// 移動版子.
		int mx = game8x8JoyStick.getJoyX();
		if (mx != 5) {
			// 版子右移.
			if (mx < 5) {
				paddleBegin++; paddleEnd++;
				if (paddleBegin >= 5) { paddleBegin = 5; }
				if (paddleEnd >= 7) { paddleEnd= 7; }
			}
			// 版子左移.
			else
			{
				paddleBegin--; paddleEnd--;
				if (paddleBegin <= 0) { paddleBegin = 0; }
				if (paddleEnd <= 2) { paddleEnd = 2; }
			}
		}
	}

	// *發球中.
	if (gameStatus == 0) {
		// 搖桿往上發球.
		if (game8x8JoyStick.getJoyY() > 5) {
			dx = 1;
			dy = 1;
			gameStatus = 1;
		}else{
			// 讓球跟著版子跑.
			x = paddleBegin + 1;
		}
	}

	//Serial.print("Time:");
	//Serial.println(time);
	//--------------------------------------------------------------------
	// 繪圖更新時脈-FPS 30.
	if (TimeGraphicsUpdate.update(33)) {

		// 清除畫面.
		game8x8Graphics.clearDisplay(0);

		//--------------------------------------------------------------------
		// 畫磚塊.
		for (int y = 0; y<8; y++) {
			for (int x = 0; x<8; x++) {
				if (brick[y][x] == 1) {
					game8x8Graphics.setPixcls(x, y, 1);
				}
				else {
					game8x8Graphics.setPixcls(x, y, 0);
				}
			}
		}

		//--------------------------------------------------------------------
		// 移動球.
		game8x8Graphics.setPixcls(x, y, 1);

		//--------------------------------------------------------------------
		// 畫版子.
		for (byte i = paddleBegin; i <= paddleEnd; i++) {
			game8x8Graphics.setPixcls(i, 7, 1);
		}

		// 更新.
		game8x8Graphics.update();
	}
}

//---------------------------------------------------------------------------
// 重新開始遊戲.
//---------------------------------------------------------------------------
void resetGame()
{
	// 暫停遊戲.
	gameStatus = 0;

	// 球初始位置.
	x = 3; y = 6;

	// 版子.
	paddleBegin = 2;
	paddleEnd = 4;

	// 初始磚塊.
	for (int y = 0; y<8; y++) {
		for (int x = 0; x<8; x++) {
			if (brick[y][x] == 2) {
				brick[y][x] = 1;
			}
		}
	}
	// 磚塊數量.
	brickMax = 18;
}