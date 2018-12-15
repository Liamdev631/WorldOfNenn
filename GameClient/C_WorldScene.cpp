#include "C_WorldScene.h"
#include "Global.h"
#include "C_Client.h"
#include "Packets.h"
#include "Target.h"
#include "InventoryTab.h"

C_WorldScene::C_WorldScene()
{
	// Create the window handle with the given settings
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	m_window.create(sf::VideoMode(GuiSize.x, GuiSize.y), "World of Nenn", sf::Style::Default, settings);
	m_window.setVerticalSyncEnabled(true);

	// Create the render texture for the game world
	if (!m_gameScene.create(WorldSceneSize.x, WorldSceneSize.y, settings))
		printf("Failed to create game scene render texture (res:(x:%u y:%u))!\n", WorldSceneSize.x, WorldSceneSize.y);
	m_worldView = sf::View(sf::Vector2f(0, 0), sf::Vector2f((float)WorldSceneSize.x, (float)WorldSceneSize.y));

	// Load the map
	m_worldMap.loadMap("assets/maps/overworld.tmx");

	// Load the interface
	m_interface = &ResourceLoader::get().getSprite("assets/graphics/gui/interface.png")->sprite;

	// Load fonts
	m_font1 = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");

	// Set the default menu tab
	m_activeMenuTab = new InventoryTab();
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
		case sf::Event::Resized:
		{
			// adjust the viewport when the window is resized
			glViewport(0, 0, ev.size.width, ev.size.height);
			break;
		}

		case sf::Event::MouseMoved:
		{
			// Update the internal record of the mouse position
			m_mousePos = sf::Vector2f((float)ev.mouseMove.x, (float)ev.mouseMove.y);
			if (m_activeMenuTab)
				m_activeMenuTab->update(sf::Vector2f((float)m_mousePos.x, (float)m_mousePos.y));
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
				onLeftClick();
			else if (ev.mouseButton.button == sf::Mouse::Button::Right)
				onRightClick();
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			if (m_activeMenuTab)
				m_activeMenuTab->onMouseReleased(ev.mouseButton.button, sf::Vector2f((float)ev.mouseButton.x, (float)ev.mouseButton.y));
			break;
		}
		}
	}

	update();
}

void C_WorldScene::update()
{
	auto thisPlayer = C_WorldManager::get().getThisEntity();

	// Calculate which cell is pointed to by the mouse
	float destx = (float)m_mousePos.x + m_worldView.getCenter().x - m_worldView.getSize().x / 2;
	float desty = (float)m_mousePos.y + m_worldView.getCenter().y - m_worldView.getSize().y / 2;
	if (destx > 0 && desty > 0)
	{
		m_mouseTargetCell.x = (u16)(destx / 16.f);
		m_mouseTargetCell.y = (u16)(desty / 16.f);
		m_crosshair.setPosition(sf::Vector2f((float)m_mouseTargetCell.x * 16, (float)m_mouseTargetCell.y * 16));
	}

	// If a right click option is showing, determine which option is currently being pointed to
	if (m_optionsList.size() != 0)
		m_highlightedOption = max<int>(min<int>((m_mousePos.y - m_optionsPos.y) / 16.f, (int)m_optionsList.size() - 1), 0);

	m_entitiesUnderMouse.clear();
	m_itemsUnderMouse.clear();
	if (WorldSceneBounds.contains(m_mousePos))
	{
		// Determine which entities are under the mouse
		for (C_Entity* e : C_WorldManager::get().getActiveEntities())
			if (e->position == m_mouseTargetCell)
				m_entitiesUnderMouse.push_back(e);

		// Determine which items are under the mouse
		for (DropableItem& item : C_ItemManager::get().getItemsList())
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
			m_mouseTarget.uid = lowest;
			m_mouseTarget._entity = C_WorldManager::get().getEntity(lowest);
		}
		else if (m_itemsUnderMouse.size() > 0)
		{
			m_mouseTarget.type = TargetType::TT_ITEM;
			m_mouseTarget._item = m_itemsUnderMouse.front();
		}
	}
}

void C_WorldScene::draw()
{
	/////// World Space ///////
	drawGameScene();

	/////// Screen Space ///////
	drawGui();
}

void C_WorldScene::drawGameScene()
{
	m_gameScene.clear(sf::Color::Magenta);

	// Center the camera on the player
	auto thisPlayer = C_WorldManager::get().getThisEntity();
	if (thisPlayer)
	{
		m_worldView.setCenter((float)thisPlayer->drawPos.x * 16, (float)thisPlayer->drawPos.y * 16);
		m_gameScene.setView(m_worldView);
	}

	// Draw the map
	m_gameScene.draw(m_worldMap);

	// Draw all items
	for (auto& item : C_ItemManager::get().getItemsList())
	{
		auto& sprite = ResourceLoader::get().getItemSprite(item.stack);
		sprite.setPosition(sf::Vector2f(item.pos.x * 16.f + 2, item.pos.y * 16.f + 2));
		m_gameScene.draw(sprite);
	}

	// Draw all entities
	for (auto& c : C_WorldManager::get().getActiveEntities())
		m_gameScene.draw(*c);

	// Draw the crosshair
	m_gameScene.draw(m_crosshair);

	// Done drawing
	m_gameScene.display();
}

