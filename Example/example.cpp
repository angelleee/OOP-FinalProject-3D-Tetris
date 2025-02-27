/* Yuhsuan: 2021.07.03 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "linalg.h"
#include "model.h"
#include "tetris.h"
#include "geometry.h"
#include "render.h"
#include "polygon_clipping.h"
#include "cube_hint.h"
#include "rules.h"
#include "keyboard.h"
#include "ReInit.h"
#include "game_body.h"
#include "tetris_menu.h"

using namespace std;

int select = 0;//BG Selection
int menu = 1;//1:start 2:quit 3:pause

const int SOLID = 100;
const int SHADED = 101;
const int BLENDED = 102;

int xLeft = 0;
int xRight = 0;
int yFront = 0;
int yClose = 0;
int zR = 0;
int zL = 0;
int space = 0;
int s_up = 0; int s_do = 0; int s_l = 0; int s_r = 0; int sz_R = 0; int sz_L = 0;
int Light_x_up = 0; int Light_x_do = 0;
int Light_y_up = 0; int Light_y_do = 0;
int Light_z_up = 0; int Light_z_do = 0;
int Left_R = 0; int Right_R = 0;
int Tt_Y_F = 0; int Tt_Y_B = 0;
int Tt_X_B = 0; int Tt_X_F = 0;
int ReStart = 0;
bool f_plot = false;
int mv = 0;
int f_ch = 0;
int sp_x = WIDTH / 2, sp_y = 10;
#define POINTS_COUNT 2

static SDL_Point points[POINTS_COUNT] = {
	{WIDTH / 2, 0},
	{sp_x, sp_y}
};

struct ImageData
{
	SDL_Texture* texture;
	int width;
	int height;
	int num;
	int wn;
	int hn;
};

struct TextData
{
	SDL_Texture* texture;
	int width;
	int height;
};

int initSDL(); // Starts up SDL and creates window
void closeSDL(); // Frees media and shuts down SDL
ImageData loadImgTexture(char* path, int num, int hn, int wn, bool ckEnable, Uint8 r, Uint8 g, Uint8 b);
TextData loadTextTexture(const char* str, const char* fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb);
TextData updateTextTexture(const char* str, const char* fontPath, int fontSize, TTF_Font* ttfFont, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb);
//TextData loadTextTexture(const char* str, const char* fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb);

// New and modified functions
int imgRender(SDL_Renderer* renderer, ImageData img, int posX, int posY, int frame, int cx, int cy, float angle, SDL_RendererFlip flip, int alpha);
Uint32 BaseAction(Uint32 interval, void* param); // Timer callback function

Uint32 startrotate(Uint32 interval, void* param);
Uint32 colorchange(Uint32 interval, void* param);
Uint32 wordtaptostart(Uint32 interval, void* param);
Uint32 DotChange(Uint32 interval, void* param);
Uint32 Black_Change(Uint32 interval, void* param);

bool loadAudio();

SDL_Window* window = NULL; // The window we'll be rendering to
SDL_Renderer* renderer = NULL; // The window renderer

Mix_Music* music1 = NULL;
Mix_Music* music01 = NULL;
Mix_Music* music02 = NULL;
Mix_Music* music03 = NULL;
Mix_Music* music04 = NULL;
Mix_Music* music_select = NULL;
//Mix_Music* music_failure = NULL;
enum MouseState
{
	NONE = 0,
	LB = 1,
	WU = 2,
	WD = 3,
	HOVER = 4,
	HOVER_RIGHT = 5,
	HOVER_LEFT = 6
};

int initSDL()
{
	// Initialize SDL	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		// Error Handling		
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	// Create window	
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("OOP SDL Tutorial", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 3;
	}
	if (TTF_Init() == -1)
	{
		// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 4;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_frame.html
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return 5;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 5;
	}

	return 0;
}

void closeSDL()
{
	// Free the music
	Mix_FreeMusic(music1);
	Mix_FreeMusic(music01);
	Mix_FreeMusic(music02);
	Mix_FreeMusic(music03);
	Mix_FreeMusic(music04);
	//Mix_FreeMusic(music_select);
	//Mix_FreeMusic(music_failure);
	// Quit Mixer subsystem
	Mix_Quit();
	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

TextData updateTextTexture(const char* str, const char* fontPath, int fontSize, TTF_Font* ttfFont, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb)
{
	// TTF Font sturct
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	//TTF_Font* ttfFont = NULL;

	// Open the font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	//ttfFont = TTF_OpenFont(fontPath, fontSize); //("../fonts/lazy.ttf", 28);
	//if (ttfFont == NULL)
	//{
	//	printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	//}

	// A structure that represents a color.
	// https://wiki.libsdl.org/SDL_Color
	SDL_Color textFgColor = { fr, fg, fb }, textBgColor = { br, bg, bb };

	// Render text surface
	// The way SDL_ttf works is that you create a new image from a font and color. 
	// We're going to be loading our image from text rendered by SDL_ttf instead of a file.
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html#SEC42		

	SDL_Surface* textSurface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html
	if (textType == SOLID)
	{
		// Quick and Dirty
		textSurface = TTF_RenderText_Solid(ttfFont, str, textFgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		textSurface = TTF_RenderText_Shaded(ttfFont, str, textFgColor, textBgColor);
	}
	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		textSurface = TTF_RenderText_Blended(ttfFont, str, textFgColor);
	}

	// Free font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return { NULL };
	}
	else
	{
		TextData text;

		// Create texture from surface pixels
		text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (text.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get text dimensions and information
		text.width = textSurface->w;
		text.height = textSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
		//free(textSurface);
		textSurface = NULL;

		//return font data
		return text;
	}
}

TextData loadTextTexture(const char* str, const char* fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb)
{
	// TTF Font sturct
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	
	// Open the font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_Font* ttfFont = NULL;
	ttfFont = TTF_OpenFont(fontPath, fontSize); //("../fonts/lazy.ttf", 28);
	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	// A structure that represents a color.
	// https://wiki.libsdl.org/SDL_Color
	SDL_Color textFgColor = { fr, fg, fb }, textBgColor = { br, bg, bb };

	// Render text surface
	// The way SDL_ttf works is that you create a new image from a font and color. 
	// We're going to be loading our image from text rendered by SDL_ttf instead of a file.
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html#SEC42		
	SDL_Surface* textSurface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html
	if (textType == SOLID)
	{
		// Quick and Dirty
		textSurface = TTF_RenderText_Solid(ttfFont, str, textFgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		textSurface = TTF_RenderText_Shaded(ttfFont, str, textFgColor, textBgColor);
	}
	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		textSurface = TTF_RenderText_Blended(ttfFont, str, textFgColor);
	}

	// Free font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_CloseFont(ttfFont);
	//ttfFont = NULL;
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return { NULL };
	}
	else
	{
		TextData text;

		// Create texture from surface pixels
		text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (text.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get text dimensions and information
		text.width = textSurface->w;
		text.height = textSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
		//free(textSurface);
		//textSurface = NULL;

		//return font data
		return text;
	}
}

int textRender(SDL_Renderer* renderer, TextData text, int posX, int posY, int cx, int cy, float angle, SDL_RendererFlip flip, int alpha)
{
	SDL_Rect r;
	r.x = posX;
	r.y = posY;
	r.w = text.width;
	r.h = text.height;

	// Set and enable standard alpha blending mode for a texture	
	if (SDL_SetTextureBlendMode(text.texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha	
	if (SDL_SetTextureAlphaMod(text.texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Point center = { cx, cy };

	SDL_RenderCopyEx(renderer, text.texture, NULL, &r, angle, &center, flip);
	return 1;
}


// ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
ImageData loadImgTexture(char* path, int num, int hn, int wn, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
		return { NULL };
	}
	else
	{
		ImageData img;

		// Set the color key (transparent pixel) in a surface.		
		SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

		// Create texture from surface pixels
		img.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (img.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get image dimensions and information
		img.width = loadedSurface->w;
		img.height = loadedSurface->h;
		img.num = num;
		img.wn = wn;
		img.hn = hn;

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		//return newTexture;
		return img;
	}
}


int imgRender(SDL_Renderer* renderer, ImageData img, int posX, int posY, int frame, int cx, int cy, float angle, SDL_RendererFlip flip, int alpha)
{
	int wc = frame % img.wn;
	int hc = frame / img.wn;

	SDL_Rect src, dst;
	src.x = img.width / img.wn * wc;
	src.y = img.height / img.hn * hc;
	src.w = img.width / img.wn;
	src.h = img.height / img.hn;
	dst.x = posX;
	dst.y = posY;
	dst.w = src.w;
	dst.h = src.h;


	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	// https://wiki.libsdl.org/SDL_BlendMode
	if (SDL_SetTextureBlendMode(img.texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(img.texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}

	//SDL_RenderCopy(renderer, img.texture, &src, &dst);

	// SDL_Point
	// A structure that defines a two dimensional point.
	// https://wiki.libsdl.org/SDL_Point
	SDL_Point center = { cx, cy };

	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2
	// https://wiki.libsdl.org/SDL_RenderCopyEx
	SDL_RenderCopyEx(renderer, img.texture, &src, &dst, angle, &center, flip);

	return 1;
}

bool loadAudio() {
	//music1 = Mix_LoadMUS("../music/piano.wav");
	music1 = Mix_LoadMUS("../music/0.mp3");
	if (music1 == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	music01 = Mix_LoadMUS("../music/1.mp3");
	if (music01 == NULL)
	{
		printf("Failed to load music01! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	music02 = Mix_LoadMUS("../music/2.mp3");
	if (music02 == NULL)
	{
		printf("Failed to load music02! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	music03 = Mix_LoadMUS("../music/3.mp3");
	if (music1 == NULL)
	{
		printf("Failed to load music03! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	music04 = Mix_LoadMUS("../music/4.mp3");
	if (music1 == NULL)
	{
		printf("Failed to load music04! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
}

Uint32 BaseAction(Uint32 interval, void* param)
{
	float* par = (float*)param;
	par[0] += 10;
	if (par[0] == 90) {
		par[0] = 0;
		return 0;
	}

	return interval;
}


Uint32 startrotate(Uint32 interval, void* param)
{
	float* pos = (float*)param;
	pos[0] ++;

	return interval;
}

Uint32 colorchange(Uint32 interval, void* param)
{
	float* pos = (float*)param;
	
	int idx = rand() % 3;
	pos[idx] += 1;
	pos[idx] = (int)pos[idx] % 360;
	return interval;
}

Uint32 wordtaptostart(Uint32 interval, void* param)
{
	int* pos = (int*)param;
	pos[0] = (pos[0] - 2);
	if (pos[0] <= 90)
		pos[0] = 255;
	return interval;
}

Uint32 DotChange(Uint32 interval, void* param)
{
	int* pos = (int*)param;
	for (int i = 0; i < 10; i++)
	{
		pos[i*2] = rand() % WIDTH;
		pos[i*2+1] = rand() % HEIGHT / 2;
	}
	return interval;
}

Uint32 LRz_Change(Uint32 interval, void* param) 
{
	int* pos = (int*)param;
	pos[0] = 1;
	return interval;
}

Uint32 TtDrop(Uint32 interval, void* param)
{
	int* pos = (int*)param;
	pos[0] = 1;
	return interval;
}

Uint32 fragment(Uint32 interval, void* param)
{
	int* pos = (int*)param;
	if (f_plot == true)
	{
		pos[0] = (pos[0] + 1) % 5;
	}
	else if (f_plot == false)
	{
		pos[0] = 0;
	}
	return interval;
}

Uint32 SunYeh(Uint32 interval, void* param)
{

	float* pos = (float*)param;
	if (pos[1] == 1){
		if (menu == 2 && f_plot == false)
		{
			pos[0] += 1;
			//pos[0] = ((int)pos[0] + 1) % 181;
			//printf("%f\n", pos[0]);
			if (pos[0] == 155) {
				pos[1] = 0;
				pos[0] = 0;
			}
		}
		else if (menu == 3 || f_plot == true)
		{
			pos[0] += 0;
		}
		else
		{
			pos[0] = 0;
		}
	}
	return interval;
}

Uint32 Black_Change(Uint32 interval, void* param) {
	float* pos = (float*)param;
	if (pos[2] == 1) {
		if (pos[1] == 1 && pos[0] < 254.1) {
			pos[0] += 1;
		}
		else if(abs(pos[0] - 254) < 0.5 ) {
			pos[0] = 254;
			pos[1] = 0;
			pos[2] = 0;
		}

		if (pos[1] == 0 && pos[0] > 0.1) {
			pos[0] -= 1;
		}
		else if (abs(pos[0] - 0) < 0.5) {
			pos[0] = 0;
			pos[1] = 1;
			pos[2] = 0;
		}
	}
	return interval;
}

void mouseHandleEvent(SDL_Event* e, MouseState* mouseState, int* x, int* y, bool & pause, bool & Game_over, int & ReStart)
{
	// If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL)
	{
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState		
		SDL_GetMouseState(x, y);
		//printf("(%d, %d)\n", *x, *y);

		static int lasttime = SDL_GetTicks();
		static int curtime = SDL_GetTicks();
		int timediv;

		lasttime = curtime;
		curtime = SDL_GetTicks();
		timediv = curtime - lasttime;

		switch (e->type)
		{
		case SDL_MOUSEBUTTONUP:
			//*mouseState = NONE;

			if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 1 && timediv > 100)
			{
				*mouseState = LB;
				if (menu == 1)
				{
					pause = false;
					if (*x >= 485 && *x <= 600 && *y <= 455 && *y >= 255)
						select = (select + 1) % 4;
					else if (*x >= 0 && *x <= 115 && *y <= 455 && *y >= 255)
					{
						select -= 1;
						if (select == -1)
							select = 3;
					}
					else if (*x >= 160 && *x <= 425 && *y <= 490 && *y >= 420)
					{
						Mix_HaltMusic();
						menu = 2;
						ReStart = 1;
					}
				}
				if (menu == 2)
				{
					if (Game_over == false  && *x >= 0 && *x <= 90 && *y <= 115 && *y >= 0)
					{
						pause = true;
						menu = 3;
					}
				}
				if (menu == 3)
				{
					pause = true;
					if (*x >= 120 && *x <= 240 && *y <= 350 && *y >= 290)//YES
					{
						menu = 1;
						Mix_HaltMusic();
					}
					else if (*x >= 375 && *x <= 490 && *y <= 350 && *y >= 290)//NO
					{
						menu = 2;
						pause = false;
					}
				}
				if (Game_over == true)
				{
					if (*x >= 210 && *x <= 390 && *y <= 537 && *y >= 447)
					{
						menu = 1;
						Mix_HaltMusic();
						Game_over = false;
					}
				}
			}
			break;
		case SDL_MOUSEWHEEL:
			if (e->wheel.y > 0) // scroll up
			{
				// Put code for handling "scroll up" here!
				*y = e->wheel.y;
				*mouseState = WU;
			}
			else if (e->wheel.y < 0) // scroll down
			{
				// Put code for handling "scroll down" here!
				*y = e->wheel.y;
				*mouseState = WD;
			}
			break;
		}
	}
}

 //When using SDL, you have to use "int main(int argc, char* args[])"
 //int main() and void main() are not allowed
int main(int argc, char* args[])
{
	int s, count;
	char score[10] = "\0";
	char final_score[10] = "\0";

	int Game_Score = 0;
	float Rx = 40, Ry = 0, Rz = 43;
	float Tx = -15, Ty = 16, Tz = -34;
	float LRx = 20, LRy = 20, LRz = 20;
	float LTx = 0, LTy = 0, LTz = 0;
	int sun_or_moon = 1;

	int animationPar[8] = { 0,0,0,0,0,255,0,35 }; // index, index, index, degree, index, laser, swgx, swgy
	float tt = 0;
	float CR = 0;
	int tetris_type;
	float Cube_Lowest;
	
	float startrotate_par[1] = { 0 };//rotate
	SDL_TimerID timerID_startrotate = SDL_AddTimer(50, startrotate, startrotate_par);
	//float color_par[] = { 47, 82, 143 }; // White
	//float color_par[] = { 220, 200, 173 }; // Dark Blue
	//float color_par[] = { 85, 172, 230 }; //Orange
	float initR = rand() % 256;
	float initG = rand() % 256;
	float initB = rand() % 256;
	float color_par[] = { initR, initG, initB };
	SDL_TimerID timerID_colorchange = SDL_AddTimer(50, colorchange, color_par);

	int taptostart_par[1] = { 255 };
	SDL_TimerID timerID_taptostart = SDL_AddTimer(30, wordtaptostart, taptostart_par);

	int dot_xy[20];
	SDL_TimerID timerID_BgDot = SDL_AddTimer(700, DotChange, dot_xy);

	int drop[1] = {0};
	SDL_TimerID timer_Tt_Drop = SDL_AddTimer(1000, TtDrop, drop);

	int light_change[1] = { 0 };
	SDL_TimerID timer_L_Change = SDL_AddTimer(20, LRz_Change, light_change);

	int fragment_par[1] = { 0 };
	SDL_TimerID timer_fragment = SDL_AddTimer(700, fragment, fragment_par);

	float yeh_sun[2] = { 35 , 1};
	//float yeh_sun_init[2] = { -30, 450 };
	float yeh_sun_init[2] = { -405, 0 };
	SDL_TimerID SandM_yeh = SDL_AddTimer(600, SunYeh, yeh_sun);

	float black_ab[3] = { 0, 1, 0 };
	SDL_TimerID Darkab = SDL_AddTimer(200, Black_Change, black_ab);

	char imgPath7[100] = "../images/1.png";//b1
	char imgPath8[100] = "../images/2.png";
	char imgPath9[100] = "../images/3.png";
	char imgPath10[100] = "../images/4.png";
	char imgPath11[100] = "../images/3D tetris-start.png";//start
	char imgPath12[100] = "../images/gamescreen.png";//games
	char imgPath13[100] = "../images/exitscreen.png";//exits
	char imgPath14[100] = "../images/taptostart.png";
	char imgPath15[100] = "../images/gameover.png";
	char imgPath16[100] = "../images/gamescore.png";
	char imgPath17[100] = "../images/gamepause.png";
	char imgPath18[100] = "../images/fragment_1.png";
	char imgPath19[100] = "../images/fragment_2.png";
	char imgPath20[100] = "../images/fragment_3.png";
	char imgPath21[100] = "../images/fragment_4.png";
	char imgPath22[100] = "../images/fragment_5.png";
	char imgPath23[100] = "../images/sun.png";
	char imgPath24[100] = "../images/moon_1.png";
	char imgPath25[100] = "../images/black.png";

	ImageData b1, b2, b3, b4;
	ImageData start, games, exits, tapfirst, gover, score_logo, pause_logo;
	ImageData fg1, fg2, fg3, fg4, fg5;
	ImageData sun, moon, black;

	// Flip type
	// https://wiki.libsdl.org/SDL_RendererFlip
	SDL_RendererFlip no = SDL_FLIP_NONE;
	SDL_RendererFlip ho = SDL_FLIP_HORIZONTAL;
	SDL_RendererFlip ve = SDL_FLIP_VERTICAL;
	SDL_RendererFlip hove = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	srand(time(NULL));
	// Start up SDL and create window
	if (initSDL())
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}

	loadAudio();

	TTF_Font*  ttfFont = TTF_OpenFont("../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 50);

	b1 = loadImgTexture(imgPath7, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	b2 = loadImgTexture(imgPath8, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	b3 = loadImgTexture(imgPath9, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	b4 = loadImgTexture(imgPath10, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	start = loadImgTexture(imgPath11, 1, 1, 1, false, 0xFF, 0xFF, 0xFF); 
	games = loadImgTexture(imgPath12, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	exits = loadImgTexture(imgPath13, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	tapfirst = loadImgTexture(imgPath14, 1, 1, 1, false, 0xFF, 0xFF, 0xFF); 
	gover = loadImgTexture(imgPath15, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	score_logo = loadImgTexture(imgPath16, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	pause_logo = loadImgTexture(imgPath17, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	fg1 = loadImgTexture(imgPath18, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	fg2 = loadImgTexture(imgPath19, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	fg3 = loadImgTexture(imgPath20, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	fg4 = loadImgTexture(imgPath21, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	fg5 = loadImgTexture(imgPath22, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	sun = loadImgTexture(imgPath23, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	moon = loadImgTexture(imgPath24, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);
	black = loadImgTexture(imgPath25, 1, 1, 1, false, 0xFF, 0xFF, 0xFF);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	MouseState mouseState;
	int mouseX, mouseY;
	
	vector<float> CamLoc = { Tx, Ty, Tz }, CamOri = { Rx, Ry, Rz };
	vector<vector<float>> CamP = RT_M(CamOri, CamLoc);
	vector<vector<float>> CamK = inK(100, 0.2);
	vector<float> LightLoc = { LTx, LTy, LTz }, LightOri = { LRx, LRy, LRz };
	vector<vector<float>> LightP = RT_M(LightOri, LightLoc);
	vector<vector<float>> LightK = inK(100, 0.2);
	vector<float> LightRay = { 0, 0, 1 };
	vector<vector<float>> LightOriMat = M_Transpose(SubM(LightP, 0, 3, 0, 3));

	float shadow_pool[120][6][2] = { 0 };
	float shadow_pool_menu[120][6][2] = { 0 };
	bool Game_over = false;
	vector<vector<float>> Base_Now_Height;
	int collect_or_not = 0;

	vector<PrimCube> cubes; // Collect all Cubes
	// Base Tetris
	vector<int> cbs_ids = { 0, 1, 2, 3, 4, 5, 6, 7, 8 }; // 9
	for (int i = 0; i < 9; i++) {
		PrimCube base_cube(i, { -1.2, -1.2, 1.3 }, { 0, 0, 0 }, { 1.1, 1.1, 1.1 }, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1);
		cubes.push_back(base_cube);
	}
	// Set Base Attribute
	Tetris Base(cbs_ids, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1., 1., 1.	}, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1, cubes);
	
	// Set Deadline Block
	Tetris_ALL Base_deadline = add_new_tetris(9, cubes, Base);
	Base.Collect_Ids(Base_deadline.ids);

	//// Test Many Cubes
	vector<int> cbs_menu_ids;
	for (int i = 0; i < 67+6; i++) {
		cbs_menu_ids.push_back(i);
	}

	// Set Random Init T_Loc
	tetris_type = rand() % 9;
	Tetris_ALL L_type = add_new_tetris(tetris_type, cubes, Base);
	
	vector<vector<float>> hint_location;
	hint_location = L_type.GetHintLocation(cubes); // x_Tt_loc, y_Tt_loc, z_Tt_loc_lowest, z_base_loc_Highest
	Base.Find_Hint_Height(cubes, hint_location, Base.ids.size());
	Base_Now_Height = Base.Find_All_Height(cubes, Base.ids.size());
	Cube_Lowest = L_type.GetLowestHeight(cubes);
	bool pause = false;
	bool cannot_sound = false;
	while (!quit)
	{
		enum Cube_State {
			// Cube is falling
			Cube_drop,
			Cube_R_Base,
			Cube_R_Base_Tt,
			// Cube Stop
			Shift_A_Layer, // And all cubes drop 1 unit in z-axis
			Generate_new_Tt,
		};
		mouseState = NONE;
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			mouseHandleEvent(&e, &mouseState, &mouseX, &mouseY, pause, Game_over, ReStart);
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (drop[0] == 0 && pause == false) {
					//printf("Non Dropping!\n");
					if ((int)CR % 90 != 0) {
						printf("Rotating!\n");
					}
					else {
						if (Game_over == false) {
							switch (e.key.keysym.sym) {
							case SDLK_RIGHT:
								if (Left_R == 0 && Right_R == 0) {
									xRight = 1;
								}
								else {
									printf("Base is R!!!!============================\n");
								}
								break;
							case SDLK_LEFT:
								if (Left_R == 0 && Right_R == 0) {
									xLeft = 1;
								}
								else {
									printf("Base is R!!!!===============================\n");
								}
								break;
							case SDLK_c:
								if (xRight == 0 && xLeft == 0) {
									Left_R = 1;
								}
								else {
									printf("Tt is R!!!!==================================\n");
								}
								break;
							case SDLK_v:
								if (xRight == 0 && xLeft == 0) {
									Right_R = 1;
								}
								else {
									printf("Tt is R!!!!==============================\n");
								}
								break;

							case SDLK_DOWN:
								yClose = 1;
								break;
							case SDLK_UP:
								yFront = 0; // If up yFront = 1;
								break;
							case SDLK_z:
								zL = 1;
								break;
							case SDLK_x:
								zR = 1;
								break;
							case SDLK_SPACE:
								space = 1;
								break;
							case SDLK_w:
								s_up = 1;
								break;
							case SDLK_s:
								s_do = 1;
								break;
							case SDLK_a:
								s_l = 1;
								break;
							case SDLK_d:
								s_r = 1;
								break;
							case SDLK_f:
								sz_L = 1;
								break;
							case SDLK_g:
								sz_R = 1;
								break;
							case SDLK_u:
								Light_x_up = 1;
								break;
							case SDLK_j:
								Light_x_do = 1;
								break;
							case SDLK_i:
								Light_y_up = 1;
								break;
							case SDLK_k:
								Light_y_do = 1;
								break;
							case SDLK_o:
								Light_z_up = 1;
								break;
							case SDLK_l:
								Light_z_do = 1;
								break;
							
							case SDLK_h:
								Tt_X_F = 1;
								break;
							case SDLK_n:
								Tt_X_B = 1;
								break;
							case SDLK_b:
								Tt_Y_B = 1;
								break;
							case SDLK_m:
								Tt_Y_F = 1;
								break;
							}
						}
						else{
							switch (e.key.keysym.sym) {
							case SDLK_e:
								ReStart = 1;
								pause = false;
								break;
							}
						}

					}
				}
				else {
					//printf("Dropping!\n");
					//printf("Pause: %d\n", pause);
				}
			}
			//mouseHandleEvent(&e, &mouseState, &mouseX, &mouseY, pause);
		}

		//LightControl(Light_x_up, Light_x_do, Light_y_up, Light_y_do, Light_z_up, Light_z_do, LRx, LRy, LRz, LTx, LTy, LTz, LightLoc, LightOri, LightP, LightOriMat, LightRay);
		
		// Open the Music
		if (menu == 1)
		{
			if (Mix_PlayingMusic() == 0)
				Mix_PlayMusic(music1, -1);
		}

		switch (select)
		{
			case 0:
				imgRender(renderer, b1, 0, 0, 0, NULL, NULL, NULL, no, 255);
				music_select = music01;
				break;
			case 1:
				imgRender(renderer, b2, 0, 0, 0, NULL, NULL, NULL, no, 255);
				music_select = music02;
				break;
			case 2:
				imgRender(renderer, b3, 0, 0, 0, NULL, NULL, NULL, no, 255);
				music_select = music03;
				break;
			case 3:
				imgRender(renderer, b4, 0, 0, 0, NULL, NULL, NULL, no, 255);
				music_select = music04;
				break;
		}
		for (int i = 0; i < 10; i++)
		{
			filledCircleColor(renderer, dot_xy[2 * i], dot_xy[2 * i + 1], 2, 0xFFFFFFAA);
		}
		if (menu == 1)
		{
			if (light_change[0] == 1) {
				LightAuto(LRx, LRy, LRz, LTx, LTy, LTz, LightLoc, LightOri, LightP, LightOriMat, LightRay);
				light_change[0] = 0;
			}
			imgRender(renderer, start, 0, 0, 0, NULL, NULL, NULL, no, 255);
			vector<PrimCube> menu_cubes;
			for (int i = 0; i < cbs_menu_ids.size(); i++) {
				PrimCube base_cube(i, { -1.2, -1.2, 1.3 }, { 0, 0, 0 }, { 1.1, 1.1, 1.1 }, { 128, 128, 128 }, 1, { 0, 51, 51 }, 1);
				menu_cubes.push_back(base_cube);
			}
			Tetris_Menu Menu_Cbs(cbs_menu_ids, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 1., 1., 1. }, { ((sin(color_par[0] / (float)180 * (float)M_PI) + (float)1)/ (float)2.) * 255, ((sin(color_par[1] / (float)180 * (float)M_PI) + (float)1) / (float)2.) * 255, ((sin(color_par[2] / (float)180 * (float)M_PI) + (float)1) / (float)2.) * 255 }, 1, { 52, 86, 110 }, 1, menu_cubes);
			Menu_Cbs.RotateTetris({ startrotate_par[0], startrotate_par[0], startrotate_par[0] }, menu_cubes);
			Menu_Cbs.MoveTetris({ 0, 0, -5 }, menu_cubes);
			Record_Shadow_Menu(menu_cubes, LightP, LightK, shadow_pool_menu);
			Plot_Cubes_Menu(renderer, menu_cubes, CamP, CamK, LightRay);
			menu_cubes.clear();
			menu_cubes.shrink_to_fit();
			imgRender(renderer, tapfirst, 5, -15, 0, NULL, NULL, NULL, no, taptostart_par[0]);
		}
		else if (menu == 2)
		{
			if (Game_over == false)
			{
				imgRender(renderer, games, 0, 0, 0, NULL, NULL, NULL, no, 255);
			}
			if (Mix_PlayingMusic() == 0)
				Mix_PlayMusic(music_select, -1); 
		}

		if (menu != 1){
			if (pause == false)
				if (light_change[0] == 1) {
					LightAuto(LRx, LRy, LRz, LTx, LTy, LTz, LightLoc, LightOri, LightP, LightOriMat, LightRay);
					light_change[0] = 0;
				}
			if (Game_over == false) {
				GameMainBody(Game_over, Game_Score, collect_or_not, Cube_Lowest, Base, L_type, cubes, Base_Now_Height, hint_location);
				Score2Text(Game_Score, score);
				TextData scoretext;
				TextData scoretext1;
				if (strcmp(score, "\0") != 0)
				{
					scoretext = loadTextTexture(score, "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 60, 255, 255, 255, BLENDED, NULL, NULL, NULL);
					scoretext1 = loadTextTexture(score, "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 60, 125, 125, 125, BLENDED, NULL, NULL, NULL);
				}
				else
				{
					scoretext = loadTextTexture("0", "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 60, 255, 255, 255, BLENDED, NULL, NULL, NULL);
					scoretext1 = loadTextTexture("0", "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 60, 125, 125, 125, BLENDED, NULL, NULL, NULL);
				}
				textRender(renderer, scoretext1, 580 - scoretext1.width, 41, NULL, NULL, NULL, SDL_FLIP_NONE, 255);
				textRender(renderer, scoretext, 577 - scoretext.width, 40, NULL, NULL, NULL, SDL_FLIP_NONE, 255);

				SDL_DestroyTexture(scoretext1.texture);
				SDL_DestroyTexture(scoretext.texture);
				if (Game_over == true)
					strcpy_s(final_score, score);
				strcpy_s(score, "\0");
			}
			if (Game_over == false) {
				if (xLeft == 0 && xRight == 0)
					if (Left_R == 1 || Right_R == 1)
						Base_Rotate(Left_R, Right_R, CR, Base, cubes, L_type, Base_Now_Height, hint_location, collect_or_not);
				if (Left_R == 0 && Right_R == 0)
					if (xLeft == 1 || xRight == 1){
						Tt_Rotate(xLeft, xRight, CR, Base, cubes, L_type, Base_Now_Height, hint_location, collect_or_not, cannot_sound);
					}
				if (drop[0] == 1 && (int)CR % 90 == 0 && Left_R == 0 && Right_R == 0 && xLeft==0 && xRight == 0 && pause == false) {
					Tt_Auto_Up_Down(Cube_Lowest, Base, cubes, L_type, Base_Now_Height, hint_location, collect_or_not, Game_Score);
					drop[0] = 0;
				}
				else if (drop[0] == 0) {
					Tt_Move_Up_Down(yClose, yFront, Cube_Lowest, tt, Base, cubes, L_type, Base_Now_Height, hint_location, collect_or_not);
				}
				Tt_Move_UDLR(Tt_X_B, Tt_X_F, Tt_Y_B, Tt_Y_F, Base, cubes, L_type, Base_Now_Height, hint_location, collect_or_not);
			}
			
			if (ReStart == 1) {
				ReInitGame(Game_over, ReStart, Game_Score, tt, CR, collect_or_not, Cube_Lowest, Base, cubes, L_type, Base_deadline, Base_Now_Height, hint_location);
				LightReset(LRx, LRy, LRz, LTx, LTy, LTz, LightLoc, LightOri, LightP, LightOriMat, LightRay);
			}
			
			if (abs((float)LRz - (float)20) < 0.5) {
				yeh_sun[1] = 1;
				yeh_sun[0] = 0;
			}
			if (yeh_sun[1] == 1){
				float yeh_x = (yeh_sun_init[0] *  cos(yeh_sun[0] / 180. * M_PI)) + (yeh_sun_init[1] * -sin(yeh_sun[0] / 180. * M_PI)) + 300 -75;
				float yeh_y = (yeh_sun_init[0] * sin(yeh_sun[0] / 180. * M_PI)) + (yeh_sun_init[1] * cos(yeh_sun[0] / 180. * M_PI)) + 450+30 ;
				if (sun_or_moon == 1 )
					imgRender(renderer, sun, yeh_x, yeh_y, 0, NULL, NULL, NULL, no, 255);
				else if (sun_or_moon == 0 )
					imgRender(renderer, moon, yeh_x, yeh_y, 0, NULL, NULL, NULL, no, 255);
			}
			//imgRender(renderer, sun, WIDTH/2-75, HEIGHT/2, 0, NULL, NULL, NULL, no, 255);
			if (LRz >= 280 && sun_or_moon == 1) {		
				black_ab[2] = 1;
				black_ab[1] = 1;
				yeh_sun[1] = 0;
				yeh_sun[0] = 0;
			}
			else if (LRz >= 280 && sun_or_moon == 0) {
				black_ab[2] = 1;
				black_ab[1] = 0;
				yeh_sun[1] = 0;
				yeh_sun[0] = 0;
			}
			imgRender(renderer, black, 0, 0, 0, NULL, NULL, NULL, no, black_ab[0]);
			//printf("Black plhap : %f\n", black_ab[0]);
			if (black_ab[2] == 1 && abs(black_ab[0] - 255)<1.1){
				sun_or_moon = 0;
			}
			else if (black_ab[2] == 1 && abs(black_ab[0] - 0) < 0.1) {
				sun_or_moon = 1;
			}
			
			Record_Shadow(cubes, LightP, LightK, shadow_pool);
			Plot_Cubes(renderer, cubes, CamP, CamK, LightRay);
			f_plot = Game_over;
			if (Game_over == true) {
				pause = true;
				drop[0] = 0;
				f_plot = true;
				TextData stt;
				TextData stt1;
				imgRender(renderer, gover, 0, 0, 0, NULL, NULL, NULL, no, 255);
				
				switch (fragment_par[0] % 5)
				{
				case 0:
					imgRender(renderer, fg1, 0, 0, 0, NULL, NULL, NULL, no, 255);
					break;
				case 1:
					imgRender(renderer, fg2, 0, 0, 0, NULL, NULL, NULL, no, 255);
					break;
				case 2:
					imgRender(renderer, fg3, 0, 0, 0, NULL, NULL, NULL, no, 255);
					break;
				case 3:
					imgRender(renderer, fg4, 0, 0, 0, NULL, NULL, NULL, no, 255);
					break;
				case 4:
					imgRender(renderer, fg5, 0, 0, 0, NULL, NULL, NULL, no, 255);
					break;
				}
				stt = loadTextTexture(final_score, "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 150, 255, 255, 255, BLENDED, NULL, NULL, NULL);
				stt1 = loadTextTexture(final_score, "../fonts/GenJyuuGothicL-Monospace-ExtraLight.ttf", 150, 25, 25, 25, BLENDED, NULL, NULL, NULL);
				
				textRender(renderer, stt1, ((WIDTH - stt1.width) / 2) + 7, 264, NULL, NULL, NULL, SDL_FLIP_NONE, 255);
				textRender(renderer, stt, (WIDTH - stt.width) / 2, 260, NULL, NULL, NULL, SDL_FLIP_NONE, 255);

				SDL_DestroyTexture(stt1.texture);
				SDL_DestroyTexture(stt.texture);
			}
			if (menu == 3) {
				pause = true;
				drop[0] = 0;
				imgRender(renderer, score_logo, 0, 0, 0, NULL, NULL, NULL, no, 255);
				imgRender(renderer, exits, 0, 0, 0, NULL, NULL, NULL, no, 255);
			}	
		}
		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Free loaded image
	SDL_RemoveTimer(timerID_startrotate);
	SDL_RemoveTimer(timerID_taptostart);
	SDL_RemoveTimer(timerID_colorchange);
	SDL_RemoveTimer(timerID_BgDot);
	SDL_DestroyTexture(games.texture);
	SDL_DestroyTexture(exits.texture);
	SDL_DestroyTexture(start.texture);
	SDL_DestroyTexture(tapfirst.texture);
	SDL_DestroyTexture(b1.texture);
	SDL_DestroyTexture(b2.texture);
	SDL_DestroyTexture(b3.texture);
	SDL_DestroyTexture(b4.texture);
	SDL_DestroyTexture(gover.texture);
	SDL_DestroyTexture(fg1.texture);
	SDL_DestroyTexture(fg2.texture);
	SDL_DestroyTexture(fg3.texture);
	SDL_DestroyTexture(fg4.texture);
	SDL_DestroyTexture(fg5.texture);
	//SDL_DestroyTexture(score_logo.texture);
	//SDL_DestroyTexture(pause_logo.texture);
	//Free resources and close SDL
	closeSDL();

	return 0;
}