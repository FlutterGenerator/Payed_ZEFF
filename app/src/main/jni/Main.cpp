#include <jni.h>
#include <android/native_activity.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <vector>

#include <pthread.h>
#include <codecvt>
#include <chrono>
#include <queue>

#include "ImGui/imgui_internal.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <sys/system_properties.h>

#include <string>

//====================================
#include "ImGui/FONTS/DEFAULT.h"
//=====================================
#include <Includes/Utils.h>
#include <Includes/Dobby/dobby.h>
#include "KittyMemory/MemoryPatch.h"
#include "Obfuscate.h"
#include "RGB.h"
#include "Chams.h"

//Ida Pro Define
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;
#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long

int screenWidth = 0;
int screenHeight = 0;
bool g_Initialized = false;
ImGuiWindow* g_window = NULL;


struct My_Patches {MemoryPatch;

} hexPatches;

const char* MineLib = OBFUSCATE("libGLESv2.so");

#include "Variables.h"

extern "C" {
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_init(JNIEnv* env, jclass cls);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_step(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY);
    JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_real(JNIEnv* env, jobject obj, jint width, jint height);
};

JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_init(JNIEnv* env, jclass cls) {

    //SetUpImGuiContext
    if(!g_Initialized) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    //Set ImGui Style
    ImGui::StyleColorsClassic();
 
    // Setup Platform/Renderer backends
    //ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 100");
	ImGui::GetStyle().ScaleAllSizes(3.5f);

    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0x0900, 0x097F, 0,};
    ImFontConfig font_config;
    ImFontConfig icons_config;
    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom3), sizeof(Custom3), 30.f, &CustomFont, io.Fonts->GetGlyphRangesCyrillic());
	
	g_Initialized = true;
	}
    return;
}

JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	if (g_Initialized) {
    screenWidth = (int) width;
    screenHeight = (int) height;
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
	}
	return;
}

void Patches() {
    
}

#include "Menu.h"

JNIEXPORT void JNICALL
Java_com_mycompany_application_GLES3JNIView_step(JNIEnv* env, jobject obj) {
    if (g_Initialized) {
	ImGuiIO& io = ImGui::GetIO();
	
	//Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    
    BeginDraw();
    
    ImGui::EndFrame();
	
    ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj){
    if (!g_Initialized)
        return;
     // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_Initialized=false;
}

JNIEXPORT void JNICALL Java_com_mycompany_application_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY){
	if (g_Initialized) {
	ImGuiIO & io = ImGui::GetIO();
	io.MouseDown[0] = down;
	io.MousePos = ImVec2(PosX,PosY);
	}
}

JNIEXPORT jstring JNICALL Java_com_mycompany_application_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz) {
    //get drawing window
    // TODO: accomplish getWindowSizePos()
	if (g_Initialized) {
    char result[512]="0|0|0|0";
    if(g_window){
        sprintf(result,"%d|%d|%d|%d",(int)g_window->Pos.x,(int)g_window->Pos.y,(int)g_window->Size.x,(int)g_window->Size.y);
    }
    return env->NewStringUTF(result);
	}
}

void *hack_thread(void *) {
    ProcMap mineMap;
    do {
        mineMap = KittyMemory::getLibraryMap(MineLib);
		handle = NULL;
		handle = dlopen(OBFUSCATE("libGLESv2.so"), RTLD_LAZY);
    } while (!mineMap.isValid());
	
	RenderHook();  
	
    return NULL;
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    //Спуф потока, который байпасит анти чит Стандпорн 2
	//Использовал я его в модуле для это пидараснутой игры: https://t.me/zeff_cheats/1161
    pthread_t gameThread = NULL;
    if (gameThread = pthread_self()) {
        pthread_create(&gameThread, NULL, hack_thread, NULL);
    }
    
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {}