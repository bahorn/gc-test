#include "model.hpp"

/* Generate a custom entity */
Model::Model(entt::registry *registry_) {
    registry = registry_;
    entity = registry->create();
    guMtxIdentity(state.modelView);
    registry->emplace<modeldraw>(entity, state);
}


/* Use a pregenerated entity, so it can be used as part of a larger system */
Model::Model(entt::registry *registry_, entt::entity entity_) {
    registry = registry_;
    entity = entity_;
    registry->emplace<modeldraw>(entity, state);
}


Model::~Model() {
    registry->erase<modeldraw>(entity);
}


void Model::set_vertices(float *vertices_)
{
    state.vertices = vertices_;
}

void Model::set_normals(struct normal *normals)
{
    state.normals = normals;
}

void Model::set_texture(struct texture_map *texture_map_)
{
    state.texture_map = texture_map_;
}

void Model::set_faces(struct faces *faces_, int face_count_)
{
    state.faces = *faces_;
    state.face_count = face_count_;
}


void Model::translate(float x, float y, float z)
{
    guMtxTransApply(state.modelView, state.modelView, x, y, z);
}

void Model::active(bool is_active)
{
    state.active = is_active;
}

void Model::update()
{
    registry->replace<modeldraw>(entity, state);
};

