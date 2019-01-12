#pragma once
#include "S_Entity.h"

class S_Entity_NPC : public S_Entity
{
private:
	u8 m_stepTimer;
	vec2<u16> m_boundsLocation;
	vec2<u8> m_boundsSize;

protected:

public:
	S_Entity_NPC(const u16 uid, const EntityType entityType, const Region region);
	~S_Entity_NPC();

	void update() override;
	void onRespawn() override;
	void onDeath() override;

	void setBounds(const vec2<u16>& position, const vec2<u8>& size);
};

