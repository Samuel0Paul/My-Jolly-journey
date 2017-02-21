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
#include <limits>

class DirectionalLightCasterApp : public mylib::App
{
public:
	std::vector<GLfloat> vertices = {
		// Positions			// Normals				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 }, textures[5]{ 0 };
	mylib::Shader lightSrcShader, shader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightPos, lightDir;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	DirectionalLightCasterApp(mylib::Window&& wind) : mylib::App(std::move(wind))
	{}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/directional.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
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
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(
			1, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 3)));
		glEnableVertexAttribArray(1);	// normal
		glVertexAttribPointer(
			2, 2,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 6)));
		glEnableVertexAttribArray(2);	// TexCoords
		glBindVertexArray(0);

		// lightSrc
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0)
		);
		glEnableVertexAttribArray(0);	// position
		glBindVertexArray(0);

		// texture
		glGenTextures(2, textures);
		// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int img_height{ 0 }, img_width{ 0 };
		unsigned char* image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img_height = 0; img_width = 0;
		image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

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
		lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);

		deltaTime = glfwGetTime();
	}

	~DirectionalLightCasterApp()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(1, VBO);
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
		glActiveTexture(GL_TEXTURE0);	// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);	// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform1i(
			glGetUniformLocation(shader.program, "material.diffuse"),
			0);
		glUniform1i(
			glGetUniformLocation(shader.program, "material.specular"),
			1);
		glUniform1f(
			glGetUniformLocation(shader.program, "material.shininess"),
			0.4f * 128.0f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.direction"),
			lightDir.x, lightDir.y, lightDir.z);
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
		for (const auto& cubePos : cubePositions) {
			model = glm::translate(glm::mat4(), cubePos);
			glUniformMatrix4fv(
				glGetUniformLocation(shader.program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// lightSrc
		// lightSrcShader.use();
		// model = glm::translate(glm::mat4(), lightPos);
		// model = glm::scale(model, glm::vec3(0.5f));
		// glUniform3f(
		// 	glGetUniformLocation(lightSrcShader.program, "lightColor"),
		// 	1.0f, 1.0f, 1.0f);
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "view"),
		// 	1, GL_FALSE, glm::value_ptr(view));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "projection"),
		// 	1, GL_FALSE, glm::value_ptr(projection));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "model"),
		// 	1, GL_FALSE, glm::value_ptr(model));
		// glBindVertexArray(VAO[1]);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);
	}

};

