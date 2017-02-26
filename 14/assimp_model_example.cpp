#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

class AssimpModelTestApp : public mylib::App
{
public:
	mylib::Shader shader, lightSrcShader;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::vec3 lightDir;
	glm::mat4 model{ 0 }, view{ 0 }, projection{ 0 };
	glm::vec3 pointLightPosition;

	struct lightColor
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} spotLight;
	bool isSpotlightOn{ false };
	enum KEY : BYTE {
		pressed,
		released,
		tapped
	} key_p, key_space;
	int key_status{ 0 };

	GLuint lightSrcVAO{ 0 }, lightSrcVBO{ 0 };
	std::vector<GLfloat> lightSrcVertices = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	mylib::Model *asset_model;

	volatile bool spacePressed{ false };
	float deltaTime{ 0 };

	AssimpModelTestApp(mylib::Window&& window) :
		mylib::App(std::move(window))
	{
		
	}

	~AssimpModelTestApp()
	{
		delete this->asset_model;
	}

	void startup() override
	{
		glGenVertexArrays(1, &lightSrcVAO);
		glGenBuffers(1, &lightSrcVBO);
		glBindBuffer(GL_ARRAY_BUFFER, lightSrcVBO);
		glBufferData(GL_ARRAY_BUFFER,
			lightSrcVertices.size() * sizeof(*(lightSrcVertices.data())),
			lightSrcVertices.data(),
			GL_STATIC_DRAW);
		glBindVertexArray(lightSrcVAO);
		glVertexAttribPointer(0, 3,
			GL_FLOAT, GL_FALSE,
			3 * sizeof(*(lightSrcVertices.data())),
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/14/14.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/14/14.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});
		lightSrcShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/14/14.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				//"/home/sam/workspace/cpp/ogl_playground/07/07.fragment.glsl",
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/13/lightsrc.fragment.glsl",
				GL_FRAGMENT_SHADER),
		});

		asset_model = new mylib::Model("C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/14/assets/nanosuit/nanosuit.obj");

		camera = mylib::Camera(
			glm::vec3(0.0f, 0.0f, 3.0f),	// cameraPos
			glm::vec3(0.0f, 1.0f, 0.0f));	// cmaeraUp
		cameraController = mylib::CameraController(
			&camera, &window);

		pointLightPosition = glm::vec3(1.0f, -1.0f, 2.0f);

		key_p = KEY::released;
		key_space = KEY::released;

		deltaTime = glfwGetTime();

		glEnable(GL_DEPTH_TEST);
	}

	void update(double time) override
	{
		deltaTime -= time;

		// key p 
		key_status = glfwGetKey(window.getWindow(), GLFW_KEY_P);
		if (key_status == GLFW_RELEASE && key_p == KEY::pressed)
			key_p = KEY::tapped;
		else if (key_status == GLFW_RELEASE && key_p != KEY::pressed)
			key_p = KEY::released;
		else if (key_status == GLFW_PRESS)
			key_p = KEY::pressed;
		// key space
		key_status = glfwGetKey(window.getWindow(), GLFW_KEY_SPACE);
		if (key_status == GLFW_RELEASE && key_space == KEY::pressed)
			key_space = KEY::tapped;
		else if (key_status == GLFW_RELEASE && key_space != KEY::pressed)
			key_space = KEY::released;
		else if (key_status == GLFW_PRESS)
			key_space = KEY::pressed;

		// isSpotlightOn
		if (key_p == KEY::tapped)
			isSpotlightOn = !isSpotlightOn;

		// the Code below is responsible for updating light pos from keyboard keystrokes lol
		// read along n follow tanya :P

		// light Src move
		if (key_space != KEY::pressed) {
			cameraController.update(cameraController, time);
		}
		else if (key_space == KEY::pressed) {
			// W - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
				pointLightPosition -= cameraController.camera->front * deltaTime * mylib::SPEED;
			}
			// S - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
				pointLightPosition += cameraController.camera->front * deltaTime * mylib::SPEED;
			}
			// A - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
				pointLightPosition += cameraController.camera->right * deltaTime * mylib::SPEED;
			}
			// D - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
				pointLightPosition -= cameraController.camera->right * deltaTime * mylib::SPEED;
			}
			// Up - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
				pointLightPosition.y -= deltaTime * mylib::SPEED;
			}
			// Down - key press
			if (glfwGetKey(window.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
				pointLightPosition.y += deltaTime * mylib::SPEED;
			}
		}


		if (isSpotlightOn) {
			spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			spotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else {
			spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
			spotLight.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
			spotLight.specular = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		view = cameraController.camera->getViewMatrix();
		projection = glm::perspective(
			camera.zoom,
			static_cast<GLfloat>(window.width) / window.height,
			0.1f, 100.0f);

		deltaTime = time;
	}

	void render() override
	{
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the loaded model
		shader.use();
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// camera pos or view pos
		glUniform3f(
			glGetUniformLocation(shader.program, "viewPos"),
			cameraController.camera->position.x,
			cameraController.camera->position.y,
			cameraController.camera->position.z);
		// material shininess
		glUniform1f(glGetUniformLocation(shader.program, "material.shininess"),
			0.4f * 128.0f);
		// direction light caster
		glUniform3f(glGetUniformLocation(shader.program, "dirLight.direction"),
			-0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(shader.program, "dirLight.ambient"),
			0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.program, "dirLight.diffuse"),
			0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(shader.program, "dirLight.specular"),
			0.5f, 0.5f, 0.5f);
		// point light caster
		glUniform3f(glGetUniformLocation(shader.program, "pointLight.position"),
			pointLightPosition.x,
			pointLightPosition.y,
			pointLightPosition.z);
		glUniform3f(glGetUniformLocation(shader.program, "pointLight.ambient"),
			// 0.05f, 0.05f, 0.05f);
			(225.0f / 225.0f) * 0.05f,
			(204.0f / 225.0f) * 0.05f,
			(0.0f / 225.0f) * 0.05f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLight.diffuse"),
			// 0.8f, 0.8f, 0.8f);
			(225.0f / 225.0f) * 0.8f,
			(204.0f / 225.0f) * 0.8f,
			(0.0f / 225.0f) * 0.8f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLight.specular"),
			// 1.0f, 1.0f, 1.0f);
			225.0f / 225.0f,
			204.0f / 225.0f,
			0.0f / 225.0f);

		glUniform1f(glGetUniformLocation(shader.program, "pointLight.constant"),
			1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLight.linear"),
			0.09);
		glUniform1f(glGetUniformLocation(shader.program, "pointLight.quadratic"),
			0.032);
		// spotlight caster
		glUniform3f(glGetUniformLocation(shader.program, "spotLight.position"),
			camera.position.x,
			camera.position.y,
			camera.position.z);
		glUniform3f(glGetUniformLocation(shader.program, "spotLight.direction"),
			camera.front.x,
			camera.front.y,
			camera.front.z);
		glUniform1f(glGetUniformLocation(shader.program, "spotLight.cutOff"),
			glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(shader.program, "spotLight.outerCutOff"),
			glm::cos(glm::radians(15.0f)));
		glUniform3f(glGetUniformLocation(shader.program, "spotLight.ambient"),
			spotLight.ambient.x, 
			spotLight.ambient.y,
			spotLight.ambient.z);
		glUniform3f(glGetUniformLocation(shader.program, "spotLight.diffuse"),
			spotLight.diffuse.x,
			spotLight.diffuse.y,
			spotLight.diffuse.z);
		glUniform3f(glGetUniformLocation(shader.program, "spotLight.specular"),
			spotLight.specular.x,
			spotLight.specular.y,
			spotLight.specular.z);
		glUniform1f(glGetUniformLocation(shader.program, "spotLight.constant"),
			1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "spotLight.linear"),
			0.09);
		glUniform1f(glGetUniformLocation(shader.program, "spotLight.quadratic"),
			0.032);
		model = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.75f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		asset_model->draw(shader);

		// draw light scr
		// Light Src
		lightSrcShader.use();
		glUniform3f(
			glGetUniformLocation(lightSrcShader.program, "lightColor"),
			//1.0f, 1.0f, 1.0f);
			225.0f / 225.0f,
			204.0f / 225.0f,
			0.0f / 225.0f);
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "view"),
			1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(lightSrcVAO);
		model = glm::translate(glm::mat4(), pointLightPosition);
		model = glm::scale(model, glm::vec3(0.2));
		glUniformMatrix4fv(
			glGetUniformLocation(lightSrcShader.program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	void shutdown() override
	{

	}

private:

};
