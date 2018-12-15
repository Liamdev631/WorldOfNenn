#include "C_WindowComponent.h"
#include <SFML/Graphics.hpp>
#include "C_Client.h"
#include "Packets.h"

C_WindowComponent::C_WindowComponent()
	: windowSize(DEFAULT_WINDOW_SIZE)
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y), "World of Nenn");
	m_font1 = &g_client->getSpriteLoader().getFont("Candara.ttf");

	m_map->loadMap("maps/map1");
}

C_WindowComponent::~C_WindowComponent()
{

}

bool C_WindowComponent::update(const GameTime& gameTime)
{
	onMouseMoved();

	sf::Event ev;
	while (m_window->pollEvent(ev))
	{
		// "close requested" event: we close the window
		if (ev.type == sf::Event::Closed)
			m_window->close();
		else if (ev.type == sf::Event::EventType::MouseMoved)
		{
			m_mousePos = Vec2s(ev.mouseMove.x, ev.mouseMove.y);
			onMouseMoved();
		}
		else if (ev.type == sf::Event::EventType::MouseButtonPressed)
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
				onLeftClick();
			else if (ev.mouseButton.button == sf::Mouse::Button::Right)
				onRightClick();
		}
	}


	return true;
}

int counter = 0;
void C_WindowComponent::render(const GameTime& gameTime)
{
	m_window->clear(sf::Color(0, 0, 0));

	#pragma region Draw the game world

	// Render renderable components
	for (auto iter = m_renderComponents.begin(); iter != m_renderComponents.end(); iter++)
		if ((*iter)->enabled)
			(*iter)->render(*m_window.get(), gameTime);

	#pragma endregion

	#pragma region Render GUI

	// Draw the name of the cursor target
	if (m_mouseTarget.type != TT_NONE)
	{
		sf::String str = "";
		switch (m_mouseTarget.type)
		{
			case TT_ENTITY:
			str = g_client->getLoader().getEntityName(m_mouseTarget.target_entity->entityType);
			break;

			case TT_ITEM:
			str = g_client->getLoader().getItemName(m_mouseTarget.target_item.stack.type);
			break;

			default:
			break;
		}
		sf::Text text = sf::Text(str, *m_font1, 18U);
		m_window->draw(text);
	}

	// Draw the right-click menu
	if (m_optionsTarget.type != TT_NONE)
	{
		auto recPos = sf::Vector2f(m_optionsPos.x, m_optionsPos.y);
		auto recSize = sf::Vector2f(120.0f, 6.0f + m_optionsList.size() * 24.0f);
		auto rec = sf::RectangleShape(recSize);
		rec.setPosition(recPos);
		rec.setFillColor(sf::Color(255, 211, 155, 127));
		rec.setOutlineColor(sf::Color(127, 105, 77, 127));
		rec.setOutlineThickness(1.5f);
		m_window->draw(rec);
		recPos += sf::Vector2f(3.0f, 3.0f);

		// Draw the highlight box
		rec.setPosition(recPos + sf::Vector2f(0, m_highlightedOption * 24.0f));
		rec.setSize(sf::Vector2f(114.0f, 24.0f));
		rec.setFillColor(sf::Color(200, 150, 120, 127));
		m_window->draw(rec);

		// Draw the options text
		for (unsigned int i = 0; i < m_optionsList.size(); i++)
		{
			sf::Text text;
			text = sf::Text(RightClickOptionNames[m_optionsList[i]], *m_font1, 17);
			text.setPosition(recPos);
			text.setFillColor(sf::Color::Black);
			recPos.y += 24.0f;
			m_window->draw(text);
		}
	}

	// Draw the mouse target rectangle
	if (m_optionsTarget.type == TT_NONE)
	{
		sf::RectangleShape grid(sf::Vector2f(16, 16));
		grid.setPosition(sf::Vector2f((float)(m_mousePos.x / 16), (float)(m_mousePos.y / 16)) * 16.f);
		grid.setOutlineThickness(1.0f);
		grid.setOutlineColor(sf::Color::White);
		grid.setFillColor(sf::Color::Transparent);
		m_window->draw(grid);
	}

	#pragma endregion

	////////////////////////////////
	// END DRAW
	////////////////////////////////

	// Swap buffers
	m_window->display();
}

void C_WindowComponent::addRenderComponent(Render::C_RenderComponent* comp)
{
	m_renderComponents.push_back(comp);
}

void C_WindowComponent::removeRenderComponent(Render::C_RenderComponent* comp)
{
	for (auto iter = m_renderComponents.begin(); iter != m_renderComponents.end(); iter++)
		if (*iter == comp)
		{
			m_renderComponents.erase(iter);
			return;
		}
}

