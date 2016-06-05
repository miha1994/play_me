#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

// opengl.. хочу о нем забыть. Не буду комментировать эти функции, может внутри них в самом коде что-нибудь важное закомментирую, но про эти прототипы мне мало что можно сказать
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void RenderSceneCB();
void InitializeGlutCallbacks();
void glut_initialization ();
void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
void CompileShaders();