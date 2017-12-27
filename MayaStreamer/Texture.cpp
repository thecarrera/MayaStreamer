#include "TextureHandler.h"

Texture::Texture()
{
	this->gTextureSRV = new ID3D11ShaderResourceView*[this->nrOfTextures];

	for (int i = 0; i < this->nrOfTextures; i++)
	{
		this->gTextureSRV[i] = nullptr;
	}
}

Texture::~Texture()
{
	for (int i = 0; i < this->nrOfTextures; i++)
	{
		SAFE_RELEASE(this->gTextureSRV[i]);
	}SAFE_DELETE(this->gTextureSRV);
}

void Texture::loadTextures(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext)
{
	//std::string test = "test";

	//wchar_t test2[256];

	//for (int i = 0; i < test.length(); i++)
	//{
	//	test2[i] = test[i];
	//}

	//HRESULT hr;

	//wchar_t fileName[256];
	//char temp1[256] = ".\\Assets\\Obj\\Textures\\Ogre\\Albedo\\FullBodypolySurface.jpg";


	//for (int i = 0; i < sizeof(temp1); i++)
	//{
	//	fileName[i] = temp1[i];
	//}

	//hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &this->gTextureSRV[0], NULL);

	//if (FAILED(hr))
	//{
	//	return exit(-1);
	//}
}