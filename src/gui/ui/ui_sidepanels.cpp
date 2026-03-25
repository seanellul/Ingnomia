#include "ui_sidepanels.h"
#include "../imguibridge.h"
#include "../../base/global.h"
#include <imgui.h>

// =============================================================================
// Kingdom / Inventory panel
// =============================================================================
void drawKingdomPanel( ImGuiBridge& bridge )
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Kingdom - Inventory", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.activeSidePanel = ImGuiBridge::SidePanel::None;

	ImGui::Columns( 4, "invCols" );
	ImGui::Text( "Category" ); ImGui::NextColumn();
	ImGui::Text( "In Stock" ); ImGui::NextColumn();
	ImGui::Text( "Total" ); ImGui::NextColumn();
	ImGui::Text( "" ); ImGui::NextColumn();
	ImGui::Separator();

	for ( const auto& cat : bridge.inventoryCategories )
	{
		ImGui::Text( "%s", cat.name.toStdString().c_str() ); ImGui::NextColumn();
		ImGui::Text( "%d", cat.countInStockpiles ); ImGui::NextColumn();
		ImGui::Text( "%d", cat.countTotal ); ImGui::NextColumn();
		ImGui::Text( "" ); ImGui::NextColumn();
	}

	ImGui::Columns( 1 );
	ImGui::End();
}

// =============================================================================
// Stockpile management
// =============================================================================
void drawStockpilePanel( ImGuiBridge& bridge )
{

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Stockpile Management", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open )
	{
		bridge.cmdCloseStockpileWindow();
	}

	auto& sp = bridge.stockpileInfo;

	// Name
	static char name[128];
	snprintf( name, sizeof( name ), "%s", sp.name.toStdString().c_str() );
	if ( ImGui::InputText( "Name", name, sizeof( name ), ImGuiInputTextFlags_EnterReturnsTrue ) )
	{
		bridge.cmdStockpileSetOptions( bridge.activeStockpileID, name, sp.priority, sp.suspended, sp.pullFromOthers, sp.allowPullFromHere );
	}

	// Priority
	int prio = sp.priority;
	if ( ImGui::SliderInt( "Priority", &prio, 0, sp.maxPriority ) )
	{
		bridge.cmdStockpileSetOptions( bridge.activeStockpileID, sp.name, prio, sp.suspended, sp.pullFromOthers, sp.allowPullFromHere );
	}

	// Toggles
	bool suspended = sp.suspended;
	if ( ImGui::Checkbox( "Suspended", &suspended ) )
	{
		bridge.cmdStockpileSetOptions( bridge.activeStockpileID, sp.name, sp.priority, suspended, sp.pullFromOthers, sp.allowPullFromHere );
	}

	bool pullOthers = sp.pullFromOthers;
	ImGui::SameLine();
	if ( ImGui::Checkbox( "Pull from others", &pullOthers ) )
	{
		bridge.cmdStockpileSetOptions( bridge.activeStockpileID, sp.name, sp.priority, sp.suspended, pullOthers, sp.allowPullFromHere );
	}

	bool allowPull = sp.allowPullFromHere;
	ImGui::SameLine();
	if ( ImGui::Checkbox( "Allow pull", &allowPull ) )
	{
		bridge.cmdStockpileSetOptions( bridge.activeStockpileID, sp.name, sp.priority, sp.suspended, sp.pullFromOthers, allowPull );
	}

	ImGui::Separator();
	ImGui::Text( "Capacity: %d / %d items (%d reserved)", sp.itemCount, sp.capacity, sp.reserved );
	ImGui::TextDisabled( "Filter editing: use Manage from tile info for now" );

	ImGui::End();
}

