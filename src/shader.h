#ifndef SHADER_H
#define SHADER_H

#include "api.h"
#include "maths.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

#define BLINN_MODEL false
#define BLINN_PHONG_MODEL true

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
	Vector3 m_specularColor = Vector3(1.0, 1.0, 1.0);
	double m_ior = 1.52;
	double m_shininess = 16.0;

	// Light
	double m_lightIntensity = 1.0;
	Vector3 m_lightColor = Vector3(1.0, 1.0, 1.0);
	Vector3 m_ambientColor = Vector3(0.0, 0.0, 0.0);

public:
	PixelShader() { };

	void setColor(Vector3& color) { m_color = color; }

	/// <summary>
	/// Simple PBR shader.
	/// https://dev.opencascade.org/doc/overview/html/specification__pbr_math.html
	/// </summary>
	/// <returns>The final Vector3 (RGB) pixel value.</returns>
	inline Vector3 process(Vector3& viewPosition,
						   Vector3& worldPosition,
						   Vector3& worldNormal,
						   Vector3& viewNormal,
						   Vector3& lightPosition)
	{
#if BLINN_MODEL
		// Calculate ambient contribution
		double ambientStrength = 0.25;
		Vector3 ambientContribution = m_ambientColor * ambientStrength;

		// Calculate lighting contribution
		Vector3 lightDirection = lightPosition - worldPosition;
		lightDirection.normalize();
		double diff = MAX(dot(worldNormal, lightDirection), 0.0);
		Vector3 lightingContribution = m_lightColor * diff;

		// Calculate specular contribution
		double specularStrength = 4.0;
		Vector3 viewDirection = viewPosition - worldPosition;
		viewDirection.normalize();
		Vector3 reflectDirection = reflect(-lightDirection, worldNormal);
		double spec = pow((MAX(dot(viewDirection, reflectDirection), 0.0)), 32);
		Vector3 specularContribution = m_specularColor * specularStrength * spec;

		// Return final color
		return (ambientContribution + (m_color * lightingContribution) + specularContribution);
#elif BLINN_PHONG_MODEL

		// Calculate normalized view direction
		Vector3 viewDirection = viewPosition - worldPosition;
		viewDirection.normalize();

		// Calculate normalized light direction to pixel position
		Vector3 lightDirection = lightPosition - worldPosition;
		lightDirection.normalize();

		// Inverse falloff distance
		double distance = pow(lightDirection.length(), 2.0);

		// Calculate lighting contribution
		double lighting = MAX(dot(lightDirection, worldNormal), 0.0);
		double specular = 0.0;

		// If lighting contribution is greater than 0, we can calculate specular contribution
		if (lighting > 0.0)
		{
			// Get half direction between light normal and view normal
			Vector3 halfDirection = lightDirection + viewDirection;
			halfDirection.normalize();

			// Calculate specular contribution
			double specularAngle = MAX(dot(halfDirection, worldNormal), 0.0);
			specular = pow(specularAngle, m_shininess);
		}

		// Add lighting + specular components
		Vector3 linearColor = m_color * lighting * m_lightColor * m_lightIntensity / distance +
							  m_specularColor * specular * m_lightColor * m_lightIntensity / distance;

		return linearColor;
#else
		return Vector3(1.0);
#endif
	}
};

MINI_NAMESPACE_CLOSE

#endif