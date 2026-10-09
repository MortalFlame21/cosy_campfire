#pragma once

#include <array> // std::array
#include <format> // std::format

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "GameObject.hpp"
#include "NamedObjects.hpp"

class UIManager {
public:
	UIManager(const Window& w) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(w.data(), true);
		ImGui_ImplOpenGL3_Init();
	}

	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;
	UIManager(UIManager&&) = delete;
	UIManager& operator=(UIManager&&) = delete;

	~UIManager() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	void draw(Scene& scene) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		ImGui::Begin("Scene");

		if (ImGui::TreeNode("Scene Objects")) {
			for (int i{}; auto& o : scene.objects()) {
				ImGui::PushID(i++);
                if (ImGui::TreeNode(o.model().path().c_str())) {
					ImGui::SliderFloat3("Position", glm::value_ptr(o.position()), -20.f, 20.f);
					ImGui::SliderFloat3("Scale", glm::value_ptr(o.scale()), 0.f, 10.f);
					ImGui::SliderFloat3("Rotation", glm::value_ptr(o.rotation()), 0.f, 360.f);
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		if (auto& l{scene.directional_light()}; ImGui::TreeNode("Scene Directional Light")) {
			ImGui::SliderFloat3("Direction", glm::value_ptr(l.direction), -20.f, 20.f);
			ImGui::ColorEdit3("Color", glm::value_ptr(l.color));
			ImGui::SliderFloat3("Ambient", glm::value_ptr(l.ambient), -20.f, 20.f);
			ImGui::SliderFloat3("Diffuse", glm::value_ptr(l.diffuse), -20.f, 20.f);
			ImGui::SliderFloat3("Specular", glm::value_ptr(l.specular), -20.f, 20.f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scene Point Lights")) {
			for (int i{}; auto& l : scene.point_lights()) {
				ImGui::PushID(i);
                if (ImGui::TreeNode(std::format("Light {}", i++).c_str())) {
					ImGui::SliderFloat3("Position", glm::value_ptr(l.position), -20.f, 20.f);
					ImGui::ColorEdit3("Color", glm::value_ptr(l.color));
					ImGui::SliderFloat3("Ambient", glm::value_ptr(l.ambient), -20.f, 20.f);
					ImGui::SliderFloat3("Diffuse", glm::value_ptr(l.diffuse), -20.f, 20.f);
					ImGui::SliderFloat3("Specular", glm::value_ptr(l.specular), -20.f, 20.f);
					ImGui::SliderFloat("Constant", &l.constant, 0.f, 1.f, "%.9f");
					ImGui::SliderFloat("Linear", &l.linear, 0.f, 10.f, "%.9f");
					ImGui::SliderFloat("Quadratic", &l.quadratic, 0.f, 10.f, "%.9f");
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::SmallButton("Save Current Scene")) 
			scene.save();
		//ImGui::ShowDemoWindow();

        ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};
