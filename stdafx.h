#pragma once

#include <d3d11.h>
#include <glm/glm.hpp>
#include <wrl/client.h>
#include <string>
#include <iostream>
#include "RenderContext.h"

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;