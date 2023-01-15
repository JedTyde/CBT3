#include <Wiwa/utilities/math/Vector2i.h>

namespace Wiwa
{
	class CheckUi
	{
	public:

		CheckUi(int id, Rect2i bounds, ResourceId text, bool initState = false);
		virtual ~CheckUi();

		bool Update(float dt);

		bool GetValue() { return checked; };
		void SetValue(bool state) { checked = state; };

		int mouseX, mouseY;
		Rect2i boundsCheck;
		unsigned int click;

		bool checked;

		int onClickFX;
		int onHoverFX;
		bool isPlaying;

		UiState state;

		bool canClick = true;
};
}