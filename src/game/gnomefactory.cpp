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
#include "gnomefactory.h"

#include "../base/config.h"
#include "../base/db.h"
#include "../base/global.h"
#include "../game/game.h"
#include "../game/gnome.h"
#include "../game/gnomemanager.h"
#include "../game/gnometrader.h"
#include "../gfx/sprite.h"
#include "../gfx/spritefactory.h"

#include <QBitmap>
#include <QDebug>
#include <QElapsedTimer>
#include <QPainter>

GnomeFactory::GnomeFactory( Game* game ) :
	g( game )
{
}
GnomeFactory::~GnomeFactory()
{
}

bool GnomeFactory::init()
{
	return true;
}

Gnome* GnomeFactory::createGnome( Position& pos )
{
	QString name  = "NotSet";
	Gender gender = ( rand() % 2 == 0 ) ? Gender::MALE : Gender::FEMALE;

	QList<QVariantMap> vnl;
	if ( gender == Gender::MALE )
	{
		vnl = DB::selectRows( "Names", "Gender", "M" );
	}
	else
	{
		vnl = DB::selectRows( "Names", "Gender", "F" );
	}

	bool foundName = false;
	while ( !foundName )
	{
		foundName = true;
		name      = vnl.value( rand() % vnl.size() ).value( "ID" ).toString();
		name.replace( 0, 1, name[0].toUpper() );
		for ( auto& gn : g->gm()->gnomes() )
		{
			if ( gn->name() == name )
			{
				foundName = false;
				break;
			}
		}
	}

	Gnome* gnome = new Gnome( pos, name, gender, g );
	gnome->init();

	auto attribs = DB::selectRows( "Attributes" );

	for ( auto row : attribs )
	{
		QString attributeID = row.value( "ID" ).toString();

		gnome->addAttribute( attributeID, rand() % 10 + 1 );
	}

	auto needs = DB::selectRows( "Needs" );

	for ( auto row : needs )
	{
		if ( row.value( "Creature" ) == "Gnome" )
		{
			QString needID = row.value( "ID" ).toString();
			int max        = row.value( "Max_" ).toInt();
			gnome->addNeed( needID, max - ( rand() % 20 ) );
		}
	}

	auto skills = DB::selectRows( "Skills" );

	for ( auto skill : skills )
	{
		QString skillID = skill.value( "ID" ).toString();

		gnome->addSkill( skillID, rand() % 2000 );
		gnome->setSkillActive( skillID, true );
	}

	generateTraits( gnome );
	assignBackstory( gnome );

	gnome->updateMoveSpeed();
	gnome->updateAttackValues();

	return gnome;
}

void GnomeFactory::generateTraits( Gnome* gnome )
{
	auto traitDefs = DB::selectRows( "Traits" );
	for ( const auto& row : traitDefs )
	{
		QString traitID = row.value( "ID" ).toString();
		// Bell curve via sum of 3 uniform randoms: range ~-50 to +50, centered ~0
		int value = ( rand() % 34 + rand() % 34 + rand() % 34 ) - 50;
		value = qBound( -50, value, 50 );
		gnome->addTrait( traitID, value );
	}
}

void GnomeFactory::assignBackstory( Gnome* gnome )
{
	auto childhoods = DB::selectRows( "Backstories", "Type", "Childhood" );
	auto adulthoods = DB::selectRows( "Backstories", "Type", "Adulthood" );

	if ( !childhoods.isEmpty() )
	{
		auto childhood = childhoods[rand() % childhoods.size()];
		gnome->setChildhoodBackstory( childhood.value( "ID" ).toString() );
		applyBackstoryModifiers( gnome, childhood );
		applyBackstoryTraitBiases( gnome, childhood );
	}
	if ( !adulthoods.isEmpty() )
	{
		auto adulthood = adulthoods[rand() % adulthoods.size()];
		gnome->setAdulthoodBackstory( adulthood.value( "ID" ).toString() );
		applyBackstoryModifiers( gnome, adulthood );
		applyBackstoryTraitBiases( gnome, adulthood );
	}
}

void GnomeFactory::applyBackstoryModifiers( Gnome* gnome, const QVariantMap& backstory )
{
	QString mods = backstory.value( "SkillModifiers" ).toString();
	if ( mods.isEmpty() ) return;

	for ( const auto& entry : mods.split( '|' ) )
	{
		auto parts = entry.split( ':' );
		if ( parts.size() == 2 )
		{
			QString skillID = parts[0];
			int bonus = parts[1].toInt();
			// Add bonus XP (skills are stored as XP values, not levels)
			int current = gnome->getSkillXP( skillID );
			gnome->setSkillLevel( skillID, current + bonus * 500 );
		}
	}
}

void GnomeFactory::applyBackstoryTraitBiases( Gnome* gnome, const QVariantMap& backstory )
{
	QString biases = backstory.value( "TraitBiases" ).toString();
	if ( biases.isEmpty() ) return;

	for ( const auto& entry : biases.split( '|' ) )
	{
		auto parts = entry.split( ':' );
		if ( parts.size() == 2 )
		{
			QString traitID = parts[0];
			int bias = parts[1].toInt();
			int current = gnome->trait( traitID );
			gnome->addTrait( traitID, current + bias );
		}
	}
}

GnomeTrader* GnomeFactory::createGnomeTrader( Position& pos )
{
	QString name  = "NotSet";
	Gender gender = ( rand() % 2 == 0 ) ? Gender::MALE : Gender::FEMALE;

	QList<QVariantMap> vnl;
	if ( gender == Gender::MALE )
	{
		vnl = DB::selectRows( "Names", "Gender", "M" );
	}
	else
	{
		vnl = DB::selectRows( "Names", "Gender", "F" );
	}
	bool foundName = false;
	while ( !foundName )
	{
		foundName = true;
		name      = vnl.value( rand() % vnl.size() ).value( "ID" ).toString();
		name.replace( 0, 1, name[0].toUpper() );
		for ( auto& gn : g->gm()->gnomes() )
		{
			if ( gn->name() == name )
			{
				foundName = false;
				break;
			}
		}
	}

	GnomeTrader* gnome = new GnomeTrader( pos, name, gender, g );
	gnome->init();
	auto attribs = DB::selectRows( "Attributes" );

	for ( auto row : attribs )
	{
		QString attributeID = row.value( "ID" ).toString();

		gnome->addAttribute( attributeID, rand() % 10 + 1 );
	}

	auto needs = DB::selectRows( "Needs" );

	for ( auto row : needs )
	{
		QString needID = row.value( "ID" ).toString();
		int max        = row.value( "Max_" ).toInt();
		gnome->addNeed( needID, max - ( rand() % 20 ) );
	}

	auto skills = DB::selectRows( "Skills" );

	for ( auto skill : skills )
	{
		QString skillID = skill.value( "ID" ).toString();

		gnome->addSkill( skillID, rand() % 2000 );
		gnome->setSkillActive( skillID, true );
	}

	gnome->updateMoveSpeed();

	return gnome;
}

Gnome* GnomeFactory::createGnome( QVariantMap values )
{
	Gnome* gnome = new Gnome( values, g );
	gnome->init();
	return gnome;
}

GnomeTrader* GnomeFactory::createGnomeTrader( QVariantMap values )
{
	GnomeTrader* gnome = new GnomeTrader( values, g );
	gnome->init();
	return gnome;
}