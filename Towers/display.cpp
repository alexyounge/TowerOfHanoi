#include "display.h"
#include <GL/glew.h>
#include <iostream>


Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	fullscreen = false;
	spinLeft = false;
	spinRight = false;
	yValue = 1;
	oneP = false;
	twoP = false;
	threeP = false;
	newInput = true;
	cancel = false;
	reset = false;
	downDisc = false;
	upDisc = false;
	discChanged = false;

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);


	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
	}

	m_isClosed = false;

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Clear(255, 0, 0, 1);
	SDL_GL_SwapWindow(m_window);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
bool Display::IsClosed()
{
	return m_isClosed;
}
void Display::ChangeBG()
{
	SDL_GL_SwapWindow(m_window);
}
void Display::Update()
{
	SDL_GL_SwapWindow(m_window);
	SDL_Event e;

	// Handles for all the input for the game
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) 
		{
			m_isClosed = true;
		}
		else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_RIGHT) {
				spinRight = true;
			}
			else if (e.key.keysym.sym == SDLK_LEFT) {
				spinLeft = true;
			}
			else if (e.key.keysym.sym == SDLK_UP) {
				if (yValue < 3)
					yValue++;
			}
			else if (e.key.keysym.sym == SDLK_DOWN) {
				if (yValue > -3)
					yValue--;
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE) {
				m_isClosed = true;
			}
			else if (e.key.keysym.sym == SDLK_1) {
				if (twoP || threeP) {

				}
				else { oneP = true; newInput = false;
				}
			}
			else if (e.key.keysym.sym == SDLK_2) {
				if (oneP || threeP) {

				}
				else { twoP = true; newInput = false;
				}
			}
			else if (e.key.keysym.sym == SDLK_3) {
				if (oneP || twoP) {

				}
				else { threeP = true; newInput = false; }
			}
			else if (e.key.keysym.sym == SDLK_c) {
				cancel = true;
			}
			else if (e.key.keysym.sym == SDLK_r) {
				reset = true;
			}
			else if (e.key.keysym.sym == SDLK_z) {
				upDisc = true;
			}
			else if (e.key.keysym.sym == SDLK_x) {
				downDisc = true;
			}
		}
		else if (e.type == SDL_KEYUP) {
			if (e.key.keysym.sym == SDLK_RIGHT) {
				spinRight = false;
			}
			else if (e.key.keysym.sym == SDLK_LEFT) {
				spinLeft = false;
			}
			else if (e.key.keysym.sym == SDLK_1) {
				oneP = false;
				newInput = true;
			}
			else if (e.key.keysym.sym == SDLK_2) {
				twoP = false;
				newInput = true;

			}
			else if (e.key.keysym.sym == SDLK_3) {
				threeP = false;
				newInput = true;

			}
			else if (e.key.keysym.sym == SDLK_r) {
				reset = false;

			}
			else if (e.key.keysym.sym == SDLK_z) {
				upDisc = false;
				discChanged = false;
			}
			else if (e.key.keysym.sym == SDLK_x) {
				downDisc = false;
				discChanged = false;
			}

		}

	}
	


}