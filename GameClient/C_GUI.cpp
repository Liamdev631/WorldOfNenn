#include "C_GUI.h"
#include <string>
#include "Loader.h"
#include <OGRE/Ogre.h>

C_GUI::C_GUI(C_Client* client) : OgreBites::ApplicationContext("OgreTutorialApp")
	: m_client(client), m_optionsTargetType(CTT_NONE), m_cursorTargetType(CTT_NONE)
{
	for (size_t i = 0; i < 20; i++)
		m_textboxHistory[i] = "";
}

C_GUI::~C_GUI()
	: OgreBites::ApplicationContext("OgreTutorialApp")
{

}

void C_GUI::loadResources()
{
	loadResources();
}

void C_GUI::loadResources()
{
	//m_font.loadFromFile("Candara.ttf");
	//m_spriteCache.loadContent();

	////if (USE_GL)
	////	m_terrain.loadTerrain("Fractal.png", Vec2<u16>(64, 64), Vec2<u16>(64, 64));

	//// Load the user interface and set the resolution of the window to match it
	//m_interfaceTexture.loadFromFile("graphics/gui/interface.png");
	//m_interfaceSprite = sf::Sprite(m_interfaceTexture);

	//m_tilemap = new tmx::TileMap("maps/Area_0.tmx");
	
	//m_selfHitSoundBuffer.loadFromFile("audio/selfhurt.wav");
	//m_hitSound = sf::Sound(m_selfHitSoundBuffer);
}

bool C_GUI::update(const GameTime& gameTime)
{
	return true;
	//	// Update mouse state
	//	m_mouseState.m_buttonPressed[0] = false;
	//	m_mouseState.m_buttonPressed[1] = false;
	//	m_mouseState.m_buttonReleased[0] = false;
	//	m_mouseState.m_buttonReleased[1] = false;
	//
	//	sf::Event ev;
	//	while (m_window.pollEvent(ev))
	//	{
	//		if (ev.type == sf::Event::MouseMoved)
	//			m_mouseState.m_position = Vec2<u16>(ev.mouseButton.button, ev.mouseButton.x);
	//		else if (ev.type == sf::Event::MouseButtonPressed)
	//		{
	//			m_mouseState.m_buttonPressed[(u8)ev.mouseButton.button] = true;
	//			m_mouseState.m_buttonStates[(u8)ev.mouseButton.button] = true;
	//		}
	//		else if (ev.type == sf::Event::MouseButtonReleased)
	//		{
	//			m_mouseState.m_buttonReleased[(u8)ev.mouseButton.button] = true;
	//			m_mouseState.m_buttonStates[(u8)ev.mouseButton.button] = false;
	//		}
	//		else if (ev.type == sf::Event::Closed)
	//			return false;
	//	}
	//
	//	// Gather a list of all the entities under the mouse
	//	auto& entities = m_client->getEntityManager().getActiveEntities();
	//	std::vector<C_Entity*> m_entitiesUnderMouse;
	//	for (auto iter = entities.begin(); iter != entities.end(); iter++)
	//		if ((*iter)->getPosition() == m_mouseState.getMousePos() / 12)
	//			m_entitiesUnderMouse.push_back(*iter);
	//
	//	const auto& items = m_client->getItemManager().getItemsList();
	//	std::vector<DropableItem> m_itemsUnderMouse;
	//	for (auto iter = items.begin(); iter != items.end(); iter++)
	//	{
	//		Vec2s mousePos = m_mouseState.getMousePos() / 12;
	//		//printf("item pos:(%u,%u)   mousePos:(%u,%u)\n", itemPos.x, itemPos.y, mousePos.x, mousePos.y);
	//		if (mousePos == iter->pos)
	//			m_itemsUnderMouse.push_back(*iter);
	//	}
	//
	//	m_cursorTargetType = CTT_NONE;
	//	if (m_entitiesUnderMouse.size() > 0)
	//	{
	//		m_cursorTargetType = CTT_ENTITY;
	//		m_cursorTarget_entity = m_entitiesUnderMouse.front();
	//	}
	//	else if (m_itemsUnderMouse.size() > 0)
	//	{
	//		m_cursorTargetType = CTT_ITEM;
	//		m_cursorTarget_item = m_itemsUnderMouse.front();
	//	}
	//
	//	// Show options on right click
	//	if (m_mouseState.isButtonPressed(MB_Right))
	//		onRightClick(m_entitiesUnderMouse, m_itemsUnderMouse);
	//	m_highlightedOption = (m_mouseState.getMousePos().y - m_optionsPos.y) / 24;
	//	
	//	// Option box disappears if mouse laves the box
	//	if (m_optionsTargetType != CTT_NONE)
	//		if (!isMouseInOptionBox())
	//			m_optionsTargetType = CTT_NONE;
	//
	//	// Left click
	//	if (m_mouseState.isButtonPressed(MB_Left))
	//		onLeftClick();
	//
	//	return true;
}

