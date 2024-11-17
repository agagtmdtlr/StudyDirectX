#pragma once
#include "Mesh.h"
class StaticMesh :
    public Mesh
{
public:
    StaticMesh();    

    virtual void LoadMesh(string path);
    void LoadBinary(string path);
    void LoadAssimp(string path);

    virtual void Initialize(Transform transform);
    virtual ~StaticMesh(){}

private:
    string assetPath;
};

