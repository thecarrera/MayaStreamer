#include "TextureHandler.h"

Normals::Normals()
{
	this->gNormalSRV = new ID3D11ShaderResourceView*[this->nrOfNormals];

	for (int i = 0; i < this->nrOfNormals; i++)
	{
		this->gNormalSRV[i] = nullptr;
	}
}
Normals::~Normals()
{
	for (int i = 0; i < this->nrOfNormals; i++)
	{
		SAFE_RELEASE(this->gNormalSRV[i]);
	}SAFE_DELETE(this->gNormalSRV);
}

void Normals::loadNormals(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext)
{
	HRESULT hr;

	wchar_t fileName[256];
	char temp1[256] = ".\\Assets\\Obj\\Textures\\Cube\\pCube1 [Normal].jpg";

	for (int i = 0; i < sizeof(temp1); i++)
	{
		fileName[i] = temp1[i];
	}

	//hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &this->gNormalSRV[0], NULL);

	/*if (FAILED(hr))
	{
	return exit(-1);
	}*/
}