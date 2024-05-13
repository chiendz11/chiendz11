
//A Sudoku
				// SDL Lazy Foo' Tutorials = https://lazyfoo.net/tutorials/SDL/index.php
				// SDL Documentation = https://wiki.libsdl.org/FrontPage
				// SDL True Type Fonts = https://www.libsdl.org/projects/SDL_ttf/

#include <SDL.h>
#include <iostream>
#include <time.h>  
#include <SDL_image.h>
#include "Button.h"
#include "SudokuBlank.h"
#include "SolveSudoku.h"

namespace Sudoku
{
	class Sudoku
	{
	private:
		// Define window dimensions
		const int WindowHeight;
		const int WindowWidth;

		// Define Sudoku grid dimensions
		const int GridHeight;
		const int GridWidth;

		// Define Sudoku max grid rows and columns (normally set to 9 x 9)
		const int GridRows;
		const int GridCols;

		// Define window and renderer
		SDL_Window* Window;
		SDL_Renderer* Renderer;

		// Texture cache to hold preloaded textures
		int TotalTextures;
		SDL_Texture* TextureButton[17];
		

		// Define true type font paramaters
		TTF_Font* Font;
		int FontSize;

		// Define total buttons
		const int TotalBlanks;

		// Define grid of cells (normally set to 9 x 9 = 81)
		Blank Grid[81];

		//Define mode buttons
		Button EasyMode;
		Button MediumMode;
		Button HardMode;
		// Define check and new buttons
		Button CheckButton;
		Button RestartButton;

		// Timer
		Button Timer;
		// Generator;
		Run Game;
		// Define colours 
		SDL_Color ClearColour;
		// Button and background images
		SDL_Texture* image;

		SDL_Surface* imageSurface;
		

	private:
		// Intialise SDL window, renderer and true type font
		bool initialiseSDL();

		// Get index of 1D array from row and col
		inline int getIndex(int row, int col) const;

		// Load textures using SDL true type fonts
		void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);

		// Preload textures using SDL true type fonts
		void preloadTextures();

		// Creat Menu
		void Menu();

		// Create interface layout by setting button parameters
		void createInterfaceLayout();
		
		// Generate a Sudoku puzzle with a unique solution
		void generateSudoku(int toremove);
		
		

		// Free textures
		void freeTextures();

	public:
		// Constructor to intialise member variables
		Sudoku();

		// Destructor to free textures
		~Sudoku();

	public:
		// Play Sudoku
		void play();

		// Close Sudoku
		void close();

		// Youlost background;
		void Youlost();
		// Life
		int life;
	};

};