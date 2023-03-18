
int mesh_size = {{ faces_vertices|length }};


float vertices[] ATTRIBUTE_ALIGN(32) = {
    {% for a, b, c in vertices %}
    {{ a }}, {{ b }}, {{ c }},
    {% endfor %}
};

struct normal normals[] ATTRIBUTE_ALIGN(32) = {
    {% for a, b, c in normals %}
    { {{ a }}, {{ b }}, {{ c }} },
    {% endfor %}
};

struct texture_map texture[] ATTRIBUTE_ALIGN(32) = {
    {% for a, b in textures %}
    { {{ a }}, {{ b }} },
    {% endfor %}
};

int faces_vertices[] ATTRIBUTE_ALIGN(32) = {
    {% for face in faces_vertices %}
    {{ face - 1 }},
    {% endfor %}
};

int faces_textures[] ATTRIBUTE_ALIGN(32) = {
    {% for face in faces_textures %}
    {{ face - 1 }},
    {% endfor %}
};

int faces_normals[] ATTRIBUTE_ALIGN(32) = {
    {% for face in faces_normals %}
    {{ face - 1 }},
    {% endfor %}
};