//!	Class AttenuationPointLightCasterApp
///
///	Attenuation
///
///	formula: 1.0 / (Kc + (Kl * d) + (Kq * pow(d, 2)))
///	
///	legend:
///		Kc - const term, 
///				usually kept at 1.0 which is mainly there to make sure 
///				the resulting denominator never gets smaller than 1 since it 
///				would otherwise boost the intensity with certain distances, 
///				which is not the effect we're looking for.
///		Kl - linear term, 
///				is multiplied with the distance value that 
///				reduces the intensity in a linear fashion.
///		Kq - quadratic term, 
///				is multiplied with the quadrant of the distance and 
///				sets a quadratic decrease of intensity for the light source. 
///				The quadratic term will be less significant compared to the linear term 
///				when the distance is small, but gets much larger than the 
///				linear term as the distance grows.
///
///		http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
///		Distance	Constant	Linear		Quadratic
///		7			1.0			0.7			1.8
///		13			1.0			0.35		0.44
///		20			1.0			0.22		0.20
///		32			1.0			0.14		0.07
///		50			1.0			0.09		0.032
///		65			1.0			0.07		0.017
///		100			1.0			0.045		0.0075
///		160			1.0			0.027		0.0028
///		200			1.0			0.022		0.0019
///		325			1.0			0.014		0.0007
///		600			1.0			0.007		0.0002
///		3250		1.0			0.0014		0.000007
///
class AttenuationPointLightCasterApp : public mylib::App
{
public:
	std::vector<GLfloat> vertices = {
		// Positions			// Normals				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 }, textures[5]{ 0 };
	mylib::Shader lightSrcShader, shader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightPos, lightDir;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	AttenuationPointLightCasterApp(mylib::Window&& wind) : mylib::App(std::move(wind))
	{}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/attenuation.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
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
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(
			1, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 3)));
		glEnableVertexAttribArray(1);	// normal
		glVertexAttribPointer(
			2, 2,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 6)));
		glEnableVertexAttribArray(2);	// TexCoords
		glBindVertexArray(0);

		// lightSrc
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0)
		);
		glEnableVertexAttribArray(0);	// position
		glBindVertexArray(0);

		// texture
		glGenTextures(2, textures);
		// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int img_height{ 0 }, img_width{ 0 };
		unsigned char* image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img_height = 0; img_width = 0;
		image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

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
		lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);

		deltaTime = glfwGetTime();
	}

	~AttenuationPointLightCasterApp()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(1, VBO);
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
		glActiveTexture(GL_TEXTURE0);	// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);	// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform1i(
			glGetUniformLocation(shader.program, "material.diffuse"),
			0);
		glUniform1i(
			glGetUniformLocation(shader.program, "material.specular"),
			1);
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
		glUniform1f(
			glGetUniformLocation(shader.program, "light.constant"),
			1.0f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.linear"),
			0.09f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.quadratic"),
			0.032f);
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
		for (const auto& cubePos : cubePositions) {
			model = glm::translate(glm::mat4(), cubePos);
			glUniformMatrix4fv(
				glGetUniformLocation(shader.program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// lightSrc
		lightSrcShader.use();
		model = glm::translate(glm::mat4(), lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
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

};
//!	Class SpotlightPointLightCasterApp
///
///	Spotlight lighting
///		Spotlight in OpenGL is represented by a world-space position, a direction 
///		and a cutoff angle that specifies the radius of the spotlight. 
///		For each fragment we calculate if the fragment is between the 
///		spotlight's cutoff directions (thus in its cone) and if so, 
///		we lit the fragment accordingly
///
class SpotlightPointLightCasterApp : public mylib::App
{
public:
	std::vector<GLfloat> vertices = {
		// Positions			// Normals				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 }, textures[5]{ 0 };
	mylib::Shader lightSrcShader, shader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightPos, lightDir;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	SpotlightPointLightCasterApp(mylib::Window&& wind) : mylib::App(std::move(wind))
	{}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/flashlight.spotlight.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
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
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(
			1, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 3)));
		glEnableVertexAttribArray(1);	// normal
		glVertexAttribPointer(
			2, 2,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 6)));
		glEnableVertexAttribArray(2);	// TexCoords
		glBindVertexArray(0);

		// lightSrc
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0)
		);
		glEnableVertexAttribArray(0);	// position
		glBindVertexArray(0);

		// texture
		glGenTextures(2, textures);
		// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int img_height{ 0 }, img_width{ 0 };
		unsigned char* image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img_height = 0; img_width = 0;
		image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

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
		lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);

		deltaTime = glfwGetTime();
	}

	~SpotlightPointLightCasterApp()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(1, VBO);
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
		glActiveTexture(GL_TEXTURE0);	// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);	// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform1i(
			glGetUniformLocation(shader.program, "material.diffuse"),
			0);
		glUniform1i(
			glGetUniformLocation(shader.program, "material.specular"),
			1);
		glUniform1f(
			glGetUniformLocation(shader.program, "material.shininess"),
			0.4f * 128.0f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.position"),
			cameraController.camera->position.x, 
			cameraController.camera->position.y, 
			cameraController.camera->position.z);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.direction"),
			cameraController.camera->front.x, 
			cameraController.camera->front.y, 
			cameraController.camera->front.z);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.cutOff"),
			glm::cos(glm::radians(12.5f)));
		glUniform3f(
			glGetUniformLocation(shader.program, "light.ambient"),
			0.2f, 0.2f, 0.2f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.diffuse"),
			0.5f, 0.5f, 0.5f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.specular"),
			1.0f, 1.0f, 1.0f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.constant"),
			1.0f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.linear"),
			0.045f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.quadratic"),
			0.0075f);
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
		for (const auto& cubePos : cubePositions) {
			model = glm::translate(glm::mat4(), cubePos);
			glUniformMatrix4fv(
				glGetUniformLocation(shader.program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// lightSrc
		// lightSrcShader.use();
		// model = glm::translate(glm::mat4(), lightPos);
		// model = glm::scale(model, glm::vec3(0.2f));
		// glUniform3f(
		// 	glGetUniformLocation(lightSrcShader.program, "lightColor"),
		// 	1.0f, 1.0f, 1.0f);
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "view"),
		// 	1, GL_FALSE, glm::value_ptr(view));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "projection"),
		// 	1, GL_FALSE, glm::value_ptr(projection));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "model"),
		// 	1, GL_FALSE, glm::value_ptr(model));
		// glBindVertexArray(VAO[1]);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);
	}
};

