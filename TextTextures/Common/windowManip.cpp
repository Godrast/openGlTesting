#include "windowManip.hpp"

WindowManip::WindowManip(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, glm::vec3 & cameraPos)
{

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	camera = Camera(cameraPos);

}
