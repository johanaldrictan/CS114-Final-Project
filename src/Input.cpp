#include "../include/Input.h"
#include <SDL/SDL.h>

void GetInput(game_input& Input)
{
	Input.NewKeyboard = SDL_GetKeyboardState(NULL);
	Input.NewMouse.MouseState = SDL_GetGlobalMouseState(&Input.NewMouse.MousePosition.X,
		&Input.NewMouse.MousePosition.Y);
}

//Copy NewKeyboard to OldKeyboard and NewMouse to OldMouse
void SwapInputs(game_input& Input)
{
	for (int32 K = 0; K < SDL_NUM_SCANCODES; ++K)
	{
		Input.OldKeyboard[K] = Input.NewKeyboard[K];
	}
	Input.OldMouse = Input.NewMouse;
}