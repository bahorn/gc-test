#!/usr/bin/env python3
"""
A terrible script to convert obj's to meshses
"""
import sys


class MeshStruct:
    def __init__(self, filename):
        vertices = []
        textures = []
        normals = []

        faces_vertices = []
        faces_textures = []
        faces_normals = []

        f = open(filename)

        for line in f.read().split('\n'):
            split = line.split(' ')
            if len(split) == 4 and split[0] == 'v':
                vertices.append(list(map(float, split[1:])))
            if len(split) == 4 and split[0] == 'f':
                for i in split[1:]:
                    a = i.split('/')
                    faces_vertices.append(int(a[0]))
                    faces_textures.append(int(a[1]))
                    faces_normals.append(int(a[2]))

            if len(split) == 3 and split[0] == 'vt':
                textures.append(list(map(float, split[1:])))

            if len(split) == 4 and split[0] == 'vn':
                normals.append(list(map(float, split[1:])))

        self.vertices = vertices
        self.textures = textures
        self.normals = normals
        self.faces_vertices = faces_vertices
        self.faces_textures = faces_textures
        self.faces_normals = faces_normals

    def mesh_size(self):
        return [f"int mesh_size = {len(self.faces_vertices)};"]

    def vertices_list(self):
        res = []
        res.append("float vertices[] ATTRIBUTE_ALIGN(32) = {")
        for vertex in self.vertices:
            res.append("{}, {}, {},".format(vertex[0], vertex[1], vertex[2]))
        res.append("};")
        return res

    def normals_list(self):
        res = []
        res.append("struct normal normals[] ATTRIBUTE_ALIGN(32)= {")
        for normal in self.normals:
            res.append(
                '{' + "{}, {}, {}".format(normal[0], normal[1], normal[2]) + '},'
            )
        res.append("};")
        return res

    def text_map_list(self):
        res = []
        res.append("struct texture_map texture[] ATTRIBUTE_ALIGN(32)= {")
        for texture in self.textures:
            res.append('{' + '{}, {}'.format(texture[0], texture[1]) + '},')
        res.append("};")
        return res;

    def faces_vertices_list(self):
        res = []
        res.append("int faces_vertices[] ATTRIBUTE_ALIGN(32) = {")
        for face in self.faces_vertices:
            res.append("{},".format(face - 1))
        res.append("};")
        return res

    def faces_textures_list(self):
        res = []
        res.append("int faces_textures[] ATTRIBUTE_ALIGN(32) = {")
        for face in self.faces_textures:
            res.append("{},".format(face - 1))
        res.append("};")
        return res

    def faces_normals_list(self):
        res = []
        res.append("int faces_normals[] ATTRIBUTE_ALIGN(32) = {")
        for face in self.faces_normals:
            res.append("{},".format(face - 1))
        res.append("};")
        return res

    def generate(self):
        return "\n".join(
            self.mesh_size() +
            self.vertices_list() +
            self.normals_list() +
            self.text_map_list() +
            self.faces_vertices_list() +
            self.faces_textures_list() +
            self.faces_normals_list()
        )


def main(filenames):
    for filename in filenames:
        fn = MeshStruct(filename)
        print(fn.generate())



if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1:])
    else:
        print('no model')
        sys.exit(-1)
