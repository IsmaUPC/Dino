#ifndef __GUISETTINGS_H__
#define __GUISETTINGS_H__


#include "Point.h"
#include "SString.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiControl.h"


class GuiSettings
{
public:

    GuiSettings(iPoint Position, SceneControl* moduleObserver);

    virtual ~GuiSettings();

    bool Update(float dt);
    bool Draw();

    bool CleanUp();

    void AbleDisableSetting() {
        active = !active;
    }

private:

    GuiSlider* sldMusic;
    GuiSlider* sldFx;
    GuiButton* btnBack;

    iPoint initialPos;

    bool active;

};
#endif // __GUISETTINGS_H__



