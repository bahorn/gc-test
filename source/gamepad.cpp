#include "gamepad.hpp"
#include <gccore.h>

Gamepad::Gamepad(int pad_, entt::registry *registry_)
{
    pad = pad_;
    PAD_Init();
    registry = registry_;
    entity = registry->create();
    fetch();
    registry->emplace<GamepadStatus>(entity, status);
}

void Gamepad::fetch()
{
    PAD_ScanPads();
    status.pad = pad;
    status.button_down = PAD_ButtonsDown(pad);
    status.button_up = PAD_ButtonsUp(pad);
    status.button_held = PAD_ButtonsHeld(pad);
    status.stickX = ((float)PAD_StickX(pad)) / 128.0;
    status.stickY = ((float)PAD_StickY(pad)) / 128.0;
    status.substickX = ((float)PAD_StickX(pad)) / 128.0;;
    status.substickY = ((float)PAD_StickX(pad)) / 128.0;;
    status.triggerL = PAD_TriggerR(pad);
    status.triggerR = PAD_TriggerR(pad);

}

void Gamepad::update()
{
    /* Update */
    fetch();
    registry->replace<GamepadStatus>(entity, status);
}
