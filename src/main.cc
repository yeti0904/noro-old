#include "app.hh"

int main(int argc, char** argv) {
	App app(argc, argv);

	while (app.run) {
		app.Update();
	}

	return 0;
}
