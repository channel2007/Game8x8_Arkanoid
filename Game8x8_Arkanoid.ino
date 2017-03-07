/*****************************************************************************
*  Game8x8-���j���C��.
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

// ø�Ϩ��.
Game8x8Graphics game8x8Graphics = Game8x8Graphics();
// �n����.
Game8x8JoyStick game8x8JoyStick = Game8x8JoyStick();
// ���ﾹ���.
Game8x8Tone		game8x8Tone = Game8x8Tone();

// ø�ϧ�s�ɯ�.
Game8x8Time TimeGraphicsUpdate = Game8x8Time();
// ���ʪ��l��s�ɯ�.
Game8x8Time TimeMovePaddle = Game8x8Time();
// ���ʲy��s�ɯ�.
Game8x8Time TimeMoveBall = Game8x8Time();

// �j��.
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
// �j���ƶq.
byte brickMax = 18;

// �y��l��m.
int x = 3;
int y = 6;
// ���ʹ���.
int dx = 1;
int dy = 1;


// ���l.
int  paddleBegin = 2;
int  paddleEnd = 4;

// �C�����A.
// 0:�o�y��.
// 1:�C����.
byte gameStatus = 0;

// ���Ķ}��.
bool soundSwitch = true;
 
//---------------------------------------------------------------------------
// ��l.
//---------------------------------------------------------------------------
void setup()
{
	Serial.begin(9600);
}

//---------------------------------------------------------------------------
// �D�j��.
//---------------------------------------------------------------------------
void loop()
{	
	// *���ʲy��s�ɯ�(60~180).
	if (TimeMoveBall.update(140) && gameStatus == 1) {
		// �P�_�y�I�j��.
		if (brick[y][x] == 1) {
			brick[y][x] = 2;
			brickMax--;
			// ���񭵮�.
			if (soundSwitch) {
				// �o�X�n��.
				game8x8Tone.playTone(262, 33);
			}
			// ���s�}�l�C��.
			if (brickMax == 0) {
				resetGame();
				return;
			}
			dy = -dy;			 // �ϼu
		}

		// �B�z�y����.
		if (x + dx > 7 || x + dx < 0) {
			dx = -dx;
		}
		if (y + dy < 0) {
			dy = -dy;
		}
		else if (y + dy > 7) {
			// �P�_�I�쩳�O�ϼu.
			if (x >= paddleBegin && x <= paddleEnd) {
				// ���񭵮�.
				if (soundSwitch) {
					// �o�X�n��.
					game8x8Tone.playTone(494, 33);
				}
				// �I����.
				if (x == paddleBegin) {
					x--;
				}
				// �I�k��.
				else if (x == paddleEnd) {
					x++;
				}
				dy = -dy;
			}
			// �S�I�쩳�O.
			else 
			{
				// �Ȱ��C��.
				gameStatus = 0;

				// �y��l��m.
				x = 3;y = 6;
				// ���l.
				paddleBegin = 2;
				paddleEnd = 4;
				return;
			}
		}
		x += dx;
		y += dy;
	}

	// *���ʪ��l��s�ɯ�.
	if (TimeMovePaddle.update(80)) {
		// ���ʪ��l.
		int mx = game8x8JoyStick.getJoyX();
		if (mx != 5) {
			// ���l�k��.
			if (mx < 5) {
				paddleBegin++; paddleEnd++;
				if (paddleBegin >= 5) { paddleBegin = 5; }
				if (paddleEnd >= 7) { paddleEnd= 7; }
			}
			// ���l����.
			else
			{
				paddleBegin--; paddleEnd--;
				if (paddleBegin <= 0) { paddleBegin = 0; }
				if (paddleEnd <= 2) { paddleEnd = 2; }
			}
		}
	}

	// *�o�y��.
	if (gameStatus == 0) {
		// �n�쩹�W�o�y.
		if (game8x8JoyStick.getJoyY() > 5) {
			dx = 1;
			dy = 1;
			gameStatus = 1;
		}else{
			// ���y��۪��l�].
			x = paddleBegin + 1;
		}
	}

	//Serial.print("Time:");
	//Serial.println(time);
	//--------------------------------------------------------------------
	// ø�ϧ�s�ɯ�-FPS 30.
	if (TimeGraphicsUpdate.update(33)) {

		// �M���e��.
		game8x8Graphics.clearDisplay(0);

		//--------------------------------------------------------------------
		// �e�j��.
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
		// ���ʲy.
		game8x8Graphics.setPixcls(x, y, 1);

		//--------------------------------------------------------------------
		// �e���l.
		for (byte i = paddleBegin; i <= paddleEnd; i++) {
			game8x8Graphics.setPixcls(i, 7, 1);
		}

		// ��s.
		game8x8Graphics.update();
	}
}

//---------------------------------------------------------------------------
// ���s�}�l�C��.
//---------------------------------------------------------------------------
void resetGame()
{
	// �Ȱ��C��.
	gameStatus = 0;

	// �y��l��m.
	x = 3; y = 6;

	// ���l.
	paddleBegin = 2;
	paddleEnd = 4;

	// ��l�j��.
	for (int y = 0; y<8; y++) {
		for (int x = 0; x<8; x++) {
			if (brick[y][x] == 2) {
				brick[y][x] = 1;
			}
		}
	}
	// �j���ƶq.
	brickMax = 18;
}