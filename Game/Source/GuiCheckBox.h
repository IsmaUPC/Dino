#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(uint32 id, SDL_Rect bounds, const char *text, bool initState = false);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw();

    bool GetValue() { return checked; };
    void SetValue(bool state) { checked = state; };

private:

    // GuiCheckBox specific properties
    // Maybe some animation properties for state change?

    SDL_Rect checkBox;

    bool checked;
};

#endif // __GUICHECKBOX_H__
