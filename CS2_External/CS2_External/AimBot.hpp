#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include <iostream>

namespace AimControl
{
	static int HotKey = VK_LMENU;	// ��׼�ȼ�
	static float AimRange = 300;	// ��׼��Χ

	inline void AimBot(const CEntity& Local, Vec3 LocalPos, Vec3 AimPos)
	{
		float Yaw, Pitch;
		float Distance;
		Vec3 OppPos;

		OppPos = AimPos - LocalPos;

		Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

		Yaw = atan2f(OppPos.y, OppPos.x) * 180 / M_PI - Local.Pawn.ViewAngle.y;
		Pitch = -atan(OppPos.z / Distance) * 180 / M_PI - Local.Pawn.ViewAngle.x;
		Yaw = Yaw * global::Smooth + Local.Pawn.ViewAngle.y;
		Pitch = Pitch * global::Smooth + Local.Pawn.ViewAngle.x;

		// Recoil control
		if (Local.Pawn.ShotsFired > 1)
		{
			Yaw = Yaw - Local.Pawn.AimPunchAngle.y * 1.2;
			Pitch = Pitch - Local.Pawn.AimPunchAngle.x * 1.4;
		}

		gGame.SetViewAngle(Yaw, Pitch);
	}
}