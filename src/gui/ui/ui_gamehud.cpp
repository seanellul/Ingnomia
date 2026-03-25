#include "ui_gamehud.h"
#include "../imguibridge.h"
#include "../../base/global.h"
#include "../../base/gamestate.h"

#include <imgui.h>

namespace
{

struct ToolbarButton
{
	const char* label;
	ButtonSelection selection;
};

const ToolbarButton toolbarButtons[] = {
	{ "Build", ButtonSelection::Build },
	{ "Mine", ButtonSelection::Mine },
	{ "Agriculture", ButtonSelection::Agriculture },
	{ "Designations", ButtonSelection::Designation },
	{ "Job", ButtonSelection::Job },
	{ "Magic", ButtonSelection::Magic },
};

// Build subcategories
struct BuildCategoryButton
{
	const char* label;
	BuildSelection selection;
};

const BuildCategoryButton buildCategories[] = {
	{ "Workshop", BuildSelection::Workshop },
	{ "Wall", BuildSelection::Wall },
	{ "Floor", BuildSelection::Floor },
	{ "Stairs", BuildSelection::Stairs },
	{ "Ramp", BuildSelection::Ramps },
	{ "Fence", BuildSelection::Fence },
	{ "Containers", BuildSelection::Containers },
	{ "Furniture", BuildSelection::Furniture },
	{ "Utility", BuildSelection::Utility },
};

// Mine actions
const char* mineActions[] = { "Mine", "ExplorativeMine", "RemoveFloor", "DigStairsDown", "DigStairsUp", "DigRampDown", "DigHole" };
const char* mineLabels[] = { "Mine", "Explorative Mine", "Remove floor", "Stairs down", "Stairs up", "Ramp", "Hole" };

// Agriculture actions
const char* agriActions[] = { "CutTree", "PlantTree", "FellTree", "Forage", "RemovePlant" };
const char* agriLabels[] = { "Cut tree", "Plant tree", "Harvest tree", "Forage", "Remove plant" };

// Designation zones
const char* designActions[] = { "Stockpile", "FarmField", "Grove", "Pasture", "PersonalRoom", "Dormitory", "DiningHall", "Hospital", "Forbidden", "Guard", "RemoveDesignation" };
const char* designLabels[] = { "Stockpile", "Farm", "Grove", "Pasture", "Personal Room", "Dormitory", "Dining Hall", "Hospital", "Forbidden", "Guard", "Remove" };

// Job actions
const char* jobActions[] = { "SuspendJob", "ResumeJob", "CancelJob", "LowerPriority", "RaisePriority" };
const char* jobLabels[] = { "Suspend", "Resume", "Cancel", "Lower priority", "Raise priority" };

void drawBuildItemList( ImGuiBridge& bridge )
{
	if ( bridge.buildItems.isEmpty() )
		return;

	ImGui::SetNextWindowPos( ImVec2( 120, ImGui::GetIO().DisplaySize.y - 200 ), ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2( 500, 180 ) );
	ImGui::Begin( "Build Items", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

	for ( const auto& item : bridge.buildItems )
	{
		ImGui::PushID( item.id.toStdString().c_str() );

		ImGui::Text( "%s", item.name.toStdString().c_str() );
		ImGui::SameLine( 250 );

		// Material dropdowns - use first available material for each required item
		QStringList mats;
		bool canBuild = true;
		for ( const auto& req : item.requiredItems )
		{
			if ( req.availableMats.isEmpty() )
			{
				canBuild = false;
				break;
			}
			mats.append( req.availableMats.first().first );
		}

		if ( canBuild )
		{
			if ( ImGui::SmallButton( "Build" ) )
			{
				bridge.cmdBuild( item.biType, "", item.id, mats );
			}
		}
		else
		{
			ImGui::TextDisabled( "(no materials)" );
		}

		ImGui::PopID();
	}

	ImGui::End();
}

} // namespace

void drawGameHUD( ImGuiBridge& bridge )
{
	ImGuiIO& io = ImGui::GetIO();

	// =========================================================================
	// Top-left: Kingdom info + render toggles
	// =========================================================================
	ImGui::SetNextWindowPos( ImVec2( 5, 5 ) );
	ImGui::SetNextWindowSize( ImVec2( 250, 0 ) );
	ImGui::Begin( "##kingdom", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground );

	ImGui::Text( "%s", bridge.kingdomName.toStdString().c_str() );
	ImGui::Text( "%s", bridge.kingdomInfo1.toStdString().c_str() );
	ImGui::Text( "%s", bridge.kingdomInfo2.toStdString().c_str() );
	ImGui::Text( "%s", bridge.kingdomInfo3.toStdString().c_str() );
	ImGui::Text( "Level: %d", bridge.viewLevel );

	// DJWA toggles
	bool d = bridge.renderDesignations, j = bridge.renderJobs, w = bridge.renderWalls, a = bridge.renderAxles;
	bool changed = false;
	ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 3.0f );
	if ( ImGui::Checkbox( "D", &d ) ) { bridge.renderDesignations = d; changed = true; }
	ImGui::SameLine();
	if ( ImGui::Checkbox( "J", &j ) ) { bridge.renderJobs = j; changed = true; }
	ImGui::SameLine();
	if ( ImGui::Checkbox( "W", &w ) ) { bridge.renderWalls = w; changed = true; }
	ImGui::SameLine();
	if ( ImGui::Checkbox( "A", &a ) ) { bridge.renderAxles = a; changed = true; }
	ImGui::PopStyleVar();

