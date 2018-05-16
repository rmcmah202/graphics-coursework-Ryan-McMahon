#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "MyShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setADSLighting();
	void setToonLighting();
	void setRimShader();
	void setToonRimLighting();
	void setLighting(float z);
	void setPhongLighting();
	void setBlinnLighting();

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Texture texture;
	Texture texture1;
	Overlay overlay;
	MyShader MyShaderPhong;
	MyShader MyShaderBlinnPhong;
	MyShader MyShaderPass;
	MyShader MyShaderBlur;
	MyShader MyShaderToon;
	MyShader MyShaderRim;
	MyShader MyShaderToonRim;
	MyShader MyShaderFog;

	glm::mat4 modelView;

	TheCamera myCamera;
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
	float counter1;
};

