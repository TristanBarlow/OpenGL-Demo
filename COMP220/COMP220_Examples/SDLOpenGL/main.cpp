//main.cpp - defines the entry point of the application

#include "main.h"

bool Init()
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_Init failed", NULL);
		return false;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL_CreateWindow failed", NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return false;
	}

	//request OpenGL core 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "context failed", NULL);
		SDL_DestroyWindow(window);
		SDL_QUIT;
		return false;
	}
	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, (char*)glewGetErrorString(err), "GLEW INIT failed", NULL);
		return false;
	}
	SDL_SetRelativeMouseMode(SDL_bool(SDL_ENABLE));
	return true;
}

void Close() 
{
	//DeleteContext
	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();
}




int main(int argc, char* args[])
{

	//initialise SDL and OPENGL
	if (Init() != true)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "INIT FAILED CLOSING PROGRAM", NULL);
		Close();
	}

	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is close
	bool running = true;

	GLuint VertexArray;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);



	//initialse vertices vector that will take the vertices of the obj file

	Grid grid;
	grid.createGridVec(101,101);


	GLuint textureID = loadTexture("Tank1DF.png");


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TexVert.txt", "TexFrag.txt");
	
	Mesh tank;
	tank.init("Tank1.FBX", programID);

	//create MVP location Struct
	MVP MVPLoc = { glGetUniformLocation(programID, "modelMatrix"),
				   glGetUniformLocation(programID, "viewMatrix"),
				   glGetUniformLocation(programID, "projectionMatrix")};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Transform MVPMatrix;

	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");

	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;

	//set up variables to handle mouse movement
	float mouseSens = 200.0;
	float TurnDegreesFromOriginX = 90.0f;
	float TurnDegreesFromOriginY = 0.0f;
	float itterator = 0;
	//main loop
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{

			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				TurnDegreesFromOriginX += ev.motion.xrel / mouseSens;
				TurnDegreesFromOriginY += -ev.motion.yrel / mouseSens;
				// Clamp Y
				if (TurnDegreesFromOriginY < -80.0f)	TurnDegreesFromOriginY = -80.0f;
				if (TurnDegreesFromOriginY > 80.0f)	TurnDegreesFromOriginY = 80.0f;

				// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
				camera.centre = camera.worldPos + camera.length * vec3(cos(TurnDegreesFromOriginX), tan(TurnDegreesFromOriginY), sin(TurnDegreesFromOriginX));
				break;

				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					camera.move(0.5f);
					break;
				case SDLK_s:
					camera.move(-0.5f);
					break;
				case SDLK_d:
					camera.strafe(-0.5f);
					break;
				case SDLK_a:
					camera.strafe(0.5f);
					break;
				case SDLK_q:
					camera.lift(-0.5);
					break;
				case SDLK_e:
					camera.lift(0.5);
						break;
				case SDLK_0:
					break;
				}
			}
		}



		//uppdate and draw game
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		glUseProgram(programID);
		
		MVPMatrix = calculateTransform(camera,aspectRatio, vec3(0.0,0.0,0.0), vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0));

		glUniformMatrix4fv(MVPLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
		glUniformMatrix4fv(MVPLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
		glUniformMatrix4fv(MVPLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));
		glUniform1i(textureLocation, 0);
		

		tank.render(camera, programID);

		grid.draw(camera, aspectRatio);

		SDL_GL_SwapWindow(window);
		
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &ebo);
	glDeleteTextures(1, &textureID);
	glDeleteVertexArrays(1, &VertexArray);
	Close();
	return 0;
}

