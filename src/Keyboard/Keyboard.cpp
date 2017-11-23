#include "Keyboard.h"

void KeyboardInputHandler::Initialize()
{
	KEY_MOVE_FORWARD = sf::Keyboard::W;
	KEY_MOVE_BACKWARD = sf::Keyboard::S;
	KEY_MOVE_UPWARD = sf::Keyboard::Y;
}


bool KeyboardInputHandler::MoveForwardKeyPressed()
{
	return sf::Keyboard::isKeyPressed(KEY_MOVE_FORWARD);
}

bool KeyboardInputHandler::MoveForwardKeyReleased()
{
	return false;
}

bool KeyboardInputHandler::MoveBackwardKeyPressed()
{
	return sf::Keyboard::isKeyPressed(KEY_MOVE_BACKWARD);
}

bool KeyboardInputHandler::MoveBackwardKeyReleased()
{
	return false;
}

bool KeyboardInputHandler::MoveUpKeyPressed()	
{
	return sf::Keyboard::isKeyPressed(KEY_MOVE_UPWARD);
}
bool KeyboardInputHandler::MoveUpKeyReleased()
{
	return false;
}