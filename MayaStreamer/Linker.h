#pragma once
#include "Camera.h"
#include "TextureHandler.h"

class Linker
{
public:
	Linker();
	~Linker();
	void Clean();

	Camera baseCamera;

	TextureHandler textures;
};