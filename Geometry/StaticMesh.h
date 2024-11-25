#pragma once
#include "Mesh.h"
#include <filesystem> // c++17

class StaticMesh :
    public Mesh
{
public:
    StaticMesh();    

    virtual void LoadMesh(string path);

    void LoadMaterial(std::filesystem::path path);
    void LoadBinary(string path);
    void LoadAssimp(string path);

    virtual void Initialize(Transform transform);
    virtual ~StaticMesh(){}

private:
    string assetPath;
};

