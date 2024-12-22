#pragma once

#include "app/student.hpp"
#include "app/utility.hpp"
#include <cstdlib>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl2.h>
#include <imgui/imgui.h>
#include <map>
#include <string>
#include <sys/stat.h>
#include <variant>

struct MainWindow {
  using MainWindowState =
      std::map<std::string, std::variant<bool, int, float, std::string, void *,
                                         std::vector<bool>>>;

  static MainWindowState state;

  static void init_state() {
    state["stus"] = nullptr;
    state["is_opening"] = true;
  }

  static void init() { init_state(); }

  static void close() {
    std::vector<Student> *p_stus =
        (std::vector<Student> *)std::get<void *>(MainWindow::state["stus"]);
    if (p_stus != nullptr) {
      delete p_stus;
      state["stus"] = p_stus;
    }
  }

  static void render_ui() {
    bool *p_opt_show = nullptr;
    static bool opt_fullscreen = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::Begin("Main Window", p_opt_show, window_flags);
    ImGuiIO &io = ImGui::GetIO();
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Options")) {
        if (ImGui::MenuItem("Load")) {
          auto p_stus = (std::vector<Student> *)std::get<void *>(state["stus"]);
          if (p_stus != nullptr) {
            delete p_stus;
            p_stus = nullptr;
            state["stus"] = p_stus;
          }

          p_stus = new std::vector(::load_data_from_csv("build/output.csv"));
          state["stus"] = p_stus;

          ::printf("[INFO] Main Window Initialized.\n");
          ::printf("[INFO] stus.size = %lld\n", p_stus->size());
        }
        if (ImGui::MenuItem("Clean")) {
          auto p_stus = (std::vector<Student> *)std::get<void *>(state["stus"]);
          if (p_stus != nullptr) {
            delete p_stus;
            p_stus = nullptr;
            state["stus"] = p_stus;
          }
        }

        if (ImGui::MenuItem("Close")) {
          *p_opt_show = false;
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Dark")) {
          ImGui::StyleColorsDark();
        }
        if (ImGui::MenuItem("Light")) {
          ImGui::StyleColorsLight();
        }
        if (ImGui::MenuItem("Classic")) {
          ImGui::StyleColorsClassic();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    render_table();

    ImGui::End();
  }

  static void render_table() {
    const auto p_stus = (std::vector<Student> *)std::get<void *>(state["stus"]);

    if (p_stus == nullptr) {
      return;
    }

    std::vector<Student> &stus = *p_stus;
    const size_t n_stus = stus.size();

    if (ImGui::Button("Imputes")) {
      ::impute_missing_values(stus);
    }

    if (ImGui::BeginTable("Student", 10,
                          ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
      ImGui::TableSetupColumn("stu and tm");

      for (int i = 0; i < 9; i++) {
        ImGui::TableSetupColumn(
            (std::string("x") + std::to_string(i + 1)).c_str());
      }

      ImGui::TableHeadersRow();

      for (int i = 0; i < n_stus; i++) {
        for (auto &tm : stus[i].m_scores) {
          size_t n_tm_scores = tm.second.size();
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("%s", (stus[i].m_name + '\t' + tm.first).c_str());
          for (int j = 0; j < n_tm_scores; j++) {
            ImGui::TableSetColumnIndex(j + 1);
            auto score = tm.second[j];
            if (std::abs(score - (-1.0)) < 0.00001) {
              ImGui::Text("*");
            } else {
              ImGui::Text("%s", std::to_string(tm.second[j]).c_str());
            }
          }
        }
      }

      ImGui::EndTable();
    }
  }
};
