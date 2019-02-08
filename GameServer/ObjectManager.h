#pragma once
#include <vector>
#include <map>
#include "Object.h"
#include "WPacket.h"
#include "RPacket.h"

using namespace std;

class ObjectManager
{
private:
	map<ObjectUID, Object> m_objects;

private:
	ObjectManager();

	void loadObjects();

public:
	~ObjectManager();
	static ObjectManager& get()
	{
		static ObjectManager o;
		return o;
	}

	const map<ObjectUID, Object>& getObjects() const;
	const void addObject(const Object& object);
	const vector<ObjectUID> getNearbyObjectUID(const vec2s& position, u8 range) const;
	const vector<Object> getNearbyObjects(const vec2s& position, u8 range) const;

	void serializeNearbyObjects(WPacket& packet, const vec2s& position, u8 range);
	vector<Object> deserializeObjects(RPacket& packet);
};
