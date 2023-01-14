#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector2i.h>
#include <Wiwa/utilities/math/Rectangle.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	class Renderer2D;

	enum class UiType {
		BUTTON,
		NONE
	};

	enum class UiState {
		DISABLED,
		NORMAL,
		FOCUS,
		PRESS,
		NONE
	};

	enum class ButtonType {
		PLAY,
		NONE
	};

	class WI_API Button {
	private:
		Renderer2D* m_Renderer2D;

		ResourceId m_SpriteID;
		Rect2i m_Bounds;
		UiType m_Type;
		Vector2i m_Position;

		UiState m_State;

		ButtonType m_BType;

		uint32_t m_InstanceId;
	public:
		Button(ButtonType btype, const Vector2i& position, const Rect2i& bounds, ResourceId spriteId);

		void Update();

		void OnClick();
	};
}