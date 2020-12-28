#include "GuiSlider.h"
#include "Log.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds,const char* text = "SLIDER", int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;

    if (min > max)
    {
        this->minValue = max;
        this->maxValue = min;
        LOG("The minimum value is higher than the maximum, they have been changed to avoid errors");
    }
    else
    {
        this->minValue = min;
        this->maxValue = max;
    }

    SetValue(minValue);

    slider.y = bounds.y;
    slider.w = bounds.w / 10;
    slider.h = bounds.h;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
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

            if (state == GuiControlState::PRESSED)
            {
                SliderControl(mouseX, mouseY);
                NotifyObserver();
            }
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(app->sceneManager->btnSlider);
			}
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw()
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:

        app->render->DrawRectangle(bounds, 100, 100, 100, 255);
        app->render->DrawRectangle(slider,120,120,120,255);

        break;
    case GuiControlState::NORMAL:
        app->render->DrawRectangle(bounds,0, 255, 0, 255);
        app->render->DrawRectangle(slider,255,0,255,255);

        break;
    case GuiControlState::FOCUSED:
        app->render->DrawRectangle(bounds,255, 255, 0, 255);
        app->render->DrawRectangle(slider,255,0,255,255);

        break;
    case GuiControlState::PRESSED:
        app->render->DrawRectangle(bounds, 0, 255, 255, 255);
        app->render->DrawRectangle(slider,255,0,0,255);

        break;
    case GuiControlState::SELECTED:
        app->render->DrawRectangle(bounds,0, 255, 0, 255);
        app->render->DrawRectangle(slider,255,0,255,255);

        break;
    default:
        break;
    }

    return false;
}

void GuiSlider::SliderControl(int mouseX, int mouseY)
{

    slider.x = mouseX - (slider.w / 2);

    value = ((maxValue - minValue) * (mouseX - (float)(bounds.x + slider.w / 2))) / (float)(bounds.w - slider.w) + minValue;

    //Limites
    if (slider.x < bounds.x)
    {
        slider.x = bounds.x;
        value = minValue;
    }
    if ((slider.x + slider.w) > (bounds.x + bounds.w))
    {
        slider.x = (bounds.x + bounds.w) - slider.w;
        value = maxValue;
    }

}

void GuiSlider::SetValue(int newValue)
{

    this->value = newValue;
    //Permite colocar el slider en la posicion que del valor inicial
    float X, h, h1, h2, h3, i;
    h1 = (((bounds.w - slider.w) + minValue) * (maxValue - minValue));
    h2 = maxValue - minValue;
    h3 = (bounds.w - slider.w) + minValue;

    h = (h1 * h2) / h3;
    i = (((bounds.w - slider.w) + minValue) * (maxValue - minValue)) * value;
    X = (i / h) + (bounds.x + (slider.w / 2));
    slider.x = (int)X;

}
