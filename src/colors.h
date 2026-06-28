#pragma once

#include "common.h"
#include "state.h"

// Catppuccin Mocha palette
// All values are 0xRRGGBB.
namespace Cat {
    constexpr uint32_t TEXT     = 0xcdd6f4;  // --text      default text
    constexpr uint32_t SUBTEXT0 = 0xa6adc8;  // --subtext0  secondary text
    constexpr uint32_t SURFACE1 = 0x45475a;  // --surface1  bar background cell
    constexpr uint32_t SURFACE2 = 0x585b70;  // --surface2  box borders
    constexpr uint32_t OVERLAY0 = 0x6c7086;  // --overlay0  brackets / separators
    constexpr uint32_t BLUE     = 0x89b4fa;  // --blue      labels / header keys
    constexpr uint32_t GREEN    = 0xa6e3a1;  // --green     good / low values
    constexpr uint32_t PEACH    = 0xfab387;  // --peach     TX / write / warm
    constexpr uint32_t RED      = 0xf38ba8;  // --red       critical / bad
    constexpr uint32_t MAUVE    = 0xcba6f7;  // --mauve     titles / CPU freq
    constexpr uint32_t YELLOW   = 0xf9e2af;  // --yellow    warn / medium
    constexpr uint32_t TEAL     = 0x94e2d5;  // --teal      RX / read direction
}

// Color / style helpers

// Set foreground from 0xRRGGBB, always reset bg to default.
inline void nc_fg(ncplane* n, uint32_t rgb) {
    ncplane_set_fg_rgb8(n,
        static_cast<unsigned>((rgb >> 16) & 0xFF),
        static_cast<unsigned>((rgb >>  8) & 0xFF),
        static_cast<unsigned>( rgb        & 0xFF));
    ncplane_set_bg_default(n);
}

// Set fg + style in one call — the main draw helper used throughout panels.
inline void nc_set(ncplane* n, uint32_t rgb, unsigned style = NCSTYLE_NONE) {
    nc_fg(n, rgb);
    ncplane_set_styles(n, style);
}

/*
True-color gradient interpolation
Returns a 0xRRGGBB value for position pos ∈ [0, 1], matching the HTML gradients:
   CPU / HIST: green  -> yellow → red (--green  -> --yellow -> --red)
   MEM:        blue   -> mauve        (--blue   -> --mauve)
   TEMP:       yellow -> peach → red  (--yellow -> --peach  -> --red)
   STORAGE:    teal   -> blue         (--teal   -> --blue)

Because notcurses supports true 24-bit color, this is a simple linear
interpolation — no LUT, no xterm-256 cube approximation needed.
*/

static inline uint32_t lerp_rgb(uint32_t a, uint32_t b, double t) {
    t = std::max(0.0, std::min(1.0, t));
    auto ch = [&](int shift) -> uint32_t {
        int av = (a >> shift) & 0xFF;
        int bv = (b >> shift) & 0xFF;
        return static_cast<uint32_t>(av + static_cast<int>((bv - av) * t));
    };
    return (ch(16) << 16) | (ch(8) << 8) | ch(0);
}

inline uint32_t grad_color(GradType gt, double pos) {
    pos = std::max(0.0, std::min(1.0, pos));
    switch (gt) {
        case GRAD_CPU:
        case GRAD_HIST:
            if (pos < 0.5) return lerp_rgb(Cat::GREEN,  Cat::YELLOW, pos * 2.0);
            else           return lerp_rgb(Cat::YELLOW,  Cat::RED,   (pos - 0.5) * 2.0);

        case GRAD_MEM:
            return lerp_rgb(Cat::BLUE, Cat::MAUVE, pos);

        case GRAD_TEMP:
            if (pos < 0.5) return lerp_rgb(Cat::YELLOW, Cat::PEACH, pos * 2.0);
            else           return lerp_rgb(Cat::PEACH,   Cat::RED,  (pos - 0.5) * 2.0);

        case GRAD_STORAGE:
            return lerp_rgb(Cat::TEAL, Cat::BLUE, pos);
    }
    return Cat::GREEN;
}

// Discrete color for textual percentage display (the " 73%" label).
inline uint32_t pct_color(double pct) {
    if (pct >= 85.0) return Cat::RED;
    if (pct >= 60.0) return Cat::YELLOW;
    return Cat::GREEN;
}