void C_WindowComponent::onMouseMoved()
{
	// Determine which entities are under the mouse
	m_entitiesUnderMouse.clear();
	m_highlightedOption = max<int>(min<int>((m_mousePos.y - m_optionsPos.y) / 24, (int)m_optionsList.size() - 1), 0);
	for (C_Entity* e : g_client->getEntityManager().getActiveEntities())
		if (e->position == (m_mousePos / 16))
			m_entitiesUnderMouse.push_back(e);

	// Determine which items are under the mouse
	m_itemsUnderMouse.clear();
	for (DropableItem& item : g_client->getItemManager().getItemsList())
		if (item.pos == (m_mousePos / 16))
			m_itemsUnderMouse.push_back(item);

	m_mouseTarget.type = TargetType::TT_NONE;
	if (m_entitiesUnderMouse.size() > 0)
	{
		// Find the entity with the lowest uid
		u16 lowest = MAX_ENTITIES;
		for (const C_Entity* e : m_entitiesUnderMouse)
			if (e->uid < lowest)
				lowest = e->uid;
		m_mouseTarget.type = TargetType::TT_ENTITY;
		m_mouseTarget.targetUID = lowest;
		m_mouseTarget.target_entity = g_client->getEntityManager().getEntity(lowest);
	}
	else if (m_itemsUnderMouse.size() > 0)
	{

	}
}

void C_WindowComponent::onLeftClick()
{
	//// Walk to destination
	//if (m_mouseTarget.type != TargetType::TT_None)
	//{

	//	if (m_mouseTarget.type == TargetType::TT_Entity)
	//	{
	//		// Attack
	//	}
	//}

	//// Move to mouseTarget
	//CP_MoveToDest p = CP_MoveToDest();
	//p.pos.x = (u16)m_mousePos.x; p.pos.x /= 16;
	//p.pos.y = (u16)m_mousePos.y; p.pos.y /= 16;
	//g_client->getPacket().write(p);

	auto& packet = g_client->getPacket();

	// Left clicked an option
	if (m_optionsTarget.type != TT_NONE)
	{
		if (isMouseInOptionBox())
			processRightClickOptionSelection(m_optionsList[m_highlightedOption]);
		m_optionsTarget.type = TT_NONE;
		return;
	}

	// Left clicked an item
	if (m_mouseTarget.type != TT_NONE)
	{
		if (m_mouseTarget.type == TT_ITEM)
		{
			packet.write<u8>(0x05);
			packet.write<DropableItem>(m_mouseTarget.target_item);
			m_mouseTarget.type = TT_NONE;
			return;
		}
		else if (m_mouseTarget.type == TT_ENTITY)
		{
			// Left click entity to attack
			packet.write<u8>(0x01);
			packet.write<u16>(m_mouseTarget.target_entity->getUID());
			return;
		}

		return; // Should be unreachable
	}

	// Open left click. Used to walk to a position
	CP_MoveToDest p = CP_MoveToDest();
	p.pos.x = (u16)m_mousePos.x / 16;
	p.pos.y = (u16)m_mousePos.y / 16;
	packet.write(p);
	return;
}

void C_WindowComponent::onRightClick()
{
	if (m_mouseTarget.type != TT_NONE)
	{
		m_optionsTarget = m_mouseTarget;
		m_optionsPos = m_mousePos - Vec2s(4, 4); // TODO: Scale

		switch (m_optionsTarget.type)
		{
		case TT_ENTITY:
		{
			m_optionsList = { RCO_INSPECT, RCO_ATTACK };
			return;
		}

		case TT_ITEM:
		{
			m_optionsList = { RCO_INSPECT, RCO_PICKUP };
			return;
		}

		default:
			return;
		}
	}
}

bool C_WindowComponent::isMouseInOptionBox()
{
	return m_mousePos.x > m_optionsPos.x && m_mousePos.x < m_optionsPos.x + 120 &&
		m_mousePos.y > m_optionsPos.y && m_mousePos.y < m_optionsPos.y + m_optionsList.size() * 24;
}

void C_WindowComponent::processRightClickOptionSelection(const RightClickOption selection)
{
	auto& packet = g_client->getPacket();
	switch (selection)
	{
	case RCO_NONE:
	{
		// Nothing
		break;
	}

	case RCO_INSPECT:
	{
		// Inspect
		switch (m_optionsTarget.type)
		{
		case TT_ENTITY:
			printInChatbox(g_client->getLoader().getEntityDescription(m_optionsTarget.target_entity->entityType));
			return;

		case TT_ITEM:
			printInChatbox(g_client->getLoader().getItemDescription(m_optionsTarget.target_item.stack.type));
			return;

		}
		return;
	}

	case RCO_ATTACK:
	{
		// Attack
		CP_AttackEntity p = CP_AttackEntity();
		p.targetUID = m_optionsTarget.target_entity->getUID();
		packet.write(p);
		//assert(m_optionsTarget.type == TT_ENTITY);
		//packet.write<u8>(0x01);
		//packet.write<u16>(m_optionsTarget.target_entity->getUID());
		break;
	}

	case RCO_PICKUP:
	{
		// Pick up
		packet.write<u8>(0x05);
		packet.write<DropableItem>(m_optionsTarget.target_item);
		return;
	}

	default:
		return;
	}
}

void C_WindowComponent::printInChatbox(const std::string& text)
{
	for (int i = 19; i > 0; i--)
		m_textboxHistory[i] = m_textboxHistory[i - 1];
	m_textboxHistory[0] = text;
	printf("%s\n", text.c_str());
}

void C_WindowComponent::render3D()
{

}
