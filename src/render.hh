#pragma once
#include "_components.hh"
#include "config.hh"

class EditorWindow;
class App;

namespace Renderers {
	namespace Noro {
		void Global(App& app, AppConfig& config);
		void RenderEditorWindow(EditorWindow& editorWindow, AppConfig& config);
	}
}
