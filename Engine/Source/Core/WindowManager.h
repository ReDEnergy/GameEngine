#pragma once
#include <vector>
#include <unordered_map>

#include <include/dll_export.h>
#include <include/gl_defines.h>

class WindowObject;
class WindowProperties;

class DLLExport WindowManager
{
	private:
		WindowManager();
		~WindowManager();

	public:
		static void Init();
		static void OnGLFWClose(GLFWwindow *W);
		static void OnGLFWResize(GLFWwindow *W, int width, int height);
		static void OnGLFWError(int error, const char* description);
		static void RegisterWindow(WindowObject * W);
		static WindowObject* GetDefaultWindow();
		static WindowObject* GetWindowObjectByName(const char *name = nullptr);
		static WindowObject* GetShaderdWindowContext();
		static WindowObject* GetWindowObject(GLFWwindow *W);
		static WindowObject* DetachCurrentContext();

	private:
		static std::vector<WindowObject*> windowList;
		static std::unordered_map<std::string, WindowObject*> windows;
		static WindowObject* sharedContextWindow;
};