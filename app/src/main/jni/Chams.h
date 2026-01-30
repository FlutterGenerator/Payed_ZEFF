#ifndef ZEFF_CHAMS
#define ZEFF_CHAMS
#include <dlfcn.h>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include "OpenGLdefine.h"
using namespace std;
static void *handle;
static int w = 0;
std::vector<std::string> shaders;
std::string mineShader = "unity_SHC";

int chamsint = 0;

ImVec4 inWallColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);                       
ImVec4 visibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

//GL pointers
void (*p_glGetIntegerv)(unsigned int, int *);
void (*p_glGetBooleanv)(unsigned int, unsigned char *);
void (*p_glDepthRangef)(float, float);
void (*p_glBlendColor)(float, float, float, float);
void (*p_glEnable)(unsigned int);
void (*p_glLineWidth)(float);
void (*p_glBlendFunc)(unsigned int, unsigned int);
void (*p_glDisable)(unsigned int);
void (*p_glCullFace)(unsigned int);

const char *isDumpLocation = ("/sdcard/Download/ShaderDump/Shaders.txt");

void int_to_char(int num, char *result) {
    int temp = num;
    int len = 0;
    while (temp > 0) {
        len++;
        temp /= 10;
    }
    for (int i = len - 1; i >= 0; i--) {
        result[i] = num % 10 + '0';
        num /= 10;
    }
    result[len] = '\0';
}

string isDump(const char *name)
{
    vector<string> isShaders = {name};

    static string isTotalShaders;

    for (const auto &isAddTo: isShaders)
        isTotalShaders += (isAddTo + "\n"); // Adding whatever is added to the string vector to isTotalShaders, with a new line added per shader.

    return isTotalShaders.c_str();
}

int (*old_glGetUniformLocation)(unsigned int, const char *);
int new_glGetUniformLocation(unsigned int program, const char *name) {
    bool isExist = false;
    std::string thisShader = {name};
    for (std::string shader : shaders) {
        if (shader.c_str() == thisShader) isExist = true;
    }
    if (!isExist) shaders.push_back({name});
    return old_glGetUniformLocation(program, name);
}

void (*old_glDrawElements)(unsigned int, int, unsigned int, const void *);
void new_glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices) {
	if (mode != GL_TRIANGLES) return old_glDrawElements(mode, count, type, indices);
	if (count < 2000) return old_glDrawElements(mode, count, type, indices);
    
    unsigned char blend;
    p_glGetBooleanv(GL_BLEND, &blend);
    if (blend) return old_glDrawElements(mode, count, type, indices);
    
    unsigned char depthTest;
    p_glGetBooleanv(GL_DEPTH_TEST, &depthTest);
    if (!depthTest) return old_glDrawElements(mode, count, type, indices);
    
    int currProgram;
    p_glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    const char* currentShader = mineShader.c_str();
    int shader = old_glGetUniformLocation(currProgram, currentShader);
    if (shader == -1) return old_glDrawElements(mode, count, type, indices);
    
    {
		if (chamsint == 0) return old_glDrawElements(mode, count, type, indices);
		
        if (chamsint == 1) {
            p_glDepthRangef(1, 0.5);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 2) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 3) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glLineWidth(1);
            old_glDrawElements(GL_LINES, count, type, indices);
        }

        if (chamsint == 4) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0);
            p_glLineWidth(10);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glBlendColor(0, 0, 0, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            old_glDrawElements(GL_LINES, count, type, indices);
        }

        if (chamsint == 5) {
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glLineWidth(10);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glDepthRangef(0.5, 1);
            old_glDrawElements(GL_LINES, count, type, indices);
            p_glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
            p_glBlendColor(0, 0, 0, 1);
            p_glDepthRangef(1, 0.5);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 6) {
            p_glDisable(GL_DEPTH_TEST);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            p_glBlendColor(inWallColor.x, inWallColor.y, inWallColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glEnable(GL_DEPTH_TEST);
            p_glDepthRangef(0.5, 1);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 7) {
            p_glDisable(GL_DEPTH_TEST);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            p_glBlendColor(inWallColor.x, inWallColor.y, inWallColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glEnable(GL_DEPTH_TEST);
            p_glDepthRangef(0.5, 1);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glLineWidth(10);
            old_glDrawElements(GL_LINES, count, type, indices);
            p_glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
            p_glDepthRangef(0.49, 1);
            p_glBlendColor(0, 0, 0, 1);
            old_glDrawElements(mode, count, type, indices);
        }
    }
		p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        p_glLineWidth(1);
        p_glBlendColor(0, 0, 0, 1);
        p_glDepthRangef(0, 1);
		return p_glDisable(GL_BLEND);
}

void RenderHook(){
	
	DobbyHook(reinterpret_cast<void*>((const void*(*)(...))dlsym(handle, OBFUSCATE("glGetUniformLocation"))), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    
    DobbyHook(reinterpret_cast<void*>((const void*(*)(...))dlsym(handle, OBFUSCATE("glDrawElements"))), reinterpret_cast<void*>(new_glDrawElements), reinterpret_cast<void**>(&old_glDrawElements));

	p_glGetIntegerv = (void (*)(unsigned int, int *))((const void*(*)(...))dlsym(handle, OBFUSCATE("glGetIntegerv")));
	
	p_glGetBooleanv = (void (*)(unsigned int, unsigned char *))((const void*(*)(...))dlsym(handle, OBFUSCATE("glGetBooleanv")));
	
	p_glDepthRangef = (void (*)(float, float))((const void*(*)(...))dlsym(handle, OBFUSCATE("glDepthRangef")));
	
	p_glBlendColor = (void (*)(float, float, float, float))((const void*(*)(...))dlsym(handle, OBFUSCATE("glBlendColor")));
	
	p_glEnable = (void (*)(unsigned int))((const void*(*)(...))dlsym(handle, OBFUSCATE("glEnable")));
	
	p_glLineWidth = (void (*)(float))((const void*(*)(...))dlsym(handle, OBFUSCATE("glLineWidth")));
	
	p_glBlendFunc = (void (*)(unsigned int, unsigned int))((const void*(*)(...))dlsym(handle, OBFUSCATE("glBlendFunc")));
	
	p_glDisable = (void (*)(unsigned int))((const void*(*)(...))dlsym(handle, OBFUSCATE("glDisable")));
	
	p_glCullFace = (void (*)(unsigned int))((const void*(*)(...))dlsym(handle, OBFUSCATE("glCullFace")));
}

#endif
