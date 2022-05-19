#pragma once
#include "_components.hh"

class EditorWindow;
class App;

namespace Renderers {
	namespace Noro {
		void Global(App& app);
		void RenderEditorWindow(EditorWindow& editorWindow);
	}
}
