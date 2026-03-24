#pragma once

#include <QOpenGLWindow>

namespace ImGuiQt5
{
	void Init( QOpenGLWindow* window );
	void Shutdown();
	void NewFrame( QOpenGLWindow* window );

	bool ProcessKeyEvent( QKeyEvent* event );
	bool ProcessMouseEvent( QMouseEvent* event );
	bool ProcessWheelEvent( QWheelEvent* event );
}
