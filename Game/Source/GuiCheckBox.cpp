#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, bool initState) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;

    
    bounds.w = (bounds.w / 6) * 5;

    checkBox.w = (bounds.w / 5) + 5;
    checkBox.h = bounds.h;

    checkBox.x = bounds.x + bounds.w;
    checkBox.y = bounds.y;

    checked = initState;

}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
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
    } break;
    case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, 255, 255, 0, 255);
        break;
    case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 0, 255, 255, 255);
        break;
    case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }

    if (checked) app->render->DrawRectangle(checkBox, 255, 0, 0, 255);
    else app->render->DrawRectangle(checkBox, 100, 100, 0, 255);

    return false;
}
