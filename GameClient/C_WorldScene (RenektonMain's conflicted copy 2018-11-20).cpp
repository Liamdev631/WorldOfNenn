#include "C_WorldScene.h"
#include "Global.h"
#include "C_Client.h"
#include "ClientPackets.h"

C_WorldScene::C_WorldScene()
	: m_worldView(sf::Vector2f(0, 0), sf::Vector2f(640, 480))
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	m_window.create(sf::VideoMode(640, 480), "World of Nenn", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);

	// Load the map
	m_worldMap.loadMap("assets/maps/overworld.tmx");

	// Load fonts
	m_font1 = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
}

C_WorldScene::~C_WorldScene()
{

}

void C_WorldScene::tick(const GameTime& gameTime)
{
	sf::Event ev;
	while (m_window.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
		{

			break;
		}
		case sf::Event::Resized:
		{
			// adjust the viewport when the window is resized
			glViewport(0, 0, ev.size.width, ev.size.height);
			break;
		}

		case sf::Event::MouseMoved:
		{
			// Update the internal record of the mouse position
			m_mousePos = vec2s(ev.mouseMove.x, ev.mouseMove.y);
			onMouseMoved();
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			switch (ev.mouseButton.button)
			{
			case sf::Mouse::Button::Left:
			{
				// On Left Pressed
				auto thisPlayer = C_WorldManager::get().getThisEntity();
				if (!thisPlayer)
					break;
				auto packet = CP_MoveToDest(m_mouseTargetCell);
				C_Client::get().getPacket().write(packet);
				break;
			}
			}
		}
		}
	}

	// Move the crosshair. This is not in onMouseMoved because playe player movement effects it too
	auto thisPlayer = C_WorldManager::get().getThisEntity();
	if (thisPlayer)
	{
		float destx = (float)m_mousePos.x;// +thisPlayer->interpPosition.x - 320;
		float desty = (float)m_mousePos.y;// +thisPlayer->interpPosition.y - 240;
		if (destx > 0 && desty > 0)
		{
			m_mouseTargetCell.x = (u16)(destx / 16.f);
			m_mouseTargetCell.y = (u16)(desty / 16.f);
			printf("%u, %u\n", m_mouseTargetCell.x, m_mouseTargetCell.y);
			m_crosshair.setPosition(sf::Vector2f((float)m_mouseTargetCell.x * 16.f, (float)m_mouseTargetCell.y * 16.f));
		}
	}
}

void C_WorldScene::draw()
{
	// clear the buffers
	m_window.clear();

	///////////////////////
	// World Space
	///////////////////////

	// Center the camera on the player
	//auto thisPlayer = C_WorldManager::get().getThisEntity();
	//if (thisPlayer)
	//	m_worldView.setCenter((float)thisPlayer->interpPosition.x, (float)thisPlayer->interpPosition.y);
	m_worldView.setCenter(320, 240);
	m_window.setView(m_worldView);

	// Draw the map
	m_window.draw(m_worldMap);

	// Draw all C_RenderableComponent
	for (auto c : components)
		m_window.draw(*c);

	///////////////////////
	// Screen Space
	///////////////////////

	m_window.setView(m_window.getDefaultView());

	// Draw the name of the cursor target
	if (m_mouseTarget.type != TT_NONE)
	{
		sf::String str = "";
		switch (m_mouseTarget.type)
		{
		case TT_ENTITY:
			str = Loader::get().getEntityName(m_mouseTarget.target_entity->entityType);
			break;

		case TT_ITEM:
			str = Loader::get().getItemName(m_mouseTarget.target_item.stack.type);
			break;

		default:
			break;
		}
		sf::Text text = sf::Text(str, *m_font1, 18U);
		text.setOutlineColor(sf::Color::White);
		text.setOutlineThickness(1.f);
		text.setFillColor(sf::Color::Black);
		m_window.draw(text);
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
		m_window.draw(rec);
		recPos += sf::Vector2f(3.0f, 3.0f);

		// Draw the highlight box
		rec.setPosition(recPos + sf::Vector2f(0, m_highlightedOption * 24.0f));
		rec.setSize(sf::Vector2f(114.0f, 24.0f));
		rec.setFillColor(sf::Color(200, 150, 120, 127));
		m_window.draw(rec);

		// Draw the options text
		for (unsigned int i = 0; i < m_optionsList.size(); i++)
		{
			sf::Text text;
			text = sf::Text(RightClickOptionNames[m_optionsList[i]], *m_font1, 17);
			text.setPosition(recPos);
			text.setFillColor(sf::Color::Black);
			recPos.y += 24.0f;
			m_window.draw(text);
		}
	}

	// Draw the crosshair
	m_window.draw(m_crosshair);

	m_window.display();
}

void C_WorldScene::onMouseMoved()
{
	// Determine the cell pointed to by the mouse
	//auto thisPlayer = C_WorldManager::get().getThisEntity();
	//if (thisPlayer)
	//{
	//	float destx = (float)m_mousePos.x + thisPlayer->interpPosition.x - 320;
	//	float desty = (float)m_mousePos.y + thisPlayer->interpPosition.y - 240;
	//	m_mouseTargetCell.x = (u16)(destx / 16.f);
	//	m_mouseTargetCell.y = (u16)(desty / 16.f);
	//}

	// Determine which entities are under the mouse
	m_entitiesUnderMouse.clear();
	m_highlightedOption = max<int>(min<int>((m_mousePos.y - m_optionsPos.y) / 24, (int)m_optionsList.size() - 1), 0);
	for (C_Entity* e : C_WorldManager::get().getActiveEntities())
		if (e->position == m_mouseTargetCell)
			m_entitiesUnderMouse.push_back(e);

	// Determine which items are under the mouse
	m_itemsUnderMouse.clear();
	for (DropableItem& item : g_client->getItemManager().getItemsList())
		if (item.pos == m_mouseTargetCell)
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
		m_mouseTarget.target_entity = C_WorldManager::get().getEntity(lowest);
	}
	else if (m_itemsUnderMouse.size() > 0)
	{

	}
}

void C_WorldScene::onLeftClick()
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
	auto p = CP_MoveToDest(vec2s((u16)m_mousePos.x / 16, (u16)m_mousePos.y / 16));
	packet.write(p);
	return;
}

void C_WorldScene::onRightClick()
{
	if (m_mouseTarget.type != TT_NONE)
	{
		m_optionsTarget = m_mouseTarget;
		m_optionsPos = m_mousePos - vec2s(4, 4); // TODO: Scale

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

bool C_WorldScene::isMouseInOptionBox()
{
	return m_mousePos.x > m_optionsPos.x && m_mousePos.x < m_optionsPos.x + 120 &&
		m_mousePos.y > m_optionsPos.y && m_mousePos.y < m_optionsPos.y + m_optionsList.size() * 24;
}

void C_WorldScene::processRightClickOptionSelection(const RightClickOption selection)
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
			printInChatbox(Loader::get().getEntityDescription(m_optionsTarget.target_entity->entityType));
			return;

		case TT_ITEM:
			printInChatbox(Loader::get().getItemDescription(m_optionsTarget.target_item.stack.type));
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

void C_WorldScene::printInChatbox(const std::string& text)
{
	for (int i = 19; i > 0; i--)
		m_textboxHistory[i] = m_textboxHistory[i - 1];
	m_textboxHistory[0] = text;
	printf("%s\n", text.c_str());
}

