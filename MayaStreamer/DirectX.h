#pragma once
#include "Linker.h"



/*
#################################################################################################################################
#																																#
#							All Direct X components, mostly the core engine of the pipeline.									#
#							Allowing us to add key techniques as modules.														#
#																																#
#################################################################################################################################
*/

class DX
{
	//Core
public:
	ID3D11Device*				gDevice = nullptr;

private:
	//Core
	ID3D11DeviceContext*		gDeviceContext = nullptr;
	IDXGISwapChain*				gSwapChain = nullptr;
	ID3D11RenderTargetView*		gBackBufferRTV = nullptr;
	ID3D11RasterizerState*		gRasterizerState = nullptr;
	ID3D11DepthStencilView*		gDSV = nullptr;
	ID3D11Texture2D*			gDepthStencil = nullptr;
	ID3D11Debug*				dbg = nullptr;

	//Buffers
	ID3D11Buffer*				VPBuffer = nullptr;
	ID3D11Buffer*				worldBuffer = nullptr;
	ID3D11Buffer*				lightBuffer = nullptr;

	//Layouts
	ID3D11InputLayout*			gVertexLayout = nullptr;

	//Vertex Shaders
	ID3D11VertexShader*			gVertexShader = nullptr;

	//Pixel Shaders
	ID3D11PixelShader*			gFragmentShader = nullptr;

	//Sampler states
	ID3D11SamplerState*			txSamplerState = nullptr;

	//Cleaners
	ID3D11Buffer* nullBuffer = nullptr;
	ID3D11ShaderResourceView*	nullSRV = nullptr;
	ID3D11ShaderResourceView*	nullSRV2 = nullptr;
	ID3D11ShaderResourceView*	nullSRV3 = nullptr;
	ID3D11GeometryShader* nullShader = nullptr;

public:
	DX();
	~DX();
	void Clean();
	void setDbg();
	void setDbgName(ID3D11DeviceChild* child, const std::string& name);
	void setDbgName(ID3D11Device* child, const std::string& name);
	void setDbgName(IDXGISwapChain* child, const std::string& name);

	void OfflineCreation(ID3D11Device* &gDevice, HMODULE hModule, HWND* wndHandle);
	void CreateDirect3DContext(HWND* wndHandle);
	void CreateDepthBuffer();
	void SetViewPort();

	void CreateShaders();
	void CreateCBuffers();
	void CreateSamplerStates();

	void Render();
	void clearRender();
	void Update();

	void pass2();

	void setCamMat();
	void updateBuffers();

	void drawAll();

	void tempSyncFunc();

private:
	std::chrono::high_resolution_clock::time_point lFrameTime;
	float delta = 0;

	Linker link;

	camMatrices		camMat;
	objMatrices		worldMat;
	lightMatrices	lightMat;
};

