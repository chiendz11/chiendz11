#include "Button.h"

Button::Button()
	: CurrentState(ButtonState::BUTTON_MOUSE_OUT),
	  Texture(nullptr),
	  ButtonRect({ 0, 0, 0, 0 }),
	  TextureRect({0, 0, 0, 0}),
	  MouseOutColour({ 190, 255, 255, SDL_ALPHA_OPAQUE }), // light blue
	  MouseOverMotionColour({ 95, 89, 191, SDL_ALPHA_OPAQUE }),//blue
	  MouseDownColour({ 91, 191, 116, SDL_ALPHA_OPAQUE }), // green
	  MouseUpColour({ 95, 89, 191, SDL_ALPHA_OPAQUE }), // blue
	  Selected(false)
{

}
Button::~Button()
{
	ButtonRect  = { -100,-100, 0, 0 };
	Texture = nullptr;
	
}

void Button::setTexture(SDL_Texture* texture)
{
	Texture = texture;
}

void Button::setButtonRect(const SDL_Rect& rect)
{
	ButtonRect = rect;
}

void Button::setTextureRect(const SDL_Rect& rect)
{
	TextureRect = rect;
}

void Button::centerTextureRect()
{
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(Texture, NULL, NULL, &textureWidth, &textureHeight);

	const int textureStartRow = ButtonRect.y + 0.5 * (ButtonRect.h - textureHeight);
	const int textureStartCol = ButtonRect.x + 0.5 * (ButtonRect.w - textureWidth);

	TextureRect = { textureStartCol, textureStartRow, textureWidth, textureHeight };
}

void Button::setSelected(const bool selected)
{
	Selected = selected;
}

bool Button::isMouseInside(const int x, const int y)
{
	// Check if mouse is in button
	bool inside = true;

	// Mouse is left of the button
	if (x < ButtonRect.x)
	{
		inside = false;
	}
	// Mouse is right of the button
	else if (x > ButtonRect.x + ButtonRect.w)
	{
		inside = false;
	}
	// Mouse above the button
	else if (y < ButtonRect.y)
	{
		inside = false;
	}
	// Mouse below the button
	else if (y > ButtonRect.y + ButtonRect.h)
	{
		inside = false;
	}

	return inside;
}

ButtonState Button::getMouseEvent(const SDL_Event* event)
{
	//If mouse event happened
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
	{
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Mouse is outside button
		if (!isMouseInside(x, y))
		{
			CurrentState = ButtonState::BUTTON_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			// Set mouse over State
			switch (event->type)
			{
			case SDL_MOUSEMOTION:
				CurrentState = ButtonState::BUTTON_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				CurrentState = ButtonState::BUTTON_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				CurrentState = ButtonState::BUTTON_MOUSE_UP;
				break;

			}
		}
	}
	return CurrentState;
}

void Button::setMouseDownColour(const SDL_Color& colour)
{
	MouseDownColour = colour;
}

void Button::renderButton(SDL_Renderer* renderer)
{
	// If current button is selected
	if (Selected)
	{
		SDL_SetRenderDrawColor(renderer, MouseDownColour.r, MouseDownColour.g, MouseDownColour.b, MouseDownColour.a);
	}
	else
	{
		// Show current button State
		switch (CurrentState)
		{
		case ButtonState::BUTTON_MOUSE_OUT:
			SDL_SetRenderDrawColor(renderer, MouseOutColour.r, MouseOutColour.g, MouseOutColour.b, MouseOutColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_OVER_MOTION:
			SDL_SetRenderDrawColor(renderer, MouseOverMotionColour.r, MouseOverMotionColour.g, MouseOverMotionColour.b, MouseOverMotionColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_DOWN:
			SDL_SetRenderDrawColor(renderer, MouseDownColour.r, MouseDownColour.g, MouseDownColour.b, MouseDownColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_UP:
			SDL_SetRenderDrawColor(renderer, MouseUpColour.r, MouseUpColour.g, MouseUpColour.b, MouseUpColour.a);
			break;

		}
	}

	SDL_RenderFillRect(renderer, &ButtonRect);
}

void Button::renderTexture(SDL_Renderer* renderer)
{
	// Set rendering space
	SDL_RenderCopy(renderer, Texture, nullptr, &TextureRect);
}

