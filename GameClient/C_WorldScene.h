#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include "C_WorldMap.h"
#include "GameTime.h"
#include "Global.h"
#include "Crosshair.h"
#include "Items.h"
#include "Chatbox.h"
#include "RightClickOption.h"
#include "RCOption.h"

//  _______________________
// |				|	   |
// |				| MENU |
// |	WORLD		|SKILLS|
// |				| INV  |
// |________________|	   |
// |	CHATBOX		|	   |
// |________________|______|

class C_WorldScene
{
	friend class sf::Drawable;

private:
	C_WorldScene();

	sf::RenderWindow m_window;
	sf::RenderTexture m_gameScene;
	sf::View m_worldView;
	C_WorldMap m_worldMap;

	// GUI
	sf::Sprite* m_interface;
	std::vector<UIComponent*> m_uiComponents;
	Chatbox m_chatbox;

	// Mouse 
	sf::Vector2f m_mousePos;
	Crosshair m_crosshair;

	// References the entities and items currently pointed to by the mouse
	Target m_mouseTarget;
	vec2s m_mouseTargetCell;
	std::vector<C_Entity*> m_entitiesUnderMouse;
	std::vector<DropableItem> m_itemsUnderMouse;

	// Right-click options
	//Target m_optionsTarget;
	sf::Vector2f m_optionsPos;
	std::vector<RCOption> m_optionsList;
	u8 m_highlightedOption;

	sf::Font* m_font1;

public:
	std::vector<sf::Drawable*> components;

	// Interface Settings
	const sf::Vector2u GuiSize = sf::Vector2u(765, 503);
	const sf::Vector2u WorldSceneSize = sf::Vector2u(512, 334);
	const sf::Vector2f WorldScenePos = sf::Vector2f(4, 4);
	const sf::FloatRect WorldSceneBounds = sf::FloatRect(WorldScenePos.x, WorldScenePos.y, (float)WorldSceneSize.x, (float)WorldSceneSize.y);
	const sf::Vector2u ChatboxSize = sf::Vector2u(505, 128);
	const sf::Vector2u ChatboxPos = sf::Vector2u(7, 345);

public:
	// Needed for singleton
	~C_WorldScene();
	C_WorldScene(C_WorldScene const&) = delete;
	void operator=(C_WorldScene const&) = delete;


	// Singleton accessor
	static C_WorldScene& get()
	{
		static C_WorldScene s;
		return s;
	}

	void update(const GameTime& gameTime);
	void checkEvents();

	void draw();
	void drawGui();
	void drawGameScene();

	//void setRightClickOptions(const Target& optionsTarget, const sf::Vector2f& optionsPos, const std::vector<RightClickOption> options);
	void setRightClickOptions(const sf::Vector2f& position, const std::vector<RCOption>& options);
	void printInChatbox(const std::wstring& text);
	void processRightClickOptionSelection(const RCOption& selection);
	bool isMouseInOptionBox();

private:
	void onLeftClick();
	void onRightClick();
};

