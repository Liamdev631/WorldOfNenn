#pragma once
#include "Global.h"
#include <deque>
#include "C_Client.h"
#include "C_MouseState.h"
#include <OGRE/Bites/OgreApplicationContext.h>

class C_Client;

enum TargetType : u8
{
	CTT_NONE,
	CTT_ENTITY,
	CTT_ITEM,
};

class C_GUI : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
private:
	//sf::RenderWindow m_window;
	//sf::Font m_font;
	C_Client* m_client;
	//C_SpriteCache m_spriteCache;

	// GUI graphics
	//sf::Texture m_interfaceTexture;
	//sf::Sprite m_interfaceSprite;

	// Right-click options
	TargetType m_optionsTargetType;
	union
	{
		C_Entity* m_optionsTarget_entity;
		DropableItem m_optionsTarget_item;
	};
	Vec2<u16> m_optionsPos;
	std::vector<RightClickOption> m_optionsList;
	u8 m_highlightedOption;

	// Mouse state and cursor highlighting
	MouseState m_mouseState;
	TargetType m_cursorTargetType;
	union
	{
		C_Entity* m_cursorTarget_entity;
		DropableItem m_cursorTarget_item;
	};

	// Textbox
	std::string m_textboxHistory[20];

	// Sounds
	//sf::SoundBuffer m_selfHitSoundBuffer;
	//sf::Sound m_hitSound;

	// Terrain
	//C_Terrain m_terrain;
	//tmx::TileMap* m_tilemap;

public:
	C_GUI(C_Client* client);
	~C_GUI();

	void loadResources();
	void loadResources();

	// Updates
	bool update(const GameTime& gameTime);
	void onRightClick(std::vector<C_Entity *> &m_entitiesUnderMouse, std::vector<DropableItem> &m_itemsUnderMouse);
	void onLeftClick();
	bool isMouseInOptionBox();
	void render(const GameTime& gameTIme);
	void renderGL(const GameTime& gameTime);
	bool isOpen() const;
	const MouseState& getMouseState() const;

	/// Prints text to the GUI's textbox
	void printInChatbox(const string& text);

private:
	void processRightClickOptionSelection(const RightClickOption selection);
};