//!	SpotlightSoftPointLightCasterApp Class
///
///	Smooth/Soft edges
///
///	formula:
///		I = (Theta - Rou) / e;
///		e = cos(Phi) - cos(Rou);
///
///		I		- intensity [0.0, 1.0]
///		Theta	- Angle btw LightDir(frag) and SpotDir
///		Phi		- inner cone
///		Rou		- outer cone
///		e		- epsilon
///
class SpotlightSoftPointLightCasterApp : public mylib::App
{
public:
	std::vector<GLfloat> vertices = {
		// Positions			// Normals				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 }, textures[5]{ 0 };
	mylib::Shader lightSrcShader, shader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightPos, lightDir;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	SpotlightSoftPointLightCasterApp(mylib::Window&& wind) : mylib::App(std::move(wind))
	{}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/smooth.spotlight.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/12/12.vertex.glsl",
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
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(
			1, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 3)));
		glEnableVertexAttribArray(1);	// normal
		glVertexAttribPointer(
			2, 2,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET((sizeof(*(vertices.data())) * 6)));
		glEnableVertexAttribArray(2);	// TexCoords
		glBindVertexArray(0);

		// lightSrc
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(*(vertices.data())) * 8,
			ML_BUFFER_OFFSET(0)
		);
		glEnableVertexAttribArray(0);	// position
		glBindVertexArray(0);

		// texture
		glGenTextures(2, textures);
		// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int img_height{ 0 }, img_width{ 0 };
		unsigned char* image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img_height = 0; img_width = 0;
		image = SOIL_load_image(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/11/assets/box_diffuse_map.png",
			&img_width, &img_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,				// minimap level
			GL_RGB,			// storing format
			img_width,
			img_height,
			0,
			GL_RGB,			// input format
			GL_UNSIGNED_BYTE,	// input type
			image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

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
		lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);

		deltaTime = glfwGetTime();
	}

	~SpotlightSoftPointLightCasterApp()
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(1, VBO);
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
		glActiveTexture(GL_TEXTURE0);	// diffuse map
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);	// specular map
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform1i(
			glGetUniformLocation(shader.program, "material.diffuse"),
			0);
		glUniform1i(
			glGetUniformLocation(shader.program, "material.specular"),
			1);
		glUniform1f(
			glGetUniformLocation(shader.program, "material.shininess"),
			0.4f * 128.0f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.position"),
			cameraController.camera->position.x,
			cameraController.camera->position.y,
			cameraController.camera->position.z);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.direction"),
			cameraController.camera->front.x,
			cameraController.camera->front.y,
			cameraController.camera->front.z);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.cutOff"),
			glm::cos(glm::radians(12.5f)));
		glUniform1f(
			glGetUniformLocation(shader.program, "light.outerCutOff"),
			glm::cos(glm::radians(17.5f)));
		glUniform3f(
			glGetUniformLocation(shader.program, "light.ambient"),
			0.2f, 0.2f, 0.2f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.diffuse"),
			0.5f, 0.5f, 0.5f);
		glUniform3f(
			glGetUniformLocation(shader.program, "light.specular"),
			1.0f, 1.0f, 1.0f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.constant"),
			1.0f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.linear"),
			0.045f);
		glUniform1f(
			glGetUniformLocation(shader.program, "light.quadratic"),
			0.0075f);
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
		for (const auto& cubePos : cubePositions) {
			model = glm::translate(glm::mat4(), cubePos);
			glUniformMatrix4fv(
				glGetUniformLocation(shader.program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// lightSrc
		// lightSrcShader.use();
		// model = glm::translate(glm::mat4(), lightPos);
		// model = glm::scale(model, glm::vec3(0.2f));
		// glUniform3f(
		// 	glGetUniformLocation(lightSrcShader.program, "lightColor"),
		// 	1.0f, 1.0f, 1.0f);
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "view"),
		// 	1, GL_FALSE, glm::value_ptr(view));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "projection"),
		// 	1, GL_FALSE, glm::value_ptr(projection));
		// glUniformMatrix4fv(
		// 	glGetUniformLocation(lightSrcShader.program, "model"),
		// 	1, GL_FALSE, glm::value_ptr(model));
		// glBindVertexArray(VAO[1]);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);
	}
};

int main(int argc, char **argv)
{
	try
	{
		mylib::Window window;

		start:
		std::cout << "(1) DirectionalLightCasterApp \n"
			<< "(2) AttenuationPointLightCasterApp \n" 
			<< "(3) SpotlightPointLightCasterApp \n"
			<< "(4) SpotlightSoftPointLightCasterApp \n"
			<< std::endl;
		unsigned int option{ 0 };
		std::cin >> option;

		switch (option)
		{
		case 1: {
			DirectionalLightCasterApp app(std::move(window));
			app.run(app);
			break;
		}
		case 2: {
			AttenuationPointLightCasterApp app(std::move(window));
			app.run(app);
			break;
		}
		case 3: {
			SpotlightPointLightCasterApp app(std::move(window));
			app.run(app);
			break;
		}
		case 4: {
			SpotlightSoftPointLightCasterApp app(std::move(window));
			app.run(app);
			break;
		}
		default:
			std::cout << "select any one of the option please." << std::endl;
			goto start;
			break;
		}
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	return EXIT_SUCCESS;
}
