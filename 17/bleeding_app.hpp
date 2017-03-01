#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

#pragma once

/*
*	Also refer to http://andersriggelsen.dk/glblendfunc.php
*/

class BleedingApp : public mylib::App
{
public:
	static std::vector<GLfloat> cubeVertices;
	static std::vector<GLfloat> planeVertices;
	static std::vector<GLfloat> transparentVertices;

	GLuint VBO[5]{ 0 }, VAO[5]{ 0 };
	GLuint &cubeVBO = VBO[0], &cubeVAO = VAO[0],
		&planeVBO = VBO[1], &planeVAO = VAO[1],
		&transparentVBO = VBO[2], &transparentVAO = VAO[2];
	GLuint cubeTexture, floorTexture, grassTexture, windowTexture,
		*transparentTexture{ nullptr };

	std::vector<glm::vec3> vegetationPos;
	std::map<float, glm::vec3> sortedVegiPos;

	int shaderOption{ 0 };
	mylib::Shader fragDiscardShader, passThroughShader,
		*activeShader{ nullptr };
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::mat4 model, view, projection;

	BleedingApp(mylib::Window&& window);

private:
	void startup() override;
	void update(double time) override;
	void render() override;
	void shutdown() override;
};
