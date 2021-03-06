#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>





#include "../Common/shader.hpp"

#include "../Shaders/fragmentShaders.hpp"
#include "../Shaders/vertexShaders.hpp"
#include "../Common/LoadTexture.hpp"
#include "../Common/camera.hpp"
#include "../Common/windowManip.hpp"
#include "../Common/Light.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



int main(void)
{


	std::map<const char*, int> textureNames;
	std::map<const char*, int> vaoAndVboNames;
	std::map<const char*, int> eboNames;

	glm::vec3 cameraPos(0.f, 0.f, 3.f);
	WindowManip wp(SCR_WIDTH, SCR_HEIGHT, cameraPos);


	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 


	int width = 640, height = 480;
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window;
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetWindowUserPointer(window, (void*)(&wp));
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}


	const GLfloat cubeVertices[] = {
		//front										 //colors
		-0.5f,  0.5f,  0.5f,    //   1.f, 1.f, 1.f,    //top left
		 0.5f,  0.5f,  0.5f,    //   1.f, 1.f, 1.f,    //top right
		-0.5f, -0.5f,  0.5f,    //   1.f, 1.f, 1.f,    //bottom left
		 0.5f, -0.5f,  0.5f,    //   1.f, 1.f, 1.f,    //bottom right

		 //back
		-0.5f,  0.5f,  -0.5f,  //   1.f, 1.f, 1.f,    //top left
		 0.5f,  0.5f,  -0.5f,  //   1.f, 1.f, 1.f,    //top right
		-0.5f, -0.5f,  -0.5f,  //   1.f, 1.f, 1.f,    //bottom left
		 0.5f, -0.5f,  -0.5f   //   1.f, 1.f, 1.f,    //bottom right

	};

	const GLfloat cubeVerticesWithNormals[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	const GLuint cubeIndices[] = {
		 0,1,3,  0,3,2, //front
		 0,4,6,  0,6,2, //left
		 1,5,7,  1,7,3, //right
		 0,1,5,  0,5,4, //top
		 2,3,7,  2,7,6, //bottom
		 4,6,7,  4,7,5  //back
	};



#pragma region Attribs, VAO, VBO, EBO

	//vaoAndVboNames.insert({ "image1", 0 });
	//vaoAndVboNames.insert({ "image2", 1 });
	vaoAndVboNames.insert({ "light", 0 });
	vaoAndVboNames.insert({ "cubeWithNormals", 1 });
	//eboNames.insert({ "image1", 0 });
	eboNames.insert({ "light", 0 });

	GLuint VAOs[2], VBOs[2];
	GLuint  EBOs[1];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(1, EBOs);


	glBindVertexArray(VAOs[vaoAndVboNames["light"]]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["light"]]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[eboNames["light"]]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);


	//position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["light"]]);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);

	glBindVertexArray(VAOs[vaoAndVboNames["cubeWithNormals"]]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["cubeWithNormals"]]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesWithNormals), cubeVerticesWithNormals, GL_STATIC_DRAW);



	//position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["cubeWithNormals"]]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	//normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["cubeWithNormals"]]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//texture coords
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["cubeWithNormals"]]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


#pragma endregion

	//GLuint uvBuffer;

	//glGenBuffers(1, &uvBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	VertexShaders vertexShaders;
	FragmentShaders fragmentShaders;


	Shader basicProgram, secondProgram, lightProgram, cubeProgram;
	//basicProgram.LoadShaders(vertexShaders.getTestShader(), fragmentShaders.getShader());
	//secondProgram.LoadShaders(vertexShaders.getGeneral3DShader(), fragmentShaders.getTempShader());
	lightProgram.LoadShadersExternal("./Shaders/lightShader.vert", "./Shaders/lightShader.frag");
	cubeProgram.LoadShadersExternal("./Shaders/lightingShader.vert", "./Shaders/lightingShader.frag");//, "./Shaders/lightingShader.geom");

#pragma region camera stuff, MVP

	//glm::mat4 Projection = glm::perspective(
	//	glm::radians(90.f), //FOV
	//	(float)width / height, //ASPECT RATIO
	//	0.1f, //NEAR CLIPPING PLANE
	//	100.f //FAR CLIPPING PLANE
	//);

	//glm::mat4 View = glm::lookAt(
	//	glm::vec3(1.f, 0, 3), // Camera position in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	//// Model matrix : an identity matrix (model will be at the origin)
	//glm::mat4 Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around




#pragma endregion

#pragma region textureLoading

	//textureStuff
	myTexture::Texture texture("./Textures/container2.png");
	textureNames.insert({ "box", 0 });

	//texture.FlipNextImageVertically();
	//texture.LoadTexture("./Textures/awesomeface.png");

	//textureNames.insert({ "face", 1 });
	//texture.FlipNextImageVertically();
	//texture.LoadTexture("./Textures/4b.png");
	//textureNames.insert({ "cat", 2 });

	//texture.LoadTexture("./Textures/container2.png", 0);
	//texture.ChangeMap(textureNames, 0, "box");

	texture.LoadTexture("./Textures/container2_specular.png", 1);
	textureNames.insert({ "box_specular" , 1 });

	texture.LoadTexture("./Textures/cat.png", 2);
	textureNames.insert({ "emission", 2 });
	//LoadTexture2D( &Texture);// , GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_NEAREST, GL_LINEAR);