void C_GUI::onRightClick(std::vector<C_Entity*> &m_entitiesUnderMouse, std::vector<DropableItem> &m_itemsUnderMouse)
//{
//	if (m_cursorTargetType != CTT_NONE)
//	{
//		m_optionsTargetType = m_cursorTargetType;
//		m_optionsPos = m_mouseState.getMousePos() - Vec2<u16>(4, 4); // TODO: Scale
//
//		switch (m_optionsTargetType)
//		{
//			case CTT_ENTITY:
//			{
//				m_optionsTarget_entity = m_entitiesUnderMouse.front();
//				m_optionsList = { RCO_INSPECT, RCO_ATTACK };
//				return;
//			}
//
//			case CTT_ITEM:
//			{
//				m_optionsTarget_item = m_itemsUnderMouse.front();
//				m_optionsList = { RCO_INSPECT, RCO_PICKUP };
//				return;
//			}
//
//			default:
//				return;
//		}
//	}
}

void C_GUI::onLeftClick()
//{
//	auto& packet = g_client->getPacket();
//
//	// Left clicked an option
//	if (m_optionsTargetType != CTT_NONE)
//	{
//		if (isMouseInOptionBox())
//			processRightClickOptionSelection(m_optionsList[m_highlightedOption]);
//		m_optionsTargetType = CTT_NONE;
//		return;
//	}
//
//	// Left clicked an item
//	if (m_cursorTargetType != CTT_NONE)
//	{
//		if (m_cursorTargetType == CTT_ITEM)
//		{
//			packet.write<u8>(0x05);
//			packet.write<DropableItem>(m_cursorTarget_item);
//			m_cursorTargetType = CTT_NONE;
//			return;
//		}
//		else if (m_cursorTargetType == CTT_ENTITY)
//		{
//			// Left click entity to attack
//			packet.write<u8>(0x01);
//			packet.write<u16>(m_cursorTarget_entity->getUID());
//			return;
//		}
//
//		return; // Should be unreachable
//	}
//
//	// Open left click. Used to walk to a position
//	packet.write<u8>(0x10);
//	packet.write<Vec2s>(m_mouseState.getMousePos() / 12);
//	return;
}

bool C_GUI::isMouseInOptionBox()
{
	return m_mouseState.getMousePos().x > m_optionsPos.x && m_mouseState.getMousePos().x < m_optionsPos.x + 120 &&
		m_mouseState.getMousePos().y > m_optionsPos.y && m_mouseState.getMousePos().y < m_optionsPos.y + m_optionsList.size() * 24;
}

