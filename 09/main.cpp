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
#include <epoxy\wgl.h>
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

/*
*   ## Phong Lighting Model

	- Ambient lighting: a distant light) so objects are almost never completely dark.
		To simulate this we use an ambient lighting constant that always
		gives the object some color.

	- Diffuse lighting: simulates the directional impact a light object has on an object.
		The more a part of an object faces the light source, the brighter it becomes.

	- Specular lighting: simulates the bright spot of a light that appears on shiny objects.
		Specular highlights are often more inclined to the color of the light
		than the color of the object.

*/

std::vector<GLfloat> vertices_box = {
	0.250000f, -0.250000f, -0.250000f,  // 0,
	0.250000f, -0.250000f, 0.250000f,   // 1
	-0.250000f, -0.250000f, 0.250000f,  // 2
	-0.250000f, -0.250000f, -0.250000f, // 3
	0.250000f, 0.250000f, -0.250000f,   // 4
	0.250000f, 0.250000f, 0.250000f,    // 5
	-0.250000f, 0.250000f, 0.250000f,   // 6
	-0.250000f, 0.250000f, -0.250000f,  // 7
};
GLfloat vertices_lightbox[24];

//std::vector<GLfloat> vertices = {
//	// bottom face
//	0.250000f, -0.250000f, -0.250000f,  // 0
//	0.250000f, -0.250000f, 0.250000f,   // 1
//	-0.250000f, -0.250000f, 0.250000f,  // 2
//	0.250000f, -0.250000f, -0.250000f,  // 0
//	-0.250000f, -0.250000f, 0.250000f,  // 2
//	-0.250000f, -0.250000f, -0.250000f, // 3
//
//	// right face
//	0.250000f, -0.250000f, -0.250000f,  // 0
//	0.250000f, -0.250000f, 0.250000f,   // 1
//	0.250000f, 0.250000f, 0.250000f,    // 5
//	0.250000f, -0.250000f, -0.250000f,  // 0
//	0.250000f, 0.250000f, 0.250000f,    // 5
//	0.250000f, 0.250000f, -0.250000f,   // 4
//
//	// back face
//	0.250000f, -0.250000f, -0.250000f,  // 0
//	0.250000f, 0.250000f, -0.250000f,   // 4
//	-0.250000f, -0.250000f, -0.250000f, // 3
//	0.250000f, 0.250000f, -0.250000f,   // 4
//	-0.250000f, 0.250000f, -0.250000f,  // 7
//	-0.250000f, -0.250000f, -0.250000f, // 3
//
//	//left face 
//	-0.250000f, -0.250000f, -0.250000f, // 3
//	-0.250000f, -0.250000f, 0.250000f,  // 2
//	-0.250000f, 0.250000f, 0.250000f,   // 6
//	-0.250000f, -0.250000f, -0.250000f, // 3
//	-0.250000f, 0.250000f, 0.250000f,   // 6
//	-0.250000f, 0.250000f, -0.250000f,  // 7
//
//	// front face
//	0.250000f, -0.250000f, 0.250000f,   // 1
//	-0.250000f, -0.250000f, 0.250000f,  // 2
//	-0.250000f, 0.250000f, 0.250000f,   // 6
//	-0.250000f, 0.250000f, 0.250000f,   // 6
//	0.250000f, 0.250000f, 0.250000f,    // 5
//	0.250000f, -0.250000f, 0.250000f,   // 1
//
//	// top face
//	-0.250000f, 0.250000f, 0.250000f,   // 6
//	-0.250000f, 0.250000f, -0.250000f,  // 7
//	0.250000f, 0.250000f, 0.250000f,    // 5
//	-0.250000f, 0.250000f, -0.250000f,  // 7
//	0.250000f, 0.250000f, -0.250000f,   // 4
//	0.250000f, 0.250000f, 0.250000f,    // 5
//};

std::vector<GLuint> indices = {
	0, 1, 2, // bottom face
	0, 2, 3,
	0, 1, 5, // right face
	0, 5, 4,
	0, 4, 3, // back face
	4, 7, 3,
	3, 2, 6, // left face
	3, 6, 7,
	1, 2, 6, // front face
	6, 5, 1,
	6, 7, 5, // top face
	7, 4, 5,
};

