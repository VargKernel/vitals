#pragma once

#include <cstdint>
#include <string>
#include <vector>

/*
Theme
Each Theme is a full palette: the accent colors used throughout the panels
plus BASE, the theme's own native background color. BASE is only actually
painted onto the screen when the person picks "Theme background" in the
Settings overlay (Esc) — otherwise the terminal's own default background
is used (transparent), see colors.h::nc_bg_apply().

All values are 0xRRGGBB.
*/
struct Theme {
    std::string name;

    uint32_t BASE;      // native background for this theme (solid-bg mode)

    uint32_t TEXT;      // default text
    uint32_t SUBTEXT0;  // secondary text
    uint32_t SURFACE1;  // bar background cell
    uint32_t SURFACE2;  // box borders
    uint32_t OVERLAY0;  // brackets / separators
    uint32_t BLUE;      // labels / header keys
    uint32_t GREEN;     // good / low values
    uint32_t PEACH;     // TX / write / warm
    uint32_t RED;       // critical / bad
    uint32_t MAUVE;     // titles / CPU freq
    uint32_t YELLOW;    // warn / medium
    uint32_t TEAL;      // RX / read direction
};

// Full list of available themes, in display order (index 0 is the default).
const std::vector<Theme>& all_themes();

// Currently active theme / background mode, driven by AppState (theme_idx, bg_idx).
const Theme& current_theme();

// Clamp-set the active theme by index (wraps are handled by the caller).
void set_theme_index(int idx);

// Looks up a theme by name (case-sensitive, matches Theme::name). Returns -1 if not found.
int find_theme_index(const std::string& name);
