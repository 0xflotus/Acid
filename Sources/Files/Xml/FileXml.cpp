#include "FileXml.hpp"

#include "Engine/Engine.hpp"
#include "Files/Files.hpp"
#include "Helpers/FileSystem.hpp"
#include "XmlNode.hpp"

namespace acid
{
	FileXml::FileXml(const std::string &filename) :
		m_filename(filename),
		m_parent(std::make_unique<Metadata>("?xml", "", std::map<std::string, std::string>{{"version",  "1.0"}, {"encoding", "utf-8"}}))
	{
	}

	void FileXml::Load()
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		m_parent->ClearChildren();

		auto fileLoaded = Files::Read(m_filename);

		if (!fileLoaded)
		{
			Log::Error("XML file could not be loaded: '%s'\n", m_filename.c_str());
			return;
		}

		XmlNode *currentSection = nullptr;
		std::stringstream summation;
		bool end = false;

		for (auto it = fileLoaded->begin(); it != fileLoaded->end(); ++it)
		{
			if (*it == '<')
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = new XmlNode(nullptr, "", "");
					continue;
				}

				if (*(it + 1) == '/') // End tag.
				{
					currentSection->SetContent(currentSection->GetContent() + summation.str());
					end = true;
				}
				else // Start tag.
				{
					auto section = new XmlNode(currentSection, "", "");
					currentSection->AddChild(section);
					currentSection = section;
				}

				summation.str(std::string());
			}
			else if (*it == '>')
			{
				if (!end)
				{
					currentSection->SetAttributes(currentSection->GetAttributes() + summation.str());
				}

				summation.str(std::string());

				if (end || *(it - 1) == '/') // End tag.
				{
					end = false;

					if (currentSection->GetParent() != nullptr)
					{
						currentSection = currentSection->GetParent();
					}
				}
			}
			else if (*it == '\n')
			{
			}
			else
			{
				summation << *it;
			}
		}

		if (currentSection != nullptr)
		{
			XmlNode::Convert(*currentSection, m_parent.get(), true);
		}

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Xml '%s' loaded in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void FileXml::Save()
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		std::stringstream data;
		XmlNode::AppendData(*m_parent, data, 0);

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data.str());

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Xml '%s' saved in %ims\n", m_filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void FileXml::Clear()
	{
		m_parent->ClearChildren();
	}

	void FileXml::Verify()
	{
		if (!FileSystem::Exists(m_filename))
		{
			FileSystem::Create(m_filename);
		}
	}
}
