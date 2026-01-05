#include "glad/glad.h"

// Prevent GLFW from including OpenGL headers
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string.h>

// Function pointers
PFNGLCLEARPROC glClear = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLDISABLEPROC glDisable = NULL;
PFNGLBLENDFUNCPROC glBlendFunc = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLGETERRORPROC glGetError = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;
PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLPIXELSTOREIPROC glPixelStorei = NULL;

int gladLoadGL(void) {
    glClear = (PFNGLCLEARPROC)glfwGetProcAddress("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)glfwGetProcAddress("glClearColor");
    glEnable = (PFNGLENABLEPROC)glfwGetProcAddress("glEnable");
    glDisable = (PFNGLDISABLEPROC)glfwGetProcAddress("glDisable");
    glBlendFunc = (PFNGLBLENDFUNCPROC)glfwGetProcAddress("glBlendFunc");
    glViewport = (PFNGLVIEWPORTPROC)glfwGetProcAddress("glViewport");
    glGetString = (PFNGLGETSTRINGPROC)glfwGetProcAddress("glGetString");
    glGetError = (PFNGLGETERRORPROC)glfwGetProcAddress("glGetError");
    glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glfwGetProcAddress("glDeleteBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfwGetProcAddress("glDeleteVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glfwGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glfwGetProcAddress("glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glfwGetProcAddress("glDeleteProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glfwGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glfwGetProcAddress("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)glfwGetProcAddress("glUniform1i");
    glUniform1f = (PFNGLUNIFORM1FPROC)glfwGetProcAddress("glUniform1f");
    glUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)glfwGetProcAddress("glUniform4f");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");
    glGenTextures = (PFNGLGENTEXTURESPROC)glfwGetProcAddress("glGenTextures");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)glfwGetProcAddress("glDeleteTextures");
    glBindTexture = (PFNGLBINDTEXTUREPROC)glfwGetProcAddress("glBindTexture");
    glTexImage2D = (PFNGLTEXIMAGE2DPROC)glfwGetProcAddress("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)glfwGetProcAddress("glTexParameteri");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)glfwGetProcAddress("glActiveTexture");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)glfwGetProcAddress("glDrawArrays");
    glPixelStorei = (PFNGLPIXELSTOREIPROC)glfwGetProcAddress("glPixelStorei");
    
    return 1;
}
