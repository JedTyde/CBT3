#include "AssetsPanel.h"

#include <imgui.h>
#include "wipch.h"
#include <Wiwa/Application.h>
#include <Wiwa/Resources.h>

AssetsPanel::AssetsPanel()
	: Panel("Assets")
{
	m_Directory.path = "Assets";
	m_CurrentPath = m_Directory.path;
	ResourceId folderId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/folder_icon.png");
	ResourceId fileId = Wiwa::Resources::Load<Wiwa::Image>("resources/icons/file_icon.png");
	m_FolderIcon = Wiwa::Resources::GetResourceById<Wiwa::Image>(folderId)->GetTextureId();
	m_FileIcon = Wiwa::Resources::GetResourceById<Wiwa::Image>(fileId)->GetTextureId();
}

AssetsPanel::~AssetsPanel()
{
}

void AssetsPanel::Update()
{
}

void AssetsPanel::Draw()
{

	ImGui::Begin(name, &active);

	
	/*bool op = false;*/
	//ImGui::Text(m_SearchPath.string().c_str());
	if (ImGui::BeginTable("##content_browser", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("##folder_browser", 1))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			for (auto& path : m_Directory.directories)
			{
				DisplayNode(path);
			}
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();

		if (m_CurrentPath != std::filesystem::path(m_Directory.path))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
			ImGui::SameLine();
		}

		ImGui::Text(m_CurrentPath.string().c_str());

	
		float padding = 16.0f;
		float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;
		
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		if (ImGui::BeginTable("##assets", columnCount))
		{
			int id = 0;
			for (auto& directoryEntry : m_Directory.directories)
			{
				ImGui::TableNextColumn();
				ImGui::PushID(id++);

				const auto& path = directoryEntry.path;
				auto relativePath = std::filesystem::relative(directoryEntry.path, m_Directory.path);
				std::string filenameString = relativePath.filename().string();
				bool isDir = std::filesystem::is_directory(path);
				uint32_t texID = isDir ? m_FolderIcon : m_FileIcon;

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)texID, { thumbnailSize, thumbnailSize });
				ImGui::PopStyleColor();

				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (isDir)
						m_CurrentPath /= path.filename();
					else
						Wiwa::Application::Get().OpenDir(path.string().c_str());
				}

				ImGui::TextWrapped(filenameString.c_str());

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
		ImGui::EndTable();
	}
	
	ImGui::End();
}

void AssetsPanel::DisplayNode(Directory directoryEntry)
{
	const auto& path = directoryEntry.path;
	
	//ImGui::Text("%s", path.string().c_str());
	//auto relativePath = std::filesystem::relative(directoryEntry.path(), s_EditorPath);
	std::string filenameString = path.filename().string();
	bool isDir = std::filesystem::is_directory(path);
	if (isDir)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::AlignTextToFramePadding();
		if (!std::filesystem::is_empty(path))
		{
			char str[25];
			sprintf_s(str, 25, "##%s", filenameString.c_str());
			bool open = ImGui::TreeNodeEx(str);
			if (open)
			{
				for (auto& p : m_Directory.directories)
				{
					DisplayNode(p);
				}
				ImGui::TreePop();
			}
			ImGui::SameLine();
		}
		
		if (ImGui::Button(filenameString.c_str()))
		{
			m_CurrentPath = path;
		}
		
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
	}
}
