#include "Sudoku.h"

//----------------------------------Constructor and Destructor---------------------------------//
Sudoku::Sudoku::Sudoku()
	: WindowHeight(880), WindowWidth(720),
	GridHeight(720), GridWidth(720),
	GridRows(9), GridCols(9),
	Window(nullptr), Renderer(nullptr),
	TotalTextures(13), TextureButton{ nullptr },
	Font(nullptr), FontSize(GridHeight / 9),
	TotalBlanks(81),
	ClearColour({ 0, 0, 0, SDL_ALPHA_OPAQUE }), life(3)
{

}

Sudoku::Sudoku::~Sudoku()
{
	freeTextures();
	close();
}

//--------------------------------------Private methods----------------------------------------//
bool Sudoku::Sudoku::initialiseSDL()
{
	// Set success initialisation flag
	bool success = true;

	// Initalise SDL video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL could not intialise! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Initialise SDL_ttf
	if (TTF_Init() == -1)
	{
		
		success = false;
	}

	// Create window
	Window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (Window == nullptr)
	{
		std::cout << "SDL could not create window! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Create renderer
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == nullptr)
	{
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Load font for text
	Font = TTF_OpenFont("assets/octin sports free.ttf", FontSize);
	if (Font == nullptr)
	{
		std::cout << "Failed to load lazy font! Error: " << TTF_GetError() << std::endl;
		success = false;
	}

	return success;
}

inline int Sudoku::Sudoku::getIndex(int row, int col) const
{
	return row * GridRows + col;
}

void Sudoku::Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour)
{
	// Create text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text, fontColour);
	if (textSurface == nullptr)
	{
		std::cout << "Could not create TTF SDL_Surface! Error: " << TTF_GetError() << std::endl;
	}
	else
	{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(Renderer, textSurface);
		if (texture == nullptr)
		{
			std::cout << "Could not create texture from surface! Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);
	}
}

void Sudoku::Sudoku::preloadTextures()
{
	// Choose colour of font
	SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black

	// Load texture for empty space
	loadTexture(TextureButton[0], " ", fontColour);

	// Load textures for numbers from 1 to 9
	for (int num = 1; num < 10; num++)
	{
		const char temp[] = { '0' + num, '\0' };
		loadTexture(TextureButton[num], temp, fontColour);
	}

	// Load texture for "Check", "Solve", and "New" buttons
	loadTexture(TextureButton[10], "Check", fontColour);
	loadTexture(TextureButton[11], "Restart", fontColour);
	loadTexture(TextureButton[12], "Wrong!", fontColour);
	loadTexture(TextureButton[13], "Right!", fontColour);
	
}
void Sudoku::Sudoku::Menu()
{   SDL_Texture* backgrounds;
    // EasyMode Pos
	int buttonWidth = 150;
	int buttonHeight = 50;
	int EasyButtonX = WindowWidth / 2 - 100;
	int EasyButtonY = 200;
	

	SDL_Rect buttonRect1 = { EasyButtonX, EasyButtonY, buttonWidth, buttonHeight };
	EasyMode.setButtonRect(buttonRect1);
	SDL_Surface* imageSurfaces  = IMG_Load("assets/easy.png");
	backgrounds = SDL_CreateTextureFromSurface(Renderer, imageSurfaces);
	EasyMode.setTexture(backgrounds);

	// MediumMode Pos
	int MediumButtonX = EasyButtonX;
	int MediumButtonY = EasyButtonY + 200;
	SDL_Rect buttonRect2 = { MediumButtonX, MediumButtonY,150, buttonHeight };
	MediumMode.setButtonRect(buttonRect2);
    imageSurfaces = IMG_Load("assets/medium.png");
	backgrounds = SDL_CreateTextureFromSurface(Renderer, imageSurfaces);
	MediumMode.setTexture(backgrounds);
	//HardMode Pos
	int HardButtonX = MediumButtonX;
	int HardButtonY = MediumButtonY + 200;
	SDL_Rect buttonRect3 = { HardButtonX, HardButtonY, buttonWidth, buttonHeight };
	imageSurfaces = IMG_Load("assets/hard.png");
	HardMode.setButtonRect(buttonRect3);
	backgrounds = SDL_CreateTextureFromSurface(Renderer, imageSurfaces);
	HardMode.setTexture(backgrounds);
}
void Sudoku::Sudoku::createInterfaceLayout()
{
	// Define thick and thin borders
	const int thinBorder = 2;
	const int thickBorder = thinBorder + 6;

	/*----------------------------------------Display Stopwatch------------------------------------------*/
	// Treat stopwatch as a button that can't be clicked
	int buttonStartRow = 0;
	int buttonWidth = GridWidth - 2 * thickBorder;
	// mWindowHeight = buttonHeight + 6 * thinBorder + 6 * thickBorder (rearange this equation)
	int buttonHeight = (WindowHeight - 6 * thinBorder - 6 * thickBorder) / 11;

	buttonStartRow += thickBorder;
	int buttonStartCol = 0;
	buttonStartCol += thickBorder;
	// Set button position and dimensions
	SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
	Timer.setButtonRect(buttonRect);
	/*----------------------------------Create buttons for each cell-------------------------------------*/
	// Define cell button dimensions
	// mGridWidth = 6 * thinBorder + 4 * thickBorder + 9 * buttonWidth (rearrange this equation)
	buttonWidth = (GridWidth - 6 * thinBorder - 4 * thickBorder) / GridCols;

	// Carry on from previous starting row
	buttonStartRow += buttonHeight;

	// Set cell button position and dimensions
	for (int gridRow = 0; gridRow < GridRows; gridRow++)
	{
		// Add to starting row
		if (gridRow == 0) buttonStartRow += thickBorder;
		else if (gridRow % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
		else buttonStartRow += buttonHeight + thinBorder;

		// Reset starting column
		int buttonStartCol = 0;

		for (int gridCol = 0; gridCol < GridCols; gridCol++)
		{
			// Add to starting column
			if (gridCol == 0) buttonStartCol += thickBorder;
			else if (gridCol % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
			else buttonStartCol += buttonWidth + thinBorder;

			// Set button position and dimensions
			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = getIndex(gridRow, gridCol);
			Grid[index].setButtonRect(buttonRect);
		}
	}

	/*----------------------------------Create check, and new buttons-------------------------------------*/
	const int numberOfOtherButtons = 2;
	CheckButton.setTexture(TextureButton[10]);
	RestartButton.setTexture(TextureButton[11]);
	Button* otherButtons[numberOfOtherButtons] = { &CheckButton, &RestartButton };

	// Redefine button width
	// mGridWidth = 3 * thickBorder + 9 * numberOfOtherButtons (rearrange this equation)
	buttonWidth = (GridWidth - 3 * thickBorder) / numberOfOtherButtons;

	// Carry on from previous starting row
	buttonStartRow += buttonHeight + thickBorder;

	// Reset starting column
	int borderWidthTotal = 0;

	// Set check, solve, and new buttons (last row)
	for (int button = 0; button < numberOfOtherButtons; button++) // colBlock is every 3 columns of cells
	{
		// Add border width to total
		if (button == 0) borderWidthTotal += thickBorder;
		else borderWidthTotal += thickBorder;
		int buttonStartCol = button * buttonWidth + borderWidthTotal;

		// Set button position and dimensions
		SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
		otherButtons[button]->setButtonRect(buttonRect);
	}

}

void Sudoku::Sudoku::generateSudoku(int toremove)
{
	// Create empty an empty grid to store generated Sudoku
	int generatedGrid[81] = { };

	// Create empty an empty grid to store solution to generated Sudoku
	int solution[81] = { };

	// Instantiate a Sudoku generator object and generate Sudoku with the empty grids
	
	Game.generate(generatedGrid, solution, toremove);

	for (int i = 0; i < 81; i++)
	{
		// Set number and solution
		Grid[i].setNumber(generatedGrid[i]);
		Grid[i].setSolution(solution[i]);

		// Set editability
		if (generatedGrid[i] == 0)
		{
			// This is a cell that can be editable
			Grid[i].setEditable(true);
		}
		else
		{
			// This cell is fixed and cannot be edited
			Grid[i].setEditable(false);
		}

		// Set texture (0 = ' ', 1 to 9 = '1' '2'... '9')
		Grid[i].setTexture(TextureButton[generatedGrid[i]]);

		// Center texture onto button
		Grid[i].centerTextureRect();

	}
}

void Sudoku::Sudoku::freeTextures()
{
	for (int i = 0; i < TotalTextures; i++)
	{
		// Free texture if it exists
		if (TextureButton[i] != nullptr)
		{
			SDL_DestroyTexture(TextureButton[i]);
			
			TextureButton[i] = nullptr;
		}
	}
}

//--------------------------------------Public methods----------------------------------------//
void Sudoku::Sudoku::play()
{
	// Initialise SDL
	if (!initialiseSDL())
	{
		close();
	}
	// Preload textures for Sudoku grid and buttons
	preloadTextures();

	// Create Menu
	Menu();

	// Set first current cell selected
	Blank* currentCellSelected = &Grid[0];
	for (int cell = 0; cell < TotalBlanks; cell++)
	{
		if (Grid[cell].isEditable())
		{
			currentCellSelected = &Grid[cell];
			currentCellSelected->setSelected(true);
			break;
		}
	}

	// Enable text input
	SDL_StartTextInput();

	// Loop variables
	SDL_Event event;
	bool StartEasy = false;
	bool StartMedium = false;
	bool StartHard = false;
	bool EasyContinue = false;
	bool MediumContinue = false;
	bool HardContinue = false;
	bool stop = false;
	bool completed = false;
	bool RestartSudoku = false;
	bool checkSolution = false;
	

	// Timing for check button
	bool measureTimeForCheckButton = false;
	time_t startTimeForCheckButton;

	// Timer
	time_t startTimer;
	time(&startTimer);

	// Game loop
	while (!stop)
	{
		// Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			// Handle Easymode
			if (EasyMode.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				StartEasy = true;
			}
			// Handle Mediummode
			if (MediumMode.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				StartMedium = true;
			}
			if (HardMode.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				StartHard = true;
			}
			// Handle quiting and completion
			if (event.type == SDL_QUIT)
			{
				// Set stop flag
				stop = true;
			}

			// Handle mouse event for "Check" button
			if (CheckButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				// Set check solution flag
				checkSolution = true;
			}
			// Handle mouse event for "New" button
			if (RestartButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				// Set generate new Sudoku flag
				RestartSudoku = true;
			}
			// Handle mouse event for cells
			for (int blank = 0; blank < TotalBlanks; blank++)
			{
				// If editable
				if (Grid[blank].isEditable())
				{
					// Set button state and return if mouse pressed on cell
					if (Grid[blank].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
					{
						// Set current cell selected to false
						currentCellSelected->setSelected(false);

						// Set new cell selected to true
						currentCellSelected = &Grid[blank];
						currentCellSelected->setSelected(true);
					}
				}
			}
			// Handle keyboard events for current cell selected
			currentCellSelected->handleKeyboardEvent(&event, TextureButton);
		}
		if (StartEasy)
		{
			createInterfaceLayout();
			generateSudoku(25);
			Game.~Run();
			EasyMode.~Button();
			MediumMode.~Button();
			HardMode.~Button();
			StartEasy = false;
			EasyContinue = true;
			SDL_RenderClear(Renderer);
		}
		if (StartMedium)
		{
			createInterfaceLayout();
			generateSudoku(40);
			Game.~Run();
			EasyMode.~Button();
			MediumMode.~Button();
			HardMode.~Button();
			SDL_DestroyTexture(TextureButton[14]);
			StartMedium = false;
			MediumContinue = true;
			SDL_RenderClear(Renderer);
		}
		if (StartHard)
		{
			createInterfaceLayout();
			generateSudoku(55);
			Game.~Run();
			EasyMode.~Button();
			MediumMode.~Button();
			HardMode.~Button();
			StartHard = false;
			HardContinue = true;
			SDL_RenderClear(Renderer);
		}
		// If "Restart" button was clicked
		if (RestartSudoku)
		{
			life = 3;
			// Generate new sudoku
			if (EasyContinue)
			{
				createInterfaceLayout();
				generateSudoku(25);
			}
			if (MediumContinue)
			{
				createInterfaceLayout();
				generateSudoku(40);
			}
			if (HardContinue)
			{
				createInterfaceLayout();
				generateSudoku(55);
			}


			// Set current cell selected to false
			currentCellSelected->setSelected(false);

			// Find new starting cell
			for (int cell = 0; cell < TotalBlanks; cell++)
			{
				if (Grid[cell].isEditable())
				{
					currentCellSelected = &Grid[cell];
					currentCellSelected->setSelected(true);
					break;
				}
			}

			// Reset flags
			RestartSudoku = false;
			completed = false;

			// Reset timer
			time(&startTimer);
		}
		// If "Check" button was clicked
		if (checkSolution)
		{
			// Check if complete
			for (int cell = 0; cell < TotalBlanks; cell++)
			{
				if (!Grid[cell].isCorrect())
				{
					completed = false;
					life--;
					break;
				}
				completed = true;
			}

			// Set measure time flag and starting time
			measureTimeForCheckButton = true;
			time(&startTimeForCheckButton);

			// Reset flag
			checkSolution = false;
		}
		
		// If currently measuring time
		if (measureTimeForCheckButton)
		{
			int seconds = 1;

			if (difftime(time(NULL), startTimeForCheckButton) < seconds && completed)
			{
				// Set colour to green
				SDL_Color colour = { 91, 191, 116, SDL_ALPHA_OPAQUE };

				// Set render colour to green
				SDL_SetRenderDrawColor(Renderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);

				// Set texture to "Right!"
				CheckButton.setTexture(TextureButton[13]);

				// Set mouse down colour to green
				CheckButton.setMouseDownColour(colour);
			}
			else if (difftime(time(NULL), startTimeForCheckButton) < seconds && !completed)
			{
				// Set colour to red
				SDL_Color colour = { 200, 73, 46, SDL_ALPHA_OPAQUE };

				// Set render colour to red
				SDL_SetRenderDrawColor(Renderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);

				// Set texture to "Wrong!"
				CheckButton.setTexture(TextureButton[12]);

				// Set mouse down colour to red
				CheckButton.setMouseDownColour(colour);
			}
			else
			{
				// Reset measure time flag
				measureTimeForCheckButton = false;
			}
		}
		else
		{
			// Set texture to "Check"
			CheckButton.setTexture(TextureButton[10]);

			// Set render colour to black
			SDL_SetRenderDrawColor(Renderer, ClearColour.r, ClearColour.g, ClearColour.b, ClearColour.a);
		}
		// Clear screen with rendered colour
		SDL_RenderClear(Renderer);
		if (life == 0)
		{
			//freeTextures();
			Timer.~Button();
			CheckButton.~Button();
			
			for (int i = 0; i < 81; i++)
			{
				Grid[i].~Blank();
			}
			Youlost();
			
		}
		
		// Render buttons and texture of each cell to backbuffer
		for (int cell = 0; cell < TotalBlanks; cell++)
		{
			// Render button
			Grid[cell].renderButton(Renderer);

			// Re-center since diffrerent numbers have different sized textures
			Grid[cell].centerTextureRect();

			// Render texture
			Grid[cell].renderTexture(Renderer);
		}
		// Render easymode button
		EasyMode.renderButton(Renderer);
		EasyMode.centerTextureRect();
		EasyMode.renderTexture(Renderer);
		// Render mediummode button
		MediumMode.renderButton(Renderer);
		MediumMode.centerTextureRect();
		MediumMode.renderTexture(Renderer);
		// Render hardmode button
		HardMode.renderButton(Renderer);
		HardMode.centerTextureRect();
		HardMode.renderTexture(Renderer);
		// Render check button
		CheckButton.renderButton(Renderer);
		CheckButton.centerTextureRect();
		CheckButton.renderTexture(Renderer);

		// Render new button
		RestartButton.renderButton(Renderer);
		RestartButton.centerTextureRect();
		RestartButton.renderTexture(Renderer);
		
		// Calculate timer
		time_t difference = time(NULL) - startTimer;
		tm formattedTime;
		localtime_s(&formattedTime, &difference);
		char timer[80];
		// Load and render timer (TO DO: use preloaded textures to render timer)
		SDL_Texture* timerTexture = nullptr;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black
		strftime(timer, sizeof(timer), "%M:%S", &formattedTime);
		loadTexture(timerTexture, timer, fontColour);
		Timer.setTexture(timerTexture);
		Timer.renderButton(Renderer);
	    Timer.centerTextureRect();
		Timer.renderTexture(Renderer);
		SDL_DestroyTexture(timerTexture);
		timerTexture = nullptr;
        
		// Update screen from backbuffer and clear backbuffer
		SDL_RenderPresent(Renderer);

		// Slow down program becuase it doesn't need to run very fast
		SDL_Delay(10);
	}
	

	// Disable text input
	SDL_StopTextInput();

	// Free button textures
	freeTextures();

	// Destroy and quit
	close();
}

void Sudoku::Sudoku::close()
{
	// Destroy
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	Renderer = nullptr;
	Window = nullptr;

	// Free font
	TTF_CloseFont(Font);
	Font = nullptr;

	// Quit
	SDL_Quit();
	TTF_Quit();
}

void Sudoku::Sudoku::Youlost()
{
        imageSurface = IMG_Load("assets/Youlost.jpg");
		image = SDL_CreateTextureFromSurface(Renderer, imageSurface);
		SDL_FreeSurface(imageSurface);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, image, NULL, NULL);
		SDL_RenderPresent(Renderer);
}
