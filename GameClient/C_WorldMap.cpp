#include "C_WorldMap.h"
#include <chrono>
#include <math.h>
#include "C_WorldManager.h"

using namespace std::chrono_literals;

C_WorldMap::C_WorldMap()
	: m_loaded(false), m_running(true), m_mapLoadingThread(&C_WorldMap::mapLoadingThread, this)
{
	m_mapSize = vec2s(640 / 16 + (u16)MaxCenterOffset * 2 + 2, 480 / 16 + (u16)MaxCenterOffset * 2 + 2);
}

C_WorldMap::~C_WorldMap()
{
	m_running = false;
	m_mapLoadingThread.detach();
}

bool C_WorldMap::loadMap(const string& filename)
{
	if (filename == m_filename)
		return true; // Already loaded
	
	// Load the TMX map
	m_loaded = false;
	m_filename = filename;
	m_loaded = m_map.load(m_filename);

	// Create the MapLayer
	loadMapAtPoint(sf::Vector2u(0, 0));

	return m_loaded;
}

static int counter = 0;
void C_WorldMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_loaded)
		return;

	m_mapLayerMutex.lock();
	{
		if (m_mapLayer)
			target.draw(*m_mapLayer);
	}
	m_mapLayerMutex.unlock();
}

void C_WorldMap::mapLoadingThread()
{
	while (m_running)
	{
		auto thisEntity = C_WorldManager::get().getThisEntity();
		if (thisEntity)
		{
			sf::Vector2u playerPos = sf::Vector2u(thisEntity->position.x, thisEntity->position.y);
			float distance = std::sqrtf(
				std::powf((float)m_mapPosition.x - (float)playerPos.x, 2) +
				std::powf((float)m_mapPosition.y - (float)playerPos.y, 2));
			if (distance >= MaxCenterOffset)
			{
				m_mapPosition = playerPos;
				loadMapAtPoint(m_mapPosition);
			}
		}
		std::this_thread::sleep_for(UpdateInterval);
	}
}

void C_WorldMap::loadMapAtPoint(const sf::Vector2u& pos)
{
	// Load the new map layer
	int mapx = pos.x - m_mapSize.x / 2, mapy = pos.y - m_mapSize.y / 2;
	if (mapx < 0)
		mapx = 0;
	if (mapy < 0)
		mapy = 0;
	auto start = std::chrono::high_resolution_clock::now();
	MapLayer* newLayer = new MapLayer(m_map, 0, tmx::IntRect(mapx, mapy, m_mapSize.x, m_mapSize.y));
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	printf("Map loaded at (%u,%u) in %f seconds\n", pos.x, pos.y, (float)elapsed.count());
	
	// Swap with the old map
	m_mapLayerMutex.lock();
	//if (m_mapLayer)
	//	delete m_mapLayer;
	m_mapLayer = newLayer;
	m_mapLayerMutex.unlock();
}

