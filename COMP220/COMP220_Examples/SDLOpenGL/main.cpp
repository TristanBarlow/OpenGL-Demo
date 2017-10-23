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

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
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
	vector<int> blah;
	vector<vertex> foo;
	Mesh sphereMesh;
	loadOBJ("only_quad_sphere.txt", sphereMesh.vertArray, sphereMesh.elementBuff);
	Sphere sphere1;
	sphere1.begin(vec3(20.0f, 0.0f, 20.0f), vec3(20.0f, 20.0f, 0.0f), vec3(10.0f, 10.0f, 10.0f), sphereMesh);
	Sphere sphere2;
	sphere2.begin(vec3(50.0f, 0.0f, 50.0f), vec3(0.0f, 0.0f, 0.0f), vec3(10.0f, 10.0f, 10.0f), sphereMesh);

	//create grid
	vector <vertex> lineVerts;
	createGridVec(lineVerts);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("vertexshader.txt", "fragmentshader.txt");

	//create MVP location Struct
	MVP MVPLoc = { glGetUniformLocation(programID, "modelMatrix"),
				   glGetUniformLocation(programID, "viewMatrix"),
				   glGetUniformLocation(programID, "projectionMatrix")};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	
	GLuint lineBuff;
	glGenBuffers(1, &lineBuff);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Transform MVPMatrix;

	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;

	//set up variables to handle mouse movement
	float mouseSens = 500.0;
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
				if (TurnDegreesFromOriginY > 85.0f)	TurnDegreesFromOriginY = 85.0f;
				else if (TurnDegreesFromOriginY < -85.0f)	TurnDegreesFromOriginY = -85.0f;

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
					camera.centre = vec3(0.0, 0.0, 0.0);
				}
			}
		}



		//uppdate and draw game
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth test

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		/*
		glUseProgram(programID);
		 MVPMatrix = calculateTransform(&camera, vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,0.0f), vec3(10.0f,10.0f,10.0f));

		glUniformMatrix4fv(MVPLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
		glUniformMatrix4fv(MVPLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
		glUniformMatrix4fv(MVPLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertarray.size() * sizeof(vertex), &vertarray[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemtarry.size() * sizeof(int), &elemtarry[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), ((void*)offsetof(vertex, vertexCol)));
		
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertarray.size());
		glDrawElements(GL_TRIANGLES, elemtarry.size(), GL_UNSIGNED_INT, 0);
		*/
		/*
		MVPMatrix = calculateTransform(&camera, vec3(30.0f,30.0f,30.0f));

		glUniformMatrix4fv(MVPLoc.modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
		glUniformMatrix4fv(MVPLoc.viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
		glUniformMatrix4fv(MVPLoc.projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));

		glDrawElements(GL_TRIANGLES, elemtarry.size(), GL_UNSIGNED_INT, 0);
		*/
		glUseProgram(programID);
		sphere1.draw(vertexbuffer, ebo, programID);
		sphere2.draw(vertexbuffer, ebo, programID);
		drawGrid(MVPLoc, lineBuff, lineVerts);

		SDL_GL_SwapWindow(window);
		
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &lineBuff);
	glDeleteVertexArrays(1, &VertexArray);
	Close();
	return 0;
}

