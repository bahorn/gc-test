#pragma once
#include <stdint.h>

#include "entt.hpp"

/* requires a custom struct as PAD_Read() doesn't seem to work? */
struct GamepadStatus {
    int pad;
    uint16_t button_down;
    uint16_t button_up;
    uint16_t button_held;
	float stickX;
	float stickY;
	float substickX;
	float substickY;
	uint8_t triggerL;
	uint8_t triggerR;
    /* Not sure how to read these */
	uint8_t analogA;
	uint8_t analogB;
	int8_t err;
};

class Gamepad {
    public:
        Gamepad(int pad, entt::registry *registry);
        void update();
        void fetch();

    private:
        int pad;
        entt::registry *registry;
        entt::entity entity;

        struct GamepadStatus status;
};
