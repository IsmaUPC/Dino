#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
	minValue = 0;
	maxValue = 100;
	value = 50;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
			if (input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
			{
				state = GuiControlState::PRESSED;
				value = ((maxValue - minValue) * (mouseX - (float)(bounds.x + slider.w / 2))) / (float)(bounds.w - slider.w) + minValue;
				if (slider.w > 0) slider.x = mouseX - slider.w / 2;  // Slider
				else if (slider.w == 0) slider.w = value;          // SliderBar
			}
            else state = GuiControlState::FOCUSED;
            // TODO.
        }
        else state = GuiControlState::NORMAL;

		if (value > maxValue) value = maxValue;
		else if (value < minValue) value = minValue;
    }
	 //Bar limits check
	//if (slider.w > 0)        // Slider
	//{
	//	if (slider.x <= (bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH))) slider.x = bounds.x + GuiGetStyle(SLIDER, BORDER_WIDTH);
	//	else if ((slider.x + slider.w) >= (bounds.x + bounds.w)) slider.x = bounds.x + bounds.w - slider.w - GuiGetStyle(SLIDER, BORDER_WIDTH);
	//}
	//else if (slider.w == 0)  // SliderBar
	//{
	//	if (slider.w > bounds.w) slider.w = bounds.w - 2 * GuiGetStyle(SLIDER, BORDER_WIDTH);
	//}
    return false;
}

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
        break;
    case GuiControlState::NORMAL: render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255);
        break;
    case GuiControlState::PRESSED: render->DrawRectangle(bounds, 0, 255, 255, 255);
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
        break;
    default:
        break;
    }

    return false;
}