// =============================================================================
// Population panel
// =============================================================================
void drawPopulationPanel( ImGuiBridge& bridge )
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Population", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.activeSidePanel = ImGuiBridge::SidePanel::None;

	if ( ImGui::BeginTabBar( "PopTabs" ) )
	{
		if ( ImGui::BeginTabItem( "Skills" ) )
		{
			// Column headers
			if ( !bridge.populationInfo.gnomes.isEmpty() && !bridge.populationInfo.gnomes[0].skills.isEmpty() )
			{
				ImGui::Columns( bridge.populationInfo.gnomes[0].skills.size() + 2 );
				ImGui::Text( "Name" ); ImGui::NextColumn();
				ImGui::Text( "Profession" ); ImGui::NextColumn();

				for ( const auto& skill : bridge.populationInfo.gnomes[0].skills )
				{
					// Vertical-ish text (abbreviated)
					ImGui::Text( "%s", skill.name.left( 4 ).toStdString().c_str() );
					ImGui::NextColumn();
				}
				ImGui::Separator();

				for ( const auto& gnome : bridge.populationInfo.gnomes )
				{
					ImGui::Text( "%s", gnome.name.toStdString().c_str() );
					ImGui::NextColumn();

					ImGui::Text( "%s", gnome.profession.toStdString().c_str() );
					ImGui::NextColumn();

					for ( const auto& skill : gnome.skills )
					{
						ImGui::PushID( ( QString::number( gnome.id ) + skill.sid ).toStdString().c_str() );
						bool active = skill.active;
						if ( ImGui::Checkbox( "", &active ) )
						{
							bridge.cmdSetSkillActive( gnome.id, skill.sid, active );
						}
						if ( ImGui::IsItemHovered() )
						{
							ImGui::SetTooltip( "%s: Level %d", skill.name.toStdString().c_str(), skill.level );
						}
						ImGui::PopID();
						ImGui::NextColumn();
					}
				}

				ImGui::Columns( 1 );
			}
			else
			{
				ImGui::TextDisabled( "No gnomes" );
			}

			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem( "Schedule" ) )
		{
			if ( bridge.scheduleInfo.schedules.isEmpty() )
			{
				bridge.cmdRequestSchedules();
				ImGui::TextDisabled( "Loading schedules..." );
			}
			else
			{
				ImGui::Columns( 26 );
				ImGui::SetColumnWidth( 0, 120.0f );
				for ( int col = 1; col <= 25; ++col )
					ImGui::SetColumnWidth( col, 28.0f );
				ImGui::Text( "Name" ); ImGui::NextColumn();
				for ( int h = 0; h < 24; ++h )
				{
					ImGui::Text( "%02d", h ); ImGui::NextColumn();
				}
				ImGui::Text( "" ); ImGui::NextColumn();
				ImGui::Separator();

				for ( const auto& gnome : bridge.scheduleInfo.schedules )
				{
					ImGui::Text( "%s", gnome.name.toStdString().c_str() ); ImGui::NextColumn();
					for ( int h = 0; h < 24; ++h )
					{
						ImGui::PushID( gnome.id * 100 + h );
						const char* label = "W";
						ImVec4 col( 0.3f, 0.3f, 0.3f, 1.0f );
						if ( h < gnome.schedule.size() )
						{
							switch ( gnome.schedule[h] )
							{
								case ScheduleActivity::None: label = "W"; col = ImVec4( 0.2f, 0.5f, 0.2f, 1.0f ); break;
								case ScheduleActivity::Eat: label = "E"; col = ImVec4( 0.7f, 0.4f, 0.0f, 1.0f ); break;
								case ScheduleActivity::Sleep: label = "S"; col = ImVec4( 0.0f, 0.3f, 0.7f, 1.0f ); break;
								case ScheduleActivity::Training: label = "T"; col = ImVec4( 0.7f, 0.0f, 0.0f, 1.0f ); break;
							}
						}
						ImGui::PushStyleColor( ImGuiCol_Button, col );
						if ( ImGui::SmallButton( label ) )
						{
							// Cycle through activities
							ScheduleActivity next = ScheduleActivity::None;
							if ( h < gnome.schedule.size() )
							{
								switch ( gnome.schedule[h] )
								{
									case ScheduleActivity::None: next = ScheduleActivity::Eat; break;
									case ScheduleActivity::Eat: next = ScheduleActivity::Sleep; break;
									case ScheduleActivity::Sleep: next = ScheduleActivity::Training; break;
									default: next = ScheduleActivity::None; break;
								}
							}
							bridge.cmdSetSchedule( gnome.id, h, next );
						}
						ImGui::PopStyleColor();
						ImGui::PopID();
						ImGui::NextColumn();
					}
					ImGui::Text( "" ); ImGui::NextColumn();
				}

				ImGui::Columns( 1 );
			}

			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem( "Professions" ) )
		{
			if ( bridge.professionList.isEmpty() )
			{
				bridge.cmdRequestProfessions();
			}

			if ( ImGui::Button( "New Profession" ) )
			{
				bridge.cmdNewProfession();
				bridge.cmdRequestProfessions();
			}

			for ( const auto& prof : bridge.professionList )
			{
				if ( ImGui::Selectable( prof.toStdString().c_str(), bridge.editingProfession == prof ) )
				{
					bridge.cmdRequestSkills( prof );
				}
			}

			if ( !bridge.editingProfession.isEmpty() && !bridge.professionSkills.isEmpty() )
			{
				ImGui::Separator();
				ImGui::Text( "Edit: %s", bridge.editingProfession.toStdString().c_str() );

				for ( auto& skill : bridge.professionSkills )
				{
					bool active = skill.active;
					if ( ImGui::Checkbox( skill.name.toStdString().c_str(), &active ) )
					{
						skill.active = active;
						// Collect all active skills and update
						QStringList activeSkills;
						for ( const auto& s : bridge.professionSkills )
						{
							if ( s.active ) activeSkills.append( s.sid );
						}
						bridge.cmdUpdateProfession( bridge.editingProfession, bridge.editingProfession, activeSkills );
					}
				}
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

// =============================================================================
// Military panel
// =============================================================================
void drawMilitaryPanel( ImGuiBridge& bridge )
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Military", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.activeSidePanel = ImGuiBridge::SidePanel::None;

	if ( ImGui::BeginTabBar( "MilTabs" ) )
	{
		if ( ImGui::BeginTabItem( "Squads" ) )
		{
			if ( ImGui::Button( "Add Squad" ) )
			{
				bridge.cmdAddSquad();
				bridge.cmdRequestMilitaryUpdate();
			}

			for ( const auto& squad : bridge.squads )
			{
				ImGui::PushID( squad.id );

				if ( ImGui::CollapsingHeader( squad.name.toStdString().c_str() ) )
				{
					char squadName[128];
					snprintf( squadName, sizeof( squadName ), "%s", squad.name.toStdString().c_str() );
					if ( ImGui::InputText( "Name", squadName, sizeof( squadName ), ImGuiInputTextFlags_EnterReturnsTrue ) )
					{
						bridge.cmdRenameSquad( squad.id, squadName );
					}

					for ( const auto& gnome : squad.gnomes )
					{
						ImGui::Text( "  %s", gnome.name.toStdString().c_str() );
						ImGui::SameLine();
						ImGui::PushID( (int)gnome.id );
						if ( ImGui::SmallButton( "Remove" ) )
						{
							bridge.cmdRemoveGnomeFromSquad( gnome.id );
							bridge.cmdRequestMilitaryUpdate();
						}
						ImGui::PopID();
					}

					// Add gnome to squad - show available gnomes from population
					if ( !bridge.populationInfo.gnomes.isEmpty() )
					{
						static int selectedGnome = 0;
						if ( ImGui::Button( "+ Add Gnome" ) )
						{
							if ( selectedGnome >= 0 && selectedGnome < bridge.populationInfo.gnomes.size() )
							{
								bridge.cmdMoveGnomeRight( bridge.populationInfo.gnomes[selectedGnome].id );
								bridge.cmdRequestMilitaryUpdate();
							}
						}
					}

					if ( ImGui::SmallButton( "Remove Squad" ) )
					{
						bridge.cmdRemoveSquad( squad.id );
						bridge.cmdRequestMilitaryUpdate();
					}
				}

				ImGui::PopID();
			}

			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem( "Roles" ) )
		{
			if ( bridge.roles.isEmpty() )
			{
				bridge.cmdRequestRoles();
			}

			if ( ImGui::Button( "Add Role" ) )
			{
				bridge.cmdAddRole();
				bridge.cmdRequestRoles();
			}

			for ( const auto& role : bridge.roles )
			{
				ImGui::PushID( role.id );
				if ( ImGui::CollapsingHeader( role.name.toStdString().c_str() ) )
				{
					bool civ = role.isCivilian;
					if ( ImGui::Checkbox( "Civilian", &civ ) )
					{
						bridge.cmdSetRoleCivilian( role.id, civ );
					}
				}
				ImGui::PopID();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

// =============================================================================
// Neighbors panel
// =============================================================================
void drawNeighborsPanel( ImGuiBridge& bridge )
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Neighbors & Missions", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.activeSidePanel = ImGuiBridge::SidePanel::None;

	if ( ImGui::BeginTabBar( "NeighborTabs" ) )
	{
		if ( ImGui::BeginTabItem( "Neighbors" ) )
		{
			for ( const auto& n : bridge.neighbors )
			{
				ImGui::PushID( n.id );
				if ( ImGui::CollapsingHeader( n.name.toStdString().c_str(), ImGuiTreeNodeFlags_DefaultOpen ) )
				{
					ImGui::Text( "Distance: %s", n.distance.toStdString().c_str() );
					ImGui::Text( "Wealth: %s", n.wealth.toStdString().c_str() );
					ImGui::Text( "Economy: %s", n.economy.toStdString().c_str() );
					ImGui::Text( "Military: %s", n.military.toStdString().c_str() );
					ImGui::Text( "Attitude: %s", n.attitude.toStdString().c_str() );

					if ( ImGui::Button( "Send Ambassador" ) )
					{
						bridge.cmdRequestAvailableGnomes();
						// TODO: Show gnome selection dialog
					}
				}
				ImGui::PopID();
			}

			if ( bridge.neighbors.isEmpty() )
			{
				ImGui::TextDisabled( "No neighbors discovered" );
			}

			ImGui::EndTabItem();
		}

		if ( ImGui::BeginTabItem( "Missions" ) )
		{
			if ( bridge.missions.isEmpty() )
			{
				ImGui::TextDisabled( "No active missions" );
			}
			else
			{
				for ( const auto& m : bridge.missions )
				{
					ImGui::Text( "Mission #%u (distance: %d days)", m.id, m.distance );
				}
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

// =============================================================================
// Workshop panel
// =============================================================================
void drawWorkshopPanel( ImGuiBridge& bridge )
{
	if ( !bridge.showWorkshopWindow )
		return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Workshop", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.cmdCloseWorkshopWindow();

	auto& ws = bridge.workshopInfo;

	static char wsName[128];
	snprintf( wsName, sizeof( wsName ), "%s", ws.name.toStdString().c_str() );
	if ( ImGui::InputText( "Name", wsName, sizeof( wsName ), ImGuiInputTextFlags_EnterReturnsTrue ) )
	{
		bridge.cmdWorkshopSetOptions( bridge.activeWorkshopID, wsName, ws.priority, ws.suspended, ws.acceptGenerated, ws.autoCraftMissing, ws.linkStockpile );
	}

	int prio = ws.priority;
	if ( ImGui::SliderInt( "Priority", &prio, 0, 9 ) )
	{
		bridge.cmdWorkshopSetOptions( bridge.activeWorkshopID, ws.name, prio, ws.suspended, ws.acceptGenerated, ws.autoCraftMissing, ws.linkStockpile );
	}

	bool suspended = ws.suspended;
	if ( ImGui::Checkbox( "Suspended", &suspended ) )
	{
		bridge.cmdWorkshopSetOptions( bridge.activeWorkshopID, ws.name, ws.priority, suspended, ws.acceptGenerated, ws.autoCraftMissing, ws.linkStockpile );
	}

	ImGui::Separator();
	ImGui::Text( "Craft Queue:" );

	for ( const auto& job : ws.jobList )
	{
		ImGui::PushID( (int)job.id );
		ImGui::Text( "%s x%d", job.itemSID.toStdString().c_str(), job.numItemsToCraft );
		ImGui::SameLine();
		if ( ImGui::SmallButton( "Up" ) ) bridge.cmdWorkshopCraftJobCommand( job.id, "Up" );
		ImGui::SameLine();
		if ( ImGui::SmallButton( "Down" ) ) bridge.cmdWorkshopCraftJobCommand( job.id, "Down" );
		ImGui::SameLine();
		if ( ImGui::SmallButton( "X" ) ) bridge.cmdWorkshopCraftJobCommand( job.id, "Delete" );
		ImGui::PopID();
	}

	ImGui::End();
}

// =============================================================================
// Agriculture panel
// =============================================================================
void drawAgriculturePanel( ImGuiBridge& bridge )
{
	if ( !bridge.showAgriWindow )
		return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( 5, 50 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10, io.DisplaySize.y - 110 ) );

	bool open = true;
	ImGui::Begin( "Agriculture", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open ) bridge.cmdCloseAgriWindow();

	auto& farm = bridge.farmInfo;

	if ( !farm.name.isEmpty() )
	{
		static char farmName[128];
		snprintf( farmName, sizeof( farmName ), "%s", farm.name.toStdString().c_str() );
		if ( ImGui::InputText( "Name", farmName, sizeof( farmName ), ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			bridge.cmdAgriSetOptions( bridge.activeAgriID, farmName, farm.priority, farm.suspended );
		}

		bool suspended = farm.suspended;
		if ( ImGui::Checkbox( "Suspended", &suspended ) )
		{
			bridge.cmdAgriSetOptions( bridge.activeAgriID, farm.name, farm.priority, suspended );
		}

		bool harvest = farm.harvest;
		if ( ImGui::Checkbox( "Harvest", &harvest ) )
		{
			bridge.cmdAgriSetHarvestOptions( bridge.activeAgriID, harvest, false, false );
		}

		ImGui::Text( "Plots: %d  Tilled: %d  Planted: %d  Ready: %d", farm.numPlots, farm.tilled, farm.planted, farm.cropReady );
	}

	ImGui::End();
}

// =============================================================================
// Creature info panel
// =============================================================================
void drawCreatureInfoPanel( ImGuiBridge& bridge )
{
	if ( !bridge.showCreatureInfo )
		return;

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos( ImVec2( io.DisplaySize.x - 310, 100 ) );
	ImGui::SetNextWindowSize( ImVec2( 300, 400 ) );

	bool open = true;
	ImGui::Begin( "Creature Info", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if ( !open )
	{
		bridge.showCreatureInfo = false;
		bridge.activeSidePanel = ImGuiBridge::SidePanel::None;
	}

	auto& ci = bridge.creatureInfo;

	ImGui::Text( "%s", ci.name.toStdString().c_str() );
	ImGui::Text( "Profession: %s", ci.profession.toStdString().c_str() );

	ImGui::Separator();

	// Needs bars
	ImGui::Text( "Hunger" ); ImGui::SameLine( 100 ); ImGui::ProgressBar( ci.hunger / 100.0f, ImVec2( -1, 0 ) );
	ImGui::Text( "Thirst" ); ImGui::SameLine( 100 ); ImGui::ProgressBar( ci.thirst / 100.0f, ImVec2( -1, 0 ) );
	ImGui::Text( "Sleep" ); ImGui::SameLine( 100 ); ImGui::ProgressBar( ci.sleep / 100.0f, ImVec2( -1, 0 ) );
	ImGui::Text( "Happiness" ); ImGui::SameLine( 100 ); ImGui::ProgressBar( ci.happiness / 100.0f, ImVec2( -1, 0 ) );

	ImGui::Separator();

	if ( ImGui::CollapsingHeader( "Stats" ) )
	{
		ImGui::Text( "STR: %d  DEX: %d  CON: %d", ci.str, ci.dex, ci.con );
		ImGui::Text( "INT: %d  WIS: %d  CHA: %d", ci.intel, ci.wis, ci.cha );
	}

	if ( !ci.activity.isEmpty() )
	{
		ImGui::Text( "Activity: %s", ci.activity.toStdString().c_str() );
	}

	ImGui::End();
}

// =============================================================================
// Inventory panel (same as Kingdom but with checkboxes)
// =============================================================================
void drawInventoryPanel( ImGuiBridge& bridge )
{
	// Kingdom panel already handles this
	drawKingdomPanel( bridge );
}

// =============================================================================
// Debug panel
// =============================================================================
void drawDebugPanel( ImGuiBridge& bridge )
{
	if ( !bridge.showDebugPanel )
		return;

	ImGui::SetNextWindowSize( ImVec2( 300, 200 ), ImGuiCond_FirstUseEver );

	ImGui::Begin( "Debug", &bridge.showDebugPanel );

	static char creatureType[64] = "Gnome";
	ImGui::InputText( "Creature type", creatureType, sizeof( creatureType ) );
	if ( ImGui::Button( "Spawn" ) )
	{
		bridge.cmdSpawnCreature( creatureType );
	}

	ImGui::End();
}
