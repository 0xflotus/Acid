#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Physics/ColliderBox.hpp>
#include <Physics/ColliderSphere.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Physics/ColliderConvexHull.hpp>
#include <Scenes/Scenes.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Renderer/Renderer.hpp>
#include <Physics/ColliderCapsule.hpp>
#include <Uis/Uis.hpp>
#include <Objects/Prefabs/PrefabObject.hpp>
#include <Helpers/FileSystem.hpp>
#include "FpsCamera.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FpsCamera(), new SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_buttonFullscreen(ButtonKeyboard({KEY_F11})),
		m_buttonCaptureMouse(ButtonKeyboard({KEY_M, KEY_ESCAPE})),
		m_buttonScreenshot(ButtonKeyboard({KEY_F12})),
		m_buttonExit(ButtonKeyboard({KEY_DELETE})),
		m_soundScreenshot(Sound("Sounds/Screenshot.ogg")),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer()))
	{
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));
		GetPhysics()->SetAirDensity(1.0f);

		// Player.
		auto playerObject = new GameObject("Objects/Player/Player.xml", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		auto skyboxObject = new GameObject("Objects/SkyboxSnowy/SkyboxSnowy.json", Transform(Vector3(), Vector3(), 1024.0f));

		// Entities.
		auto sun = new GameObject(Transform(Vector3(1000.0f, 5000.0f, 4000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto sphere = new GameObject(Transform(Vector3(i, j, -6.0f), Vector3(), 0.5f));
				sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
				sphere->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Testing/Diffuse.png"),
					(float) j / 4.0f, (float) i / 4.0f, Texture::Resource("Objects/Testing/Material.png"), Texture::Resource("Objects/Testing/Normal.png"));
				sphere->AddComponent<MeshRender>();
				sphere->AddComponent<ShadowRender>();

				auto teapot = new GameObject(Transform(Vector3(i * 1.6f, j, 6.0f), Vector3(), 0.14f));
				teapot->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
				teapot->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, (float) j / 4.0f, (float) i / 4.0f);
				teapot->AddComponent<MeshRender>();
				teapot->AddComponent<ShadowRender>();
			}
		}

		/*auto dragon = new GameObject(Transform(Vector3(6.0f, 0.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f), 0.4f));
		dragon->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Dragon.obj"));
		dragon->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.1f, 0.7f);
		dragon->AddComponent<MeshRender>();
		dragon->AddComponent<ShadowRender>();*/
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen.WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse.WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot.WasDown())
		{
			m_soundScreenshot.Play();
			Renderer::Get()->CaptureScreenshot(FileSystem::GetWorkingDirectory() + "/Screenshots/" + Engine::GetDateTime() + ".png");
		}

		if (m_buttonExit.WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}

	bool Scene1::IsPaused() const
	{
		return false;
	}
}
