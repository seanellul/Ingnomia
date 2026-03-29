#include "ui_helpers.h"
#include "../imgui_impl_qt5.h"
#include <imgui.h>

void sectionHeader( const char* text )
{
	auto& fonts = GetImGuiFonts();

	// Push the medium font first so we can measure it
	if ( fonts.uiMedium )
		ImGui::PushFont( fonts.uiMedium );

	float fontSize = ImGui::GetFontSize();
	float height = fontSize + 8.0f;

	// Full-width tinted background bar
	ImVec2 avail = ImGui::GetContentRegionAvail();
	ImVec2 cursor = ImGui::GetCursorScreenPos();

	ImDrawList* dl = ImGui::GetWindowDrawList();
	dl->AddRectFilled(
		cursor,
		ImVec2( cursor.x + avail.x, cursor.y + height ),
		ImGui::GetColorU32( ImGuiCol_Header )
	);

	// Text centered vertically in the bar
	float textY = cursor.y + ( height - fontSize ) * 0.5f;
	ImGui::SetCursorScreenPos( ImVec2( cursor.x + 6.0f, textY ) );
	ImGui::TextUnformatted( text );

	if ( fonts.uiMedium )
		ImGui::PopFont();

	// Advance cursor past the bar
	ImGui::SetCursorScreenPos( ImVec2( cursor.x, cursor.y + height + 4.0f ) );
}

void sectionLabel( const char* text, ImVec4 color )
{
	ImGui::TextColored( color, "%s", text );
}
