#include "app/ui/main_window.hpp"
#include <GLFW/glfw3.h>
#include <app/ui/ui.hpp>
#include <cstdio>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl2.h>
#include <imgui/imgui.h>

GLFWwindow *window = nullptr;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void glfw_error_callback(int error, const char *description) {
  ::fprintf(stderr, "[ERROR] GLFW Error %d: %s\n", error, description);
}

int ui_init() {
  ::glfwSetErrorCallback(::glfw_error_callback);
  if (!glfwInit()) {
    return 1;
  }

  // Create window with graphics context
  window = ::glfwCreateWindow(1280, 720, "App", nullptr, nullptr);
  if (window == nullptr) {
    return 1;
  }
  ::glfwMakeContextCurrent(window);
  ::glfwSwapInterval(1);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ::ImGui_ImplGlfw_InitForOpenGL(window, true);
  ::ImGui_ImplOpenGL2_Init();

  MainWindow::init();

  return 0;
}

static void ui_render() {
  ImGui::Render();
  int display_w, display_h;
  ::glfwGetFramebufferSize(window, &display_w, &display_h);
  ::glViewport(0, 0, display_w, display_h);
  ::glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
  ::glClear(GL_COLOR_BUFFER_BIT);

  ::ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  ::glfwMakeContextCurrent(window);
  ::glfwSwapBuffers(window);
}

int ui_loop() {
  while (!::glfwWindowShouldClose(window)) {
    ::glfwPollEvents();
    if (::glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ::ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ::ImGui_ImplOpenGL2_NewFrame();
    ::ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    MainWindow::render_ui();

    ::ui_render();
  }

  return 0;
}

int ui_close() {
  MainWindow::close();

  ::ImGui_ImplOpenGL2_Shutdown();
  ::ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  ::glfwDestroyWindow(window);
  ::glfwTerminate();

  return 0;
}
