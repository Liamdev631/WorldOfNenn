#include "ObjectManager.h"
#include "CSVReader.h"

ObjectManager::ObjectManager()
{
	m_objects.reserve(1024);
	loadObjects();
}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::loadObjects()
{

}

const map<ObjectUID, Object>& ObjectManager::getObjects() const
{
	return m_objects;
}

const void ObjectManager::addObject(const Object& object)
{

}

const vector<ObjectUID> ObjectManager::getNearbyObjectUID(const vec2s& position, u8 range) const
{
	vector<ObjectUID> nearbyObjects;
	for (auto& object : m_objects)
		if (object.second.position.x > position.x - range &&
			object.second.position.x < position.x + range &&
			object.second.position.y > position.y - range &&
			object.second.position.y < position.y + range)
		{
			nearbyObjects.push_back(object.first);
		}
	return nearbyObjects;
}

const vector<Object> ObjectManager::getNearbyObjects(const vec2s& position, u8 range) const
{
	vector<Object> nearbyObjects;
	for (auto& object : m_objects)
		if (object.second.position.x > position.x - range &&
			object.second.position.x < position.x + range &&
			object.second.position.y > position.y - range &&
			object.second.position.y < position.y + range)
		{
			nearbyObjects.push_back(object.second);
		}
	return nearbyObjects;
}

void ObjectManager::serializeNearbyObjects(WPacket& packet, const vec2s& position, u8 range)
{
	auto nearbyObjects = getNearbyObjectUID(position, range);
	packet.write((u16)m_objects.size());
	for (auto& o : nearbyObjects)
		packet.write<Object>(m_objects[o]);
}

vector<Object> ObjectManager::deserializeObjects(RPacket& packet)
{
	vector<Object> nearbyObjects;
	size_t numObjects = (size_t)packet.read<u16>();
	nearbyObjects.reserve(numObjects);
	nearbyObjects.push_back(*packet.read<Object>());
}
