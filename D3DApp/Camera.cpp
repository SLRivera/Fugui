#include "Camera.h"
#include "DXInput.h"

using namespace DirectX;
using namespace Fugui;

Camera::Camera()
{
	XMStoreFloat4x4(&m_view, XMMatrixIdentity());
	XMStoreFloat4x4(&m_proj, XMMatrixIdentity());
	XMStoreFloat4x4(&m_viewProj, XMMatrixIdentity());

	m_posW = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_rightW = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_upW = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	m_lookW = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

	m_speed = 15.0f;
}


Camera::~Camera()
{
}

void Fugui::Camera::lookAt(DirectX::XMFLOAT4& pos, DirectX::XMFLOAT4& target, DirectX::XMFLOAT4& up)
{
	XMVECTOR vPos = XMLoadFloat4(&pos);
	XMVECTOR vTarget = XMLoadFloat4(&target);
	XMVECTOR vUp = XMLoadFloat4(&up);

	XMVECTOR L = XMVectorSubtract(vTarget, vPos);
	L = XMVector3Normalize(L);

	XMVECTOR R = XMVector3Cross(vUp, L);
	R = XMVector3Normalize(L);

	XMVECTOR U = XMVector3Cross(L, R);
	U = XMVector3Normalize(U);

	m_posW = pos;
	XMStoreFloat4(&m_rightW, R);
	XMStoreFloat4(&m_upW, U);
	XMStoreFloat4(&m_lookW, L);

	buildView();
	buildFrustumPlanes();

	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX viewProj = XMLoadFloat4x4(&m_viewProj);

	viewProj = XMMatrixMultiply(view, proj);
	XMStoreFloat4x4(&m_viewProj, viewProj);
}

void Fugui::Camera::setLens(float fov, float aspect, float nearZ, float farZ)
{
	XMMATRIX proj = XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
	
	XMStoreFloat4x4(&m_proj, proj);

	buildView();

	buildFrustumPlanes();
}

void Fugui::Camera::buildView()
{
	XMVECTOR pos = XMLoadFloat4(&m_posW);
	XMVECTOR up = XMLoadFloat4(&m_upW);
	XMVECTOR look = XMLoadFloat4(&m_lookW);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);

	XMMATRIX view = XMMatrixLookAtLH(pos, look, up);
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_viewProj, viewProj);
}

void Fugui::Camera::buildFrustumPlanes()
{

}

void Fugui::Camera::update(float dt, float offsetHeight)
{
	XMVECTOR look = XMLoadFloat4(&m_lookW);
	XMVECTOR dir = XMVectorZero();
	XMVECTOR right = XMLoadFloat4(&m_rightW);
	XMVECTOR pos = XMLoadFloat4(&m_posW);
	XMVECTOR up = XMLoadFloat4(&m_upW);

	if (g_input->keyDown(DIK_W))
		dir = XMVectorAdd(look, dir);
	if (g_input->keyDown(DIK_S))
		dir = XMVectorAdd(-look, dir);
	if (g_input->keyDown(DIK_D))
		dir = XMVectorAdd(right, dir);
	if (g_input->keyDown(DIK_A))
		dir = XMVectorAdd(-right, dir);

	dir = XMVector3Normalize(dir);

	XMVECTOR newPos = pos + dir * m_speed * dt/*XMVectorAdd(pos, dir*m_speed*dt)*/;

	XMStoreFloat4(&m_posW, newPos);

	static float pitch = 0.0f;
	pitch = g_input->mouseMoveY() / 150.0f;
	float yAngle = g_input->mouseMoveX() / 150.0f;

	XMMATRIX R = XMMatrixRotationAxis(right, pitch);
	look = XMVector3TransformCoord(look, R);
	up = XMVector3TransformCoord(up, R);
	//right = XMVector3TransformCoord(right, R);
// 
// 	R = XMMatrixRotationY(yAngle);
// 	right = XMVector3TransformCoord(right, R);
// 	up = XMVector3TransformCoord(up, R);
// 	look = XMVector3TransformCoord(look, R);
// 
	look = XMVector3Normalize(look);
	up = XMVector3Normalize(up);
	right = XMVector3Normalize(right);

	XMStoreFloat4(&m_rightW, right);
	XMStoreFloat4(&m_upW, up);
	XMStoreFloat4(&m_lookW, look);

	buildView();
}
