#include "Camera.h"

Camera::Camera()
{
	this->viewM = DirectX::XMMatrixIdentity();
	this->projM = DirectX::XMMatrixIdentity();

	this->tButtonPress = GetCurrentTime();
	this->lButtonPress = GetCurrentTime();

	SetCursorPos(WIDTH / 2, HEIGHT / 2);
}
Camera::~Camera()
{
}

void Camera::createCamera()
{
	DirectX::XMVECTOR cameraPos = { 0.0f, 0.0f, -45.0f };
	DirectX::XMVECTOR lookTo = { 0, 0, 1 };
	DirectX::XMVECTOR upVec = { 0, 1, 0 };

	DirectX::XMStoreFloat3(&this->camPos, cameraPos);
	DirectX::XMStoreFloat3(&this->lookAt, lookTo);
	DirectX::XMStoreFloat3(&this->upVec, upVec);

	float FOV = { 0.45f * DirectX::XM_PI };
	float ARO = (float)WIDTH / (float)HEIGHT;
	float nPlane = 0.1f;
	float fPlane = 200.0f;

	this->viewM = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(cameraPos, lookTo, upVec));
	this->projM = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(FOV, ARO, nPlane, fPlane));

}
void Camera::createCamera(DirectX::XMVECTOR cameraPos, DirectX::XMVECTOR lookAt, DirectX::XMVECTOR upVec, float FOV, float ARO, float nPlane, float fPlane)
{
	DirectX::XMStoreFloat3(&this->camPos, cameraPos);
	DirectX::XMStoreFloat3(&this->lookAt, lookAt);
	DirectX::XMStoreFloat3(&this->upVec, upVec);

	this->viewM = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(cameraPos, lookAt, upVec));
	this->projM = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(FOV, ARO, nPlane, fPlane));
}

void Camera::cameraSetUp(float delta)
{
	this->moveCamera(delta);
	this->rotateCamera();
	this->updateCamera();
}
void Camera::moveCamera(float delta)
{
	DirectX::XMVECTOR cPos = DirectX::XMLoadFloat3(&this->camPos);

	if (GetAsyncKeyState(0x57)) // w
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * this->speed);
		DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&this->lookAt);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, lookAt, cPos));
	}
	if (GetAsyncKeyState(0x53)) // s
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * -this->speed);
		DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&this->lookAt);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, lookAt, cPos));
	}
	if (GetAsyncKeyState(0x41)) // a
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * -this->speed);
		DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat3(&this->rightVec);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, rightVec, cPos));
	}
	if (GetAsyncKeyState(0x44)) // d
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * this->speed);
		DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat3(&this->rightVec);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, rightVec, cPos));
	}
	if (GetAsyncKeyState(0x51)) // q
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * -5);
		DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&this->upVec);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, upVec, cPos));
	}
	if (GetAsyncKeyState(0x45)) // e
	{
		DirectX::XMVECTOR speed = DirectX::XMVectorReplicate(delta * 5);
		DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&this->upVec);
		DirectX::XMStoreFloat3(&this->camPos, DirectX::XMVectorMultiplyAdd(speed, upVec, cPos));
	}
	//if (GetAsyncKeyState(0x46) & 0x8000) // f
	//{
	//	this->ioBlur = -this->ioBlur;
	//}
}
void Camera::rotateCamera()
{
	POINT mPos;
	GetCursorPos(&mPos);

	if (mPos.y > HEIGHT / 2 + 1)
	{
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&this->rightVec), 0.009f);

		DirectX::XMStoreFloat3(&this->upVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->upVec), rotMat));
		DirectX::XMStoreFloat3(&this->lookAt, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->lookAt), rotMat));
	}
	if (mPos.y < HEIGHT / 2 - 1)
	{
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&this->rightVec), -0.009f);

		DirectX::XMStoreFloat3(&this->upVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->upVec), rotMat));
		DirectX::XMStoreFloat3(&this->lookAt, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->lookAt), rotMat));

	}
	if (mPos.x > WIDTH / 2 + 1)
	{
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationY(0.009f);

		DirectX::XMStoreFloat3(&this->rightVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->rightVec), rotMat));
		DirectX::XMStoreFloat3(&this->upVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->upVec), rotMat));
		DirectX::XMStoreFloat3(&this->lookAt, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->lookAt), rotMat));
	}
	if (mPos.x < WIDTH / 2 - 1)
	{
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationY(-0.009f);

		DirectX::XMStoreFloat3(&this->rightVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->rightVec), rotMat));
		DirectX::XMStoreFloat3(&this->upVec, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->upVec), rotMat));
		DirectX::XMStoreFloat3(&this->lookAt, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&this->lookAt), rotMat));
	}

	SetCursorPos(WIDTH / 2, HEIGHT / 2);
}
void Camera::updateCamera()
{
	DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat3(&this->rightVec);
	DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&this->upVec);
	DirectX::XMVECTOR lookVec = DirectX::XMLoadFloat3(&this->lookAt);
	DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&this->camPos);

	lookVec = DirectX::XMVector3Normalize(lookVec);
	upVec = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(lookVec, rightVec));
	rightVec = DirectX::XMVector3Cross(upVec, lookVec);

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(camPos, rightVec));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(camPos, upVec));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(camPos, lookVec));

	DirectX::XMStoreFloat3(&this->rightVec, rightVec);
	DirectX::XMStoreFloat3(&this->upVec, upVec);
	DirectX::XMStoreFloat3(&this->lookAt, lookVec);

	DirectX::XMFLOAT4X4 tempMat;

	tempMat._11 = this->rightVec.x;
	tempMat._12 = this->rightVec.y;
	tempMat._13 = this->rightVec.z;
	tempMat._14 = x;

	tempMat._21 = this->upVec.x;
	tempMat._22 = this->upVec.y;
	tempMat._23 = this->upVec.z;
	tempMat._24 = y;

	tempMat._31 = this->lookAt.x;
	tempMat._32 = this->lookAt.y;
	tempMat._33 = this->lookAt.z;
	tempMat._34 = z;

	tempMat._41 = 0.0f;
	tempMat._42 = 0.0f;
	tempMat._43 = 0.0f;
	tempMat._44 = 1.0f;

	this->viewM = DirectX::XMLoadFloat4x4(&tempMat);
}