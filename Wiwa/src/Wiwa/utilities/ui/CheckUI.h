#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/math/Vector2i.h>

namespace Wiwa
{
	enum class UiState;
	class CheckUi
	{
	public:

		CheckUi(int id, Rect2i bounds, ResourceId spriteId, bool initState = false);
		virtual ~CheckUi();

		bool Update(float dt);

		bool GetValue() { return checked; };
		void SetValue(bool state) { checked = state; };

		int mouseX, mouseY;		
		unsigned int click;

		Rect2i boundsCheck;
		//ResourceId spriteIdCheck;
		bool checked;

		int onClickFX;
		int onHoverFX;
		bool isPlaying;

		//Need to fix STATE
		UiState state;

		bool canClick = true;
};
}