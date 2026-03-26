#pragma once

#include <QOpenGLWindow>

struct ImFont;

struct ImGuiFonts
{
	ImFont* title    = nullptr; // HermeneusOne ~48px (decorative title)
	ImFont* ui       = nullptr; // PT Root UI ~18px (default UI font)
	ImFont* uiSmall  = nullptr; // PT Root UI ~14px (version, captions)
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
