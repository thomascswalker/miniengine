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
	// Shader file
	std::string m_filename = "";

	// Shader components
	Vector3 m_color = Vector3(0.5, 0.5, 0.5);
	Vector3 m_emission = Vector3(0.0);
	double m_ior = 1.52;

	// Light
	double m_lightIntensity = 1.0;

public:
	PixelShader() { };

	void setColor(Vector3& color) { m_color = color; }

	/// <summary>
	/// Simple PBR shader.
	/// https://dev.opencascade.org/doc/overview/html/specification__pbr_math.html
	/// </summary>
	/// <returns>The final Vector3 (RGB) pixel value.</returns>
	inline Vector3 process(Vector3& screenPosition,
						   Vector3& viewPosition,
						   Vector3& worldPosition,
						   Vector3& worldNormal,
						   Vector3& viewNormal,
						   Vector3& lightPosition)
	{
		Vector3 lightDirection = lightPosition - worldPosition;
		lightDirection.normalize();

		Vector3 viewDirection = viewPosition - worldPosition;
		viewDirection.normalize();

		// Angle between surface normal (in world space) and light direction
		double cosLightDirection = MAX(0.0, dot(worldNormal, lightDirection));

		// Specular reflection vector
		Vector3 specularReflection((worldNormal * 2.0 * cosLightDirection) - lightDirection);

		//// Lighting component
		double lightHit = dot(lightPosition, worldNormal);
		double lighting = lightHit * m_lightIntensity;

		// Compute final color
		Vector3 finalColor = (m_color * lighting) + specularReflection;

		return finalColor;
	}
};

MINI_NAMESPACE_CLOSE

#endif