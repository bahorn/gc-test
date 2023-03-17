#pragma once

#include <array>
#include <vector>

#include "video/video.hpp"
#include "scene/scene.hpp"
#include "scene/model.hpp"
#include "gamepad/gamepad.hpp"

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
