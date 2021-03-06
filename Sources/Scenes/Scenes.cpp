#include "Scenes.hpp"

#include "Physics/Collider.hpp"

namespace acid
{
	Scenes::Scenes() :
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
	{
	}

	void Scenes::Update()
	{
		if (m_scene == nullptr)
		{
			return;
		}

		if (!m_scene->IsStarted())
		{
			m_scene->Start();
			m_scene->SetStarted(true);
		}

		m_scene->GetPhysics()->Update();
		m_scene->Update();

		if (m_scene->GetStructure() != nullptr)
		{
			m_scene->GetStructure()->Update();
		}

		if (m_scene->GetCamera() != nullptr)
		{
			m_scene->GetCamera()->Update();
		}
	}
}
