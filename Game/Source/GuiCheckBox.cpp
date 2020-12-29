#include "GuiCheckBox.h"
#include "SceneManager.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, bool initState) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;

    checkBox.w = (bounds.w / 5);
    checkBox.h = bounds.h;

    checkBox.x = bounds.x + ((bounds.w / 6) * 5);
    checkBox.y = bounds.y;

    this->font = app->sceneManager->GetGuiFont();

    checked = initState;

}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{

	bool ret = true;
    checkBox.x = bounds.x + ((bounds.w / 6) * 5);
    checkBox.y = bounds.y;
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
				checked = !checked;
				ret = NotifyObserver();
			}
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			app->audio->PlayFx(app->sceneManager->btnDisabled);
	}
	else if (state != GuiControlState::DISABLED) state = GuiControlState::NORMAL, mouseIn = false;

	return ret;
}

bool GuiCheckBox::Draw()
{


    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (checked) {
            app->render->DrawRectangle(bounds, 100, 100, 100, 255);
            app->render->DrawRectangle(checkBox, 180, 180, 180, 255);
        }
        else {
            app->render->DrawRectangle(bounds, 100, 100, 100, 255);
            app->render->DrawRectangle(checkBox, 130, 130, 130, 255);
        }
    } break;
    case GuiControlState::NORMAL: 
    {
        if (checked) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        else app->render->DrawRectangle(bounds, 0, 255, 0, 255);

        if (checked) app->render->DrawRectangle(checkBox, 255, 0, 0, 255);
        else app->render->DrawRectangle(checkBox, 100, 100, 0, 255);
        break;
    }
    case GuiControlState::FOCUSED: 
    {
        app->render->DrawRectangle(bounds, 255, 255, 0, 255);

        if (checked) app->render->DrawRectangle(checkBox, 255, 0, 0, 255);
        else app->render->DrawRectangle(checkBox, 100, 100, 0, 255);

        break;
    }
    case GuiControlState::PRESSED:
    {
        app->render->DrawRectangle(bounds, 0, 255, 255, 255);

        if (checked) app->render->DrawRectangle(checkBox, 255, 0, 0, 255);
        else app->render->DrawRectangle(checkBox, 100, 100, 0, 255);
        break;
    }
    case GuiControlState::SELECTED:
    {
        app->render->DrawRectangle(bounds, 0, 255, 0, 255);

        if (checked) app->render->DrawRectangle(checkBox, 255, 0, 0, 255);
        else app->render->DrawRectangle(checkBox, 100, 100, 0, 255);
        break;
    }
    default:
        break;
    }


    int centradoY, centradoX;

    centradoX = (bounds.w / 2) - (((text.Length() / 2) + 0.5f) * 14) - checkBox.w/2;

    centradoY = bounds.h / 4;

    app->fonts->BlitText(bounds.x + centradoX, bounds.y + centradoY, font, text.GetString());


    return false;
}
