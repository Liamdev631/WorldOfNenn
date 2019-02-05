#include "S_ObjectManager.h"
#include "CSVReader.h"

S_ObjectManager::S_ObjectManager()
{
	m_objects.reserve(1024);
	loadObjects();
}

S_ObjectManager::~S_ObjectManager()
{

}

void S_ObjectManager::loadObjects()
{
	CSVReader reader;
	reader.open("assets/data/ObjectInstanceData.csv");
	reader.readNextRow();
	while (reader.size() > 1)
	{
		Object o;
		o.type = (ObjectType)std::stoi(reader[0]);
		o.position.x = (u16)std::stoi(reader[1]);
		o.position.y = (u16)std::stoi(reader[2]);
		m_objects.push_back(o);
		reader.readNextRow();
	}
}

vector<Object>& S_ObjectManager::getObjects()
{
	return m_objects;
}
