#pragma once
#include "D3DUtils.h"


namespace Fugui
{
	class Camera
	{
	public:
		enum ClipPlanes {
			NEAR_CLIP,
			FAR_CLIP,
			LEFT_CLIP,
			RIGHT_CLIP,
			TOP_CLIP,
			BOTTOM_CLIP,
			END_CLIPS
		};

		inline DirectX::XMFLOAT4X4& view();
		inline DirectX::XMFLOAT4X4& proj();
		inline DirectX::XMFLOAT4X4& viewProj();

		inline const DirectX::XMFLOAT4& right() const;
		inline const DirectX::XMFLOAT4& up() const;
		inline const DirectX::XMFLOAT4& look() const;

		inline DirectX::XMFLOAT4& pos();

		void lookAt(DirectX::XMFLOAT4& pos, DirectX::XMFLOAT4& target, DirectX::XMFLOAT4& up);
		void setLens(float fov, float aspect, float nearZ, float farZ);
		void setSpeed(float s);

		void update(float dt, float offsetHeight);

		Camera();
		~Camera();

	protected:
		DirectX::XMFLOAT4X4	m_view,
										m_proj,
										m_viewProj;

		DirectX::XMFLOAT4 m_frustumPlanes[6];

		DirectX::XMFLOAT4	m_posW,
									m_rightW,
									m_upW,
									m_lookW;

		float	m_speed;

		void buildView();
		void buildFrustumPlanes();
	};

	DirectX::XMFLOAT4X4& Camera::view() { return m_view; }
	DirectX::XMFLOAT4X4& Camera::proj() { return m_proj; }
	DirectX::XMFLOAT4X4& Camera::viewProj() { return m_viewProj; }

	const DirectX::XMFLOAT4& Camera::right() const		{ return m_rightW; }
	const DirectX::XMFLOAT4& Camera::up() const	{ return m_upW; }
	const DirectX::XMFLOAT4& Camera::look() const	{ return m_lookW; }

	DirectX::XMFLOAT4& Camera::pos()		{ return m_posW; }
} // end namespace Fugui
