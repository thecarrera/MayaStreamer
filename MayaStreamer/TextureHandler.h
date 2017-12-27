#pragma once

#include "Includes.h"

class Texture
{
public:
	//Diffuse
	ID3D11ShaderResourceView**	gTextureSRV = nullptr;

	int nrOfTextures = 1;

public:
	Texture();
	~Texture();

	void loadTextures(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext);
};

class Normals
{
public:
	//Normal maps
	ID3D11ShaderResourceView**	gNormalSRV = nullptr;

	int nrOfNormals = 1;

public:
	Normals();
	~Normals();

	void loadNormals(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext);

};

class TextureHandler
{
public:
	Texture texture;
	Normals Normal;

	int getNrOfTextures();
	int getNrOfNormals();
};
