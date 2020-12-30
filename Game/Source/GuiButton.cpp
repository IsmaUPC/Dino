#include "GuiButton.h"
#include "SceneManager.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* texture ) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    this->texture = texture;
	SDL_QueryTexture(texture, NULL, NULL, &texturW, &texturH);
	texturH /= 4;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
	bool ret = true;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	// Check collision between mouse and button bounds
	if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
		(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
	{
		if (state != GuiControlState::DISABLED)
		{
			state = GuiControlState::FOCUSED;
			if (!mouseIn)app->audio->PlayFx(app->sceneManager->btnSelected), mouseIn = true;
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				ret = NotifyObserver();
				app->audio->PlayFx(app->sceneManager->btnPressed);
			}
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			app->audio->PlayFx(app->sceneManager->btnDisabled);
	}
	else if (state != GuiControlState::DISABLED) state = GuiControlState::NORMAL, mouseIn = false;


	return ret;
}

bool GuiButton::Draw()
{
	SDL_Rect rect = { 0,texturH,texturW,texturH };

    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: 
		app->render->DrawRectangle(bounds, 100, 100, 100, 255);
		rect.y*= 3;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::NORMAL: 
		app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		rect.y *= 0;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::FOCUSED: 
		app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		rect.y *= 1;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::PRESSED:
		app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		rect.y *= 2;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::SELECTED:
		app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }

    return true;
}
