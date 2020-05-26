#ifndef INPUT_H
#define INPUT_H

#include "MR_EngineTypeDefs.h"
#include "Math.h"

#define KEY_PRESSED(Scancode) (Input.NewKeyboard[Scancode] && !Input.OldKeyboard[Scancode])
#define KEY_RELEASED(Scancode) (!Input.NewKeyboard[Scancode] && Input.OldKeyboard[Scancode])
#define KEY_HELD(Scancode) (Input.NewKeyboard[Scancode] && Input.OldKeyboard[Scancode])
#define KEY_UNTOUCHED(Scancode) (!Input.NewKeyboard[Scancode] && !Input.OldKeyboard[Scancode])

#define MOUSE_PRESSED(Button) (((Input.NewMouse.MouseState & SDL_BUTTON(Button)) != 0) && ((Input.OldMouse.MouseState & SDL_BUTTON(Button)) == 0))
#define MOUSE_RELEASED(Button) (((Input.NewMouse.MouseState & SDL_BUTTON(Button)) == 0) && ((Input.OldMouse.MouseState & SDL_BUTTON(Button)) != 0))
#define MOUSE_HELD(Button) (((Input.NewMouse.MouseState & SDL_BUTTON(Button)) != 0) && ((Input.OldMouse.MouseState & SDL_BUTTON(Button)) != 0))
#define MOUSE_UNTOUCHED(Button) (((Input.NewMouse.MouseState & SDL_BUTTON(Button)) == 0) && ((Input.OldMouse.MouseState & SDL_BUTTON(Button)) == 0))

struct mouse_input
{
	vector2 MousePosition;
	int32 MouseState;
};

struct game_input
{
	const uint8* NewKeyboard;
	//Copy of NewKeyboard from last frame
	uint8* OldKeyboard;
	mouse_input NewMouse;
	mouse_input OldMouse;
};

void GetInput(game_input& Input);

void SwapInputs(game_input& Input);

#endif