#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

class StencilTestingApp : public mylib::App
{
public:
	std::vector<GLfloat> cubeVertices = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	std::vector<GLfloat> planeVertices = {
		// Positions        // Texture Coords (note we set these higher 
		// than 1 that together with GL_REPEAT as 
		// texture wrapping mode will cause the 
		// floor texture to repeat)
		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
	};
	GLuint VAO[2]{ 0 }, VBO[2]{ 0 };
	GLuint &cubeVAO = VAO[0], &planeVAO = VAO[1],
		&cubeVBO = VBO[0], &planeVBO = VBO[1];
	mylib::Shader shader, singleColorShader;
	mylib::Shader *activeShader{ nullptr };
	GLuint cubeTex, floorTex;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::mat4 model, view, projection;

	int option{ 0 };

	StencilTestingApp(mylib::Window&& window) : mylib::App(std::move(window))
	{}

	void startup() override
	{
		singleColorShader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/16/16.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/16/16.fragment.glsl",
				GL_FRAGMENT_SHADER)
		});
		shader = mylib::Shader({
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/15/15.vertex.glsl",
				GL_VERTEX_SHADER),
			std::make_pair(
				"C:/Users/Samuel Vishesh Paul/Documents/GitHub/My-Jolly-journey/15/15.fragment.glsl",
				GL_FRAGMENT_SHADER)
		});

		glGenVertexArrays(2, VAO);
		glGenBuffers(2, VBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER,
			cubeVertices.size() * sizeof(*(cubeVertices.data())),
			cubeVertices.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER,
			planeVertices.size() * sizeof(*(planeVertices.data())),
			planeVertices.data(),
			GL_STATIC_DRAW);

		// cube
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			5 * sizeof(*(cubeVertices.data())),
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(*(cubeVertices.data())),
			ML_BUFFER_OFFSET(3 * sizeof(*(cubeVertices.data()))));
		glBindVertexArray(0);
		// plane
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			5 * sizeof(*(planeVertices.data())),
			ML_BUFFER_OFFSET(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(*(planeVertices.data())),
			ML_BUFFER_OFFSET(3 * sizeof(*(planeVertices.data()))));
		glBindVertexArray(0);

		cubeTex = mylib::loadTexture(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/marble.jpg");
		floorTex = mylib::loadTexture(
			"C:/Users/Samuel Vishesh Paul/Documents/GitHub/LearnOpenGL/resources/textures/metal.png");

		camera = mylib::Camera(
			glm::vec3(0.0f, 0.0f, 3.0f),	// cameraPos
			glm::vec3(0.0f, 1.0f, 0.0f));	// cmaeraUp
		cameraController = mylib::CameraController(
			&camera, &window);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	}

	void update(double time) override
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
		activeShader = &singleColorShader;
		activeShader->use();
		glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "view"),
			1, GL_FALSE, glm::value_ptr(view));
	}

	void render() override
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Floor
		//	Draw floor as normal, we only care about the containers. 
		//	The floor should NOT fill the stencil buffer so we set its mask to 0x00
		glStencilMask(0x00);
		activeShader = &shader;
		activeShader->use();
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTex);
		model = glm::translate(glm::mat4(), glm::vec3(0));
		glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
			1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		// clear stencil buffer to 0s and for boxes we update the stencil buffer to 1 for 
		// each fragment drawn
		// // 1st. Render pass, draw objects as normal, filling the stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // All fragments should update the stencil buffer
		glStencilMask(0xFF); // Enable writing to the stencil buffer
		// draw boxes
		{
			activeShader->use();
			glBindVertexArray(cubeVAO);
			glBindTexture(GL_TEXTURE_2D, cubeTex);
			model = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f + 0.0001f, -1.0f));
			glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f + 0.0001f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		// Stencil Render Pass
		//	2nd. Render pass, now draw slightly scaled versions of the objects, 
		//	this time disabling stencil writing.
		//	Because stencil buffer is now filled with several 1s. 
		//	The parts of the buffer that are 1 are now not drawn, thus only drawing 
		//	the objects' size differences, making it look like borders.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);	// disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		// draw border boxes
		{
			activeShader = &singleColorShader;
			activeShader->use();
			glBindVertexArray(cubeVAO);
			glBindTexture(GL_TEXTURE_2D, cubeTex);
			model = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f + 0.0001f, -1.0f));
			model = glm::scale(model, glm::vec3(1.1));
			glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f + 0.0001f, 0.0f));
			model = glm::scale(model, glm::vec3(1.1));
			glUniformMatrix4fv(glGetUniformLocation(activeShader->program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);
	}

private:

};
