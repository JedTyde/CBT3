#pragma once

#include "ButtonUI.h"
#include <vector>

namespace Wiwa
{
	class ManagerUi
	{
	public:

		// Constructor
		ManagerUi();

		// Destructor
		virtual ~ManagerUi();

		bool Start();

		bool Update(float dt);

		bool UpdateAll(float dt, bool logic);

		bool Draw();

		// Called before quitting
		bool CleanUp();

		// Additional methods
		Button* CreateControlUi(UiType type, ButtonType btype, Vector2i position, ResourceId spriteId, Wiwa::Rect2i bounds);
		void DestroyControlUi(Button* entity);
		void AddControlUi(Button* entity);

	public:

		std::vector<Button*> controls;
	};
}

