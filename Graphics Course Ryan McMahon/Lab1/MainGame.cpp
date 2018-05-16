#include "MainGame.h"
#include "TheCamera.h"
#include <iostream>
#include <string>


Transform transMesh1;
Transform transMesh2;
Transform transMesh3;
Transform transMesh4;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Mesh* mesh4();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Overlay* overlay(); //load texture
	MyShader* MyShaderPhong();
	MyShader* MyShaderPass();
	MyShader* MyShaderBlur();
	MyShader* MyShaderToon();
	MyShader* MyShaderRim();
	MyShader* MyShaderToonRim();
	MyShader* MyShaderLighting();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	//initialise the display
	_gameDisplay.initDisplay(); 

	//load textures
	texture.init("..\\res\\bricks.jpg");
	texture1.init("..\\res\\water.jpg");

	//load shaders in from file
	MyShaderPhong.init("..\\res\\shaderPhong.vert","..\\res\\shaderPhong.frag");
	MyShaderBlinnPhong.init("..\\res\\shaderBlinnPhong.vert", "..\\res\\shaderBlinnPhong.frag");
	MyShaderPass.init("..\\res\\shaderRim.vert","..\\res\\shaderRim.frag");
	MyShaderBlur.init("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	MyShaderToon.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	MyShaderRim.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	MyShaderToonRim.init("..\\res\\shaderToonRim.vert", "..\\res\\shaderToonRim.frag");
	MyShaderFog.init("..\\res\\shaderFog.vert", "..\\res\\shaderFog.frag");

	overlay.init("..\\res\\bricks.jpg");

	//load models in from file
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\f.obj");
	mesh3.loadModel("..\\res\\spraycan.obj");
	mesh4.loadModel("..\\res\\spraycan.obj");
	
	myCamera.initTheCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
	counter1 = 0.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(-100.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	modelView = transMesh1.GetModel() * myCamera.GetView();
	
	MyShaderPass.setMat4("ModelViewMatrix", modelView);
	MyShaderPass.setMat4("ProjectionMatrix", myCamera.GetProjection()); 
	
	glm::mat4 normalMatrix = transpose(inverse(modelView));
	
	MyShaderPass.setMat4("NormalMatrix", normalMatrix);

	MyShaderPass.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	MyShaderPass.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	MyShaderPass.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	MyShaderPass.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	MyShaderPass.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	MyShaderPass.setFloat("Shininess", 0.5);
}

void MainGame::setToonLighting()
{
	MyShaderToon.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
}

void MainGame::setRimShader()
{
	MyShaderRim.setMat4("u_vm", myCamera.GetView());
	MyShaderRim.setMat4("u_pm", myCamera.GetProjection());
}

void MainGame::setToonRimLighting()
{
	MyShaderToonRim.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	MyShaderToonRim.setMat4("u_vm", myCamera.GetView());
	MyShaderToonRim.setMat4("u_pm", myCamera.GetProjection());
	MyShaderToonRim.setMat4("v_pos", transMesh1.GetModel());
}

void MainGame::setLighting(float z)
{
	MyShaderFog.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5)); 
	
	MyShaderFog.setMat4("u_vm", myCamera.GetView());
	MyShaderFog.setMat4("u_pm", myCamera.GetProjection());

	MyShaderFog.setVec3("fogColor", glm::vec3(0.2, 0.2, 0.2));
	MyShaderFog.setFloat("minDist", -5.0f);
	MyShaderFog.setFloat("maxDist", 5.0f);

	MyShaderFog.setFloat("zPos", z);
}

void MainGame::setPhongLighting() {
	MyShaderPhong.setMat4("view", myCamera.GetView());
	MyShaderPhong.setMat4("projection", myCamera.GetProjection());
	MyShaderPhong.setMat4("model", transMesh3.GetModel());

	MyShaderPhong.setVec3("lightPos", glm::vec3(0.5f, 1.5f, 0.5f));
	MyShaderPhong.setVec3("viewPos", myCamera.getPos());
	MyShaderPhong.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	MyShaderPhong.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
}

void MainGame::setBlinnLighting() {
	MyShaderBlinnPhong.setMat4("projection", myCamera.GetProjection());
	MyShaderBlinnPhong.setMat4("view", myCamera.GetView());
	MyShaderBlinnPhong.setMat4("model", transMesh4.GetModel());

	MyShaderBlinnPhong.setVec3("viewPos", myCamera.getPos());
	MyShaderBlinnPhong.setVec3("lightPos", glm::vec3(1.5f, 1.5f, 0.5f));
}

void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(MyShaderBlur.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(MyShaderBlur.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(MyShaderBlur.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(MyShaderBlur.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::drawGame()
{
	//assigns texture
	texture.Bind(0);	
	
	counter1 = counter1 + 0.1f;

	//clears the display to allow the next frame to be drawn
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//sets the transforms for the first model
	transMesh1.SetPos(glm::vec3(sinf(counter) - 1, 1.5f, 0.0));
	transMesh1.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transMesh1.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	//binds the RimToon shader to the first model and updates
	MyShaderToonRim.Bind();
	setToonRimLighting();
	MyShaderToonRim.Update(transMesh1, myCamera);

	//renders the first model onto the display
	mesh1.draw();
	mesh1.updateSphereData(*transMesh1.GetPos(), 0.62f);
	

	transMesh2.SetPos(glm::vec3(-sinf(counter), -2.0, -sinf(counter)*5));
	transMesh2.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transMesh2.SetScale(glm::vec3(0.25, 0.25, 0.25));
	
	MyShaderFog.Bind();
	setLighting(mesh2.getSpherePos().z + 4);
	MyShaderFog.Update(transMesh2, myCamera);

	mesh2.draw();
	mesh2.updateSphereData(*transMesh2.GetPos(), 0.62f);

	transMesh3.SetPos(glm::vec3(0.0, -sinf(counter) + 1, 0.0));
	transMesh3.SetRot(glm::vec3(0.0, 0.0, -sinf(counter)));
	transMesh3.SetScale(glm::vec3(0.1, 0.1, 0.1));
	
	MyShaderPhong.Bind();
	setPhongLighting();
	MyShaderPhong.Update(transMesh3, myCamera);
	
	mesh3.draw();
	mesh3.updateSphereData(*transMesh3.GetPos(), 0.62f);

	transMesh4.SetPos(glm::vec3(1.0f, -sinf(counter) + 1, 0.0));
	transMesh4.SetRot(glm::vec3(0.0, 0.0, -sinf(counter)));
	transMesh4.SetScale(glm::vec3(0.1, 0.1, 0.1));

	MyShaderBlinnPhong.Bind();
	setBlinnLighting();
	MyShaderBlinnPhong.Update(transMesh4, myCamera);

	mesh4.draw();
	mesh4.updateSphereData(*transMesh4.GetPos(), 0.62f);

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 