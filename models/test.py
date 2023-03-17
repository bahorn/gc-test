import sys
vertices = []
textures = []
normals = []

faces_vertices = []
faces_textures = []
faces_normals = []

f = open(sys.argv[1])

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

print(f"int mesh_size = {len(faces_vertices)};")

print("float vertices[] ATTRIBUTE_ALIGN(32) = {")
for vertex in vertices:
    print("{}, {}, {},".format(vertex[0], vertex[1], vertex[2]))
print("};")

print("struct normal normals[] ATTRIBUTE_ALIGN(32)= {")
for normal in normals:
    print('{' + "{}, {}, {}".format(normal[0], normal[1], normal[2]) + '},')
print("};")

print("struct texture_map texture[] ATTRIBUTE_ALIGN(32)= {")
for texture in textures:
    print('{' + '{}, {}'.format(texture[0], texture[1]) + '},')
print("};")

print("int faces_vertices[] ATTRIBUTE_ALIGN(32) = {")
for face in faces_vertices:
    print("{},".format(face - 1))
print("};")

print("int faces_textures[] ATTRIBUTE_ALIGN(32) = {")
for face in faces_textures:
    print("{},".format(face - 1))
print("};")

print("int faces_normals[] ATTRIBUTE_ALIGN(32) = {")
for face in faces_normals:
    print("{},".format(face - 1))
print("};")
