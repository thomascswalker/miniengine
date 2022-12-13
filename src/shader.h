#ifndef SHADER_H
#define SHADER_H

#include "api.h"
#include "matrix.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

#define BLINN_MODEL false
#define BLINN_PHONG_MODEL true

/// <summary>
/// Base class for all shaders.
/// </summary>
class IShader
{
public:
	IShader() { };

	virtual void vertex(Vector3* v) = 0;
	virtual Vector3 fragment() = 0;

	int width = 0;
	int height = 0;

	Matrix4 matrix = Matrix4();

	Vector3 worldPosition;
	Vector3 worldNormal;
	Vector3 viewPosition;
	Vector3 viewNormal;
	Vector3 pixelPosition;
	Vector3 uvw;
};

/// <summary>
/// Standard Blinn-Phong shader.
/// </summary>
class StandardShader
	: public IShader
{
public:
	void vertex(Vector3* v)
	{
		// Convert to normalized device coords
		Vector4 ndc = matrix * Vector4(*v, 1.0);

		double x = ((ndc._x + 1.0) * width) / 2.0;
		double y = ((ndc._y + 1.0) * height) / 2.0;

		v->_x = x;
		v->_y = y;
		v->_z = 1.0 / ndc._z;
	}

	Vector3 fragment()
	{
		Vector3 ambient(0.1);
		Vector3 color(0.5, 0.25, 0.5);
		Vector3 lightPosition(25.0, 25.0, 0.0);
		Vector3 lightColor(1.0, 0.5, 0.25);
		double shininess = 16.0;
		double lightIntensity = 1.0;
		Vector3 specularColor(1.0, 1.0, 1.0);

		// Calculate normalized view direction
		Vector3 viewDirection = normalize(viewPosition - worldPosition);

		// Calculate normalized light direction to pixel position
		Vector3 lightDirection = normalize(lightPosition - worldPosition);

		// Inverse falloff distance
		double distance = pow(lightDirection.length(), 2.0);

		// Calculate lighting contribution
		double lighting = MAX(dot(lightDirection, worldNormal), 0.0);
		double specular = 0.0;

		// If lighting contribution is greater than 0, we can calculate specular contribution
		if (lighting > 0.0)
		{
			// Get half direction between light normal and view normal
			Vector3 halfDirection = normalize(lightDirection + viewDirection);

			// Calculate specular contribution
			double specularAngle = MAX(dot(halfDirection, worldNormal), 0.0);
			specular = pow(specularAngle, shininess);
		}

		// Add lighting + specular components
		Vector3 linearColor = ambient + 
							  color * lighting * lightColor * lightIntensity / distance +
							  specularColor * specular * lightColor * lightIntensity / distance;

		return linearColor;
	}
};

MINI_NAMESPACE_CLOSE

#endif