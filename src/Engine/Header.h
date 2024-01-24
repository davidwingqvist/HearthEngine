#pragma once
#include <recs_base.h>

// DirectXTK
#include <SimpleMath.h>
#include <BufferHelpers.h>
#include <Keyboard.h>
#include <Mouse.h>

// STANDARD LIBRARY
#include <memory>
#include <time.h>
#include <string>
#include <functional>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

// DirectX
#include <d3d11.h>
#include <d2d1.h>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Lua
#include <lua.hpp>
#pragma comment(lib, "liblua53.a")

// ImGUI
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// Helper classes
#include "DXPointer.h"

namespace dx = DirectX;
namespace sm = DirectX::SimpleMath;

#include "Structs.h"

const std::string UIPATH = "Assets/Graphics2D/";
const std::string MODELPATH = "Assets/Models/";
const std::string SCRIPTPATH = "Scripts/";
const std::string TEXTUREPATH = "Assets/Textures/";