void C_GUI::render(const GameTime& gameTime)
{
	//if (USE_GL)
	//{
	//	renderGL(gameTime);
	//	return;
	//}

	//m_window.clear(sf::Color(128, 128, 128));

	//// Draw the dropped items
	//for (auto& item : m_client->getItemManager().getItemsList())
	//	m_spriteCache.drawItemSprite(m_window, item.stack.type, item.pos * 12);

	//// Draw the entities
	//sf::CircleShape circ;
	//circ.setRadius(6);
	//circ.setOutlineThickness(1.0f);
	//circ.setOutlineColor(sf::Color::Black);
	//sf::Color color = sf::Color::White;
	//auto& entities = m_client->getEntityManager().getActiveEntities();
	//for (auto iter = entities.begin(); iter != entities.end(); iter++)
	//{
	//	auto entity = *iter;
	//	circ.setPosition(sf::Vector2f((float)entity->getPosition().x * 12.f, (float)entity->getPosition().y * 12.f));
	//	switch (entity->getEntityType())
	//	{
	//		case ET_ADMIN:
	//			color = sf::Color::Cyan;
	//			break;
	//		case ET_PLAYER:
	//			color = sf::Color::White;
	//			break;
	//		case ET_RAT:
	//		case ET_BIGRAT:
	//			color = sf::Color::Yellow;
	//			break;
	//		default:
	//			break;
	//	}
	//	if (entity->combatState.dead)
	//		color.r = std::max(color.r + 100, 0xFF);
	//	circ.setFillColor(color);
	//	m_window.draw(circ);
	//}

	//// Draw the hit markers
	//int count = 1;
	//for (auto defender = m_client->getEntityManager().getActiveEntities().begin(); defender != m_client->getEntityManager().getActiveEntities().end(); defender++)
	//{
	//	 for (auto hit = (*defender)->hitMarkers.begin(); hit != (*defender)->hitMarkers.end(); hit++)
	//	 {
	//		 sf::CircleShape circle = sf::CircleShape(7.0f, 12);
	//		 if (hit->damage == 0)
	//			 circle.setFillColor(sf::Color::Blue);
	//		 else
	//			 circle.setFillColor(sf::Color::Red);
	//		 circle.setPosition((float)(*defender)->getPosition().x * 12 - 1, (float)(*defender)->getPosition().y * 12 - 30 + hit->timer * 10.0f);

	//		 sf::Text text = sf::Text(to_string(hit->damage), m_font, 12);
	//		 text.setPosition(circle.getPosition() + sf::Vector2f(2, -2));

	//		 m_window.draw(circle);
	//		 m_window.draw(text);
	//		 count++;
	//	 }
	//}

	//// Draw health bars
	//sf::RectangleShape healthbarBackground = sf::RectangleShape();
	//healthbarBackground.setSize(sf::Vector2f(14, 3));
	//healthbarBackground.setFillColor(sf::Color::Red);
	//healthbarBackground.setOutlineThickness(1.0f);
	//healthbarBackground.setOutlineColor(sf::Color::Black);

	//sf::RectangleShape healthbarForeground = sf::RectangleShape();
	//healthbarForeground.setFillColor(sf::Color::Green);
	//for (auto iter = entities.begin(); iter != entities.end(); iter++)
	//{
	//	const auto& entity = *(*iter);
	//	if (entity.combatState.currentHealth == entity.combatState.maxHealth)
	//		continue;
	//	const sf::Vector2f healthbarPosition = sf::Vector2f(entity.getPosition().x * 12.f - 2.f, entity.getPosition().y * 12.f - 10.f);
	//	healthbarBackground.setPosition(healthbarPosition);
	//	healthbarForeground.setPosition(healthbarPosition);
	//	healthbarForeground.setSize(sf::Vector2f(12.f * (float)(*iter)->combatState.currentHealth / (float)(*iter)->combatState.maxHealth, 3.f));
	//	m_window.draw(healthbarBackground);
	//	m_window.draw(healthbarForeground);
	//}

	////----- DRAW THE INTERFACE -----//
	//m_window.draw(m_interfaceSprite);

	//// Draw the name of the cursor target
	//if (m_cursorTargetType != CTT_NONE)
	//{
	//	std::string str = "";
	//	switch (m_cursorTargetType)
	//	{
	//		case CTT_ENTITY:
	//		str = g_client->getLoader().getEntityName(m_cursorTarget_entity->getEntityType());
	//		break;

	//		case CTT_ITEM:
	//		str = g_client->getLoader().getItemName(m_cursorTarget_item.stack.type);
	//		break;

	//		default:
	//		break;
	//	}
	//	sf::Text text = sf::Text(str, m_font, 18);
	//	m_window.draw(text);
	//}

	//// Draw the right-click menu
	//if (m_optionsTargetType != CTT_NONE)
	//{
	//	auto recPos = sf::Vector2f(m_optionsPos.x, m_optionsPos.y);
	//	auto recSize = sf::Vector2f(120.0f, 6.0f + m_optionsList.size() * 24.0f);
	//	auto rec = sf::RectangleShape(recSize);
	//	rec.setPosition(recPos);
	//	rec.setFillColor(sf::Color(255, 211, 155, 127));
	//	rec.setOutlineColor(sf::Color(127, 105, 77, 127));
	//	rec.setOutlineThickness(1.5f);
	//	m_window.draw(rec);
	//	recPos += sf::Vector2f(3.0f, 3.0f);

	//	// Draw the highlight box
	//	rec.setPosition(recPos + sf::Vector2f(0, m_highlightedOption * 24.0f));
	//	rec.setSize(sf::Vector2f(114.0f, 24.0f));
	//	rec.setFillColor(sf::Color(200, 150, 120, 127));
	//	m_window.draw(rec);

	//	// Draw the options text
	//	for (unsigned int i = 0; i < m_optionsList.size(); i++)
	//	{
	//		sf::Text text;
	//		text = sf::Text(RightClickOptionNames[m_optionsList[i]], m_font, 17);
	//		text.setPosition(recPos);
	//		text.setFillColor(sf::Color::Black);
	//		recPos.y += 24.0f;
	//		m_window.draw(text);
	//	}
	//}

	//// Draw the textbox history
	//for (int i = 0; i < 7; i++)
	//{
	//	sf::Text lineText = sf::Text(m_textboxHistory[i], m_font, 17);
	//	lineText.setFillColor(sf::Color::Black);
	//	lineText.setPosition(15, 455 - 15.0f * (float)i);
	//	m_window.draw(lineText);
	//}

	//// Draw the inventory
	//for (int y = 0; y < 7; y++)
	//	for (int x = 0; x < 4; x++)
	//	{
	//		auto& itemStack = g_client->getPlayerInventory().itemStacks[y * 4 + x];
	//		if (itemStack.count == 0)
	//			continue;
	//		sf::Vector2f tileOffset = InventoryTileOffset;
	//		tileOffset.x *= x;
	//		tileOffset.y *= y;
	//		m_spriteCache.drawItemSprite(m_window, itemStack.type, InventoryPos + tileOffset, InventoryTileScale);
	//	}

	////m_terrain.render();

	//m_window.display();
}

