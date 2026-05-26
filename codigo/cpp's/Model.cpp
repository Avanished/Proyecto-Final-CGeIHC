#include "Model.h"
#include <unordered_map>
#include <cstdio>

Model::Model()
{
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		fileName,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices
	);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		printf("Fallo al cargar el modelo: %s | %s\n", fileName.c_str(), importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::ClearModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;
		}
	}

	for (unsigned int i = 0; i < TextureList.size(); i++)
	{
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}

	MeshList.clear();
	TextureList.clear();
	meshTotex.clear();
}

void Model::RenderModel()
{
	for (unsigned int i = 0; i < MeshList.size(); i++)
	{
		unsigned int materialIndex = 0;

		if (i < meshTotex.size())
		{
			materialIndex = meshTotex[i];
		}

		if (materialIndex < TextureList.size() && TextureList[materialIndex] != nullptr)
		{
			TextureList[materialIndex]->UseTexture();
		}

		if (MeshList[i] != nullptr)
		{
			MeshList[i]->RenderMesh();
		}
	}
}

Model::~Model()
{
	ClearModel();
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	if (!node || !scene) return;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		if (node->mMeshes[i] < scene->mNumMeshes)
		{
			LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	if (!mesh) return;
	if (mesh->mNumVertices == 0) return;
	if (mesh->mNumFaces == 0) return;

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
			});

		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), {
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
				});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		if (mesh->mNormals)
		{
			vertices.insert(vertices.end(), {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
				});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 1.0f, 0.0f });
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (vertices.empty() || indices.empty()) return;

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	meshTotex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	if (!scene) return;

	std::unordered_map<std::string, Texture*> loadedTextures;
	TextureList.resize(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		TextureList[i] = nullptr;

		if (material && material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = path.C_Str();
				size_t slashPos = fullPath.find_last_of("/\\");
				std::string filename = (slashPos == std::string::npos) ? fullPath : fullPath.substr(slashPos + 1);

				std::string texPath = std::string("Textures/") + filename;

				auto it = loadedTextures.find(texPath);
				if (it != loadedTextures.end())
				{
					TextureList[i] = it->second;
				}
				else
				{
					Texture* newTex = new Texture(texPath.c_str());

					std::string ext = "";
					size_t dotPos = filename.find_last_of('.');
					if (dotPos != std::string::npos)
					{
						ext = filename.substr(dotPos + 1);
					}

					bool loaded = false;
					if (ext == "tga" || ext == "png" || ext == "TGA" || ext == "PNG")
					{
						loaded = newTex->LoadTextureA();
					}
					else
					{
						loaded = newTex->LoadTexture();
					}

					if (loaded)
					{
						TextureList[i] = newTex;
						loadedTextures[texPath] = newTex;
					}
					else
					{
						printf("Fallo en cargar la textura: %s\n", texPath.c_str());
						delete newTex;
						newTex = nullptr;
					}
				}
			}
		}

		if (!TextureList[i])
		{
			TextureList[i] = new Texture("Textures/plain.png");
			if (!TextureList[i]->LoadTextureA())
			{
				printf("Fallo en cargar la textura por defecto: Textures/plain.png\n");
				delete TextureList[i];
				TextureList[i] = nullptr;
			}
		}
	}
}