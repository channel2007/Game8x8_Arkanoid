/*****************************************************************************
*  8x8���ﾹ.
*
*  created  by channel
*
*  todo list:
*
*  note:
****************************************************************************/

#include "Game8x8Tone.h"

//---------------------------------------------------------------------------
// �غc��.
//---------------------------------------------------------------------------
Game8x8Tone::Game8x8Tone() {
	pinMode( SP_PIN, OUTPUT);
}

//---------------------------------------------------------------------------
// ���񭵮�.
//---------------------------------------------------------------------------
void Game8x8Tone::playTone( unsigned int frequency, unsigned long duration = 0) {
	tone( SP_PIN, frequency, duration);
}