#pragma once
#include "nvse/PluginAPI.h"
#include "nvse/GameData.h"
#include "nvse/GameForms.h"
#include "nvse/GameObjects.h"
#include "nvse/SafeWrite.h"
#include "nvse/Utilities.h"
#include "nvse/GameTiles.h"
extern IDebugLog g_Log;

__declspec(naked) void __fastcall TESNPC::SetRace(void* pRace)
{
	__asm
	{
		cmp[ecx + 0x110], edx
		jz		done
		mov		eax, g_thePlayer
		cmp[eax + 0x20], ecx
		jnz		notPlayer
		push	0
		push	edx
		mov eax, 0x60B240
		call eax
		retn
		notPlayer :
		mov[ecx + 0x110], edx
			done :
		retn
	}
}