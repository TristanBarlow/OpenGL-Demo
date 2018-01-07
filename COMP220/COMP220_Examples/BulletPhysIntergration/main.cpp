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

	//create new camera
	camera = new Camera(aspectRatio);

	vector<const char *> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/blood_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/blood_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/blood_up.tga");
	skyboxFaces.push_back("Textures/Skybox/blood_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/blood_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/blood_ft.tga");

	//create new textureManager
	textureManager = new TextureManager;
	GLuint skyboxTex = textureManager->loadSkyboxTexture(skyboxFaces);

	//create new shadermanager
	shaderManager = new ShaderManager;

	//Create physics simulation
	physSim = new PhysicsSimulation;

	//create rigidbodies
	btRigidBody* ground = physSim->creatRigidBodyCube(btVector3(100, 1, 100), 0, btVector3(0, 0, 0));
	btRigidBody* celing = physSim->creatRigidBodyCube(btVector3(100, 1, 100), 0.0, btVector3(0, 56, 0));

	//Create grid
	Grid* grid =  new Grid(*camera);
	grid->createGridVec(101, 101, shaderManager->getShader("default"));

	// load sphere Mesh
	Mesh sphere;
	sphere.init("Meshes/only_quad_sphere.txt");

	//load and create the static mesh for the tank
	Mesh tank;
	tank.init("Meshes/Tank1.FBX");

	//load and create the static mesh drum mag
	Mesh drumMag;
	drumMag.init("Meshes/drumMag.obj");

	//load cube mesh
	Mesh cubeMesh;
	cubeMesh.init("Meshes/Cube.obj");

	// init light
	Light light(*camera);
	light.init(sphere, shaderManager->getShader("default"));
	light.w_Transform.setWorldLocation(vec3(-10.0, 30.0, 10.0));
	light.w_Transform.setWorldScale(vec3(1.0f, 1.0f, 1.0f));
	light.setIsLitt(false);

	vector <WorldObject*> worldObjects;

	SkyBox * skybox = new SkyBox(*camera);
	skybox->init(cubeMesh, shaderManager->getShader("skybox"), skyboxTex);

	// init sphere and set up attributes
	WorldObject* sphereObj = new WorldObject(*camera);
	sphereObj->init(sphere, shaderManager->getShader("light"));
	sphereObj->w_Transform.setWorldLocation(vec3(4.0f, 10.0f, 1.0f));
	sphereObj->getMaterial().setAmbientColour(0.5, 0.5, 0.5, 1.0);
	sphereObj->getMaterial().setSpecularColour(1.0, 1.0, 0.3);
	sphereObj->getMaterial().setDiffuseColour(1.0, 1.0, 0.8);
	sphereObj->getMaterial().setSpecularPower(40);
	worldObjects.push_back(sphereObj);

	for (int i = 0; i < 10; i++)
	{
		WorldObject* cube = new WorldObject(*camera);
		cube->init(cubeMesh, shaderManager->getShader("textureLight"), textureManager->getTexture("Textures/Crate.jpg"));
		cube->w_Transform.setWorldLocation(vec3(10.0, 10 +(i*5), 1.0f));
		cube->w_Transform.setWorldScale(vec3(3.0f, 3.0f, 3.0f));
		cube->addCompoundBody(*physSim);
		worldObjects.push_back(cube);
	}

	for (int i = 0; i < 1; i++)
	{
		WorldObject* newTank = new WorldObject(*camera);
		newTank->init(tank, shaderManager->getShader("textureLight"), textureManager->getTexture(("Textures/Tank1DF.png")));
		newTank->w_Transform.setWorldLocation(vec3(((rand()% 30)-20), 20, ((rand() % 30) -20)));
		newTank->w_Transform.setWorldScale(vec3(3.0, 3.0, 3.0));
		newTank->getMaterial().setSpecularPower(75);
		newTank->addCompoundBody(*physSim);
		worldObjects.push_back(newTank);
	}

	vector <RayCast*> rayCastVec;

	// PostProcessoring SHTUFF
	PostProcessor postProcGrey;
	postProcGrey.init("Shaders/PostProcVert.txt","Shaders/PostProcGreyScaleFrag.txt", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	PostProcessor postProcBlur;
	postProcBlur.init("Shaders/PostProcVert.txt", "Shaders/PostProcBlurFrag.txt", SCREEN_WIDTH, SCREEN_HEIGHT);

	PostProcessor postProcOutline;
	postProcOutline.init("Shaders/PostProcVert.txt", "Shaders/PostProcOutlineFrag.txt", SCREEN_WIDTH, SCREEN_HEIGHT);

	PostProcessBloom postProcessBloom;
	postProcessBloom.PostProcBloomInit("Shaders/PostProcVert.txt",10, SCREEN_WIDTH, SCREEN_HEIGHT);

	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;

	bool bloom = true;

	int yGrav = -10;

	//set up variables to handle mouse movement
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
				camera->rotate(ev.motion.xrel, ev.motion.yrel);
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(ev.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						RayCast* newRayCast = new RayCast(*physSim,*camera, camera->getWorldPos(), camera->forward, 500, shaderManager->getShader("default"), vec4(1.0, 0.5, 1.0, 0.8));
						rayCastVec.push_back(newRayCast);
						break;
					}
					case SDL_BUTTON_RIGHT:
						for (int i = 0; i < worldObjects.size(); i++)
						{
							btVector3 temp;
							if (worldObjects[i]->getRigidBody() != NULL) { temp = worldObjects[i]->getRigidBody()->getWorldTransform().getOrigin(); }
								
							RayCast* newRayCast = new RayCast(*physSim, *camera, vec3(temp.x(), temp.y(), temp.z()), vec3(0.0,1.0,0.0), 500, shaderManager->getShader("default"), vec4(0.0, 0.2, 1.0, 1.0));
							rayCastVec.push_back(newRayCast);
						}
						break;
				}
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
					camera->move(0.5f);
					break;
				case SDLK_s:
					camera->move(-0.5f);
					break;
				case SDLK_d:
					camera->strafe(-0.5f);
					break;
				case SDLK_a:
					camera->strafe(0.5f);
					break;
				case SDLK_q:
					camera->lift(-0.5);
					break;
				case SDLK_e:
					camera->lift(0.5);
						break;
				case SDLK_b:
					if (bloom)bloom = false;
					else bloom = true;
					break;
				case SDLK_0:
					yGrav *= -1;
					physSim->dynamicsWorld->clearForces();
					physSim->dynamicsWorld->setGravity(btVector3(0.0, yGrav, 0.0));
					break;
				case SDLK_9:
					physSim->dynamicsWorld->clearForces();
					for (int i = 0; i < worldObjects.size(); i++)
					{
						btVector3 force(0.0f, 10000.0f, 0.0f); 
						btVector3 direction(0.0f, 1,0);
						if (worldObjects[i]->getRigidBody() != NULL)
						{
							worldObjects[i]->getRigidBody()->applyForce(force, direction);
						}
						}
					break;
				}
				break;
			}
		}
		physSim->dynamicsWorld->stepSimulation(1 / 60.0f);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		//bind post processor buffer
		if (bloom) postProcessBloom.bind1stBuff();

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the mesh into the stencil buffer.
		light.draw();
		light.moveCircle();

		//Range bassed for loop
		//for (auto obj : worldObjects)
		//{
		//	obj->draw(light.w_Transform.getWorldLocation());
		//}
		// raw world objects
		for (int i = 0; i < worldObjects.size(); i++)
		{
			worldObjects[i]->draw(light.w_Transform.getWorldLocation());
		}

		// draw any raycasts
		if (rayCastVec.size() > 0)
		{
			for (int i = 0; i < rayCastVec.size(); i++)
			{
				rayCastVec[i]->draw();
			}
		}

		grid->draw();

		skybox->render();

		// post processor draw
		if (bloom)postProcessBloom.applyBloom();

		SDL_GL_SwapWindow(window);
		
	}
	if (grid) {
		delete grid;
		grid = nullptr;
	}
	delete textureManager;
	delete camera;
	delete physSim;9
	destroyWorldObjects(worldObjects);
	destroyRaycast(rayCastVec);
	delete skybox;
	delete shaderManager;
	Close();
	return 0;
}

