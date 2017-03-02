#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

#include "bleeding_app.hpp"

std::vector<GLfloat> BleedingApp::cubeVertices = {
	// Positions          // Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f
};

std::vector<GLfloat> BleedingApp::planeVertices = {
	5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f,  0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  2.0f,

	5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f,  2.0f,
	5.0f, -0.5f, -5.0f,  2.0f,  2.0f
};

std::vector<GLfloat> BleedingApp::transparentVertices = {
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

BleedingApp::BleedingApp(mylib::Window && window) :
	mylib::App(std::move(window))
{
	vegetationPos.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetationPos.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetationPos.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetationPos.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetationPos.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
}

void BleedingApp::startup()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	fragDiscardShader = mylib::Shader({
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/17/17.vertex.glsl",
			GL_VERTEX_SHADER),
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/17/discard.fragment.glsl",
			GL_FRAGMENT_SHADER)
	});

	passThroughShader = mylib::Shader({
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/17/17.vertex.glsl",
			GL_VERTEX_SHADER),
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/17/passthru.fragment.glsl",
			GL_FRAGMENT_SHADER)
	});

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER,
		cubeVertices.size() * sizeof(*(cubeVertices.data())), 
		cubeVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER,
		planeVertices.size() * sizeof(*(planeVertices.data())), 
		planeVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER,
		transparentVertices.size() * sizeof(*(transparentVertices.data())),
		transparentVertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glEnableVertexAttribArray(0);	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(*cubeVertices.data()), 
		ML_BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);	// tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(*(cubeVertices.data())),
		ML_BUFFER_OFFSET(3 * sizeof(*(cubeVertices.data()))));
	glBindVertexArray(0);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glEnableVertexAttribArray(0);	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(*planeVertices.data()),
		ML_BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);	// tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(*(planeVertices.data())),
		ML_BUFFER_OFFSET(3 * sizeof(*(planeVertices.data()))));
	glBindVertexArray(0);

	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glEnableVertexAttribArray(0);	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(*transparentVertices.data()),
		ML_BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);	// tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(*(transparentVertices.data())),
		ML_BUFFER_OFFSET(3 * sizeof(*(transparentVertices.data()))));
	glBindVertexArray(0);

	cubeTexture = mylib::loadTexture(
		"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/marble.jpg");
	floorTexture = mylib::loadTexture(
		"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/metal.png");
	grassTexture = mylib::loadTextureRGBA(
		"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/grass.png");
	windowTexture = mylib::loadTextureRGBA(
		"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/window.png");

	camera = mylib::Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),	// cameraPos
		glm::vec3(0.0f, 1.0f, 0.0f));	// cmaeraUp
	cameraController = mylib::CameraController(
		&camera, &window);
}

void BleedingApp::update(double time)
{
	cameraController.update(cameraController, time);
	view = cameraController.camera->getViewMatrix();
	projection = glm::perspective(
		camera.zoom,
		static_cast<GLfloat>(window.width) / window.height,
		0.1f, 100.0f);

	if (glfwGetKey(window.getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		shaderOption = 1;
	else if (glfwGetKey(window.getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		shaderOption = 2;

	switch (shaderOption)
	{
	case 1: {
		activeShader = &fragDiscardShader;
		transparentTexture = &grassTexture;
		break;
	}
	case 2: {
		activeShader = &passThroughShader;
		transparentTexture = &windowTexture;
		break;
	}
	default: {
		activeShader = &fragDiscardShader;
		transparentTexture = &grassTexture;
		break;
	}
	}

	activeShader->use();
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "projection"),
		1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "view"),
		1, GL_FALSE, glm::value_ptr(view));

	for (int i = 0; i < vegetationPos.size(); ++i) {
		float distance = glm::length(cameraController.camera->position - vegetationPos[i]);
		sortedVegiPos[distance] = vegetationPos[i];
	}

}

void BleedingApp::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	activeShader->use();
	// Cubes
	glBindVertexArray(cubeVAO); 
	// We omit the glActiveTexture part since 
	// TEXTURE0 is already the default active texture unit. 
	// (a single sampler used in fragment is set to 0 as well by default)
	glBindTexture(GL_TEXTURE_2D, cubeTexture);
	model = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"), 
		1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"), 
		1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	// Floor
	glBindVertexArray(planeVAO);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"), 
		1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// Vegetation
	glBindVertexArray(transparentVAO);
	glBindTexture(GL_TEXTURE_2D, *transparentTexture);
	//for (auto vegetation_pos : vegetationPos)
	//{
	//	model = glm::translate(glm::mat4(), vegetation_pos);
	//	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"), 
	//		1, GL_FALSE, glm::value_ptr(model));
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//}
	for (std::map<float, glm::vec3>::reverse_iterator it = sortedVegiPos.rbegin(); 
		it != sortedVegiPos.rend(); 
		++it)
	{
		model = glm::translate(glm::mat4(), it->second);
		glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	sortedVegiPos.clear();
	glBindVertexArray(0);
}

void BleedingApp::shutdown()
{
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
}
