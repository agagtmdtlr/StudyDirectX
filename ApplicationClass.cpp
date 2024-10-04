#include "stdafx.h"
#include "ApplicationClass.h"
#include "Mesh.h"

ApplicationClass::ApplicationClass()
{
    sphere.Initialize("sphere", Transform());
}

Mesh* ApplicationClass::GetMesh()
{
    return &sphere;
}
