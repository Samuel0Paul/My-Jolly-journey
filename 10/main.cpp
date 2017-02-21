#ifdef _DEBUG
#undef _NDEBUG
#undef NDEBUG
#else
#define _NDEBUG
#define NDEBUG
#endif

#include "../lib/mylib.hpp"
#include "../lib/camera.hpp"

#include <epoxy/gl.h>
#ifdef _WIN32
#include <epoxy/wgl.h>
#else
#include <epoxy/glx.h>
#endif

#include <SOIL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cassert>

class MaterialsApp : public mylib::App
{
public:
	std::vector<GLfloat> vertices = {
		-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,			0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,			0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,			0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,			0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,			0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,			0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,			1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,			1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,			1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,			1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,			1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,			1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,			0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,			0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,			0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,			0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,			0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,			0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f
	};
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 };
	mylib::Shader lightSrcShader, shader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightPos;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	MaterialsApp(mylib::Window&& wind) : mylib::App(std::move(wind))
	{}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/10/10.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/10/material.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/10/10.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				//"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/lightSrc.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});

		glGenVertexArrays(2, VAO);
		glGenBuffers(1, &VBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(*(vertices.data())) * vertices.size(),
			vertices.data(),
			GL_STATIC_DRAW);

		// Box
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 6,
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(
			1, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 6,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 3)));
		//	(GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);	// normal
		glBindVertexArray(0);

		// lightSrc
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 6,
			ML_BUFFER_OFFSET(0)
		);
		glEnableVertexAttribArray(0);	// position
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);

		camera = mylib::Camera(
			glm::vec3(0.0f, 0.0f, 3.0f),	// cameraPos
			glm::vec3(0.0f, 1.0f, 0.0f));	// cmaeraUp
		cameraController = mylib::CameraController(
			&camera, &window);

		projection = glm::perspective(
			45.0f,
			static_cast<GLfloat>(window.width) / window.height,
			0.1f, 100.0f);

		lightPos = glm::vec3(1.2f, 1.0f, -2.0f);

		deltaTime = glfwGetTime();
	}

	void update(double time) override
	{
		deltaTime -= time;
		if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS && spacePressed != true)
			spacePressed = true;
		else if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE)
			spacePressed = false;

		if (!spacePressed) {
			cameraController.update(cameraController, time);
		}
		else {
			// W - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
				lightPos -= cameraController.camera->front * deltaTime * mylib::SPEED;
			}
			// S - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
				lightPos += cameraController.camera->front * deltaTime * mylib::SPEED;
			}
			// A - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
				lightPos += cameraController.camera->right * deltaTime * mylib::SPEED;
			}
			// D - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
				lightPos -= cameraController.camera->right * deltaTime * mylib::SPEED;
			}
			// Up - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
				lightPos.y -= deltaTime * mylib::SPEED;
			}
			// Down - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
				lightPos.y += deltaTime * mylib::SPEED;
			}
		}
		view = cameraController.camera->getViewMatrix();

		deltaTime = time;
	}

	void render() override
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Box
		shader.use();
		
		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform3f(
			glGetUniformLocation(shader.program, "material.ambient"), 
			0.24725f, 0.1995f, 0.0745f);
		glUniform3f(
			glGetUniformLocation(shader.program, "material.diffuse"), 
			0.75164, 0.60648f, 0.22648f);
		glUniform3f(
			glGetUniformLocation(shader.program, "material.specular"), 
			0.628281f, 0.555802f, 0.366065f);
		glUniform1f(
			glGetUniformLocation(shader.program, "material.shininess"), 
			0.4f * 128.0f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.position"),
			lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.ambient"),
			0.2f, 0.2f, 0.2f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.diffuse"),
			0.5f, 0.5f, 0.5f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.specular"),
			1.0f, 1.0f, 1.0f);
		glUniform3f(
			glGetUniformLocation(shader.program, "viewPos"),
			cameraController.camera->position.x,
			cameraController.camera->position.y,
			cameraController.camera->position.z);
		glUniformMatrix4fv(
			glGetUniformLocation(shader.program, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(
			glGetUniformLocation(shader.program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(
			glGetUniformLocation(shader.program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// lightSrc
		lightSrcShader.use();
		model = glm::translate(glm::mat4(), lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		glUniform3f(
			glGetUniformLocation(lightSrcShader.program, "lightColor"),
			1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

protected:

private:

};

int main(int argc, char **argv)
{
	try
	{
		mylib::Window window;
		MaterialsApp app(std::move(window));
		app.run(app);
	}
	catch (const std::exception& e)
	{

	}

	std::cin.get();
	return EXIT_SUCCESS;
}
