#pragma once

#include <QOpenGLWindow>

struct ImFont;

struct ImGuiFonts
{
	ImFont* title     = nullptr; // HermeneusOne ~48px (decorative title)
	ImFont* uiMedium  = nullptr; // PT Root UI ~20px (section headers)
	ImFont* ui        = nullptr; // PT Root UI ~18px (main menu buttons)
	ImFont* uiDefault = nullptr; // PT Root UI ~16px (in-game default)
	ImFont* uiSmall   = nullptr; // PT Root UI ~14px (captions, log entries)
};

ImGuiFonts& GetImGuiFonts();

namespace ImGuiQt5
{
	void Init( QOpenGLWindow* window );
	void Shutdown();
	void NewFrame( QOpenGLWindow* window );

	bool ProcessKeyEvent( QKeyEvent* event );
	bool ProcessMouseEvent( QMouseEvent* event );
	bool ProcessWheelEvent( QWheelEvent* event );
}
