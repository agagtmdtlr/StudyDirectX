#include "stdafx.h"
#include "StaticMesh.h"
#include "PrimitiveBuffer.h"

#include <filesystem> // c++17
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
StaticMesh::StaticMesh()
	:Mesh()
{
	

}

void StaticMesh::LoadMesh(string path)
{
	LoadBinary(path);
}

void StaticMesh::LoadBinary(string path)
{
	path = "./Content/" + path;

	if (PrimitiveBufferManager::IsExisted(path))
	{
		buffer = PrimitiveBufferManager::RequestBuffer(path);
		return;
	}

	ifstream inputFile(path.c_str(), std::ios::binary);
	char* readbuf = nullptr;
	UINT mSize;
	inputFile.read(reinterpret_cast<char*>(&mSize), sizeof(UINT));

	for (UINT i = 0; i < mSize; i++)
	{
		UINT vSize;
		inputFile.read(reinterpret_cast<char*>(&vSize), sizeof(UINT));
		vector<Vertex> vertices(vSize);
		UINT vdatasize = sizeof(Vertex)* vSize;
		char* vp = reinterpret_cast<char*>(vertices.data());
		inputFile.read(vp, vdatasize);

		UINT iSize;
		inputFile.read(reinterpret_cast<char*>(&iSize), sizeof(UINT));
		vector<UINT> indices(iSize);
		UINT idatasize= sizeof(UINT) * iSize;
		inputFile.read(reinterpret_cast<char*>(indices.data()), idatasize);

		PrimitiveBufferManager::RegistBuffer(path, vertices, indices);
	}

	inputFile.close();
	buffer = PrimitiveBufferManager::RequestBuffer(path);
}

void StaticMesh::LoadAssimp(string path)
{
	Assimp::Importer importer;
	path = "../Resources/Model/" + path;

	if (PrimitiveBufferManager::IsExisted(path))
	{
		buffer = PrimitiveBufferManager::RequestBuffer(path);
		return;
	}

	int flag = 0;
	//flag |= aiProcess_GenSmoothNormals;
	//flag |= aiProcess_GenUVCoords;
	//flag |= aiProcess_GenBoundingBoxes;
	//flag |= aiProcess_JoinIdenticalVertices;
	//flag |= aiProcess_SortByPType;
	flag |= aiProcess_Triangulate; ;
	//aiProcess_FlipWindingOrder |
	//aiProcess_FlipUVs;


// Note: matrix.Transpose();

	const aiScene* scene = importer.ReadFile(path, flag);

	if (scene == nullptr)
	{
		std::cout << importer.GetErrorString() << std::endl;;
		return;
	}

	aabb.Extents = Vector3(0.0f);
	Vector3 extent(0.0);


	if (scene->HasMeshes())
	{
		bool initaabb = false;
		aiAABB aabb;

		for (unsigned int m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* aimesh = scene->mMeshes[m];
			vector<Vertex> vertices(aimesh->mNumVertices);
			//aiVector3D* aimesh->mVertices;

			aiVector3D* aivertices = aimesh->mVertices;
			aiVector3D* ainormals = aimesh->mNormals;
			aiVector3D* aiuvs = nullptr;
			if (aimesh->HasTextureCoords(0))
				aiuvs = aimesh->mTextureCoords[0];

			size_t off = 0;

			for (Vertex& v : vertices)
			{
				memcpy(&v.position.x, aivertices, sizeof(Vector3));
				memcpy(&v.normal.x, ainormals, sizeof(Vector3));
				if (aiuvs != nullptr)
					memcpy(&v.uv, aiuvs, sizeof(Vector2));
				off++;
				if (off <= vertices.size())
				{
					aivertices++;
					ainormals++;
					aiuvs++;
				}

			}

			vector<UINT> indices(aimesh->mNumFaces * 3);
			for (UINT i = 0; i < aimesh->mNumFaces; i++)
			{
				memcpy(indices.data() + (i * 3), aimesh->mFaces[i].mIndices, sizeof(UINT) * 3);
			}

			PrimitiveBufferManager::RegistBuffer(path, vertices, indices);

			if (initaabb == false)
			{
				aabb = aimesh->mAABB;
			}

			aabb.mMax = max(aabb.mMax, aimesh->mAABB.mMax);
			aabb.mMin = min(aabb.mMax, aimesh->mAABB.mMax);

		}

	}



	buffer = PrimitiveBufferManager::RequestBuffer(path);
}

void StaticMesh::Initialize(Transform transform)
{
	Mesh::Initialize(transform);

	

}

