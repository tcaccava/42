𝘛𝘩𝘪𝘴 𝘱𝘳𝘰𝘫𝘦𝘤𝘵 𝘩𝘢𝘴 𝘣𝘦𝘦𝘯 𝘤𝘳𝘦𝘢𝘵𝘦𝘥 𝘢𝘴 𝘱𝘢𝘳𝘵 𝘰𝘧 𝘵𝘩𝘦 42 𝘤𝘶𝘳𝘳𝘪𝘤𝘶𝘭𝘶𝘮 𝘣𝘺 𝘦𝘤𝘢𝘳𝘶𝘴𝘰.

Description:

The project is a pretty basic 3D engine that loads an .obj model and a .ppm texture. Then with a dedicated rendering pipeline the engine processes all the data and sends the information to the GPU.

Due to the prohibition of using external libraries(except for window and event managements) i had to implement my own obj parser, texture loader, mathematical structures and a transformations system.

The engine supports 3D transformations, face coloring, texture mapping and a basic Lambert lighting that uses normals to create a realistic lighting.

The user can interact with the model in real time(keys listed below) by rotating, translating, scaling and by toggling the texture, all in an abstracted 3D space that is projected onto the 2D screen using mathematical transformations.

Instruction:

(OPTIONAL) The subject requires a one line command that install all dependencies but for me it's pretty invasive so first test without it
sudo apt update && sudo apt install -y build-essential libglfw3-dev libgl1-mesa-dev && make

Makefile have all basic commands for compiling like make, make all, make clean/fclean and make re. Added make deps to check dependencies.

The program has to be executed with the command line ./scop path_to_obj path_to_texture.ppm

Below are listed all of the possible commands.

W/S = translate on y
A/D = translate on x
Q/E = translate on z (scaling not implemented because it's visually the same as translation on z)

KEY_UP/KEY_DOWN = rotation on x
KEY_RIGHT/KEY_LEFT = rotation on y
R/F = rotation on z

T = toggle texture

SPACE = reset position

Resources

[Opengl complete tutoria](https://learnopengl.com/)

Stack, youtube and github were used for implementing little features like the ppm loader.

AI(ChatGpt) was used to give the project a professional structure from the start and to correct and clean some of the functions. Also it has generated some test like the test.ppm file