class myApp : public mylib::App
{
public:
	myApp(mylib::Window &&window) : mylib::App(std::move(window)) {}

	void startup() override
	{
		lightingShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/09.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				//"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/ambientLighting.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/09.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				//"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/lightSrc.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});

		glGenVertexArrays(1, VAO);
		glGenBuffers(1, VBO);
		glGenBuffers(1, EBO);
		boxVAO = VAO[0];

		lightPos = glm::vec3(1.f, 1.f, -1.5f);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // box
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(*(vertices_box.data())) * vertices_box.size(), vertices_box.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(*(indices.data())) * indices.size(), indices.data(),
			GL_STATIC_DRAW);

		glBindVertexArray(boxVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		objectColorLoc = glGetUniformLocation(lightingShader.program, "objectColor");
		lightColorLoc = glGetUniformLocation(lightingShader.program, "lightColor");

		camera = mylib::FPSCamera(
			glm::vec3(0.0f, 0.0f, 3.0f), // cameraPos
			glm::vec3(0.0f, 1.0f, 0.0f));
		cameraController = mylib::CameraController(
			&camera, // worldUp
			&window);

		glEnable(GL_DEPTH_TEST);
	}

	~myApp()
	{
	}

	void update(double time) override
	{
		cameraController.update(cameraController, time);

		view = cameraController.camera->getViewMatrix();
		projection = glm::perspective(
			45.0f,
			static_cast<GLfloat>(window.width) / window.height,
			0.1f, 100.0f);
		
	}

	void render() override
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(
			glGetUniformLocation(lightingShader.program, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(
			glGetUniformLocation(lightingShader.program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(
			glGetUniformLocation(lightingShader.program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(boxVAO);
		glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

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
		glBindVertexArray(boxVAO);
		glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

protected:
	GLuint VAO[5]{ 0 }, VBO[5]{ 0 }, EBO[5]{ 0 };
	mylib::Shader lightingShader, lightSrcShader;

	glm::vec3 lightPos;

	GLuint boxVAO{ 0 };
	GLint objectColorLoc{ 0 }, lightColorLoc{ 0 };

	mylib::FPSCamera camera;
	mylib::CameraController cameraController;

	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };

private:
};

class BasicLighting : public mylib::App
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

	BasicLighting(mylib::Window&& wind) :
		mylib::App(std::move(wind))
	{
	}

	void startup() override
	{
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/09.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				//"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/ambientLighting.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/09/09.vertex.glsl",
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
		glUniform3f(
			glGetUniformLocation(shader.program, "lightPos"),
			lightPos.x, lightPos.y, lightPos.z);
		model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		glUniform3f(
			glGetUniformLocation(shader.program, "objectColor"),
			1.0f, 0.5f, 0.31f);
		glUniform3f(
			glGetUniformLocation(shader.program, "lightColor"),
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

};

int main(const int argc, const char *const argv[])
{
	GLfloat* arr = new GLfloat[5];
	std::cout << "\n[0]: " << sizeof(arr)
		<< "\n[1]: " << (2 * sizeof(arr))
		<< "\n[2]: " << (GLvoid*)(3 * sizeof(arr));
	std::cout << "\n\nfun: \n" << "\n[0]: " << ML_BUFFER_OFFSET(sizeof(GLfloat*) * 1)
		<< "\n[1]: " << ML_BUFFER_OFFSET(sizeof(GLfloat) * 2)
		<< "\n[2]: " << static_cast<const GLvoid*>(static_cast<char*>(0) + (sizeof(GLfloat) * 2));

	std::vector<GLfloat> fv(10);
	GLfloat *fa = new GLfloat[10];

	std::cout << "\nfv: " << ML_BUFFER_OFFSET(sizeof(*fv.data()))
		<< "\nfa: " << ML_BUFFER_OFFSET(sizeof(*fa))
		<< std::endl;
	assert(ML_BUFFER_OFFSET(sizeof(*fv.data()) * 2) == ML_BUFFER_OFFSET(sizeof(*fa) * 2),
		"Damn Son... my Ass erts");

	delete[] arr, fa;
	
	try
	{
		mylib::Window wind;
		BasicLighting app(std::move(wind));
		app.run(app);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cin.get();
		std::exit(EXIT_FAILURE);
	}

	std::cin.get();
	std::exit(EXIT_SUCCESS);
}
