#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <map>
#include "Entity.h"

namespace Render
{
	// 骨骼绘制
	void DrawBone(const CEntity& Entity, ImColor Color, float Thickness)
	{
		BoneJointPos Previous, Current;
		for (auto i : BoneJointList::List)
		{
			Previous.Pos = Vec3(0, 0, 0);
			for (auto Index : i)
			{
				Current = Entity.GetBone().BonePosList[Index];
				if (Previous.Pos == Vec3(0, 0, 0))
				{
					Previous = Current;
					//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
					continue;
				}
				if (Previous.IsVisible && Current.IsVisible)
				{
					Gui.Line(Previous.ScreenPos, Current.ScreenPos, Color, Thickness);
					//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
				}
				Previous = Current;
			}
		}
	}

	// 朝向绘制
	void ShowLosLine(const CEntity& Entity, const float Length, ImColor Color, float Thickness)
	{
		Vec2 StartPoint, EndPoint;
		Vec3 Temp;
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		StartPoint = Head.ScreenPos;

		float LineLength = cos(Entity.Pawn.ViewAngle.x * M_PI / 180) * Length;

		Temp.x = Head.Pos.x + cos(Entity.Pawn.ViewAngle.y * M_PI / 180) * LineLength;
		Temp.y = Head.Pos.y + sin(Entity.Pawn.ViewAngle.y * M_PI / 180) * LineLength;
		Temp.z = Head.Pos.z - sin(Entity.Pawn.ViewAngle.x * M_PI / 180) * Length;

		if (!gGame.View.WorldToScreen(Temp, EndPoint))
			return;

		Gui.Line(StartPoint, EndPoint, Color, Thickness);
	}

	void ShowHeadBox(const CEntity& Entity, ImColor Color, float Thickness)
	{
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		if (Head.IsVisible)
		{
			ImVec2 Size = ImVec2(15.0f, 15.0f);

			ImVec2 Min = ImVec2(Head.ScreenPos.x - Size.x / 2, Head.ScreenPos.y - Size.y / 2);
			ImVec2 Max = ImVec2(Head.ScreenPos.x + Size.x / 2, Head.ScreenPos.y + Size.y / 2);

			Gui.Rectangle(Min, ImVec2(Max.x - Min.x, Max.y - Min.y), Color, Thickness);
		}
	}

	void ShowBoxBox(const CEntity& Entity, ImColor Color, float Thickness)
	{
		BoneJointPos Head = Entity.GetBone().BonePosList[BONEINDEX::head];

		if (Head.IsVisible)
		{
			Vec2 Size;
			Size.y = Entity.Pawn.ScreenPos.y - Head.ScreenPos.y;
			Size.x = Size.y * 0.64;

			Vec2 Pos = ImVec2(Entity.Pawn.ScreenPos.x - Size.x / 2, Head.ScreenPos.y);

			Gui.Rectangle(Pos, Size, Color, Thickness);
		}
	}

	// 2D骨骼框绘制
	ImVec4 Draw2DBoneRect(const CEntity& Entity, ImColor Color, float Thickness)
	{
		Vec2 Min, Max, Size;
		Min = Max = Entity.GetBone().BonePosList[0].ScreenPos;

		for (auto &BoneJoint : Entity.GetBone().BonePosList)
		{
			if (!BoneJoint.IsVisible)
				continue;
			Min.x = min(BoneJoint.ScreenPos.x, Min.x);
			Min.y = min(BoneJoint.ScreenPos.y, Min.y);
			Max.x = max(BoneJoint.ScreenPos.x, Max.x);
			Max.y = max(BoneJoint.ScreenPos.y, Max.y);
		}
		Size.x = Max.x - Min.x;
		Size.y = Max.y - Min.y;

		Gui.Rectangle(Min, Size, Color, Thickness);
		return ImVec4(Min.x, Min.y, Size.x, Size.y);
	}

