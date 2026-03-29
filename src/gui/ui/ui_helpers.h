#pragma once

#include <imgui.h>

// Section header: full-width tinted bar with text in medium font.
// Creates a strong visual anchor for scanning panel content.
void sectionHeader( const char* text );

// Compact section label: colored text without background bar.
// Use for subsections within a section.
void sectionLabel( const char* text, ImVec4 color = ImVec4( 0.7f, 0.85f, 1.0f, 1.0f ) );
