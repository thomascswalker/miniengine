#ifndef SHADER_H
#define SHADER_H

#include "api.h"
#include "maths.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class VertexShader
{
public:
	VertexShader() { };
};

class PixelShader
{
	// Shader components
	Vector3 m_diffuse = Vector3(0.5, 0.5, 0.5);
	Vector3 m_emission = Vector3(0.0);
	double m_ior = 1.52;

	// Context components
	Vector3 m_position;
	Vector3 m_lightDirection;
	Vector3 m_worldNormal;
	Vector3 m_viewNormal;
	Vector3 m_cameraNormal;

	// Light
	double m_lightIntensity = 1.0;

public:
	PixelShader(Vector3& const position,
				Vector3& const worldNormal,
			    Vector3& const viewNormal,
				Vector3& const lightDirection)
					: m_position(position),
					  m_worldNormal(worldNormal),
					  m_viewNormal(viewNormal),
					  m_lightDirection(lightDirection) { };

	/// <summary>
	/// Simple PBR shader.
	/// https://dev.opencascade.org/doc/overview/html/specification__pbr_math.html
	/// </summary>
	/// <returns>The final Vector3 (RGB) pixel value.</returns>
	inline Vector3 process()
	{
		// Lambertian shading
		double lighting = dot(m_lightDirection, m_worldNormal);
		lighting *= m_lightIntensity;
		Vector3 finalColor = m_diffuse * lighting;

		// Add some fresnel
		double facingRatio = (1.0 - m_viewNormal._z) * m_ior;

		finalColor._x = finalColor._x + facingRatio;
		finalColor._y = finalColor._y + facingRatio;
		finalColor._z = finalColor._z + facingRatio;

		// Floor
		finalColor._x = clamp(finalColor._x, 0.0, 1.0);
		finalColor._y = clamp(finalColor._y, 0.0, 1.0);
		finalColor._z = clamp(finalColor._z, 0.0, 1.0);

		return finalColor;
	}
};

MINI_NAMESPACE_CLOSE

#endif