	class HealthBar
	{
	private:
		using TimePoint_ = std::chrono::steady_clock::time_point;
	private:
		// 显示备份血条时间(ms)
		const int ShowBackUpHealthDuration = 450;
		// 最大血量
		float MaxHealth = 0.f;
		// 当前血量
		float CurrentHealth = 0.f;
		// 最近备份血量大小
		float LastestBackupHealth = 0.f;
		// 血条坐标
		ImVec2 RectPos{};
		// 血条大小
		ImVec2 RectSize{};
		// 正在显示备份血量
		bool InShowBackupHealth = false;
		// 显示备份血量起始时间戳
		TimePoint_ BackupHealthTimePoint{};
	public:
		// 初始化
		HealthBar(float MaxHealth_) :MaxHealth(MaxHealth_) {}
		HealthBar() { MaxHealth = 100; }
		// 绘制血条
		void DrawHealthBar(float CurrentHealth, ImVec2 Pos, ImVec2 Size);
	private:
		// 第一阶段血条颜色 0.5-1
		ImColor FirstStageColor = ImColor(96, 246, 113, 220);
		// 第二阶段血条颜色 0.5-0.2
		ImColor SecondStageColor = ImColor(247, 214, 103, 220);
		// 第三阶段血条颜色 0.2-0.0
		ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
		// 备份血条颜色
		ImColor BackupHealthColor = ImColor(255, 255, 255, 220);
		// 边框颜色
		ImColor FrameColor = ImColor(45, 45, 45, 40);
		// 背景颜色
		ImColor BackGroundColor = ImColor(90, 90, 90, 40);
	};

	void HealthBar::DrawHealthBar(float CurrentHealth, ImVec2 Pos, ImVec2 Size)
	{
		auto InRange = [&](float value, float min, float max) -> bool
		{
			return value > min && value <= max;
		};

		ImDrawList* DrawList = ImGui::GetForegroundDrawList();

		this->CurrentHealth = CurrentHealth;
		this->RectPos = Pos;
		this->RectSize = Size;

		// 占比
		float Proportion = CurrentHealth / MaxHealth;
		// 血量条宽度
		float Width = RectSize.x * Proportion;
		// 血量条颜色
		ImColor Color;

		// 背景
		DrawList->AddRectFilled(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			BackGroundColor, 5, 15);

		// 颜色切换
		if (InRange(Proportion, 0.5, 1))
			Color = FirstStageColor;
		else if (InRange(Proportion, 0.2, 0.5))
			Color = SecondStageColor;
		else if (InRange(Proportion, 0, 0.2))
			Color = ThirdStageColor;

		// 更新最近备份血量
		if (LastestBackupHealth == 0
			|| LastestBackupHealth < CurrentHealth)
			LastestBackupHealth = CurrentHealth;

		if (LastestBackupHealth != CurrentHealth)
		{
			if (!InShowBackupHealth)
			{
				BackupHealthTimePoint = std::chrono::steady_clock::now();
				InShowBackupHealth = true;
			}

			std::chrono::steady_clock::time_point CurrentTime = std::chrono::steady_clock::now();
			if (CurrentTime - BackupHealthTimePoint > std::chrono::milliseconds(ShowBackUpHealthDuration))
			{
				// 超时就停止显示备份血量，并且更新最近备份血量
				LastestBackupHealth = CurrentHealth;
				InShowBackupHealth = false;
			}

			if (InShowBackupHealth)
			{
				// 备份血量绘制宽度
				float BackupHealthWidth = LastestBackupHealth / MaxHealth * RectSize.x;
				// 备份血量alpha渐变
				float BackupHealthColorAlpha = 1 - 0.95 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
				ImColor BackupHealthColorTemp = BackupHealthColor;
				BackupHealthColorTemp.Value.w = BackupHealthColorAlpha;
				// 备份血量宽度缓动
				float BackupHealthWidth_Lerp = 0.7 * (std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - BackupHealthTimePoint).count() / (float)ShowBackUpHealthDuration);
				BackupHealthWidth_Lerp *= (BackupHealthWidth - Width);
				BackupHealthWidth -= BackupHealthWidth_Lerp;
				// 备份血条
				DrawList->AddRectFilled(RectPos,
					{ RectPos.x + BackupHealthWidth,RectPos.y + RectSize.y },
					BackupHealthColorTemp, 5);
			}
		}

		// 血条
		DrawList->AddRectFilled(RectPos,
			{ RectPos.x + Width,RectPos.y + RectSize.y },
			Color, 5);

		// 边框
		DrawList->AddRect(RectPos,
			{ RectPos.x + RectSize.x,RectPos.y + RectSize.y },
			FrameColor, 5, 15, 2);
	}

}