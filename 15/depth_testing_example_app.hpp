#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

class DepthTestingApp : public mylib::App
{
public:
	static std::vector<GLfloat> cubeVertices;
	static std::vector<GLfloat> planeVertices;
	GLuint VAO[2]{ 0 }, VBO[2]{ 0 };
	GLuint &cubeVAO = VAO[0], &planeVAO = VAO[1],
		&cubeVBO = VBO[0], &planeVBO = VBO[1];
	mylib::Shader shader, visualizeDepthShader;
	mylib::Shader *activeShader{ nullptr };
	GLuint cubeTex, floorTex;
	mylib::Camera camera;
	mylib::CameraController cameraController;
	glm::mat4 model, view, projection;

	int option{ 0 };

	DepthTestingApp(mylib::Window&& window);

	void startup() override;

	void update(double time) override;

	void render() override;

	void shutdown() override;
private:

};


