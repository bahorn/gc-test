# gamecube tests

Some attempts at getting 3d models and lighting working on the gamecube.

Building right now should show a textured suzzane with lighting working
correctly + allow moving the camera position with the gamepad.

Also had `entt` built in, as that was part of the system I was writing to
convert this closer to an actual engine.

## Build

Setup the build environment with something like:
```
git clone https://github.com/devkitPro/gamecube-examples
mkdir -p gamecube-examples/projects
cd gamecube-examples/projects/
git clone url/for/this/repo
cd ..
```

I'd modify the makefile at the root of the `gamecube-examples` directory to
something like:
```
MAKEFILES := $(shell find projects -mindepth 1 -name Makefile)
```

To avoid building all the other example projects.

The build with:
```
docker run -ti -v `pwd`:/build devkitpro/devkitppc:latest bash
```
