#include "terminal.hh"
#include "iohandle.hh"
#include "constants.hh"

void Terminal::Run() {
	IOHandle::Quit();
	puts(APP_NAME " has suspended to the terminal");

	const char* shell   = getenv("SHELL");
	const char* argv[]  = {shell, nullptr};

	pid_t pid = fork();
	if (pid == 0) {
		execvp(shell, (char**)argv); // call shell
	}
	else if (pid > 0) {
		pid = wait(nullptr); // wait for child shell process to finish
	}
	else {
		perror("[ERROR] execvp failed");
		exit(1);
	}

	IOHandle::Init();
}
