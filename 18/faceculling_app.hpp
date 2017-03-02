#include "c:/Users/samuel vishesh paul/documents/visual studio 2015/Projects/OpenGL Playground/OpenGL Playground/stdafx.h"

class FaceCullingApp :
	public mylib::App
{
public:
	static std::vector<GLfloat> CCW_vertices,
		CW_vertices;

	GLuint VBO[5], VAO[5];
	GLuint &cw_VBO = VBO[0], &cw_VAO = VAO[0],
		&counterCw_VBO = VBO[1], &counterCw_VAO = VAO[1];

	glm::mat4 model, view, projection;
	mylib::Shader shader, *activeShader{ nullptr };
	mylib::CameraController cameraController;
	mylib::Camera camera;

	FaceCullingApp(mylib::Window&&);
	~FaceCullingApp();

private:

	void startup() override;
	void update(double) override;
	void render() override;
	void shutdown() override;
};
