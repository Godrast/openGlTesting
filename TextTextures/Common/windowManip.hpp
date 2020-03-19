#ifndef WINDOW_MANIP
#define WINDOW_MANIP
#include "camera.hpp"

class WindowManip
{

public:

	float deltaTime = 0.f;
	float lastFrame = 0.f;

	Camera camera;//(glm::vec3(0.f, 0.f, 3.f));


	float lastX;// = SCR_WIDTH / 2.0f;
	float lastY;// = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	WindowManip(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, glm::vec3 &cameraPos);

	//processI


};

#endif // !WINDOW_MANIP
