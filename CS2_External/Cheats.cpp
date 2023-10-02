#include "Cheats.h"
#include "Render.hpp"
#include "global.hpp"
#include <iostream>
#include "D:\CSGO-Cheats-DLL\CS2_External\CS2_External\OS-ImGui\OS-ImGui.h"
#include "D:\CSGO-Cheats-DLL\CS2_External\CS2_External\OS-ImGui\imgui\imgui.h"
#include "ConfigSaver.hpp"

bool global::aimbot = false;
bool global::ShowWeaponESP = false;
bool global::headbox = false;
bool global::Boxbox = false;
bool global::DrawBone = false;
bool global::ShowLosLine = false;
bool global::showHealthBar = false;
bool global::showWatermark = false;
bool global::fps = false;
bool global::time = false;
bool global::cheat = true;
ImVec4 global::BoxColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
ImVec4 global::BoxBoxColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
ImVec4 global::HeadColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
ImVec4 global::BoneColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 global::LOSColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
float global::watermarkX = 10.0f;
float global::watermarkY = 10.0f;
int  global::AimPosition = 0;
DWORD  global::AimPositionIndex = BONEINDEX::head;
float global::Smooth = 0.7;

void Cheats::Run()
{
	ImGui::Begin("CS2 Cheats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	// Create a tab bar for different sections
	if (ImGui::BeginTabBar("Sections"))
	{
		// Visuals Tab
		if (ImGui::BeginTabItem("Visuals"))
		{
			// Visuals-related UI elements go here
			ImGui::Checkbox("Draw Bone", &global::DrawBone);
			ImGui::SameLine();

			ImGui::PushID("Bone Color");
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::ColorEdit4("Bone Color", &global::BoneColor.x);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::Checkbox("Show LOS Lines", &global::ShowLosLine);
			ImGui::SameLine();

			ImGui::PushID("LOS Color");
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::ColorEdit4("LOS Color", &global::LOSColor.x);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::Checkbox("Headbox", &global::headbox);
			ImGui::SameLine();

			ImGui::PushID("Headbox Color");
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::ColorEdit4("Box Color", &global::HeadColor.x);
				ImGui::EndPopup();
			}
			ImGui::PopID();

			ImGui::Checkbox("Static box", &global::Boxbox);
			ImGui::SameLine();

			ImGui::PushID("Static box Color");
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::ColorEdit4("Static box Color", &global::BoxBoxColor.x);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			
			ImGui::Checkbox("Weapon Esp", &global::ShowWeaponESP);

			ImGui::Checkbox("Health Bar", &global::showHealthBar);
			//ImGui::ColorEdit4("Box Color", &global::BoxColor.x);

			ImGui::EndTabItem();
		}

		// Aimbot Tab
		if (ImGui::BeginTabItem("Aimbot"))
		{
			Gui.MyCheckBox("Aimbot", &global::aimbot);
			float SmoothMin = 0.1f, SmoothMax = 1.f;
			Gui.SliderScalarEx1("Smooth", ImGuiDataType_Float, &global::Smooth, &SmoothMin, &SmoothMax, "%.1f", ImGuiSliderFlags_None);
			if (ImGui::Combo("AimPos", &global::AimPosition, "Head\0Neck\0Spine"))
			{
				switch (global::AimPosition)
				{
				case 0:
					global::AimPositionIndex = BONEINDEX::head;
					break;
				case 1:
					global::AimPositionIndex = BONEINDEX::neck_0;
					break;
				case 2:
					global::AimPositionIndex = BONEINDEX::spine_1;
					break;
				default:
					break;
				}
			}

			ImGui::EndTabItem();
		}

		// Misc Tab
		if (ImGui::BeginTabItem("Misc"))
		{
			ImGui::Checkbox("Watermark", &global::showWatermark);
			ImGui::SameLine();

			ImGui::PushID("Watermark");
			if (ImGui::Button("..."))
				ImGui::OpenPopup("");

			if (ImGui::BeginPopup("")) {
				ImGui::Checkbox("Fps", &global::fps);
				ImGui::Checkbox("Time", &global::time);
				ImGui::EndPopup();
			}
			ImGui::PopID();
			ImGui::InputFloat("X Position", &global::watermarkX);
			ImGui::InputFloat("Y Position", &global::watermarkY);

			ImGui::EndTabItem();
		}

		// Config Saving Tab
		if (ImGui::BeginTabItem("Config"))
		{
			if (ImGui::Button("Save Config"))
			{
				// Call a function to save the configuration using ConfigSaver
				MyConfigSaver::SaveConfig("config.json"); // Replace "config.json" with your desired config file name
			}

			if (ImGui::Button("Load Config"))
			{
				// Call a function to save the configuration using ConfigSaver
				MyConfigSaver::LoadConfig("config.json"); // Replace "config.json" with your desired config file name
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

	if (global::showWatermark)
	{
		// Watermark: FPS | Time | Aeonix
		ImGui::SetNextWindowPos(ImVec2(global::watermarkX, global::watermarkY));
		// ImGui::SetNextWindowSize(ImVec2(200, 20));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.25f)); // Set window background color to transparent
		ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

		// Display Aeonix
		if (global::cheat) {
			ImGui::Text("Aeonix");
			ImGui::SameLine();
		}

		if (global::cheat && (global::fps || global::time))
			ImGui::Text("|");

		if (global::fps) {
			ImGui::SameLine();
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::SameLine();
		}

		if (global::fps && global::time)
			ImGui::Text("|");

		if (global::time) {

			time_t t;
			struct tm now;
			if (time(&t) != -1 && localtime_s(&now, &t) == 0) // Use localtime_s
			{
				char timeStr[32];
				strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &now);
				ImGui::SameLine();
				ImGui::Text("Time: %s", timeStr);
			}
		}
		ImGui::PopStyleColor(); // Restore the original window background color
		ImGui::End();
	}


	// 更新矩阵数据
	if (!ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix, 64))
		return;

	// 更新实体链表地址
	gGame.UpdateEntityListEntry();

	DWORD64 LocalControllerAddress = 0;
	DWORD64 LocalPawnAddress = 0;

	if (!ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress))
		return;
	if (!ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress))
		return;

	// 本地实体
	CEntity LocalEntity;
	if (!LocalEntity.UpdateController(LocalControllerAddress))
		return;
	if (!LocalEntity.UpdatePawn(LocalPawnAddress))
		return;

	// 血条Map
	static std::map<DWORD64, Render::HealthBar> HealthBarMap;

	// 自瞄数据
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  HeadPos{ 0,0,0 };
	Vec3  AimPos{ 0,0,0 };

	for (int i = 0; i < 64; i++)
	{
		CEntity Entity;
		DWORD64 EntityAddress = 0;
		if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (i + 1) * 0x78, EntityAddress))
			continue;
		if (EntityAddress == LocalEntity.Controller.Address)
			continue;
		if (!Entity.UpdateController(EntityAddress))
			continue;
		if (!Entity.UpdatePawn(Entity.Pawn.Address))
			continue;

		if (Entity.Controller.TeamID == LocalEntity.Controller.TeamID)
			continue;
		if (!Entity.IsAlive())
			continue;
		if (!Entity.IsInScreen())
			continue;

		// ¹Ç÷Àµ÷ÊÔ»æÖÆ
	/*	for (int BoneIndex = 0; BoneIndex < Entity.BoneData.BonePosList.size(); BoneIndex++)
		{
			Vec2 ScreenPos{};
			if (gGame.View.WorldToScreen(Entity.BoneData.BonePosList[BoneIndex].Pos, ScreenPos))
			{
				Gui.Text(std::to_string(BoneIndex), ScreenPos, ImColor(255, 255, 255, 255));
			}
		}*/

		DistanceToSight = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos.DistanceTo({Gui.Window.Size.x / 2,Gui.Window.Size.y / 2});

		if (DistanceToSight < AimControl::AimRange)
		{
			if (DistanceToSight < MaxAimDistance)
			{
				MaxAimDistance = DistanceToSight;
				AimPos = Entity.GetBone().BonePosList[global::AimPositionIndex].Pos;
				if (global::AimPositionIndex == BONEINDEX::head)
					AimPos.z -= 1.f;
			}
		}


		if (global::DrawBone) {
			Render::DrawBone(Entity, ImColor(global::BoneColor), 1.3);
		}


		if (global::ShowLosLine) {
			Render::ShowLosLine(Entity, 50, ImColor(global::LOSColor), 1.3);
		}


		if (global::headbox) {
			Render::ShowHeadBox(Entity, ImColor(global::HeadColor), 1);
		}

		if (global::Boxbox) {
			Render::ShowBoxBox(Entity, ImColor(global::BoxBoxColor), 1);
		}

		auto Rect = Render::Draw2DBoneRect(Entity, ImColor(global::BoxColor), 1.3);

		if (global::showHealthBar) {
			if (!HealthBarMap.count(EntityAddress))
				HealthBarMap.insert({ EntityAddress,Render::HealthBar(100) });

			HealthBarMap[EntityAddress].DrawHealthBar(Entity.Controller.Health,
				{ Rect.x + Rect.z / 2 - 70 / 2,Rect.y - 20 }, { 70,8 });
		}


		if (global::ShowWeaponESP){
			Gui.StrokeText(Entity.Pawn.WeaponName, { Rect.x,Rect.y + Rect.w }, ImColor(255, 255, 255, 255), 17);
		}
	}


	if (global::aimbot) {
		if (GetAsyncKeyState(AimControl::HotKey))
		{
			if (AimPos != Vec3(0, 0, 0))
			{
				AimControl::AimBot(LocalEntity, LocalEntity.Pawn.CameraPos, AimPos);
			}
		}

	}
}
