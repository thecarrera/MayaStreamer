#pragma once
#include "Includes.h"

class Camera
{
public:
	DirectX::XMMATRIX viewM;
	DirectX::XMMATRIX projM;

	bool ioBlur = 0;

private:
	time_t tButtonPress;
	time_t lButtonPress;

private:
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 upVec;
	DirectX::XMFLOAT3 lookAt;
	DirectX::XMFLOAT3 rightVec = { 1, 0 ,0 };

	int speed = 10;

public:
	Camera();
	~Camera();

	void createCamera();
	void createCamera(DirectX::XMVECTOR cameraPos,
		DirectX::XMVECTOR lookAt,
		DirectX::XMVECTOR upVec,
		float FOV,
		float ARO,
		float nPlane,
		float fPlane);

	void cameraSetUp(float delta);
	void moveCamera(float delta);
	void rotateCamera();
	void updateCamera();
};
