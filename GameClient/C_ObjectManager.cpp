#include "C_ObjectManager.h"
#include "CSVReader.h"

C_ObjectManager::C_ObjectManager()
{
	m_objects.reserve(1024);
	loadObjects();
}

C_ObjectManager::~C_ObjectManager()
{

}

void C_ObjectManager::loadObjects()
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
	}
}

vector<Object>& C_ObjectManager::getObjects()
{
	return m_objects;
}

void C_ObjectManager::addObject(const Object& object)
{
	m_objects.push_back(object);
	printf("Added object type:%u, x:%u, y:%u\n", object.type, object.position.x, object.position.y);
}

void C_ObjectManager::removeObject(const Object& object)
{
	for (auto iter = m_objects.begin(); iter != m_objects.end(); iter++)
		if (*iter == object)
		{
			m_objects.erase(iter);
		}
}
