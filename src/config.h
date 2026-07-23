#pragma once

#include <string>

// Persisted at ~/.config/vitals/config as plain "key=value" lines:
//   theme=Catppuccin Mocha
//   background=transparent      (or "solid")
struct Config {
    std::string theme_name = "Catppuccin Mocha";
    std::string bg_mode    = "transparent"; // "transparent" | "solid"
};

// Reads the config file. Returns defaults (Catppuccin Mocha, transparent) if
// the file or the config directory doesn't exist yet, or on any parse error.
Config load_config();

// Writes the config file, creating ~/.config/vitals if needed.
// Best-effort: failures (e.g. read-only $HOME) are silently ignored.
void save_config(const Config& cfg);
