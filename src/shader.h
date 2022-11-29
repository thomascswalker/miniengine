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

	// Context components
	Vector3 m_position;
	Vector3 m_lightDirection;
	Vector3 m_viewNormal;
	Vector3 m_cameraNormal;

public:
	PixelShader(Vector3& const position,
			    Vector3& const viewNormal,
				Vector3& const lightDirection)
					: m_position(position),
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
		double lightIntensity = dot(m_lightDirection, m_viewNormal);
		Vector3 finalColor = m_diffuse * lightIntensity;
		if (finalColor._x < 0.0)
			finalColor._x = 0.0;
		if (finalColor._y < 0.0)
			finalColor._y = 0.0;
		if (finalColor._z < 0.0)
			finalColor._z = 0.0;
		return finalColor;
	}
};

MINI_NAMESPACE_CLOSE

#endif