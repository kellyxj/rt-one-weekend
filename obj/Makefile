CXXFLAGS=-Wall -O2 -std=c++17
CXX=g++
LDLIBS=-lstdc++fs

objects = main.o vec4.o color.o ray.o pinholeCamera.o geometry.o mat4.o plane.o scene.o image.o rayTracer.o sphere.o groundGrid.o baseMaterial.o square.o mirror.o glass.o \
triangle.o mesh.o bvh.o scene_loader.o light.o rectangleLight.o spectralRenderer.o
VPATH = ../src:../src/geometry:../src/materials:../src/renderers:../bin:../src/util:../src/math:../src/lights:../src/cameras

edit: $(objects)
	$(CXX) -o ../bin/main $(objects) $(LDLIBS)

debug: $(objects)
	$(CXX) -o ../bin/main $(objects) -g $(LDLIBS)

main.o: main.cpp vec4.hpp color.hpp ray.hpp camera.hpp mat4.hpp rayTracer.hpp scene_loader.hpp spectralRenderer.hpp
light.o: light.cpp ray.hpp mat4.hpp hit.hpp color.hpp geometry.hpp
rectangleLight.o: rectangleLight.cpp light.hpp
scene.o: scene.cpp geometry.hpp plane.hpp camera.hpp sphere.hpp square.hpp mesh.hpp rectangleLight.hpp
ray.o: ray.cpp vec4.hpp color.hpp indexOfRefraction.hpp
color.o: color.cpp
camera.o: camera.cpp vec4.hpp ray.hpp
pinholeCamera.o: pinholeCamera.cpp camera.hpp
mat4.o: mat4.cpp vec4.hpp
geometry.o: geometry.cpp vec4.hpp mat4.hpp material.hpp hit.hpp baseMaterial.hpp glass.hpp mirror.hpp groundGrid.hpp
plane.o: plane.cpp geometry.hpp
rayTracer.o: rayTracer.cpp scene.hpp image.hpp geometry.hpp hit.hpp material.hpp sphere.hpp plane.hpp groundGrid.hpp
square.o: square.cpp geometry.hpp
mirror.o: mirror.cpp material.hpp
glass.o: glass.cpp material.hpp
triangle.o: triangle.cpp geometry.hpp
mesh.o: mesh.cpp triangle.hpp bvh.hpp
bvh.o: bvh.cpp geometry.hpp
scene_loader.o: scene_loader.cpp scene.hpp
spectralRenderer.o: spectralRenderer.cpp rayTracer.hpp

.PHONY: clean
clean: 
	rm -rf *.o
	rm ../bin/main