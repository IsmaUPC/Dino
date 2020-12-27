#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int min=0, int max=100);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw();

    void SliderControl(int mouseX, int mouseY);

    int GetValue() { return value; };
private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    SDL_Rect slider;
    int value;
    int lastValue;

    int minValue;
    int maxValue;
};

#endif // __GUISLIDER_H__
