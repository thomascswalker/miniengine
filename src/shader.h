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
	Vector3 m_diffuse = Vector3(0.5, 0.30, 0.25);
	Vector3 m_emission = Vector3(0.0);
	double m_ior = 1.52;

	// Light
	double m_lightIntensity = 1.0;

public:
	PixelShader() { };
	PixelShader(std::string filename)
		: m_filename(filename)
	{
		if (!readShaderFile())
		{
			throw std::invalid_argument("Failed to read file.");
		}
	}

	/// <summary>
	/// Simple PBR shader.
	/// https://dev.opencascade.org/doc/overview/html/specification__pbr_math.html
	/// </summary>
	/// <returns>The final Vector3 (RGB) pixel value.</returns>
	inline Vector3 process(Vector3& position,
						   Vector3& worldNormal,
						   Vector3& viewNormal,
						   Vector3& lightDirection)
	{
		// Lighting component
		double lightHit = dot(lightDirection, worldNormal);
		double lighting = lightHit * m_lightIntensity;

		// Specular component
		double specularIntensity = 0.0;

		// Compute final color
		Vector3 finalColor = (m_diffuse * lighting) + specularIntensity;

		// Clamp final values so they're within the 0 => 1 range.
		finalColor._x = clamp(finalColor._x, 0.0, 1.0);
		finalColor._y = clamp(finalColor._y, 0.0, 1.0);
		finalColor._z = clamp(finalColor._z, 0.0, 1.0);

		return finalColor;
	}

private:
	bool readShaderFile()
	{
		return true;
	}
};

MINI_NAMESPACE_CLOSE

#endif