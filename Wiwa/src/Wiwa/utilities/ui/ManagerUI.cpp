#include "wipch.h"

#include "ManagerUI.h"
#include "Wiwa/core/Application.h"
#include <Wiwa/core/Resources.h>


#include "CheckUI.h"

namespace Wiwa
{
	ManagerUi::ManagerUi()
	{
	}

	ManagerUi::~ManagerUi() {}


	bool ManagerUi::Start()
	{

		return true;
	}

	Button* ManagerUi::CreateControlUi(UiType type, ButtonType btype, Vector2i position, ResourceId spriteId, Wiwa::Rect2i bounds)
	{
		// Create a UI control and add it to the list of controls

		Button* control = nullptr;

		//Call the constructor according to the ControlType
		switch (type)
		{
		case UiType::BUTTON:
		{
			control = new Button(btype, position, bounds, spriteId);
		}
			break;

		default:
			break;
		}

		// Created GuiControls are added to the list of controls
		if (control != nullptr) controls.push_back(control);

		return control;
	}

	void ManagerUi::DestroyControlUi(Button* entity)
	{

	}

	bool ManagerUi::Update(float dt)
	{
		bool ret = true;

		return ret;
	}

	bool ManagerUi::UpdateAll(float dt, bool doLogic) {
		bool ret = true;

	}


	bool ManagerUi::CleanUp()
	{

		return true;
	}
}