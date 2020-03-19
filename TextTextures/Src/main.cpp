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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



int main(void)
{


	std::map<std::string, int> textureNames;
	std::map<std::string, int> vaoAndVboNames;
	std::map<std::string, int> eboNames;

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

	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;



	//GLuint VertexArrayID;
	//glGenVertexArrays(1, &VertexArrayID);
	//glBindVertexArray(VertexArrayID);

	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.

	const GLfloat image1Vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	const GLfloat image2Vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   .5f, 0.0f, 0.0f,//   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.4f, 1.0f, 0.3f,//   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.3f, 0.0f, .7f,//   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   .9f, .4f, 0.3f,//   0.0f, 1.0f    // top left 
	};

	const GLuint indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	const GLfloat cubeVertices[] = {
		//front                //colors
		-0.5f,  0.5f,  0.5f,//   1.f, 1.f, 1.f,    //top left
		 0.5f,  0.5f,  0.5f,//   1.f, 1.f, 1.f,    //top right
		-0.5f, -0.5f,  0.5f,//   1.f, 1.f, 1.f,    //bottom left
		 0.5f, -0.5f,  0.5f,//   1.f, 1.f, 1.f,    //bottom right

		 //back
		-0.5f,  0.5f,  -0.5f,//   1.f, 1.f, 1.f,    //top left
		 0.5f,  0.5f,  -0.5f,//   1.f, 1.f, 1.f,    //top right
		-0.5f, -0.5f,  -0.5f,//   1.f, 1.f, 1.f,    //bottom left
		 0.5f, -0.5f,  -0.5f//   1.f, 1.f, 1.f,    //bottom right

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

	vaoAndVboNames.insert({ "image1", 0 });
	vaoAndVboNames.insert({ "image2", 1 });
	vaoAndVboNames.insert({ "light", 2 });
	eboNames.insert({ "image1", 0 });
	eboNames.insert({ "light", 1 });

	GLuint VAOs[3], VBOs[3];
	GLuint  EBOs[2];
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
	glGenBuffers(2, EBOs);
#pragma region oldTextureStuff

	glBindVertexArray(VAOs[vaoAndVboNames["image1"]]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["image1"]]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(image1Vertices), image1Vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[eboNames["image1"]]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["image1"]]);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		8 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);

	//color
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		8 * sizeof(float),                // stride
		(void*)(3 * sizeof(float))        // array buffer offset
	);

	//uv - texture coord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		8 * sizeof(float),                // stride
		(void*)(6 * sizeof(float))        // array buffer offset
	);



	glBindVertexArray(VAOs[vaoAndVboNames["image2"]]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["image2"]]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(image2Vertices), image2Vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[eboNames["image1"]]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vaoAndVboNames["image2"]]);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);

	//color
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		6 * sizeof(float),                // stride
		(void*)(3 * sizeof(float))        // array buffer offset
	);

#pragma endregion



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

	////color
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(
	//	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	//	3,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	6 * sizeof(float),                // stride
	//	(void*)(3 * sizeof(float))        // array buffer offset
	//);


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
	cubeProgram.LoadShadersExternal("./Shaders/lightingShader.vert", "./Shaders/lightingShader.frag");

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
	myTexture::Texture texture("./Textures/wall.jpg");
	textureNames.insert({ "wall", 0 });

	texture.FlipNextImageVertically();
	texture.LoadTextureTransparent("./Textures/awesomeface.png");

	textureNames.insert({ "face", 1 });
	texture.FlipNextImageVertically();
	texture.LoadTextureTransparent("./Textures/4b.png");
	textureNames.insert({ "cat", 2 });
	//LoadTexture2D( &Texture);// , GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_NEAREST, GL_LINEAR);


#pragma endregion

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	


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

#pragma region OldTextureStuff
		/*
		secondProgram.use();
		glBindVertexArray(VAOs[vaoAndVboNames["image2"]]);
		secondProgram.setMat4("MVP", 1, &mvp[0][0]);
		secondProgram.setVec3("offset", glm::vec3(-.5f, .5f, 0));
		secondProgram.setFloat("alpha", 0.5f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle


		basicProgram.use();
		glBindVertexArray(VAOs[vaoAndVboNames["image1"]]);
		basicProgram.setVec3("offset", glm::vec3(-1.f, .5f, 0));
		basicProgram.setMat4("MVP", 1, &mvp[0][0]);
		basicProgram.setVec3("offset", glm::vec3(.2f, -.5f, 0));
		basicProgram.setInt("texture1", textureNames["wall"]);
		basicProgram.setInt("texture2", textureNames["cat"]);
		basicProgram.setFloat("interpolation", 1.f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle
		*/


#pragma endregion


		glm::mat4 projection = glm::perspective(glm::radians(wp.camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = wp.camera.GetViewMatrix();

		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

		lightProgram.use();
		glBindVertexArray(VAOs[vaoAndVboNames["light"]]);
		glm::mat4 lightModel = glm::translate(model, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		lightProgram.setMat4("model", lightModel);
		lightProgram.setMat4("view", view);
		lightProgram.setMat4("projection", projection);
		//lightProgram.setMat4("MVP", 1, );
		//lightProgram.setVec3("offset", lightPos);

		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(cubeIndices[0]), GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle

		cubeProgram.use();


		cubeProgram.setMat4("view", view);
		cubeProgram.setMat4("projection", projection);

		model = glm::mat4(1.0f);
		cubeProgram.setMat4("model", model);
		//cubeProgram.setMat4("MVP", 1, &mvp[0][0]);
		cubeProgram.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		cubeProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//cubeProgram.setVec3("offset", glm::vec3(1.5f, -1.0f, -1.0f));

		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(cubeIndices[0]), GL_UNSIGNED_INT, 0); // Starting from vertex 0; 3 vertices total -> 1 triangle


		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]) / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);
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
	
	wp =reinterpret_cast<WindowManip*>(glfwGetWindowUserPointer(window));

	wp->camera.ProcessMouseScroll(yoffset);


}