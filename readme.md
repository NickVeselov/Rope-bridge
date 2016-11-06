Link to the Youtube demo: https://www.youtube.com/watch?v=q4zvxuyA93Y

This is the physics demo application made for the first assignment on the course "Tools & Middleware" at the Goldsmiths University. The application is based on the octet framework (designed by Andy Thomasson), which is based on Bullet physics engine.

The idea of the application is a model of rope bridge, which is created as a set of blocks, connected using hinge constraints. The user controls the character model (ball), which can roll across the scene.

Most of the game parameters are read from the CSV file (test.csv in the project directory) such as:
-bridge size,
-bridge plank measurements,
-cliffs location,
-cliff measurements,
-ball properties,
-ground measurements.

The source code files, that were designed for this application are:
1. bridge.h
 Contains 'Bridge' class, which constructs the bridge and the cliffs.
2. collision handler.h
 Handler of the collisions between objects, based on comparing manifold pair between current frame and the previous and calling collision or separation event, depends on the difference between sets.
3. read scv file.h
 Reader of the SCV files.
4. rope bridge.h
 Contains inherited from the app class, which handles initialization of the game and simulate ('draw world') function, which is called every frame. 
5. helper_fps_controller.h
 This file is the core file of the framework, but there were made some changes, because of the limits of the original implementation (constants used instead of parameters).
 
Most of implementation ideas were taken from the book "Learning game physics with Bullet physics and OpenGL" published by Packt and Bullet online documentation. Also, coursemates Witek Gawlowski, Matthew Duddington, Robert Doig and Louis Bennette gived me few tips with undestanding the constraints concept and bullet syntax.

Unfortunanetly, spring constraints wasn't used in the application, because of the lack of reasonable use in this model.

The bridge has durability, measured in maximum impulse, which is set to constant value, which can be tested by increasing the mass of ball in the CSV file, and which is demonstrated in the video demo on youtube.

The collision handler is set to detect not only collisions, but also the situation, when objects are very close (the distance is lesser then 0.1), which allows to detect collision states more accurately, however using more resources.

