#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

// Resources
#include <Wiwa/utilities/render/Shader.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/json/JSONValue.h>

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

typedef size_t ResourceId;

namespace Wiwa {
	class WI_API Resources
	{
	public:
		// Enum that indicates all the resources that can be loaded by the engine
		enum ResourceType {
			WRT_IMAGE,
			WRT_AUDIOCLIP,
			WRT_SHADER,
			WRT_MODEL,
			WRT_MATERIAL,
			WRT_LAST
		};

		struct Resource {
			// Path to resource
			std::string filePath;

			// Any type
			void* resource;
		};
		struct ImportSettings
		{
			ImportSettings() = default;
		};
		enum class CompressionType {
			NONE = 0,
			RGB_DXT1,
			RGBA_DXT1,
			RGBA_DXT3,
			RGBA_DXT5
		};
		struct ImageSettings : public ImportSettings
		{
			ImageSettings() : ImportSettings(){}
			CompressionType Compression = CompressionType::RGBA_DXT5;
			bool Interlaced = false;
			int32_t OffsetX = 0;
			int32_t OffsetY = 0;
			bool MipMapping = true;
			bool Anystropy = true;
			bool MaxAnystropy = true;
			bool BlurGausian = false;
			bool BlurAverage = false;
			bool Contrast = false;
			uint16_t AmountOfContrast = 0;
			bool Alienify = false;
			bool GammaCorrection = false;
			bool Noise = false;
			bool Equialize = false;
			bool Negative = false;
			bool Pixelize = false;
			bool Sharpen = false;
			uint16_t SharpenFactor = 0;
			uint16_t SharpenIterations = 0;
			bool Scale = 0;
		};
	private:
		Resources();

		static std::vector<Resource*> m_Resources[WRT_LAST];

		static void PushResource(ResourceType rt, const char* file, void* rsc);
		static ResourceId getResourcePosition(ResourceType rt, const char* file);
	public:
		template<class T> static ResourceId Load(const char* file);
		template<class T> static T* GetResourceById(ResourceId id);
		template<class T> static void Import(const char* file);
		template<class T, class T2> static void CreateMeta(const char* file, T2 settings);

		inline static void SaveFile(const char* file, std::string& shaderFile)
		{
			std::string path = "Library/";
			path += file;
			size_t index = path.rfind('/');
			path = path.substr(0, index);
			if (!std::filesystem::exists(path))
			{
				if (std::filesystem::create_directories(path))
					WI_CORE_ERROR("Can't create a folder at {0}", path.c_str());
			}
			path = "Library/";
			path += file;
			path += ".wiasset";
			std::ofstream outFile(path.c_str(), std::ios::out | std::ios::binary);
			outFile.write(shaderFile.c_str(), shaderFile.size());
			outFile.close();
		}
		inline static std::string* getFileData(const char* file)
		{
			std::fstream shaderFile;

			shaderFile.open(file, std::ios::in);

			if (!shaderFile.is_open()) return NULL;

			std::stringstream buffer;

			buffer << shaderFile.rdbuf();

			shaderFile.close();

			return new std::string(buffer.str());
		}
		static void Clear();
	};

