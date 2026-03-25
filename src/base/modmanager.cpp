/*
	This file is part of Ingnomia https://github.com/rschurade/Ingnomia
    Copyright (C) 2017-2020  Ralph Schurade, Ingnomia Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "modmanager.h"
#include "io.h"
#include "db.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>

ModManager::ModManager()
{
}

ModManager::~ModManager()
{
}

QString ModManager::modsFolder()
{
	return IO::getDataFolder() + "/mods/";
}

void ModManager::scanMods()
{
	m_mods.clear();

	QString folder = modsFolder();
	if ( !QDir( folder ).exists() )
	{
		QDir().mkpath( folder );
		return;
	}

	QDirIterator dirs( folder, QDir::Dirs | QDir::NoDotAndDotDot );
	while ( dirs.hasNext() )
	{
		dirs.next();
		QString modPath = dirs.filePath() + "/";

		// Check for mod.json metadata file
		QString metaPath = modPath + "mod.json";
		if ( !QFile::exists( metaPath ) )
		{
			qDebug() << "Mod folder missing mod.json:" << dirs.fileName();
			continue;
		}

		QFile metaFile( metaPath );
		if ( !metaFile.open( QIODevice::ReadOnly ) )
		{
			continue;
		}

		QJsonDocument doc = QJsonDocument::fromJson( metaFile.readAll() );
		metaFile.close();

		if ( !doc.isObject() )
		{
			continue;
		}

		QJsonObject meta = doc.object();

		ModInfo mod;
		mod.id          = dirs.fileName();
		mod.name        = meta.value( "name" ).toString( dirs.fileName() );
		mod.author      = meta.value( "author" ).toString( "Unknown" );
		mod.version     = meta.value( "version" ).toString( "1.0" );
		mod.description = meta.value( "description" ).toString();
		mod.path        = modPath;
		mod.enabled     = true;
		mod.loadOrder   = m_mods.size();

		qDebug() << "Found mod:" << mod.name << "by" << mod.author << "v" << mod.version;
		m_mods.append( mod );
	}

	qDebug() << "Scanned" << m_mods.size() << "mods";
}

void ModManager::applyMods()
{
	for ( const auto& mod : m_mods )
	{
		if ( mod.enabled )
		{
			qDebug() << "Applying mod:" << mod.name;
			applyMod( mod );
		}
	}
}

void ModManager::setEnabled( const QString& modId, bool enabled )
{
	for ( auto& mod : m_mods )
	{
		if ( mod.id == modId )
		{
			mod.enabled = enabled;
			return;
		}
	}
}

void ModManager::applyMod( const ModInfo& mod )
{
	// Look for JSON files in the mod's data/ subfolder
	// Each JSON file maps to a DB table: items.json → Items table, etc.
	QString dataPath = mod.path + "data/";
	if ( !QDir( dataPath ).exists() )
	{
		return;
	}

	QDirIterator files( dataPath, { "*.json" }, QDir::Files );
	while ( files.hasNext() )
	{
		files.next();
		QString filePath = files.filePath();
		QString tableName = files.fileInfo().baseName(); // "items.json" → "Items"

		// Capitalize first letter to match DB table names
		if ( !tableName.isEmpty() )
		{
			tableName[0] = tableName[0].toUpper();
		}

		QFile file( filePath );
		if ( !file.open( QIODevice::ReadOnly ) )
		{
			continue;
		}

		QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );
		file.close();

		if ( doc.isArray() )
		{
			QVariantList entries = doc.array().toVariantList();
			qDebug() << "  Applying" << entries.size() << "overrides to table" << tableName;
			applyJsonOverrides( tableName, entries );
		}
	}
}

void ModManager::applyJsonOverrides( const QString& tableName, const QVariantList& entries )
{
	// For each entry, update matching row in the DB table by ID
	// Entry format: { "ID": "SomeItem", "Column1": value1, "Column2": value2 }
	for ( const auto& entry : entries )
	{
		QVariantMap row = entry.toMap();
		if ( row.isEmpty() || !row.contains( "ID" ) )
		{
			continue;
		}

		QString id = row.value( "ID" ).toString();

		// Update each field individually using existing DB infrastructure
		for ( auto it = row.constBegin(); it != row.constEnd(); ++it )
		{
			if ( it.key() == "ID" ) continue;

			QString sql = "UPDATE \"" + tableName + "\" SET \"" + it.key() + "\" = '" +
				it.value().toString() + "' WHERE \"ID\" = '" + id + "'";

			bool ok;
			DB::execQuery3( sql, ok );
			if ( !ok )
			{
				qDebug() << "  Mod override failed for" << tableName << id << it.key();
			}
		}
	}
}
