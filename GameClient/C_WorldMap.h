#pragma once
#include "Global.h"
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <mutex>
#include <atomic>
#include "SFMLOrthogonalLayer.hpp"

using namespace std;

// How often the map should check the camera position. The check is very quick, it just doesn't
// Make sense to do this 20 times each second.
constexpr auto UpdateInterval = 200ms;

 // How many tiles away from the center the camera needs to be before the map updates again.
constexpr float MaxCenterOffset = 8.0f;

//#define MapSize vec2s(640 / 16 + 6, 480 / 16 + 6)

class C_WorldMap : public sf::Drawable
{
private:
	string m_filename;
	tmx::Map m_map;
	MapLayer** m_mapLayers;
	std::atomic_bool m_running, m_loaded;
	mutable std::mutex m_mapLayerMutex;
	sf::Vector2u m_mapPosition;
	std::thread m_mapLoadingThread;
	vec2s m_mapSize;

public:
	C_WorldMap();
	~C_WorldMap();

	bool loadMap(const string& filename);
	void loadMapAtPoint(const sf::Vector2u& pos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void mapLoadingThread();
};

