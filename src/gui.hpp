#pragma once

#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <algorithm>
#include <string>

#define GRAPH_LEN 100000

extern "C" {
#include "main.h"
}
