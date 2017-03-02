#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

#include "faceculling_app.hpp"


std::vector<GLfloat> FaceCullingApp::CCW_vertices = {
	0.f, 0.f, 0.f,	// 1
	1.f, 0.f, 0.f,	// 2
	0.f, 1.f, 0.f,	// 3
};

std::vector<GLfloat> FaceCullingApp::CW_vertices = {
	0.f, 0.f, 0.f,	// 1
	0.f, 1.f, 0.f,	// 3
	1.f, 0.f, 0.f,	// 2
};

FaceCullingApp::FaceCullingApp(mylib::Window&& window) :
	mylib::App(std::move(window))
{
}


FaceCullingApp::~FaceCullingApp()
{
}

void FaceCullingApp::startup()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);	// frontFace default: GL_CCW

	shader = mylib::Shader({
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/18/18.vertex.glsl",
			GL_VERTEX_SHADER),
		std::make_pair(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/18/18.fragment.glsl",
			GL_FRAGMENT_SHADER)
	});

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, cw_VBO);
	glBufferData(GL_ARRAY_BUFFER,
		CW_vertices.size() * sizeof(*(CW_vertices.data())), CW_vertices.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, counterCw_VBO);
	glBufferData(GL_ARRAY_BUFFER,
		CCW_vertices.size() * sizeof(*(CCW_vertices.data())), CCW_vertices.data(),
		GL_STATIC_DRAW);

	glBindVertexArray(cw_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cw_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		0, ML_BUFFER_OFFSET(0));
	glBindVertexArray(0);

	glBindVertexArray(counterCw_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, counterCw_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		0, ML_BUFFER_OFFSET(0));
	glBindVertexArray(0);

	camera = mylib::Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),	// cameraPos
		glm::vec3(0.0f, 1.0f, 0.0f));	// cmaeraUp
	cameraController = mylib::CameraController(
		&camera, &window);
}

void FaceCullingApp::update(double time)
{
	cameraController.update(cameraController, time);
	view = cameraController.camera->getViewMatrix();
	projection = glm::perspective(
		camera.zoom,
		static_cast<GLfloat>(window.width) / window.height,
		0.1f, 100.0f);

	activeShader = &shader;
	activeShader->use();
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "projection"),
		1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "view"),
		1, GL_FALSE, glm::value_ptr(view));
}

void FaceCullingApp::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(cw_VAO);
	model = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
		1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glBindVertexArray(counterCw_VAO);
	model = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
		1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void FaceCullingApp::shutdown()
{
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
}
