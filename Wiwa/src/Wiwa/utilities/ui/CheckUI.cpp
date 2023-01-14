#include <wipch.h>

#include <Wiwa/core/Input.h>
#include "CheckUi.h"
#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Window.h>
#include <Wiwa/utilities/ui/ButtonUI.h>

namespace Wiwa
{
	CheckUi::CheckUi(int id, Rect2i bounds, ResourceId text, bool initState = false)
	{
		//this->boundsCheck = bounds;
		//this->text = text;

		canClick = true;

		checked = initState;

	};

	CheckUi::~CheckUi()
	{

	}

	bool CheckUi::Update(float dt)
	{
		bool ret = true;
		
		if (state != UiState::DISABLED)
		{
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			Wiwa::Size2i resolution = Wiwa::Application::Get().GetTargetResolution();
			float ar = resolution.w / (float)resolution.h;
			Wiwa::Size2f scales = { viewportPanelSize.x / (float)resolution.w, viewportPanelSize.y / (float)resolution.h };

			ImVec2 mpos = ImGui::GetMousePos();
			ImVec2 cspos = ImGui::GetCursorScreenPos();
			ImVec2 rectPos = ImGui::GetItemRectMin();

			float scale = scales.x < scales.y ? scales.x : scales.y;

			ImVec2 isize = { resolution.w * scale, resolution.h * scale };
			ImVec2 rpos = { mpos.x - rectPos.x, mpos.y - rectPos.y };
			CLAMP(rpos.x, 0.0f, isize.x);
			CLAMP(rpos.y, 0.0f, isize.y);

			glm::vec3 out_dir;
			glm::vec3 out_origin;
			rpos.y -= isize.y;
			rpos.y = glm::abs(rpos.y);

			if ((rpos.x > boundsCheck.x) && (rpos.x < (boundsCheck.x + boundsCheck.width)) &&
				(rpos.y > boundsCheck.y) && (rpos.y < (boundsCheck.y + boundsCheck.height)))
			{
				state = UiState::FOCUS;
				if (isPlaying == false)
				{
					isPlaying = true;
				}

				if (Wiwa::Input::IsMouseButtonPressed(0))
				{
					state = UiState::PRESS;
				}
			}
			else
			{
				state = UiState::NORMAL;
				isPlaying = false;
			}

		}

		return ret;
	}
}