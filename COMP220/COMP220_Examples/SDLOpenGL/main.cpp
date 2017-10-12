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

Transform calculateTransform(Camera* camera) 
{

	vec3 trianglePosition = vec3(0.0f, 0.5f, 0.0f);
	mat4 translationMatrix = translate(trianglePosition);
	float oop = (sin(rand())) / 10;
	//create rotation matrix
	vec3 trianglRotation = vec3(0.0, 0.0f, 0.0f);
	mat4 rotationXMatrix = rotate(trianglRotation.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationYMatrix = rotate(trianglRotation.y, vec3(0.0, 1.0f, 0.0f));
	mat4 rotationZMatrix = rotate(trianglRotation.z, vec3(0.0, 0.0f, 1.0f));
	mat4 rotationMatix = rotationZMatrix*rotationYMatrix*rotationXMatrix;

	//create scaling matrix
	vec3 scaleVec = vec3(10.0f, 10.0f, 10.0f);
	mat4 ScalingMatrix = scale(scaleVec);


	mat4 modelMatrix = translationMatrix*rotationMatix*ScalingMatrix;

	mat4 cameraMatrix = lookAt(camera->worldPos, camera->centre, camera->up);

	float aspectRatio = (SCREEN_WIDTH / SCREEN_HEIGHT);

	mat4 projectionMatrix = perspective(radians(90.0f),aspectRatio, 0.1f, 500.0f);

	Transform finalTransform = {modelMatrix, cameraMatrix, projectionMatrix };
	return finalTransform;
}

int main(int argc, char* args[])
{

	//initialise SDL and OPENGL
	if (Init() != true)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "INIT FAILED CLOSING PROGRAM", NULL);
		Close();
	}

	bool running = true;


	GLuint VertexArray;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	//initialse vertices vector that will take the vertices of the obj file
	vector <vertex> vertarray;
	vector<int> elemtarry;
	loadOBJ("lifebot.obj", vertarray,elemtarry);
	if (vertarray.size() == 0) 
	{
		cout << "failed to Load file, CYA!" << endl;
		running = false;

	}
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("vertexshader.txt", "fragmentshader.txt");
	
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");


	vertex vertexData[3] =
	{
		{vec3(0.0,0.5,0.0), vec4(1.0f,0.0,0.0,1.0)},
		{vec3(-0.5,0.0,0.0), vec4(0.0f,1.0,0.0,1.0)},
		{vec3(0.5,0.0,0.0), vec4(0.0f,0.0,1.0,1.0)}

	};

	vertex square[]=
	{
		{vec3(-1.0f,-1.0f,-1.0f), vec4(1.0f,0.0,0.0,1.0)}, // triangle 1 : begin
		{vec3(-1.0f,-1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{vec3(-1.0f, 1.0f, 1.0f), vec4(0.0f,1.0,0.0,1.0) }, // triangle 1 : end
		{vec3(1.0f, 1.0f,-1.0f),vec4(0.0f,1.0,0.0,1.0) }, // triangle 2 : begin
		{vec3(-1.0f,-1.0f,-1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{vec3(-1.0f, 1.0f,-1.0f), vec4(1.0f,1.0,0.0,1.0) },  // triangle 2 : end
		{vec3(1.0f,-1.0f, 1.0f),vec4(1.0f,0.0,1.0,1.0) },
		{vec3(-1.0f,-1.0f,-1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{vec3(1.0f,-1.0f,-1.0f),vec4(1.0f,0.0,0.0,1.0) },
		{vec3(1.0f, 1.0f,-1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f,-1.0f,-1.0f),vec4(0.0f,1.0,0.0,1.0) },
		{ vec3(-1.0f,-1.0f,-1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(-1.0f,-1.0f,-1.0f),vec4(1.0f,1.0,0.0,1.0) },
		{ vec3(-1.0f, 1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(-1.0f, 1.0f,-1.0f),vec4(0.0f,1.0,0.0,1.0) },
		{ vec3(1.0f,-1.0f, 1.0f),vec4(1.0f,1.0,0.0,1.0) },
		{ vec3(-1.0f,-1.0f, 1.0f),vec4(1.0f,1.0,1.0,1.0) },
		{ vec3(-1.0f,-1.0f,-1.0f),vec4(1.0f,0.0,1.0,1.0) },
		{ vec3(-1.0f, 1.0f, 1.0f),vec4(1.0f,1.0,0.0,1.0) },
		{ vec3(-1.0f,-1.0f, 1.0f),vec4(0.0f,1.0,1.0,1.0) },
		{ vec3(1.0f,-1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f,-1.0f,-1.0f),vec4(1.0f,1.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f,-1.0f),vec4(0.0f,0.0,0.1,1.0) },
		{ vec3(1.0f,-1.0f,-1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f, 1.0f),vec4(0.0f,1.0,0.0,1.0) },
		{ vec3(1.0f,-1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f, 1.0f),vec4(0.0f,0.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f,-1.0f),vec4(1.0f,0.0,0.0,1.0) },
		{ vec3( -1.0f, 1.0f,-1.0f),vec4(1.0f,0.0,0.0,1.0) },
		{ vec3(1.0f, 1.0f, 1.0f),vec4(0.0f,1.0,0.0,1.0) },
		{ vec3( -1.0f, 1.0f,-1.0f),vec4(1.0f,1.0,0.0,1.0) },
		{ vec3( -1.0f, 1.0f, 1.0f),vec4(1.0f,0.0,0.1,1.0) },
		{ vec3(1.0f, 1.0f, 1.0f),vec4(1.0f,1.0,1.0,1.0) },
		{ vec3( -1.0f, 1.0f, 1.0f),vec4(1.0f,0.0,0.0,1.0) },
		{ vec3(1.0f,-1.0f, 1.0f),vec4(1.0f,1.0,0.0,1.0) }
	};

	vector <vertex> test=
	{

		{vec3 (- 45.1767, 7.8740, 68.1530), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 45.1458, 7.8740, 62.2478), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 56.5327, 7.8740, 59.1967), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 59.4587, 7.8740, 64.3262), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 65.1407, 7.8740, 50.6349), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 70.2553, 7.8740, 53.5878), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 68.2514, 7.8740, 38.8993), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 74.1567, 7.8740, 38.8685), vec4(1.0f,0.0,0.0,1.0) },
		{vec3 (- 65.2003, 7.8740, 27.5124), vec4(1.0f,0.0,0.0,1.0) }
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertarray.size()* sizeof(vertex), &vertarray[0], GL_STATIC_DRAW);

	
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemtarry.size()*sizeof(int),&elemtarry[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), ((void*)offsetof(vertex, vertexCol)));


	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed

	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	float mouseSens = 200.0;
	float TurnDegreesFromOriginX = 90.0f;
	float TurnDegreesFromOriginY = 0.0f;
	// 1rst attribute buffer : vertices

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
				if (TurnDegreesFromOriginY > 90.0f)	TurnDegreesFromOriginY = 90.0f;
				else if (TurnDegreesFromOriginY < -90.0f)	TurnDegreesFromOriginY = -90.0f;

				// Move camera lookatpoint to a trigonometry calculated position, CameraDistance far away, relative to the camera position
				camera.centre = camera.worldPos + camera.forward * vec3(cos(TurnDegreesFromOriginX), tan(TurnDegreesFromOriginY), sin(TurnDegreesFromOriginX));
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
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);
		Transform MVPMatrix = calculateTransform(&camera);

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(MVPMatrix.projectionMatrix));



		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, vertarray.size());
		glDrawElements(GL_TRIANGLES, elemtarry.size(), GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(window);
		
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &VertexArray);
	Close();
	return 0;
}