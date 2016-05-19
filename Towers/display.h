#ifndef DISPLAY_H
#define DISPLAY_H

#include <sdl/SDL.h>
#include <string>
#include <iostream>

class Display
{
public:
	Display(int widght, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	//void SwapBuffers();
	void Update();
	bool IsClosed();
	void ChangeBG();

	int currentColour;
	bool fullscreen;
	bool spinLeft, spinRight;
	float yValue;
	bool oneP, twoP, threeP, newInput, cancel, reset, upDisc, downDisc, discChanged;

	virtual ~Display();

protected:
private:
	Display(const Display& other) {}
	void operator = (const Display& other) {}

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;

};

#endif // DISPLAY_H