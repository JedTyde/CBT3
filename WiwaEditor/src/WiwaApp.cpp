#include <Wiwa.h>

#include "Layers/EditorLayer.h"

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/scripting/ScriptEngine.h>
#include <Wiwa/core/Renderer2D.h>

USE_REFLECTION;
class WiwaApp : public Wiwa::Application
{
public:
	REFLECTION_BODY_INL;

	WiwaApp(int argc, char** argv) : Wiwa::Application(argc, argv)
	{
		REFLECTION_REGISTER();

		PushOverlay(new EditorLayer());

		const Type* type = Wiwa::ScriptEngine::getSystems()[FNV1A_HASH("EnemyController")];

		Wiwa::System* system = (Wiwa::System*)type->New();
		system->AddEntity(0);
		system->Awake();
		system->Init();
		system->Update();

		delete system;
		ResourceId imgId = Wiwa::Resources::Load<Wiwa::Image>("Assets/Mark.png");
		Wiwa::Image* img = Wiwa::Resources::GetResourceById<Wiwa::Image>(imgId);
		Wiwa::Vector2i position = {960,540};
		Wiwa::Size2i size = img->GetSize()/4;
		Wiwa::Rect2i rect = { 0,0,img->GetWidth()/2,img->GetHeight()/2};
		Wiwa::Color4f color = Wiwa::Color::WHITE;
		Wiwa::Application::Get().GetRenderer2D().CreateInstancedQuadTex(imgId, position, size, color, rect, Wiwa::Renderer2D::Pivot::CENTER);
	}

	~WiwaApp()
	{

	}
};

Wiwa::Application* Wiwa::CreateApplication(int argc, char** argv)
{
	return new WiwaApp(argc, argv);
}