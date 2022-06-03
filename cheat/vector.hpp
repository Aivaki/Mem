#pragma once

#include "entity.hpp"


/* Vector class */


#pragma region vector
#ifndef vectorDefine
#define vectorDefine

namespace vector
{
	constexpr auto PI = 3.14159265359;

	class Vector
	{
	public:

		/* Functions for the correct work of the aimbot */
		D3DXVECTOR3 clampAngles(D3DXVECTOR3 angles) const noexcept(true) {
			angles.x = std::clamp(angles.x, -89.0f, 89.0f);
			angles.y = std::clamp(angles.y, -180.0f, 180.0f);
			angles.z = 0.0f;

			return angles;
		}
		//
		D3DXVECTOR3 clamAngles(D3DXVECTOR3 currangle) const noexcept(true) {
			D3DXVECTOR3 angles = currangle;
			if (angles.x < -180.0f) angles.x += 360.0f;
			if (angles.x > 180.0f) angles.x -= 360.0f;

			if (angles.y < -180.0f) angles.y += 360.0f;
			if (angles.y > 180.0f) angles.y -= 360.0f;

			if (angles.x > 89.0f && angles.x <= 180.0f) { angles.x = 89.0f; }
			return angles;
		}
		//
		D3DXVECTOR3 calcAngle(const D3DXVECTOR3& src, const D3DXVECTOR3& dst) const noexcept(true) {
			D3DXVECTOR3 angles;

			D3DXVECTOR3 delta = { dst.x - src.x, dst.y - src.y, dst.z - src.z };

			auto Hyp = static_cast<float>(sqrt(delta.x * delta.x + delta.y * delta.y));

			angles.x = static_cast<float>((atan2(-delta.z, Hyp) * (180.0f / PI)));
			angles.y = static_cast<float>((atan2(delta.y, delta.x) * (180.0f / PI)));

			angles.z = 0.0f;

			return angles;
		}
		//
		void normalizeAngles(D3DXVECTOR3& angles) noexcept(true) {
			if (angles.x < -180.0f) angles.x += 360.0f;
			if (angles.x > 180.0f) angles.x -= 360.0f;

			if (angles.y < -180.0f) angles.y += 360.0f;
			if (angles.y > 180.0f) angles.y -= 360.0f;
		}
		//
		void vectorAngles(D3DXVECTOR3& forward, D3DXVECTOR3& angles) noexcept(true) {
			float tmp, yaw, pitch;

			if (forward.y == 0.0f && forward.x == 0.0f) {
				yaw = 0.0f;
				if (forward.z > 0.0f) { pitch = 270.0f; }
				else { pitch = 90.0f; }
			}
			else {
				yaw = static_cast<float>((atan2(forward.y, forward.x) * 180.0f / PI));
				if (yaw < 0.0f) { yaw += 360.0f; }
				tmp = static_cast<float>(sqrt(forward.x * forward.x + forward.y * forward.y));
				pitch = static_cast<float>((atan2(-forward.z, tmp) * 180.0f / PI));

				if (pitch < 0.0f) { pitch += 360.0f; }
			}

			angles.x = pitch;
			angles.y = yaw;
			angles.z = 0.0f;
		}
	};
}

#endif
#pragma endregion