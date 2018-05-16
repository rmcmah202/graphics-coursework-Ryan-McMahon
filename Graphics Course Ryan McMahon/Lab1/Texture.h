#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();

	void Bind(unsigned int unit); // bind upto 32 textures
	void init(const std::string& fileName);
	~Texture();
	GLint getTexHandler() { return textureHandler; }

protected:
private:

	GLuint textureHandler;
};

