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
#pragma once

#include "../game/gnome.h"
#include "../game/job.h"

class GnomeManager;

enum class AutomatonTier : int
{
	Clockwork = 1, // copper/bronze, 1 labor, slow, needs wind-up
	Steam     = 2, // iron/steel+coal, 2-3 labors, self-powered, needs maintenance
	Arcane    = 3  // mana-infused, any labor, requires mana upkeep
};

class Automaton : public Gnome
{
public:
	Automaton( Position pos, unsigned int automatonItem, Game* game );
	Automaton( QVariantMap& in, Game* game );
	~Automaton();

	virtual void init();

	virtual void serialize( QVariantMap& out );

	virtual void updateSprite();

	virtual CreatureTickResult onTick( quint64 tickNumber, bool seasonChanged, bool dayChanged, bool hourChanged, bool minuteChanged );

	void installCore( unsigned int itemID );
	unsigned int coreItem();

	unsigned int automatonItem()
	{
		return m_automatonItem;
	}

	bool getRefuelFlag();
	void setRefuelFlag( bool flag );

	void setCoreType( QString coreSID );
	QString coreType();

	void uninstallCore( bool uninstall );
	bool uninstallFlag();

	void setMaintenanceJob( QSharedPointer<Job> job );
	unsigned int maintenanceJobID();

	bool maintenanceJobChanged();

	int getFuelLevel();
	void fillUp( int burnValue );

	// Tier system (Milestone 4.2)
	AutomatonTier tier() const { return m_tier; }
	void setTier( AutomatonTier t ) { m_tier = t; }
	int maxLabors() const;           // 1 for Clockwork, 3 for Steam, unlimited for Arcane
	float degradationRate() const;   // HP loss per tick — higher tiers degrade slower
	float workSpeedMultiplier() const; // 0.6 Clockwork, 0.9 Steam, 1.0 Arcane

	static int maxAutomatonsForGnomes( int gnomeCount ) { return gnomeCount / 10; } // Anti-cheese: max 1 per 10 gnomes

protected:
	unsigned int m_automatonItem = 0;
	unsigned int m_core          = 0;
	float m_fuel                 = 0;
	bool m_refuel                = true;
	QString m_coreType;
	bool m_uninstallCore = false;

	AutomatonTier m_tier = AutomatonTier::Clockwork;
	float m_durability   = 100.0f; // 0-100, degrades over time

	QWeakPointer<Job> m_maintenaceJob;
	bool m_maintJobChanged       = false;
};
