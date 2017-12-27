#include "DirectX.h"

DX::DX()
{
	/*static const char srv1[] = "srv1";
	static const char srv2[] = "srv2";
	static const char srv3[] = "srv3";
	this->gDeviceContext->SetPrivateData(this->nullSRV, sizeof(srv1) - 1, srv1);*/
}
DX::~DX()
{
	this->setDbg();

	this->Clean();

	if (dbg != nullptr)
	{
		dbg->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		dbg = nullptr;
	}
}
void DX::Clean()
{
	SAFE_RELEASE(this->gDevice);

	SAFE_RELEASE(this->gDeviceContext);
	SAFE_RELEASE(this->gSwapChain);
	SAFE_RELEASE(this->gBackBufferRTV);
	SAFE_RELEASE(this->gRasterizerState);
	SAFE_RELEASE(this->gDSV);
	SAFE_RELEASE(this->gDepthStencil);
	SAFE_RELEASE(this->dbg);

	SAFE_RELEASE(this->VPBuffer);
	SAFE_RELEASE(this->worldBuffer);
	SAFE_RELEASE(this->lightBuffer);

	SAFE_RELEASE(this->gVertexLayout);

	SAFE_RELEASE(this->gVertexShader);

	SAFE_RELEASE(this->gFragmentShader);

	SAFE_RELEASE(this->txSamplerState);

	SAFE_RELEASE(this->nullBuffer);
	SAFE_RELEASE(this->nullSRV);
	SAFE_RELEASE(this->nullSRV2);
	SAFE_RELEASE(this->nullSRV3);
	SAFE_RELEASE(this->nullShader);
}
void DX::setDbg()
{
	this->setDbgName(this->gDevice, "device");

	this->setDbgName(this->gDeviceContext, "Context");
	this->setDbgName(this->gSwapChain, "swap chain");
	this->setDbgName(this->gBackBufferRTV, "RTV");
	this->setDbgName(this->gDSV, "depth stencil view");
	this->setDbgName(this->gDepthStencil, "depth stencil");

	this->setDbgName(this->VPBuffer, "VPBuffer");
	this->setDbgName(this->worldBuffer, "wBuffer");
	this->setDbgName(this->lightBuffer, "lBuffer");

	this->setDbgName(this->gVertexLayout, "vertex layout");

	this->setDbgName(this->gVertexShader, "vertex shader");

	this->setDbgName(this->gFragmentShader, "frag shader");

	this->setDbgName(this->txSamplerState, "tx samp");

	this->setDbgName(this->nullBuffer, "null buff");
	this->setDbgName(this->nullSRV, "null SRV");
	this->setDbgName(this->nullSRV2, "null SRV2");
	this->setDbgName(this->nullSRV3, "null SRV3");
	this->setDbgName(this->nullShader, "null shader");


	for (int i = 0; i < this->link.textures.getNrOfTextures(); i++)
	{
		this->setDbgName(this->link.textures.texture.gTextureSRV[i], "textures" + i);
	}

	for (int i = 0; i < this->link.textures.getNrOfNormals(); i++)
	{
		this->setDbgName(this->link.textures.Normal.gNormalSRV[i], "Normals" + i);
	}

}
void DX::setDbgName(ID3D11DeviceChild* child, const std::string& name)
{
	if (child != nullptr)
	{
		child->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
	}
}
void DX::setDbgName(ID3D11Device* child, const std::string& name)
{
	if (child != nullptr)
	{
		child->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
	}
}
void DX::setDbgName(IDXGISwapChain* child, const std::string& name)
{
	if (child != nullptr)
	{
		child->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
	}
}

void DX::OfflineCreation(ID3D11Device* &gDevice, HMODULE hModule, HWND * wndHandle)
{
	this->CreateDirect3DContext(wndHandle);
	this->SetViewPort();
	this->link.baseCamera.createCamera();
	this->setCamMat();

	this->lightMat.lightPos = DirectX::XMFLOAT4(0.0f, 20.0f, -20.0f, 1.f);

	this->link.textures.texture.loadTextures(this->gDevice, this->gDeviceContext);

	this->CreateCBuffers();
	this->CreateShaders();
	this->CreateSamplerStates();
}
void DX::CreateDirect3DContext(HWND * wndHandle)
{
	CoInitialize(NULL);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = *wndHandle;
	scd.BufferDesc.Width = (UINT)WIDTH;
	scd.BufferDesc.Height = (UINT)HEIGHT;
	scd.SampleDesc.Count = (int)PIXELSAMPLE;
	scd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->gSwapChain,
		&this->gDevice,
		NULL,
		&this->gDeviceContext);

	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->CreateDepthBuffer();

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = gDevice->CreateDepthStencilView(this->gDepthStencil, nullptr, &this->gDSV);

		hr = gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackBufferRTV);

		SAFE_RELEASE(pBackBuffer);

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);

		hr = gDevice->QueryInterface(IID_PPV_ARGS(&dbg));
	}

	D3D11_RASTERIZER_DESC rasDesc;
	rasDesc.FillMode = D3D11_FILL_SOLID;
	rasDesc.CullMode = D3D11_CULL_NONE;
	rasDesc.FrontCounterClockwise = false;
	rasDesc.DepthBias = 0;
	rasDesc.DepthBiasClamp = 0;
	rasDesc.SlopeScaledDepthBias = 0;
	rasDesc.DepthClipEnable = true;
	rasDesc.ScissorEnable = false;
	rasDesc.MultisampleEnable = false;
	rasDesc.AntialiasedLineEnable = false;

	hr = this->gDevice->CreateRasterizerState(&rasDesc, &this->gRasterizerState);

	if (FAILED(hr))
	{
		return exit(-1);
	}
}
void DX::CreateDepthBuffer()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC dBufferDesc;
	dBufferDesc.Width = (UINT)WIDTH;
	dBufferDesc.Height = (UINT)HEIGHT;
	dBufferDesc.MipLevels = 1;
	dBufferDesc.ArraySize = 1;
	dBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dBufferDesc.SampleDesc.Count = PIXELSAMPLE;
	dBufferDesc.SampleDesc.Quality = 0;
	dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	dBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dBufferDesc.CPUAccessFlags = 0;
	dBufferDesc.MiscFlags = 0;

	hr = gDevice->CreateTexture2D(&dBufferDesc, NULL, &this->gDepthStencil);

	if (FAILED(hr))
	{
		return exit(-1);
	}
}
void DX::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->gDeviceContext->RSSetViewports(1, &vp);
}

