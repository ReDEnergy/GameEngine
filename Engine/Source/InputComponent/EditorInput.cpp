#include "EditorInput.h"

#include <Core/WindowObject.h>
#include <Manager/Manager.h>
#include <Manager/RenderingSystem.h>
#include <UI/MenuSystem.h>

using namespace std;

EditorInput::EditorInput()
{
	SubscribeToEvent("ssao");
	SubscribeToEvent("forward-rendering");
	SubscribeToEvent("vsync-rendering");
}

void EditorInput::OnKeyPress(int key, int mods) {
}

void EditorInput::OnEvent(const string& eventID, void *data)
{
	ToggleEntry *entry = (ToggleEntry*)data;

	if (eventID.compare("ssao") == 0) {
		Manager::RenderSys->Set(RenderState::SS_AO, entry->GetState());
	}
	if (eventID.compare("forward-rendering") == 0) {
		Manager::RenderSys->Set(RenderState::POST_PROCESS, entry->GetState());
	}
	if (eventID.compare("vsync-rendering") == 0) {
		Manager::RenderSys->Set(RenderState::VSYNC, entry->GetState());
		window->SetVSync(entry->GetState());
	}
}