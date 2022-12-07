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
	Vector3 m_color = Vector3(0.85, 0.4, 0.3);
	Vector3 m_emission = Vector3(0.0);
	double m_ior = 1.52;

	// Light
	double m_lightIntensity = 1.0;
	Vector3 m_lightColor = Vector3(1.0);

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
		// Calculate ambient contribution
		double ambientStrength = 0.1;
		Vector3 ambientContribution = m_lightColor * ambientStrength;

		// Calculate lighting contribution
		Vector3 lightDirection = lightPosition - worldPosition;
		lightDirection.normalize();
		double diff = MAX(dot(worldNormal, lightDirection), 0.0);
		Vector3 lightingContribution = m_lightColor * diff;

		// Calculate specular contribution
		double specularStrength = 2.0;
		Vector3 viewDirection = viewPosition - worldPosition;
		viewDirection.normalize();
		Vector3 reflectDirection = reflect(-lightDirection, worldNormal);
		double spec = pow((MAX(dot(viewDirection, reflectDirection), 0.0)), 32);
		Vector3 specularContribution = specularStrength * spec;

		// Return final color
		return (ambientContribution + lightingContribution + specularContribution) * m_color;
	}
};

MINI_NAMESPACE_CLOSE

#endif