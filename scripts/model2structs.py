#!/usr/bin/env python3
"""
A terrible script to convert obj's to meshses
"""
import sys
import jinja2


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

    def generate(self):
        with open('./template.cpp') as file_:
            template = jinja2.Template(file_.read())
            return template.render(
                    vertices=self.vertices,
                    normals=self.normals,
                    textures=self.textures,
                    faces_vertices=self.faces_vertices,
                    faces_textures=self.faces_textures,
                    faces_normals=self.faces_normals,
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
