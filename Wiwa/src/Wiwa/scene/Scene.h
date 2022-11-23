#pragma once

#include <Wiwa/core/Core.h>

#include <vector>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/Camera.h>
#include <Wiwa/utilities/render/CameraManager.h>

namespace Wiwa {
	class WI_API Scene {
	public:
		Scene();
		~Scene();

		enum State {
			SCENE_ENTERING,
			SCENE_LOOP,
			SCENE_LEAVING
		};

		void Update();

		void Start();

		virtual void Init() {};
		virtual void Load();
		virtual void Unload() {}

		State getState() { return m_CurrentState; }

		void ChangeScene(size_t scene);

		EntityManager& GetEntityManager() { return m_EntityManager; }
	protected:
		virtual void ProcessInput() {}

		virtual void UpdateEnter() {}
		virtual void UpdateLoop() {}
		virtual void UpdateLeave() {}

		virtual void RenderEnter() {}
		virtual void RenderLoop() {}
		virtual void RenderLeave() {}

		size_t mMaxTimeEntering, mMaxTimeLeaving = 0;

		EntityManager m_EntityManager;
	private:
		State m_CurrentState = SCENE_ENTERING;
		size_t m_TransitionTimer = 0;
		size_t m_SceneToChange = 0;
		Camera* m_SceneCamera = nullptr;
		CameraId m_SceneCameraId = -1;
	};
}