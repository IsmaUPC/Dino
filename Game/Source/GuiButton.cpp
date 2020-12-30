#include "GuiButton.h"
#include "SceneManager.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, TypeButton typeButton, SDL_Texture* texture ) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;
    this->texture = texture;
	this->typeButton = typeButton;
	this->font = app->sceneManager->GetGuiFont();

	//SDL_QueryTexture(texture, NULL, NULL, &texturW, &texturH);
	//texturH /= 4;


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
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				app->audio->PlayFx(app->sceneManager->btnPressed);
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				ret = NotifyObserver();
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
	
	DefinePositionAtlas();


    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: 
		//app->render->DrawRectangle(bounds, 100, 100, 100, 255);
		rect.x+= 3 * rect.w;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::NORMAL: 
		//app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::FOCUSED: 
		//app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		rect.x+= 1*rect.w;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::PRESSED:
	//	app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		rect.x+= 2 * rect.w;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
        break;
    case GuiControlState::SELECTED:
	//	app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }

	int centradoY,centradoX;

	centradoX = (bounds.w / 2) - (((float)(text.Length() / 2)+0.5f) * 14);

	centradoY = bounds.h/4;
	
	app->fonts->BlitText(bounds.x + centradoX, bounds.y + centradoY, font, text.GetString());

    return true;
}

void GuiButton::DefinePositionAtlas()
{

	switch (typeButton)
	{
	case RECTANGLE:
		rect = { rectAtlasPos->x,rectAtlasPos->y,rectTexW + margin,rectTexH };
		break;
	case REMOVE:
		rect = { removeAtlasPos->x,removeAtlasPos->y,squareTexW + marginSquare,squareTexH };
		break;
	case CREDITS:
		rect = { creditAtlasPos->x,creditAtlasPos->y,squareTexW + marginSquare,squareTexH };
		break;
	case EXIT:
		rect = { exitAtlasPos->x,exitAtlasPos->y,squareTexW + marginSquare,squareTexH };
		break;
	default:
		break;
	}

}
