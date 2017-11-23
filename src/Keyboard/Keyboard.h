#include "main_header.h"

#ifndef KEYBOARD_H_
#define KEYBOARD_H_


static bool A_KEY;
static bool B_KEY;
static bool C_KEY;
static bool D_KEY;
static bool E_KEY;
static bool F_KEY;
static bool G_KEY;
static bool H_KEY;
static bool I_KEY;
static bool J_KEY;
static bool K_KEY;
static bool L_KEY;
static bool M_KEY;
static bool N_KEY;
static bool O_KEY;
static bool P_KEY;
static bool Q_KEY;
static bool R_KEY;
static bool S_KEY;
static bool T_KEY;
static bool U_KEY;
static bool V_KEY;
static bool W_KEY;
static bool X_KEY;
static bool Y_KEY;
static bool Z_KEY;

static bool _1_KEY;
static bool _2_KEY;
static bool _3_KEY;
static bool _4_KEY;
static bool _5_KEY;
static bool _6_KEY;
static bool _7_KEY;
static bool _8_KEY;
static bool _9_KEY;
static bool _0_KEY;

static unsigned char cInput;

static void ReleaseKey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			W_KEY = false;
			break;
		case 's':
			S_KEY = false;
			break;
		case 'a':
			A_KEY = false;
			break;
		case 'd':
			D_KEY = false;
			break;
		case 'g':
			G_KEY = false;
			break;
		case 'h':
			H_KEY = false;
			break;
		case 'z':
			Z_KEY = false;
			break;
		case 'b':
			B_KEY = false;
			break;
		case 'c':
			C_KEY = false;
			break;
		case 'q':
			Q_KEY = false;
			break;
		case 'e':
			E_KEY = false;
			break;
	}
}

static void PressKey(unsigned char key, int x, int y)
{	

	switch (key)
	{
		case 'w':
			W_KEY = true;
			break;
		case 's':
			S_KEY = true;
			break;
		case 'a':
			A_KEY = true;
			break;
		case 'd':
			D_KEY = true;
			break;
		case 'g':
			G_KEY = true;
			break;
		case 'h':
			H_KEY = true;
			break;
		case 'z':
			Z_KEY = true;
			break;
		case 'b':
			B_KEY = true;
			break;
		case 'c':
			C_KEY = true;
			break;
		case 'q':
			Q_KEY = true;
			break;
		case 'e':
			E_KEY = true;
			break;
	}
}

class KeyboardInputHandler
{
private:
	sf::Keyboard::Key KEY_MOVE_FORWARD;
	sf::Keyboard::Key KEY_MOVE_BACKWARD;
	sf::Keyboard::Key KEY_MOVE_UPWARD;

public:
	 void Initialize();

	 bool MoveForwardKeyPressed();
	 bool MoveForwardKeyReleased();
	 bool MoveBackwardKeyPressed();
	 bool MoveBackwardKeyReleased();
	 bool MoveUpKeyPressed();
	 bool MoveUpKeyReleased();
};


#endif