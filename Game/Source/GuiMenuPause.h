#ifndef __GUIMENUPAUSE_H__
#define __GUIMENUPAUSE_H__

#include "Point.h"
#include "SString.h"

#include "GuiButton.h"
#include "GuiControl.h"
#include "GuiSettings.h"

class GuiMenuPause
{
public:
	GuiMenuPause(iPoint Position,SceneControl* moduleObserver);
	~GuiMenuPause();

    bool Update(float dt);
    bool Draw();

    bool CleanUp();

    bool Event(GuiControl* control);

    void AbleDisableSetting() {
        active = !active;
    }
    void AbleDisableMenu() {
        activeMenu = !activeMenu;
    }

    void MovePosition();

private:
    
    GuiButton* btnResume;
    GuiButton* btnSettings;
    GuiButton* btnBackToTitle;
    GuiButton* btnExit;

    GuiSettings* menuSettings;

    iPoint initialPos;

    SceneControl* observer;

    bool active;
    bool activeMenu;
};

#endif // !__GUIMENUPAUSE_H__

