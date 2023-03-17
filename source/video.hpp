#pragma once
/* Core code to handle the video output, including console.  */
#include <gccore.h>
#include <stdint.h>

#include "entt.hpp"

struct VideoDetails {
    uint64_t frame;
};

class Video {
    public:
        Video(entt::registry *registry);
        void vsync();

        GXRModeObj *get_rmode();
        void *get_framebuffer();

        void toggle_console();
        void setup_console();

    private:
        void *xfb[2];
        void *console_xfb;
        int active_buffer = 0;
        bool console = 0;

        GXRModeObj *rmode = NULL;
        entt::registry *registry;
        entt::entity entity;

        uint64_t frame = 0;
};
