#pragma once

#include "app/logger.hpp"
#include <app/student.hpp>
#include <app/utility.hpp>
#include <cstdlib>
#include <fstream>
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
                                         std::vector<std::string>>>;

  static MainWindowState state;

  static void init_state() {
    state["stus"] = nullptr;
    state["is_opening"] = true;
    state["log_file"] = nullptr;
    state["logs"] = std::vector<std::string>();
  }

  static void init(const std::string &log_file_path) {
    init_state();
    state["log_file"] = new std::ifstream(log_file_path);
  }

  static void close() {
    std::vector<Student> *p_stus =
        (std::vector<Student> *)std::get<void *>(MainWindow::state["stus"]);
    if (p_stus != nullptr) {
      delete p_stus;
      state["stus"] = p_stus;
    }
  }

  static void render_ui() {
    bool *p_open = nullptr;
    static bool opt_fullscreen = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::Begin("Main Window", p_open, window_flags);
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

          Logger::info("[INFO] Main Window Initialized.");
          Logger::info("[INFO] stus.size = %lld", p_stus->size());
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
          *p_open = false;
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

    /*render_logger();*/

    render_table();

    ImGui::End();
  }

  static void render_logger() {
    auto &log_file = *(std::ifstream *)std::get<void *>(state["log_file"]);
    auto &logs = std::get<std::vector<std::string>>(state["logs"]);
    std::string line;
    std::string text;
    std::getline(log_file, line);

    if (logs.size() == 10) {
      logs.erase(logs.begin());
    }
    logs.push_back(line);

    for (int i = 0; i < logs.size(); i++) {
      if (i == 0) {
        text += i;
      } else {
        text += i + '\n';
      }
    }

    ImGui::Text("%s", text.c_str());
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

    ImGui::SameLine();

    if (ImGui::Button("Sort")) {
      ::sort(stus);
    }

    if (ImGui::BeginTable("Student", 13,
                          ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
      ImGui::TableSetupColumn("index", ImGuiTableColumnFlags_WidthFixed, 20.0f);
      ImGui::TableSetupColumn("weigted score");
      ImGui::TableSetupColumn("stu", ImGuiTableColumnFlags_WidthFixed, 30.0f);
      ImGui::TableSetupColumn("tm", ImGuiTableColumnFlags_WidthFixed, 30.0f);

      for (int i = 0; i < 9; i++) {
        ImGui::TableSetupColumn(
            (std::string("x") + std::to_string(i + 1)).c_str());
      }

      ImGui::TableHeadersRow();

      for (int i = 0; i < n_stus; i++) {
        bool have_indexed = false;
        for (auto &tm : stus[i].m_scores) {
          size_t n_tm_scores = tm.second.size();
          ImGui::TableNextRow();
          if (!have_indexed) {
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", i + 1);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%f", stus[i].calc_weighted_score());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", stus[i].m_name.c_str());
            have_indexed = true;
          }
          ImGui::TableSetColumnIndex(3);
          ImGui::Text("%s", tm.first.c_str());
          for (int j = 0; j < n_tm_scores; j++) {
            ImGui::TableSetColumnIndex(j + 4);
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
