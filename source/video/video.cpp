#include "video.hpp"

#include <gccore.h>

Video::Video(entt::registry *registry_)
{
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);

    /* Setup framebuffers for double buffering */
    active_buffer = 0;
    xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb[active_buffer]);
    VIDEO_SetBlack(FALSE);
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

    /* Enable the console */
    console = false;
    setup_console();

    /* registry an entity for the frame id */

    registry = registry_;
    entity = registry->create();
    registry->emplace<VideoDetails>(entity, frame);
}

void Video::vsync()
{
    VIDEO_Flush();
    VIDEO_WaitVSync();
    frame += 1;
    registry->replace<VideoDetails>(entity, frame);

    active_buffer ^= 1;
}

GXRModeObj *Video::get_rmode()
{
    return rmode;
}

/* Return the framebuffer to draw into */
void *Video::get_framebuffer()
{
    return xfb[active_buffer ^ 1];
}

void Video::setup_console()
{
    console_xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(
            console_xfb,
            20,
            20,
            rmode->fbWidth,
            rmode->xfbHeight,
            rmode->fbWidth*VI_DISPLAY_PIX_SZ
    );
}

void Video::toggle_console()
{
    console = !console;
    VIDEO_Configure(rmode);
	if (!console) {
        VIDEO_SetNextFramebuffer(console_xfb);
    } else {
        VIDEO_SetNextFramebuffer(xfb[active_buffer]);
    }
    VIDEO_SetBlack(FALSE);
    if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}