void C_WorldScene::drawGui()
{
	m_window.clear(sf::Color::Black);

	// Draw the game scene in the top-left corner
	sf::Sprite gameSceneSprite = sf::Sprite(m_gameScene.getTexture());
	gameSceneSprite.setPosition(WorldScenePos);
	m_window.draw(gameSceneSprite);

	// Draw the interface skin
	m_window.draw(*m_interface);

	// Draw the chat box
	m_window.draw(m_chatbox);

	// Draw the active menu tab
	m_activeMenuTab->draw(m_window);

	// Draw the name of the cursor target
	if (m_mouseTarget.type != TT_NONE)
	{
		sf::String str = "";
		switch (m_mouseTarget.type)
		{
		case TT_ENTITY:
			str = Loader::get().getEntityName(m_mouseTarget._entity->entityType);
			break;

		case TT_ITEM:
			str = Loader::get().getItemName(m_mouseTarget._item.stack.type);
			break;

		default:
			break;
		}
		sf::Text text = sf::Text(str, *m_font1, 14U);
		text.setOutlineColor(sf::Color::White);
		text.setOutlineThickness(1.f);
		text.setFillColor(sf::Color::Black);
		m_window.draw(text);
	}

	// Draw the right-click menu
	if (m_optionsList.size() > 0)
	{
		// Create an array of the sf::Text objects
		sf::Text* texts = new sf::Text[m_optionsList.size()];
		for (unsigned int i = 0; i < m_optionsList.size(); i++)
		{
			auto& curOption = m_optionsList[i];
			std::wstring targetName = L"<target-name>";
			switch (curOption.target.type)
			{
			case TT_ENTITY: targetName = Loader::get().getEntityName(curOption.target._entity->entityType); break;
			case TT_ITEM:
			case TT_INV_ITEM: targetName = Loader::get().getItemName(curOption.target._item.stack.type); break;
			}
			texts[i] = sf::Text(RightClickOptionNames[m_optionsList[i].option] + L" " + targetName, *m_font1, 14);
			texts[i].setFillColor(sf::Color::Black);
		}

		float menuWidth = 160.0f;

		auto recPos = sf::Vector2f(m_optionsPos.x, m_optionsPos.y);
		auto recSize = sf::Vector2f(menuWidth, 6.f + m_optionsList.size() * 16.f);
		auto rec = sf::RectangleShape(recSize);
		rec.setPosition(recPos);
		rec.setFillColor(sf::Color(255, 211, 155, 127));
		rec.setOutlineColor(sf::Color(127, 105, 77, 127));
		rec.setOutlineThickness(1.f);
		m_window.draw(rec);
		recPos += sf::Vector2f(3.0f, 3.0f);

		// Draw the highlight box
		rec.setPosition(recPos + sf::Vector2f(0, m_highlightedOption * 16.f));
		rec.setSize(sf::Vector2f(menuWidth - 6.f, 16.f));
		rec.setFillColor(sf::Color(200, 150, 120, 127));
		m_window.draw(rec);

		// Draw the options text
		for (unsigned int i = 0; i < m_optionsList.size(); i++)
		{
			texts[i].setPosition(recPos);
			recPos.y += 16.0f;
			m_window.draw(texts[i]);
		}
	}


	// End draw
	m_window.display();
}

void C_WorldScene::onLeftClick()
{
	auto& packet = C_Client::get().getPacket();

	// Left clicked an option
	if (m_optionsList.size() > 0)
	{
		if (isMouseInOptionBox())
			processRightClickOptionSelection(m_optionsList[m_highlightedOption]);
		m_optionsList.clear();
		return;
	}

	if (WorldSceneBounds.contains(m_mousePos)) // Clicked the scene
	{
		// Left clicked an item
		if (m_mouseTarget.type != TargetType::TT_NONE)
		{
			if (m_mouseTarget.type == TargetType::TT_ITEM)
			{
				packet.write(CP_ItemPicked(m_mouseTarget._item));
				m_mouseTarget.type = TargetType::TT_NONE;
				return;
			}
			else if (m_mouseTarget.type == TargetType::TT_ENTITY)
			{
				// Left click entity to attack
				packet.write(CP_AttackEntity(m_mouseTarget._entity->uid));
				return;
			}
			else if (m_mouseTarget.type == TargetType::TT_INV_ITEM)
			{
				return;
			}

			return; // Should be unreachable
		}

		// Open left click. Used to walk to a position
		auto thisPlayer = C_WorldManager::get().getThisEntity();
		if (thisPlayer)
			packet.write(CP_MoveToDest(m_mouseTargetCell));
	}
	else if (m_activeMenuTab && m_activeMenuTab->getBounds().contains(m_mousePos))
		m_activeMenuTab->onMousePressed(sf::Mouse::Button::Left, m_mousePos);


	return;
}