	// SPECIALIZATION FOR SHADER
	template<>
	inline void Resources::CreateMeta<Shader>(const char* file, ImportSettings settings)
	{	
		std::string filePath = file;
		filePath += ".meta";
		std::ofstream metaFile(filePath.c_str(), std::ios::out | std::ios::binary);
		metaFile.write(file, filePath.size());
		metaFile.close();
	}
	template<>
	inline ResourceId Resources::Load<Shader>(const char* file) {
		ResourceId position = getResourcePosition(WRT_SHADER, file);
		size_t size = m_Resources[WRT_SHADER].size();

		ResourceId resourceId;

		if (position == size) {
			Shader* shader = new Shader();
			shader->Init(file);

			PushResource(WRT_SHADER, file, shader);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Shader* Resources::GetResourceById<Shader>(ResourceId id) {
		Shader* resource = NULL;

		if (id >= 0 && id < m_Resources[WRT_SHADER].size()) {
			resource = static_cast<Shader*>(m_Resources[WRT_SHADER][id]->resource);
		}

		return resource;
	}
	template<>
	inline void Resources::Import<Shader>(const char* file)
	{
		std::string filePath = file;
		filePath += ".vs";
		std::string* vertexShader = getFileData(filePath.c_str());
		
		filePath = file;
		filePath += ".fs";
		std::string* fragmentShader =getFileData(filePath.c_str());
		
		filePath = file;
		filePath += ".gs";
		std::string* geometryShader = getFileData(filePath.c_str());

		std::string shaderFile;

		if (vertexShader)
		{
			shaderFile += "SV\n";
			shaderFile += vertexShader->c_str();
			shaderFile += "\nEV\n";
		}
		if (fragmentShader)
		{
			shaderFile += "SF\n";
			shaderFile += fragmentShader->c_str();
			shaderFile += "\nEF\n";
		}
		if (geometryShader)
		{
			shaderFile += "SG\n";
			shaderFile += geometryShader->c_str();
			shaderFile += "\nEG\n";
		}


		delete vertexShader;
		delete fragmentShader;
		delete geometryShader;

		if (shaderFile.empty())
			return;

		SaveFile(file, shaderFile);
		CreateMeta<Shader>(file, NULL);
	}
	//--SPECIALIZATION FOR SPRITE
	template<>
	inline void Resources::CreateMeta<Image>(const char* file, ImageSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		JSONDocument doc;
		doc.save_file(filePath.string().c_str());
		doc.AddMember("fileFormatVersion", 1);
		doc.AddMember("file", file);
		doc.AddMember("folderAsset", false);
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		doc.AddMember("timeCreated", std::ctime(&time));
		JSONValue imageSettingsObj = doc.AddMemberObject("imageImportSettings");
		imageSettingsObj.AddMember("compression", (int)settings->Compression);
		imageSettingsObj.AddMember("interlaced", settings->Interlaced);
		imageSettingsObj.AddMember("offset_x", (int)settings->OffsetX);
		imageSettingsObj.AddMember("offset_y", (int)settings->OffsetY);
		imageSettingsObj.AddMember("anystropy", settings->Anystropy);
		imageSettingsObj.AddMember("max_anystropy", settings->MaxAnystropy);
		imageSettingsObj.AddMember("blur_gausian", settings->BlurGausian);
		imageSettingsObj.AddMember("blur_average", settings->BlurAverage);
		imageSettingsObj.AddMember("contrast", settings->Contrast);
		imageSettingsObj.AddMember("amount_of_contrast", (int)settings->AmountOfContrast);
		imageSettingsObj.AddMember("alienify", settings->Alienify);
		imageSettingsObj.AddMember("gamma_correction", settings->GammaCorrection);
		imageSettingsObj.AddMember("noise", settings->Noise);
		imageSettingsObj.AddMember("equialize", settings->Equialize);
		imageSettingsObj.AddMember("pixelize", settings->Pixelize);
		imageSettingsObj.AddMember("sharpen", settings->Sharpen);
		imageSettingsObj.AddMember("sharpen_factor", (int)settings->SharpenFactor);
		imageSettingsObj.AddMember("sharpen_iterations", (int)settings->SharpenIterations);
		imageSettingsObj.AddMember("scale", settings->Scale);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::Load<Image>(const char * file)
	{
		ResourceId position = getResourcePosition(WRT_IMAGE, file);
		size_t size = m_Resources[WRT_IMAGE].size();
		ImageSettings* settings = new ImageSettings();
		CreateMeta<Image>(file, settings);
		ResourceId resourceId;

		if (position == size) {
			Image* image = new Image();
			image->Init(file);

			PushResource(WRT_IMAGE, file, image);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Image* Resources::GetResourceById<Image>(ResourceId id)
	{
		Image* image = NULL;
	
		if (id >= 0 && id < m_Resources[WRT_IMAGE].size()) {
			image = static_cast<Image*>(m_Resources[WRT_IMAGE][id]->resource);
		}
	
		return image;
	}
	template<>
	inline void Resources::Import<Image>(const char* file)
	{

	}
	//--SPECIALIZATION FOR MODEL
	template<>
	inline void Resources::CreateMeta<Model>(const char* file, ImageSettings* settings)
	{

	}
	template<>
	inline ResourceId Resources::Load<Model>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MODEL, file);
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		if (position == size) {
			Model* model = new Model(file);

			PushResource(WRT_MODEL, file, model);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Model* Resources::GetResourceById<Model>(ResourceId id)
	{
		Model* model = NULL;

		if (id >= 0 && id < m_Resources[WRT_MODEL].size()) {
			model = static_cast<Model*>(m_Resources[WRT_MODEL][id]->resource);
		}

		return model;
	}
	//--SPECIALIZATION FOR MATERIAL
	template<>
	inline void Resources::CreateMeta<Material>(const char* file, ImageSettings* settings)
	{

	}
	template<>
	inline ResourceId Resources::Load<Material>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MATERIAL, file);
		size_t size = m_Resources[WRT_MATERIAL].size();

		ResourceId resourceId;

		if (position == size) {
			Material* material = new Material(file);

			PushResource(WRT_MATERIAL, file, material);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Wiwa::Material* Resources::GetResourceById<Wiwa::Material>(ResourceId id)
	{
		Wiwa::Material* material = NULL;

		if (id >= 0 && id < m_Resources[WRT_MATERIAL].size()) {
			material = static_cast<Wiwa::Material*>(m_Resources[WRT_MATERIAL][id]->resource);
		}

		return material;
	}
}

// SPECIALIZATION FOR MIX_CHUNK
//template<>
//inline ResourceId Resources::Load<AudioClip>(const char * file)
//{
//	Uint32 size = static_cast<Uint32>(mResources[ResourceType::RT_AUDIOCLIP].size());
//	Uint32 i;
//
//	Uint32 resourceId = -1;
//
//	for (i = 0; i < size; i++) {
//		if (mResources[RT_AUDIOCLIP][i]->filePath == file) {
//			resourceId = static_cast<Uint32>(i);
//			break;
//		}
//	}
//
//	if (i == size) {
//		Mix_Chunk* audioclip = Mix_LoadWAV(file);
//
//		if (!audioclip) {
//			std::cout << Mix_GetError() << std::endl;
//		}
//		else {
//			Resource* resource = new Resource();
//
//			resource->resource = audioclip;
//
//			resource->filePath = file;
//
//			resourceId = static_cast<Uint32>(size);
//
//			mResources[RT_AUDIOCLIP].push_back(resource);
//
//			std::cout << "RESOURCES: Loaded " << file << std::endl;
//		}
//	}
//
//	return resourceId;
//}

//template<>
//inline AudioClip* Resources::GetResourceById<AudioClip>(ResourceId id)
//{
//	AudioClip* audioclip = NULL;
//
//	if (id >= 0 && id < mResources[RT_AUDIOCLIP].size()) {
//		audioclip = static_cast<AudioClip*>(mResources[RT_AUDIOCLIP][id]->resource);
//	}
//
//	return audioclip;
//}

// BOMBERMAN

// SPECIALIZATION FOR MAP

//template<>
//inline ResourceId Resources::Load<Map>(const char* file) {
//	Uint32 size = static_cast<Uint32>(mResources[ResourceType::RT_MAP].size());
//	Uint32 i;
//
//	Uint32 resourceId = -1;
//
//	for (i = 0; i < size; i++) {
//		if (mResources[RT_MAP][i]->filePath == file) {
//			resourceId = static_cast<Uint32>(i);
//			break;
//		}
//	}
//
//	if (i == size) {
//		Map* map = new Map();
//		map->LoadMap(file);
//
//		Resource* resource = new Resource();
//
//		resource->resource = map;
//
//		resource->filePath = file;
//
//		resourceId = static_cast<Uint32>(size);
//
//		mResources[RT_MAP].push_back(resource);
//
//		std::cout << "RESOURCES: Loaded " << file << std::endl;
//	}
//
//	return resourceId;
//}
//
//template<>
//inline Map* Resources::GetResourceById<Map>(ResourceId resource) {
//	Map* map = NULL;
//
//	if (resource >= 0 && resource < mResources[RT_MAP].size()) {
//		map = static_cast<Map*>(mResources[RT_MAP][resource]->resource);
//	}
//
//	return map;
//}
