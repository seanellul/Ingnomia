/*
	Sprite-to-ImGui texture cache implementation.
*/
#include "spritetexturecache.h"
#include "../base/global.h"
#include "../gfx/sprite.h"
#include "../gfx/spritefactory.h"
#include "../gui/eventconnector.h"
#include "../game/game.h"

#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPixmap>

SpriteTextureCache::SpriteTextureCache()
{
}

SpriteTextureCache::~SpriteTextureCache()
{
	clear();
}

ImTextureID SpriteTextureCache::getTexture( Sprite* sprite, const QString& season, unsigned char rotation )
{
	if ( !sprite ) return (ImTextureID)0;

	// Check cache
	auto it = m_cache.find( sprite->uID );
	if ( it != m_cache.end() )
	{
		return (ImTextureID)(intptr_t)it.value();
	}

	// Get OpenGL functions
	if ( !m_gl )
	{
		auto ctx = QOpenGLContext::currentContext();
		if ( !ctx ) return (ImTextureID)0;
		m_gl = ctx->functions();
	}

	// Get pixmap from sprite and convert to QImage
	QPixmap& pm = sprite->pixmap( season, rotation, 0 );
	if ( pm.isNull() ) return (ImTextureID)0;

	QImage img = pm.toImage().convertToFormat( QImage::Format_RGBA8888 );
	if ( img.isNull() ) return (ImTextureID)0;

	// Create OpenGL texture
	GLuint texID = 0;
	m_gl->glGenTextures( 1, &texID );
	m_gl->glBindTexture( GL_TEXTURE_2D, texID );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	m_gl->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.constBits() );
	m_gl->glBindTexture( GL_TEXTURE_2D, 0 );

	// Cache and return
	m_cache.insert( sprite->uID, texID );
	return (ImTextureID)(intptr_t)texID;
}

ImTextureID SpriteTextureCache::getTextureForItem( const QString& itemSID, const QStringList& materialSIDs )
{
	if ( !Global::eventConnector || !Global::eventConnector->game() ) return (ImTextureID)0;

	Sprite* sprite = Global::eventConnector->game()->sf()->createSprite( itemSID, materialSIDs );
	if ( !sprite ) return (ImTextureID)0;

	return getTexture( sprite );
}

ImTextureID SpriteTextureCache::getTextureFromBuffer( unsigned int key, const unsigned char* data, int width, int height )
{
	if ( !data || width <= 0 || height <= 0 ) return (ImTextureID)0;

	// Check cache
	auto it = m_cache.find( key );
	if ( it != m_cache.end() )
	{
		return (ImTextureID)(intptr_t)it.value();
	}

	// Get OpenGL functions
	if ( !m_gl )
	{
		auto ctx = QOpenGLContext::currentContext();
		if ( !ctx ) return (ImTextureID)0;
		m_gl = ctx->functions();
	}

	// Create OpenGL texture from raw RGBA buffer
	GLuint texID = 0;
	m_gl->glGenTextures( 1, &texID );
	m_gl->glBindTexture( GL_TEXTURE_2D, texID );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	m_gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	m_gl->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
	m_gl->glBindTexture( GL_TEXTURE_2D, 0 );

	m_cache.insert( key, texID );
	return (ImTextureID)(intptr_t)texID;
}

void SpriteTextureCache::clear()
{
	if ( m_gl && !m_cache.isEmpty() )
	{
		for ( auto texID : m_cache )
		{
			m_gl->glDeleteTextures( 1, &texID );
		}
	}
	m_cache.clear();
}
