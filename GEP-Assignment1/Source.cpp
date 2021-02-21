/*
*********************************************************************
Rock Paper Scissors Game
Name: Joshua Adeyemo 
Student Number: 991456766


*********************************************************************

*/
#pragma once
#include "Game.h"
#include "SDL.h"



int main(int argc, char* args[])
{
	if (Game::Instance()->Init("Poker C++ GUI", 100,
		50, 1024, 768, 0) == false) return 1;
	while (Game::Instance()->Running())
	{
		Game::Instance()->HandleEvents();
		if (Game::Instance()->Tick())
		{
			Game::Instance()->Update();
			Game::Instance()->Render();
		}
	}
	Game::Instance()->Clean();
	return 0;
}
