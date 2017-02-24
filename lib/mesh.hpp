#ifndef __MYLIB_MESH_HPP__
#define __MYLIB_MESH_HPP__

#ifdef _DEBUG
#undef _NDEBUG
#undef NDEBUG
#else
#define _NDEBUG
#define NDEBUG
#endif

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <epoxy/gl.h>
#ifdef _WIN32
#include <epoxy/wgl.h>
#else
#include <epoxy/glx.h>
#endif

#include <glm/glm.hpp>

#include "mylib.hpp"

namespace mylib
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct Texture
	{
		GLuint id;
		std::string type;
		aiString path;
	};

	class Mesh
	{
	public:
		//! mesh data
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Mesh(
			std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			std::vector<Texture> textures) :
			vertices(vertices),
			indices(indices),
			textures(textures)
		{
			this->setupMesh();
		}

		//! draw() renders the mesh onto app window
		///
		///	assumes sampler2D naming conventions as follows:
		///		diffuse textures:	texture_diffuseN
		///		speculat textures:	texture_specularN
		///		where N: [0, MAX_TEXTURE_UNITS]
		///
		void draw(mylib::Shader& shader)
		{
			GLuint diffuseNr{ 1 }, specularNr{ 1 };
			for (GLuint i = 0; i < textures.size(); ++i) {
				glActiveTexture(GL_TEXTURE0 + i);		// activate *_textureN
				std::string number;						// *_texture<"N">
				std::string name = textures[i].type;	// <"*_texture">N
				if (name == "texture_diffuse") {
					number = std::to_string(diffuseNr++);
				}
				else if (name == "texture_specular") {
					number = std::to_string(specularNr++);
				}

				glUniform1f(
					glGetUniformLocation(
						shader.program,
						// material.<"*_textureN">
						("material." + name + number).c_str()),
					i);
#ifdef _DEBUG
				//std::clog << "texture: " << ("material." + name + number).c_str()
					//<< std::endl;
#endif // _DEBUG

				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			glActiveTexture(GL_TEXTURE0);

			// draw
			shader.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES,
				indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		//! render data
		GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };

		void setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER,
				vertices.size() * sizeof(*(vertices.data())),
				vertices.data(),
				GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				indices.size() * sizeof(*(indices.data())),
				indices.data(),
				GL_STATIC_DRAW);

			// Vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3,
				GL_FLOAT, GL_FALSE, sizeof(Vertex), ML_BUFFER_OFFSET(0));
			// Vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3,
				GL_FLOAT, GL_FALSE, sizeof(Vertex),
				ML_BUFFER_OFFSET(offsetof(Vertex, normal)));
			// Vertex texture coordinates
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2,
				GL_FLOAT, GL_FALSE, sizeof(Vertex),
				ML_BUFFER_OFFSET(offsetof(Vertex, texCoords)));
			glBindVertexArray(0);
		}
	};

}; // namespace mylib

#endif
