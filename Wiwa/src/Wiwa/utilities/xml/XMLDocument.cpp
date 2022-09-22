#include "Empathy/headers/utilities/xml/XMLDocument.h"

EM::XMLDocument::XMLDocument()
{
	
}

EM::XMLDocument::XMLDocument(const char* path)
{
	Load(path);
}

void EM::XMLDocument::Load(const char* path)
{
	pugi::xml_parse_result result = m_Doc.load_file(path);

	if (result != NULL) {
		m_Node = m_Doc;
	}
}

void EM::XMLDocument::Save(const char* path)
{
	m_Doc.save_file(path);
}