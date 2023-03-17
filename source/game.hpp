#pragma once

#include <array>
#include <vector>

#include "video.hpp"
#include "scene.hpp"
#include "gamepad.hpp"
#include "model.hpp"
#include "entt.hpp"

class Game {
    public:
        Game();
        void frame();

    private:
        float tick;
        Video *video;
        Scene *scene;
        std::array<Gamepad *, 4> gamepads;

        entt::registry registry;
        std::vector<Model *> models;
};
