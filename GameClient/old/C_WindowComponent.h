#pragma once
#include "Global.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "C_RenderComponent.h"
#include "C_Entity.h"
#include "Items.h"
#include "Target.h"
#include "C_WorldMap.h"

#define DEFAULT_WINDOW_SIZE Vec2s(640, 480)

class C_WindowComponent
{
private:
	Vec2s windowSize;
	Vec2s mousePos;

	// A list of all the components to call ->render() on
	// Do not clear this anywhere other than the clearRenderComponents() function
	std::vector<Render::C_RenderComponent*> m_renderComponents;

	// The actual window object
	std::unique_ptr<sf::RenderWindow> m_window;

	// References the entities and items currently pointed to by the mouse
	Target m_mouseTarget;
	std::vector<C_Entity*> m_entitiesUnderMouse;
	std::vector<DropableItem> m_itemsUnderMouse;

	// Right-click options
	Target m_optionsTarget;
	Vec2<u16> m_optionsPos;
	std::vector<RightClickOption> m_optionsList;
	u8 m_highlightedOption;

	// Textbox
	std::string m_textboxHistory[20];
	sf::Font* m_font1;

	// Map
	unique_ptr<C_WorldMap> m_map;

public:
	C_WindowComponent();
	~C_WindowComponent();

	bool update(const GameTime& gameTime);
	void render(const GameTime& gameTime);
	void render3D();
	void onMouseMoved();

	void addRenderComponent(Render::C_RenderComponent* comp);
	void removeRenderComponent(Render::C_RenderComponent* comp);

	void printInChatbox(const std::string& text);
	void processRightClickOptionSelection(const RightClickOption selection);
	bool isMouseInOptionBox();


private:
	void onLeftClick();
	void onRightClick();
};

