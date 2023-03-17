#pragma once

#include "ogcsys.h"
#include "entt.hpp"

/* For each face, we want to map the vertex_idx, texture_idx and normal_idx */
struct faces {
    int *vertex_idx = NULL;
    int *texture_idx = NULL;
    int *normal_idx = NULL; 
};

struct normal {
    float x;
    float y;
    float z;
};

struct texture_map {
    float s;
    float t;
};

struct modeldraw {
    bool active = false;
    float *vertices = NULL;
    struct normal *normals = NULL;
    struct texture_map *texture_map = NULL;
    struct faces faces;
    int face_count = 0;
    Mtx modelView;
};


class Model {
    public:
        Model(entt::registry *registry);
        Model(entt::registry *registry, entt::entity entity);
        ~Model();

        void set_vertices(float *vertices);
        void set_normals(struct normal *normals);
        void set_texture(struct texture_map *texture_map);
        void set_faces(struct faces *faces, int face_count);
        void active(bool state);

        void translate(float x, float y, float z);

        void update();

    private:
        entt::registry *registry;
        entt::entity entity;


        struct modeldraw state;
};