#pragma endregion

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	//const float radius = 10.f;


	ownLight::Light light1;

	glm::vec3 spotlightColor(.8f, .8f, .8f);
	spotlightColor *= glm::vec3(0.5f);

	light1.type = ownLight::SPOTLIGHT;

	light1.ambient = spotlightColor * glm::vec3(0.3f);// glm::vec3(0.4f, 0.4f, 0.4f);
	light1.diffuse = spotlightColor * glm::vec3(0.5f);// glm::vec3(0.7f, 0.7f, 0.7f); // darken diffuse light a bit
	light1.specular = spotlightColor;

	light1.cutOff = glm::cos(glm::radians(12.5f));
	light1.outerCutOff = glm::cos(glm::radians(19.5f));
	light1.constant = 1.0f;
	light1.linear = 0.14f;
	light1.quadratic = 0.07f;

	cubeProgram.addLight("spotlight", &light1);


	glm::vec3 lightColor(0.0f, 1.0f, 0.2f);


	ownLight::Light light2;

	light2.type = ownLight::POINT;

	light2.ambient = lightColor * glm::vec3(0.2f);// glm::vec3(0.0f, 0.2f, 0.1f);
	light2.diffuse = lightColor * glm::vec3(0.5f);// glm::vec3(0.0f, 0.5f, 0.25f); // darken diffuse light a bit
	light2.specular = lightColor;

	light2.position = lightPos; // move to change every frame


	light2.constant = 1.0f;
	light2.linear = 0.045f;
	light2.quadratic = 0.0075f;

	cubeProgram.addLight("point", &light2);


	glm::vec3 dirLightColor(1.0f, 0.2f, 0.9f);
	ownLight::Light light3;

	light3.type = ownLight::DIRECTIONAL;

	light3.ambient = dirLightColor * glm::vec3(0.2f);// glm::vec3(0.0f, 0.2f, 0.1f);
	light3.diffuse = dirLightColor * glm::vec3(0.7f);// glm::vec3(0.0f, 0.5f, 0.25f); // darken diffuse light a bit
	light3.specular = dirLightColor;

	light3.direction = glm::vec3(0.1f, 0.2f, 1.0f); // move to change every frame



	cubeProgram.addLight("directional", &light3);




	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		//frame stuff
		float currentFrame = glfwGetTime();
		wp.deltaTime = currentFrame - wp.lastFrame;
		wp.lastFrame = currentFrame;

		processInput(window);

		/* Render here */
		glClearColor(0.f, 0.f, 0.2f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)



		glm::mat4 projection = glm::perspective(glm::radians(wp.camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = wp.camera.GetViewMatrix();
		//glm::mat4 view = wp.camera.GetFixedViewMatrix(glm::vec3(0.f));




		cubeProgram.use();
		glBindVertexArray(VAOs[vaoAndVboNames["cubeWithNormals"]]);
		cubeProgram.setInt("material.diffuse", textureNames["box"]);
		cubeProgram.setInt("material.specular", textureNames["box_specular"]);
		cubeProgram.setFloat("material.shininess", 32.0f);
		cubeProgram.setInt("material.emission", textureNames["emission"]);
		cubeProgram.setFloat("time", glfwGetTime());
		cubeProgram.setBool("displayEmission", true);
		//cubeProgram.setVec3("material.ambient", glm::vec3(0.15f));

		light1.position = wp.camera.Position; // move to change every frame
		light1.direction = wp.camera.Front;



		cubeProgram.setMat4("view", view);
		cubeProgram.setMat4("projection", projection);



		cubeProgram.setVec3("viewPos", wp.camera.Position);

		cubeProgram.useLights();


		glm::mat4 model = glm::mat4(1.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0));
		float angle = 20.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		cubeProgram.setMat4("model", model);

		//glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(cubeIndices[0]), GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDrawArrays(GL_TRIANGLES, 0, 36);



		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

		lightProgram.use();
		glBindVertexArray(VAOs[vaoAndVboNames["light"]]);


		lightProgram.setMat4("model", model);
		lightProgram.setMat4("view", view);
		lightProgram.setMat4("projection", projection);
		//lightProgram.setMat4("MVP", 1, );
		//lightProgram.setVec3("offset", lightPos);
		lightProgram.setVec3("lightColor", lightColor);

		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(cubeIndices[0]), GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle




		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteVertexArrays(sizeof(VAOs) / sizeof(VAOs[0]), VAOs);
	glDeleteBuffers(sizeof(VBOs) / sizeof(VBOs[0]), VBOs);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	WindowManip* wp;

	wp = reinterpret_cast<WindowManip*>(glfwGetWindowUserPointer(window));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		wp->camera.ProcessKeyboard(FORWARD, wp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		wp->camera.ProcessKeyboard(BACKWARD, wp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		wp->camera.ProcessKeyboard(LEFT, wp->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		wp->camera.ProcessKeyboard(RIGHT, wp->deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	WindowManip* wp;

	wp = reinterpret_cast<WindowManip*>(glfwGetWindowUserPointer(window));
	if (wp->firstMouse)
	{
		wp->lastX = xpos;
		wp->lastY = ypos;
		wp->firstMouse = false;
	}

	float xoffset = xpos - wp->lastX;
	float yoffset = wp->lastY - ypos; // reversed since y-coordinates go from bottom to top

	wp->lastX = xpos;
	wp->lastY = ypos;

	wp->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	WindowManip* wp;

	wp = reinterpret_cast<WindowManip*>(glfwGetWindowUserPointer(window));

	wp->camera.ProcessMouseScroll(yoffset);


}