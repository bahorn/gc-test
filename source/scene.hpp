#pragma once
#include <vector>
#include <cstddef>
#include <gccore.h>
#include <math.h>
#include "entt.hpp"
#include "video.hpp"

/* Wrapper around GX to draw a scene */

#define DEFAULT_FIFO_SIZE   (256*1024)

class Scene {
    public:
        Scene(Video *video, entt::registry *registry);
        void draw();

        void set_camera(guVector camera_pos, guVector lookat_pos);
        void set_background(GXColor background);

    private:
        void *gp_fifo = NULL;
        Video *video;
        
        guVector camera;
        guVector up;
        guVector look;
        Mtx view;
        Mtx44 projection;

        entt::registry *registry;
};
