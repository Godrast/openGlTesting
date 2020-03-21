#ifndef OWN_LIGHT
#define OWN_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace own
{
	enum LightType
	{
		DIRECTIONAL,
		SPOTLIGHT,
		POINT
	};

	struct Light
	{
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;
		glm::vec3 direction;

		LightType type;

	};
}
#endif // !OWN_LIGHT
