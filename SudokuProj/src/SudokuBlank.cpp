#include "SudokuBlank.h"

Sudoku::Blank::Blank()
	: mEditable(false),
	  mCharNumber(' '), 
	  mCharSolution(' ')
{

}
Sudoku::Blank::~Blank()
{
	S.~Button();
}
void Sudoku::Blank::setNumber(const int number)
{
	if (number == 0)
	{
		mCharNumber = ' ';
	}
	else
	{
		mCharNumber = '0' + number;
	}
}

char Sudoku::Blank::getNumber() const
{
	return mCharNumber;
}

void Sudoku::Blank::setSolution(const int solution)
{
	if (solution == 0)
	{
		mCharSolution = ' ';
	}
	else
	{
		mCharSolution = '0' + solution;
	}
}

void Sudoku::Blank::setEditable(const bool editable)
{
	mEditable = editable;
	if (mEditable)
	{
		MouseOutColour = { 255, 255, 255, SDL_ALPHA_OPAQUE }; //  white
		MouseOverMotionColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // light purple
		MouseDownColour = { 91, 191, 116, SDL_ALPHA_OPAQUE }; // green
		MouseUpColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
	}
	else
	{
		MouseOutColour = { 190,255, 255, SDL_ALPHA_OPAQUE }; // light blue
	    MouseOverMotionColour = { 190, 255, 255, SDL_ALPHA_OPAQUE }; // light blue
        MouseDownColour = { 190, 255, 255, SDL_ALPHA_OPAQUE }; // light blue
		MouseUpColour = { 190, 255, 255, SDL_ALPHA_OPAQUE }; // light blue;
	}
}

bool Sudoku::Blank::isEditable() const
{
	return mEditable;
}

void Sudoku::Blank::handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[])
{
	// Handle backspace
	if (event->key.keysym.sym == SDLK_BACKSPACE && mCharNumber != ' ')
	{
		// Empty char
		mCharNumber = ' ';

		// Set empty texture
		setTexture(textureCache[0]);
	}
	// Handle text input
	else if (event->type == SDL_TEXTINPUT)
	{
		// Check if integer > 0
		if (atoi(event->text.text))
		{
			// Replace char
			mCharNumber = *(event->text.text);

			// Set character based on number
			setTexture(textureCache[atoi(event->text.text)]);

		}
	}
}


bool Sudoku::Blank::isCorrect() const
{
	return mCharNumber == mCharSolution;
}

