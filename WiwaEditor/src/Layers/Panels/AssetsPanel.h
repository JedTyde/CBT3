#pragma once

#include "Panel.h"

#include <filesystem>
typedef void* ImTextureID;
struct FileSpecs
{
	std::filesystem::path path;
	uintmax_t size = 0;
};
struct DirectorySpecs
{
	std::filesystem::path path;
	std::vector<DirectorySpecs*> directories;
	std::vector<FileSpecs> files;
};

class AssetsPanel : public Panel
{
public:
	AssetsPanel(EditorLayer* instance);
	virtual ~AssetsPanel();

	void Update() override;
	void UpdateDir(const std::filesystem::directory_entry &p1, DirectorySpecs*dir);
	void Draw() override;

private:
	void DisplayNode(DirectorySpecs* directoryEntry);

private:
	ImTextureID m_FileIcon;
	ImTextureID m_FolderIcon;
	ImTextureID m_BackIcon;
	ImTextureID m_MaterialIcon;
	DirectorySpecs m_Directory;
	std::filesystem::path m_CurrentPath;
	std::filesystem::file_time_type lastWriteTime;
	float m_ButtonSize = 1.0f;
};