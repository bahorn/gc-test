#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <ogcsys.h>
#include "scene.hpp"
#include "model.hpp"

#include <ogc/tpl.h>

#include "textures_tpl.h"
#include "textures.h"


GXTexObj texObj;
TPLFile spriteTPL;


Scene::Scene(Video *video_, entt::registry *registry_)
{
    registry = registry_;
    float yscale;
    uint32_t xfbHeight;
    video = video_;
    /* setup fifo + flipper */
    gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
    memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

    /* Clear */
    GXColor background = {0, 0, 0, 0xff};
    GX_SetCopyClear(background, 0x00ffffff);

    auto rmode = video->get_rmode();

    GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight,0,1);
    yscale = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
    GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GX_SetFieldMode(
            rmode->field_rendering,
            (rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE
    );

    if (rmode->aa) {
        GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GX_SetCullMode(GX_CULL_FRONT);
    GX_CopyDisp(video->get_framebuffer(), GX_TRUE);
    GX_SetDispCopyGamma(GX_GM_1_0);

    up = {0.0F, 1.0F, 0.0F};
    
    camera = {0.0F, 0.0F, 5.0F};
    look = {0.0F, 0.0F, -1.0F};

    guPerspective(projection, 60, 1.33F, 1.0F, 300.0F);
    GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);

    GX_ClearVtxDesc();

    GX_SetNumChans(1);
    GX_SetNumTexGens(1);

    /* Position */
    GX_SetVtxDesc(GX_VA_POS, GX_INDEX16);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
 
    /* Normals */
    GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);

    /* Textures */
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    /* Load textures */
    TPL_OpenTPLFromMemory(&spriteTPL, (void *)textures_tpl,textures_tpl_size);
	TPL_GetTexture(&spriteTPL,modeltex,&texObj);
}


void Scene::set_camera(guVector camera_pos, guVector lookat_pos)
{
    camera = camera_pos;
    look = lookat_pos;
}


void Scene::set_background(GXColor background)
{
    GX_SetCopyClear(background, 0x00ffffff);
}


void Scene::draw()
{
    Mtx modelView;
    auto rmode = video->get_rmode();
    guLookAt(view, &camera, &up, &look);
    GX_SetViewport(0,0, rmode->fbWidth, rmode->efbHeight, 0, 1);

    /* Go through everything it was set to do and draw */
    GX_InvVtxCache();
    GX_InvalidateTexAll();

    GX_SetNumTexGens(1);

    /* Setup texture mapping */
    GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    /* Color Channels */
	GX_SetNumChans(1);

    GXLightObj lobj;
    GX_InitLightSpot(&lobj, 10.0, GX_SP_COS);
    GX_InitLightDir(&lobj, -1.0, -1.0, -1.0);
    GX_InitLightPos(&lobj, -10.0, 0.0, 0.0);
    GX_InitLightColor(&lobj, {0xff, 0x00, 0x00, 0xff});
    GX_LoadLightObj(&lobj, GX_LIGHT0);

    GXLightObj lobj2;
    GX_InitLightSpot(&lobj2, 10.0, GX_SP_COS);
    GX_InitLightDir(&lobj2, -1.0, -1.0, -1.0);
    GX_InitLightPos(&lobj2, -10.0, 1.0, 0.0);
    GX_InitLightColor(&lobj2, {0x00, 0x7f, 0x00, 0xff});
    GX_LoadLightObj(&lobj2, GX_LIGHT1);

    /* Ambient and Material Colors  */
    //GX_SetChanAmbColor(GX_COLOR0A0, {0x00, 0x00, 0x00, 0x00});
    //GX_SetChanMatColor(GX_COLOR0A0, {0x00, 0x00, 0x00, 0x00});

    //GX_SetChanAmbColor(GX_COLOR1A1, {0x00, 0x00, 0x00, 0xff});
    //GX_SetChanMatColor(GX_COLOR1A1, {0xff, 0xff, 0xff, 0xff});

    /* Enable the lighting sources */
    GX_SetChanCtrl(
            GX_COLOR0A0,
            GX_ENABLE,
            GX_SRC_REG,
            GX_SRC_REG,
            GX_LIGHT0 + GX_LIGHT1,
            GX_DF_CLAMP,
            GX_AF_NONE
    );

 
    auto to_draw = registry->view<modeldraw>();
    for (auto entity: to_draw) {
        auto &model = to_draw.get<modeldraw>(entity);

        if (!model.active) {
            continue;
        }

        /* Load the objects texture */
		GX_LoadTexObj(&texObj, GX_TEXMAP0);

        /* Transform the models position in relation to its view */
        guMtxConcat(view, model.modelView, modelView);
        GX_LoadPosMtxImm(modelView, GX_PNMTX0);
        GX_SetCurrentMtx(GX_PNMTX0);

        /* Set the vertex array to draw with */
        GX_SetArray(GX_VA_POS, model.vertices, 3 * sizeof(float));
        GX_SetArray(GX_VA_NRM, model.normals, 3 * sizeof(float));

        GX_Begin(GX_TRIANGLES, GX_VTXFMT0, model.face_count);
        for (int i = 0; i < model.face_count; i++) {
            GX_Position1x16(model.faces.vertex_idx[i]);
            GX_Normal1x16(model.faces.normal_idx[i]);
            struct texture_map points = \
                    model.texture_map[model.faces.texture_idx[i]];
            GX_TexCoord2f32(points.s, 1.0 - points.t);
        }

        GX_End();
    }

    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ZERO, GX_LO_CLEAR);
	GX_SetColorUpdate(GX_TRUE);
	GX_SetAlphaUpdate(GX_TRUE);
    GX_CopyDisp(video->get_framebuffer(), GX_TRUE);

    GX_DrawDone();
}
