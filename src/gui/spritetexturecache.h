/*
	Sprite-to-ImGui texture cache.
	Converts game Sprite QPixmaps to OpenGL textures usable by ImGui::Image().
*/
#pragma once

#include <QHash>
#include <QOpenGLFunctions>
#include <imgui.h>

class Sprite;

class SpriteTextureCache
{
public:
	SpriteTextureCache();
	~SpriteTextureCache();

	// Get an ImGui-compatible texture ID for a sprite. Cached after first creation.
	ImTextureID getTexture( Sprite* sprite, const QString& season = "Spring", unsigned char rotation = 0 );

	// Get texture for a sprite by creating one from itemSID + materialSIDs
	ImTextureID getTextureForItem( const QString& itemSID, const QStringList& materialSIDs );

	// Clear all cached textures
	void clear();

private:
	QHash<unsigned int, GLuint> m_cache; // sprite uID → GL texture ID
	QOpenGLFunctions* m_gl = nullptr;
};
