#pragma once
#include "_components.hh"

struct AppConfig {
	uint8_t tabSize;
	bool    highlightColumn;
	size_t  highlightedColumn;
	bool    autoIndent;
	bool    spacesIndent;
	bool    highlightLine;
	bool    ruler;
};