void C_WorldScene::onRightClick()
{
	if (WorldSceneBounds.contains(m_mousePos))
	{
		if (m_entitiesUnderMouse.size() > 0 || m_itemsUnderMouse.size() > 0)
		{
			// Open the rco menu
			std::vector<RCOption> options;

			// Add entities
			for (size_t i = 0; i < m_entitiesUnderMouse.size(); i++)
			{
				// Add an entity to the rco menu options list
				// TODO: Entities with a highe level than the player have the attack option shown last
				Target t;
				t.type = TT_ENTITY;
				t._entity = m_entitiesUnderMouse[i];
				t.uid = m_entitiesUnderMouse[i]->uid;
				options.push_back(RCOption(t, RCO_ATTACK));
				options.push_back(RCOption(t, RCO_INSPECT));
			}

			// Add items
			for (auto i = 0; i < m_itemsUnderMouse.size(); i++)
			{
				// Add an entity to the rco menu options list
				// TODO: Entities with a highe level than the player have the attack option shown last
				Target t;
				t.type = TT_ITEM;
				t._item = m_itemsUnderMouse[i];
				options.push_back(RCOption(t, RCO_PICKUP));
				options.push_back(RCOption(t, RCO_INSPECT));
			}

			C_WorldScene::get().setRightClickOptions(m_mousePos - sf::Vector2f(4, 4), options);
		}

		//if (m_mouseTarget.type != TT_NONE)
		//{
		//	switch (m_mouseTarget.type)
		//	{
		//	case TT_ENTITY:
		//	{
		//		//setRightClickOptions(m_mouseTarget, m_mousePos - sf::Vector2f(4, 4), { RCO_INSPECT, RCO_ATTACK });
		//		std::vector<RCOption> options;
		//		options.push_back(RCOption(m_mouseTarget, RCO_INSPECT));
		//		options.push_back(RCOption(m_mouseTarget, RCO_ATTACK));
		//		C_WorldScene::get().setRightClickOptions(m_mousePos - sf::Vector2f(4, 4), options);
		//		return;
		//	}

		//	case TT_ITEM:
		//	{
		//		//setRightClickOptions(m_mouseTarget, m_mousePos - sf::Vector2f(4, 4), { RCO_INSPECT, RCO_PICKUP });
		//		std::vector<RCOption> options;
		//		options.push_back(RCOption(m_mouseTarget, RCO_INSPECT));
		//		options.push_back(RCOption(m_mouseTarget, RCO_PICKUP));
		//		C_WorldScene::get().setRightClickOptions(m_mousePos - sf::Vector2f(4, 4), options);
		//		return;
		//	}
		//	default:
		//		return;
		//	}
		//}
	}
	else if (m_activeMenuTab && m_activeMenuTab->getBounds().contains(m_mousePos))
		m_activeMenuTab->onMousePressed(sf::Mouse::Button::Right, m_mousePos);
}

void C_WorldScene::setRightClickOptions(const sf::Vector2f& position, const std::vector<RCOption>& options)
{
	m_optionsPos = position;
	m_optionsList = options;
}

bool C_WorldScene::isMouseInOptionBox()
{
	return m_mousePos.x > m_optionsPos.x && m_mousePos.x < m_optionsPos.x + 120 &&
		m_mousePos.y > m_optionsPos.y && m_mousePos.y < m_optionsPos.y + m_optionsList.size() * 24;
}

void C_WorldScene::processRightClickOptionSelection(const RCOption& selection)
{
	if (selection.option == RightClickOption::RCO_NONE)
		return;
	auto& packet = C_Client::get().getPacket();
	switch (selection.option)
	{
	case RCO_INSPECT:
	{
		// Inspect
		switch (selection.target.type)
		{
		case TT_ENTITY:
			printInChatbox(Loader::get().getEntityDescription(selection.target._entity->entityType));
			return;

		case TT_ITEM:
		case TT_INV_ITEM:
			printInChatbox(Loader::get().getItemDescription(selection.target._item.stack.type));
			return;

		}
		return;
	}

	case RCO_ATTACK:
	{
		// Attack
		assert(selection.target.type == TT_ENTITY);
		packet.write(CP_AttackEntity(selection.target._entity->uid));
		break;
	}

	case RCO_PICKUP:
	{
		assert(selection.target.type == TT_ITEM);
		packet.write(CP_ItemPicked(selection.target._item));
		return;
	}

	case RCO_DROP:
	{
		assert(selection.target.type == TT_INV_ITEM);
		packet.write(CP_ItemDropped(selection.target._item.stack, selection.target.uid));
		return;
	}

	default:
		return;
	}
}

void C_WorldScene::printInChatbox(const std::wstring& text)
{
	m_chatbox.addText(text);
}

