#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdint.h>
#include <math.h>

#include "game.hpp"
#include "model.h"


Game::Game() {
    /* Initial Setup */
    video = new Video(&registry);
    scene = new Scene(video, &registry);
    scene->set_camera({0.0, 2.5, 5.0}, {0.0, 0.0, -1.0} );
    for (int i = 0; i < 4; i++) {
        gamepads[i] = new Gamepad(i, &registry);
    }

    for(auto i = 0u; i < 1u; ++i) {
        struct faces faces_build = {faces_vertices, faces_textures, faces_normals};
        auto model = new Model(&registry);
        model->set_vertices(vertices);
        model->set_texture(texture);
        model->set_normals(normals);
        model->set_faces(&faces_build, mesh_size);
        model->active(true);
        model->update();
        model->translate(0.0, 0.0, -1.0);
        models.push_back(model);
    }

    std::cout << "Starting game!" << std::endl;


    tick = 0.0;
}


void Game::frame()
{
    /* Get the info we need for a frame. */
    video->vsync();
    for (auto gamepad : gamepads) {
        gamepad->update();
    }

    /* Start running the games systems. */
    
    auto view = registry.view<GamepadStatus>();
    for (auto entity: view) {
        auto vel = view.get<GamepadStatus>(entity);
        /* Don't care about the other controllers. */
        if (vel.pad != 0) {
            continue;
        }

        /* Console toggling for debug */
        if (vel.button_down & PAD_BUTTON_START && vel.button_held & PAD_BUTTON_A) {
            video->toggle_console();
        }

        if (vel.button_down & PAD_BUTTON_B) {
        }

        models[0]->translate(0.1 * vel.stickX, 0.0, 0.1 * vel.stickY);
        models[0]->update();
        break;
    }

    scene->set_background({0, 0, 0});//128 + 64 * sin(tick), 255});

    scene->draw();
    tick += 0.01;
}