	if ( changed )
	{
		bridge.cmdSetRenderOptions( d, j, w, a );
	}

	ImGui::End();

	// =========================================================================
	// Top-right: Time + speed controls
	// =========================================================================
	ImGui::SetNextWindowPos( ImVec2( io.DisplaySize.x - 200, 5 ) );
	ImGui::SetNextWindowSize( ImVec2( 195, 0 ) );
	ImGui::Begin( "##time", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground );

	ImGui::Text( "Day %d of %s", bridge.day, bridge.season.toStdString().c_str() );
	ImGui::Text( "Year %d   %02d:%02d", bridge.year, bridge.hour, bridge.minute );

	// Speed controls
	if ( ImGui::SmallButton( bridge.paused ? ">" : "||" ) )
	{
		bridge.cmdSetPaused( !bridge.paused );
	}
	ImGui::SameLine();
	bool isNormal = ( bridge.gameSpeed == GameSpeed::Normal && !bridge.paused );
	bool isFast = ( bridge.gameSpeed == GameSpeed::Fast && !bridge.paused );
	if ( isNormal ) ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.2f, 0.6f, 0.2f, 1.0f ) );
	if ( ImGui::SmallButton( ">>" ) )
	{
		bridge.cmdSetPaused( false );
		bridge.cmdSetGameSpeed( GameSpeed::Normal );
	}
	if ( isNormal ) ImGui::PopStyleColor();
	ImGui::SameLine();
	if ( isFast ) ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.2f, 0.6f, 0.2f, 1.0f ) );
	if ( ImGui::SmallButton( ">>>" ) )
	{
		bridge.cmdSetPaused( false );
		bridge.cmdSetGameSpeed( GameSpeed::Fast );
	}
	if ( isFast ) ImGui::PopStyleColor();

	ImGui::End();

	// =========================================================================
	// Bottom toolbar
	// =========================================================================
	float toolbarHeight = 50;
	ImGui::SetNextWindowPos( ImVec2( 0, io.DisplaySize.y - toolbarHeight ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x, toolbarHeight ) );
	ImGui::Begin( "##toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar );

	// Left side: action buttons
	for ( int i = 0; i < 6; ++i )
	{
		if ( i > 0 ) ImGui::SameLine();
		bool active = ( bridge.currentToolbar == toolbarButtons[i].selection );
		if ( active ) ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.3f, 0.5f, 0.7f, 1.0f ) );
		if ( ImGui::Button( toolbarButtons[i].label, ImVec2( 90, 30 ) ) )
		{
			if ( active )
			{
				bridge.currentToolbar = ButtonSelection::None;
				bridge.currentBuildCategory = BuildSelection::None;
				bridge.buildItems.clear();
			}
			else
			{
				bridge.currentToolbar = toolbarButtons[i].selection;
				bridge.currentBuildCategory = BuildSelection::None;
				bridge.buildItems.clear();
			}
		}
		if ( active ) ImGui::PopStyleColor();
	}

	// Right side: management panels
	ImGui::SameLine( io.DisplaySize.x - 500 );
	if ( ImGui::Button( "Kingdom", ImVec2( 80, 30 ) ) )
	{
		bridge.activeSidePanel = bridge.activeSidePanel == ImGuiBridge::SidePanel::Kingdom ? ImGuiBridge::SidePanel::None : ImGuiBridge::SidePanel::Kingdom;
		if ( bridge.activeSidePanel == ImGuiBridge::SidePanel::Kingdom )
			bridge.cmdRequestInventoryCategories();
	}
	ImGui::SameLine();
	if ( ImGui::Button( "Stockpiles", ImVec2( 80, 30 ) ) )
	{
		bridge.activeSidePanel = bridge.activeSidePanel == ImGuiBridge::SidePanel::Stockpile ? ImGuiBridge::SidePanel::None : ImGuiBridge::SidePanel::Stockpile;
	}
	ImGui::SameLine();
	if ( ImGui::Button( "Military", ImVec2( 80, 30 ) ) )
	{
		bridge.activeSidePanel = bridge.activeSidePanel == ImGuiBridge::SidePanel::Military ? ImGuiBridge::SidePanel::None : ImGuiBridge::SidePanel::Military;
		if ( bridge.activeSidePanel == ImGuiBridge::SidePanel::Military )
			bridge.cmdRequestMilitaryUpdate();
	}
	ImGui::SameLine();
	if ( ImGui::Button( "Population", ImVec2( 80, 30 ) ) )
	{
		bridge.activeSidePanel = bridge.activeSidePanel == ImGuiBridge::SidePanel::Population ? ImGuiBridge::SidePanel::None : ImGuiBridge::SidePanel::Population;
		if ( bridge.activeSidePanel == ImGuiBridge::SidePanel::Population )
			bridge.cmdRequestPopulationUpdate();
	}
	ImGui::SameLine();
	if ( ImGui::Button( "Missions", ImVec2( 80, 30 ) ) )
	{
		bridge.activeSidePanel = bridge.activeSidePanel == ImGuiBridge::SidePanel::Missions ? ImGuiBridge::SidePanel::None : ImGuiBridge::SidePanel::Missions;
		if ( bridge.activeSidePanel == ImGuiBridge::SidePanel::Missions )
			bridge.cmdRequestNeighborsUpdate();
	}

	ImGui::End();

	// =========================================================================
	// Toolbar expansion panels (above the bottom toolbar)
	// =========================================================================
	if ( bridge.currentToolbar == ButtonSelection::Build )
	{
		ImGui::SetNextWindowPos( ImVec2( 5, io.DisplaySize.y - toolbarHeight - 280 ) );
		ImGui::SetNextWindowSize( ImVec2( 130, 270 ) );
		ImGui::Begin( "##buildcats", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

		for ( int i = 0; i < 9; ++i )
		{
			bool active = ( bridge.currentBuildCategory == buildCategories[i].selection );
			if ( active ) ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.3f, 0.5f, 0.7f, 1.0f ) );
			if ( ImGui::Button( buildCategories[i].label, ImVec2( 110, 25 ) ) )
			{
				bridge.currentBuildCategory = buildCategories[i].selection;
				bridge.cmdRequestBuildItems( buildCategories[i].selection, "" );
			}
			if ( active ) ImGui::PopStyleColor();
		}

		if ( ImGui::Button( "Deconstruct", ImVec2( 110, 25 ) ) )
		{
			bridge.cmdSetSelectionAction( "Deconstruct" );
		}

		ImGui::End();

		// Build items list
		drawBuildItemList( bridge );
	}
	else if ( bridge.currentToolbar == ButtonSelection::Mine )
	{
		ImGui::SetNextWindowPos( ImVec2( 5, io.DisplaySize.y - toolbarHeight - 200 ) );
		ImGui::SetNextWindowSize( ImVec2( 160, 190 ) );
		ImGui::Begin( "##mineactions", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

		for ( int i = 0; i < 7; ++i )
		{
			if ( ImGui::Button( mineLabels[i], ImVec2( 140, 22 ) ) )
			{
				bridge.cmdSetSelectionAction( mineActions[i] );
			}
		}

		ImGui::End();
	}
	else if ( bridge.currentToolbar == ButtonSelection::Agriculture )
	{
		ImGui::SetNextWindowPos( ImVec2( 5, io.DisplaySize.y - toolbarHeight - 140 ) );
		ImGui::SetNextWindowSize( ImVec2( 140, 130 ) );
		ImGui::Begin( "##agriactions", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

		for ( int i = 0; i < 5; ++i )
		{
			if ( ImGui::Button( agriLabels[i], ImVec2( 120, 22 ) ) )
			{
				bridge.cmdSetSelectionAction( agriActions[i] );
			}
		}

		ImGui::End();
	}
	else if ( bridge.currentToolbar == ButtonSelection::Designation )
	{
		ImGui::SetNextWindowPos( ImVec2( 5, io.DisplaySize.y - toolbarHeight - 290 ) );
		ImGui::SetNextWindowSize( ImVec2( 140, 280 ) );
		ImGui::Begin( "##designactions", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

		for ( int i = 0; i < 11; ++i )
		{
			if ( ImGui::Button( designLabels[i], ImVec2( 120, 22 ) ) )
			{
				bridge.cmdSetSelectionAction( designActions[i] );
			}
		}

		ImGui::End();
	}
	else if ( bridge.currentToolbar == ButtonSelection::Job )
	{
		ImGui::SetNextWindowPos( ImVec2( 5, io.DisplaySize.y - toolbarHeight - 140 ) );
		ImGui::SetNextWindowSize( ImVec2( 140, 130 ) );
		ImGui::Begin( "##jobactions", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

		for ( int i = 0; i < 5; ++i )
		{
			if ( ImGui::Button( jobLabels[i], ImVec2( 120, 22 ) ) )
			{
				bridge.cmdSetSelectionAction( jobActions[i] );
			}
		}

		ImGui::End();
	}

	// =========================================================================
	// Event popups
	// =========================================================================
	if ( !bridge.pendingEvents.isEmpty() )
	{
		auto& evt = bridge.pendingEvents.first();
		ImGui::SetNextWindowPos( ImVec2( io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f ), ImGuiCond_Always, ImVec2( 0.5f, 0.5f ) );
		ImGui::SetNextWindowSize( ImVec2( 400, 200 ) );
		ImGui::Begin( evt.title.toStdString().c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );

		ImGui::TextWrapped( "%s", evt.msg.toStdString().c_str() );
		ImGui::Spacing();

		if ( evt.yesno )
		{
			if ( ImGui::Button( "Yes", ImVec2( 80, 30 ) ) )
			{
				bridge.cmdEventAnswer( evt.id, true );
				bridge.pendingEvents.removeFirst();
			}
			ImGui::SameLine();
			if ( ImGui::Button( "No", ImVec2( 80, 30 ) ) )
			{
				bridge.cmdEventAnswer( evt.id, false );
				bridge.pendingEvents.removeFirst();
			}
		}
		else
		{
			if ( ImGui::Button( "OK", ImVec2( 80, 30 ) ) )
			{
				bridge.cmdEventAnswer( evt.id, true );
				bridge.pendingEvents.removeFirst();
			}
		}

		ImGui::End();
	}
}
