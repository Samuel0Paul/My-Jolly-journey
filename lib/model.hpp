#ifndef __MYLIB_MODEL_HPP__
#define __MYLIB_MODEL_HPP__

#include "c:\Users\samuel vishesh paul\documents\visual studio 2015\Projects\OpenGL Playground\OpenGL Playground\stdafx.h"

namespace mylib
{
	static GLint TextureFromFile(const char* path, std::string directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(),
			&width, &height, 0, SOIL_LOAD_RGB);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);

		return textureID;
	}

	class Model
	{
	public:

		Model() {}
		Model(const GLchar* path)
		{
			loadModel(path);
		}

		void draw(Shader shader)
		{
			for (GLuint i = 0; i < meshes.size(); ++i) {
				meshes[i].draw(shader);
			}
		}

	private:
		//! model data
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> textures_loaded;

		void loadModel(std::string path)
		{
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, 
				aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				std::cerr << "ERROR: ASSIMP: " << __PRETTY_FUNCTION__
					<< "\n" << import.GetErrorString() << std::endl;
				throw std::runtime_error(
					std::string("ERROR::ASSIMP::") + import.GetErrorString());
			}
			directory = path.substr(0, path.find_last_of('/'));
			processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode* node, const aiScene* scene)
		{
			// for all the node's meshes (if any)
			for (GLuint j = 0; j < node->mNumMeshes; ++j) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[j]];
				this->meshes.push_back(processMesh(mesh, scene));
			}
			// for all it's children
			for (GLuint i = 0; i < node->mNumChildren; ++i) {
				processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			std::vector<GLuint> indices;
			std::vector<Texture> textures;

			// process and extract Vertices, Normals and texture Coords
			for (GLuint i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;

				glm::vec3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;

				if (mesh->HasNormals()) {
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.normal = vector;
				}

				if (mesh->mTextureCoords[0]) {
					glm::vec2 coords;
					coords.x = mesh->mTextureCoords[0][i].x;
					coords.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoords = coords;
				}
				else {
					vertex.texCoords = glm::vec2(0.0f, 0.0f);
				}

				vertices.push_back(vertex);
			}
			// process and extract indices
			for (GLuint i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// process and extract materials
			if (mesh->mMaterialIndex >= 0) {
				// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
				// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
				// Same applies to other texture as the following list summarizes:
				// Diffuse: texture_diffuseN
				// Specular: texture_specularN
				// Normal: texture_normalN
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				std::vector<Texture> diffuseMaps = this->loadMaterialTextures(
					material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

				std::vector<Texture> specularMaps = this->loadMaterialTextures(
					material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
			aiTextureType type, std::string typeName)
		{
			std::vector<Texture> textures;
			for (GLuint i = 0; i < mat->GetTextureCount(type); ++i) {
				aiString str;
				mat->GetTexture(type, i, &str);
				bool skip = false;
				for (GLuint j = 0; j < textures_loaded.size(); ++j) {
					if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
						textures.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip) {
					Texture texture;
					texture.id = TextureFromFile(str.C_Str(), this->directory);
					texture.type = typeName;
					texture.path = str;
					textures.push_back(texture);
					this->textures_loaded.push_back(texture);
				}
			}

			return textures;
		}
	};

}

#endif