void C_GUI::renderGL(const GameTime& gameTime)
{
//	m_window.clear(sf::Color(128, 128, 128));
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Apply some transformations for the cube
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(0.f, 0.f, -200.f);
//	const float angle = (float)gameTime.totalTime / 1000.f;
//	glRotatef(angle * 50, 1.f, 0.f, 0.f);
//	glRotatef(angle * 30, 0.f, 1.f, 0.f);
//	glRotatef(angle * 90, 0.f, 0.f, 1.f);
//
//	//Draw a cube
//	glBegin(GL_QUADS);//draw some squares
//	{
//		glColor3f(0, 1, 1); //cyan
//		glVertex3f(-50.f, -50.f, -50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(50.f, -50.f, -50.f);
//
//		glColor3f(0, 0, 1); //blue
//		glVertex3f(50.f, -50.f, 50.f);
//		glVertex3f(50.f, 50.f, 50.f);
//		glVertex3f(-50.f, 50.f, 50.f);
//		glVertex3f(-50.f, -50.f, 50.f);
//
//		glColor3f(1, 0, 1); //magenta
//		glVertex3f(-50.f, -50.f, 50.f);
//		glVertex3f(-50.f, 50.f, 50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(-50.f, -50.f, -50.f);
//
//		glColor3f(0, 1, 0); //green
//		glVertex3f(50.f, -50.f, -50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, 50.f);
//		glVertex3f(50.f, -50.f, 50.f);
//
//		glColor3f(1, 1, 0); //yellow
//		glVertex3f(-50.f, -50.f, 50.f);
//		glVertex3f(-50.f, -50.f, -50.f);
//		glVertex3f(50.f, -50.f, -50.f);
//		glVertex3f(50.f, -50.f, 50.f);
//
//		glColor3f(1, 0, 0); //red
//		glVertex3f(50.f, 50.f, 50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(-50.f, 50.f, 50.f);
//	}
//	glEnd();
//
//	//m_terrain.render();
//
//	m_window.display();
}

bool C_GUI::isOpen() const
{
	return m_window.isOpen();
}

const MouseState& C_GUI::getMouseState() const
{
	return m_mouseState;
}

void C_GUI::processRightClickOptionSelection(const RightClickOption selection)
{
	auto& packet = m_client->getPacket();
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
			switch (m_optionsTargetType)
			{
				case CTT_ENTITY:
					printInChatbox(g_client->getLoader().getEntityDescription(m_optionsTarget_entity->getEntityType()));
					return;

				case CTT_ITEM:
					printInChatbox(g_client->getLoader().getItemDescription(m_optionsTarget_item.stack.type));
					return;

			}
			return;
		}

		case RCO_ATTACK:
		{
			// Attack
			assert(m_optionsTargetType == CTT_ENTITY);
			packet.write<u8>(0x01);
			packet.write<u16>(m_optionsTarget_entity->getUID());
			break;
		}

		case RCO_PICKUP:
		{
			// Pick up
			packet.write<u8>(0x05);
			packet.write<DropableItem>(m_cursorTarget_item);
			return;
		}

		default:
			return;
	}
}

void C_GUI::printInChatbox(const string& text)
{
	for (int i = 19; i > 0; i--)
		m_textboxHistory[i] = m_textboxHistory[i - 1];
	m_textboxHistory[0] = text;
}