void DX::CreateShaders()
{
	HRESULT hr;

	ID3DBlob* error = nullptr;

	//#############################################################
	//#					  Main Vertex Shader					  #
	//#############################################################

	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"MainVertexShader.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&pVS,
		&error
	);
	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), NULL, &this->gVertexShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->gVertexLayout);


	SAFE_RELEASE(pVS);
	SAFE_RELEASE(error);

	//#############################################################
	//#					  Main Fragment Shader					  #
	//#############################################################
	ID3DBlob* pFS = nullptr;
	D3DCompileFromFile(
		L"MainPixelShader.hlsl",
		nullptr,
		nullptr,
		"FS_main",
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&pFS,
		&error
	);

	hr = this->gDevice->CreatePixelShader(pFS->GetBufferPointer(), pFS->GetBufferSize(), nullptr, &this->gFragmentShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(pFS);
	SAFE_RELEASE(error);
}
void DX::CreateCBuffers()
{
	HRESULT hr = 0;
	D3D11_BUFFER_DESC mBufferDesc;
	D3D11_SUBRESOURCE_DATA mdata;


	//#############################################################
	//#						Camera buffer 						  #
	//#############################################################

	mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufferDesc.ByteWidth = sizeof(camMatrices);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufferDesc.MiscFlags = 0;
	mBufferDesc.StructureByteStride = 0;

	mdata.pSysMem = &this->camMat;

	hr = gDevice->CreateBuffer(&mBufferDesc, &mdata, &this->VPBuffer);

	if (FAILED(hr))
	{
		exit(-1);
	}

	//#############################################################
	//#						Object buffer 						  #
	//#############################################################

	mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufferDesc.ByteWidth = sizeof(objMatrices);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufferDesc.MiscFlags = 0;
	mBufferDesc.StructureByteStride = 0;

	mdata.pSysMem = &this->worldMat;

	hr = gDevice->CreateBuffer(&mBufferDesc, &mdata, &this->worldBuffer);

	if (FAILED(hr))
	{
		exit(-1);
	}

	//#############################################################
	//#						Light buffer						  #
	//#############################################################

	mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufferDesc.ByteWidth = sizeof(lightMatrices);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufferDesc.MiscFlags = 0;
	mBufferDesc.StructureByteStride = 0;

	mdata.pSysMem = &this->lightMat;

	hr = gDevice->CreateBuffer(&mBufferDesc, &mdata, &this->lightBuffer);

	if (FAILED(hr))
	{
		exit(-1);
	}
}
void DX::CreateSamplerStates()
{
	HRESULT hr;

	//Diffuse Sampler State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = PIXELSAMPLE;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = 0;

	hr = this->gDevice->CreateSamplerState(&sampDesc, &this->txSamplerState);

	if (FAILED(hr))
	{
		exit(-1);
	}
}

void DX::Render()
{
	this->link.baseCamera.cameraSetUp(delta);

	this->pass2();
}
void DX::clearRender()
{
	float clearColor[] = { 0.17f, 0.84f, 0.9f, 1.0f };

	this->gDeviceContext->ClearRenderTargetView(this->gBackBufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(this->gDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void DX::Update()
{
	this->clearRender();
	this->Render();
	this->gSwapChain->Present(0, 0);
}

void DX::pass2()
{
	this->drawAll();
}

void DX::setCamMat()
{
	//this->camMat.projM = link.FX.ShMap.lightProj.projM;
	//this->camMat.viewM = link.FX.ShMap.lightProj.viewM;
}
void DX::updateBuffers()
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;

	this->gDeviceContext->Map(this->VPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	this->camMat.viewM = this->link.baseCamera.viewM;
	this->camMat.projM = this->link.baseCamera.projM;
	memcpy(dataPtr.pData, &this->camMat, sizeof(this->camMat));
	this->gDeviceContext->Unmap(this->VPBuffer, 0);

	ZeroMemory(&dataPtr, sizeof(dataPtr));
}

void DX::drawAll()
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	UINT32 vertexSize = 0;
	UINT32 offset = 0;

	this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);

	this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->worldBuffer);

	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->lightBuffer);

	this->gDeviceContext->PSSetShaderResources(0, 1, &this->link.textures.texture.gTextureSRV[0]);

	this->gDeviceContext->PSSetSamplers(0, 1, &this->txSamplerState);

	this->updateBuffers();
}