# rt-one-weekend
Peter Shirley book implementation in C++

My first ray tracer implementation, written in javascript can be found here https://github.com/kellyxj/ray-tracer. 
(Made for CS 351-2 Intermediate Graphics at Northwestern)

The goal of this project was to overcome some shortcomings of the original project namely:
1. Poor performance. The eventual goal of this project is to mirror the functionality of this C++ ray tracer in CUDA. Implementing support for BVH will allow
for more complex scenes to be rendered.
2. No scene editing from the frontend. This project will have a GUI with a click and drag scene editing interface.
3. Unrealistic camera. Improving the performance will allow for brute force defocus. Stretch goal is implementing realistic lenses.
4. Simplistic geometry. This project will include a parser for .obj files.
5. Simplistic materials. This project will implement distribution ray tracing for more realistic reflections/transparency
6. General code reorganization
