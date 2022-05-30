#include "Yup.h"
#include "params.h"

 
UInt32 g_MyPluginVersion = 100;
const char* g_MyPluginName = "Yup";


const char* newSrc[4] = { "Data\\Sound\\Voice\\FalloutNV.esm\\MaleOld02\\VFreeformG_VFreeformGoodsp_00104C57_1.ogg", 
"Data\\Sound\\Voice\\FalloutNV.esm\\MaleOld02\\VFreeformG_GOODBYE_0015EBF2_1.ogg", 
"Data\\Sound\\Voice\\FalloutNV.esm\\MaleOld02\\VFreeformG_MurderNoCrime_001254E6_1.ogg", 
"Data\\Sound\\Voice\\FalloutNV.esm\\MaleOld02\\VFreeformG_VFreeformGoodsp_0015D3DE_1.ogg" };

errno_t __cdecl Actor_DoSpeechLoadLipFiles_strcpy_s_Hook(char* Dst, rsize_t SizeInBytes, char* Src)
{
	return strcpy_s(Dst, SizeInBytes, newSrc[rand() % 4]);
}

BOOL __fastcall TopicInfoResponseHook(void* a1, char* a2, int a3) {
	strcpy(a2, "Yup.");
	return ThisStdCall(0x4037F0, a1, a2, a3);
}
void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{

	case NVSEMessagingInterface::kMessage_DeferredInit:
		TESNPC* pete = (TESNPC*)LookupFormByID(0x104C7F);
		for (auto iter = DataHandler::Get()->boundObjectList->first; iter; iter = iter->next)
		{
			if (iter->typeID == 42)
			{
				TESNPC* npc = (TESNPC*)iter;
				if (npc->refID == pete->refID) continue;
				std::string name = "Easy Pete";			
				npc->fullName.name.Set(name.c_str());
				npc->SetRace(pete->race.race);  
				npc->baseData.voiceType = pete->baseData.voiceType;
				npc->CopyAppearance(pete);
			}
		}

		WriteRelCall(0x8A2200, (UInt32)Actor_DoSpeechLoadLipFiles_strcpy_s_Hook);  
		WriteRelCall(0x83D2EA, (UInt32)TopicInfoResponseHook);
		break;

	}
}



extern "C"
{

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = g_MyPluginName;
		info->version = g_MyPluginVersion;

		
		if (nvse->nvseVersion < PACKED_NVSE_VERSION)  
		{
			char buffer[100];
			sprintf_s(buffer, 100,"NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, PACKED_NVSE_VERSION);
			MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);  
			_ERROR("%s", buffer);
			return false;
		}
		
		if (!nvse->isEditor)
		{
			if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
			{
				char buffer[100];
				sprintf_s(buffer, 100, "Incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
				MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);
				_ERROR("%s", buffer);
				return false;
			}

			if (nvse->isNogore)
			{
				char buffer[] = "NoGore is not supported";
				MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);
				_ERROR("%s", buffer);
				return false;
			}
		}
		else {
			return false;
		}

		return true;
	}


	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		
		((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(nvse->GetPluginHandle(), "NVSE", MessageHandler);
		
		return true;
	}

};
