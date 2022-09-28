#pragma once

#include <Wiwa/Layer.h>
#include <Wiwa/utilities/containers/List.h>

#include "Panels/AboutPanel.h"
#include "Panels/ConfigurationPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/AssetsPanel.h"

class EditorLayer : public Wiwa::Layer
{
public:
	EditorLayer();
	~EditorLayer();
	

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Wiwa::Event& event) override;
private:
	void MainMenuBar();
	void DockSpace();
private:
	bool m_ShowConsole = false;
	bool m_ShowDemo = false;


	AboutPanel* m_About = nullptr;
	ConfigurationPanel* m_Configuration = nullptr;
	ConsolePanel* m_Console = nullptr;
	ScenePanel* m_Scene = nullptr;
	HierarchyPanel* m_Hierarchy = nullptr;
	AssetsPanel* m_Assets = nullptr;

	std::vector<Panel*> m_Panels;
};