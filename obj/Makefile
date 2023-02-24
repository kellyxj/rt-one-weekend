CXXFLAGS=-Wall -O2
CXX=g++

objects = main.o vec4.o color.o ray.o camera.o geometry.o mat4.o plane.o scene.o image.o rayTracer.o sphere.o groundGrid.o baseMaterial.o square.o mirror.o glass.o \
triangle.o mesh.o bvh.o
VPATH = ../src:../src/geometry:../src/materials:../src/renderers:../bin:../src/util

edit: $(objects)
	$(CXX) -o ../bin/main $(objects)

main.o: main.cpp vec4.hpp color.hpp ray.hpp camera.hpp mat4.hpp rayTracer.hpp
scene.o: scene.cpp geometry.hpp plane.hpp camera.hpp sphere.hpp square.hpp mesh.hpp
ray.o: ray.cpp vec4.hpp
camera.o: camera.cpp vec4.hpp ray.hpp
mat4.o: mat4.cpp vec4.hpp
geometry.o: geometry.cpp vec4.hpp mat4.hpp material.hpp hit.hpp baseMaterial.hpp
plane.o: plane.cpp geometry.hpp
scene.o: scene.cpp camera.hpp geometry.hpp
rayTracer.o: rayTracer.cpp scene.hpp image.hpp hit.hpp material.hpp sphere.hpp plane.hpp groundGrid.hpp
square.o: square.cpp geometry.hpp
mirror.o: mirror.cpp material.hpp
glass.o: glass.cpp material.hpp
triangle.o: triangle.cpp geometry.hpp
mesh.o: mesh.cpp triangle.hpp bvh.hpp
bvh.o: bvh.cpp geometry.hpp

.PHONY: clean
clean: 
	rm -rf *.o
	rm -rf ../data/*.ppm
	rm ../bin/main