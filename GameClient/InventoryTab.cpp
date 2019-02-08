#include "InventoryTab.h"
#include "C_Client.h"
#include "ResourceLoader.h"
#include "Packets.h"
#include "C_WorldManager.h"
#include "RCOption.h"
#include "SceneManager.h"

InventoryTab::InventoryTab()
{
	auto itemCountFont = ResourceLoader::get().getFont("assets/fonts/Candarab.ttf");
	const auto tabSize = sf::Vector2f(32, 32);
	for (int i = 0; i < 28; i++)
	{
		// Calculate the bounds of the item spriteS
		sf::Vector2f centerPos;
		int x = i % 4, y = i / 4;
		centerPos.x = m_menuBounds.left + (m_menuBounds.width / 5.f) * (x + 1);
		centerPos.y = m_menuBounds.top + (m_menuBounds.height / 8.f) * (y + 1);
		sf::Vector2f topLeft = centerPos - tabSize / 2.f;

		auto& itemImage = m_itemSlotImages[i];
		itemImage.setPosition(topLeft);
		itemImage.setSize(tabSize);
		itemImage.setOutlineColor(sf::Color::White);

		// Place the item count text
		auto& itemCount = m_itemCountText[i];
		itemCount.setFont(*itemCountFont);
		itemCount.setPosition(topLeft + sf::Vector2f(0, -10.f));
		itemCount.setString(L"");
		itemCount.setFillColor(sf::Color::Black);
		itemCount.setCharacterSize(13);
	}
}

InventoryTab::~InventoryTab()
{

}

void InventoryTab::setHighlightedSlot(const int& slotClicked)
{
	for (int i = 0; i < 28; i++)
		m_itemSlotImages[i].setOutlineThickness(0.0f);
	if (slotClicked > -1 && slotClicked < 28)
		m_itemSlotImages[slotClicked].setOutlineThickness(1.0f);
}

int InventoryTab::getSlotUnderMouse(const sf::Vector2f& mousePos) const
{
	if (!m_menuBounds.contains(mousePos))
		return -1;
	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 28; i++)
	{
		if (inventory.itemStacks[i].count == 0)
			continue;
		if (m_itemSlotImages[i].getGlobalBounds().contains(mousePos))
			return i;
	}
	return -1;
}

// UIComponent override

void InventoryTab::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	switch (ev.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		setHighlightedSlot(-1);

		if (!m_menuBounds.contains(mousePos))
			return;

		int slotClicked = getSlotUnderMouse(mousePos);
		if (slotClicked < 0 || slotClicked >= 28)
		{
			// Mouse did not click a slot
			return;
		}

		auto& playerInventory = C_Client::get().getPlayerInventory();
		auto& clickedStack = playerInventory.itemStacks[slotClicked];
		if (clickedStack.count == 0)
			return; // The slot clicked was empty
		auto& tabClicked = m_itemSlotImages[slotClicked];

		switch (ev.mouseButton.button)
		{
		case sf::Mouse::Button::Left:
		{
			// Shift + Left Click to drop items
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				C_Client::get().getPacket().write(CP_ItemDropped(clickedStack, (uint8_t)slotClicked));
			else 
				C_Client::get().getPacket().write(CP_UseItem(clickedStack.type, (uint8_t)slotClicked));
			return;
		}

		case sf::Mouse::Button::Right:
		{
			openRCOMenuForSlot(slotClicked);
			return;
		}

		default:
			return;
		}
		return;
	}
	}
}

void InventoryTab::openRCOMenuForSlot(const int &slotClicked)
{
	auto& playerInventory = C_Client::get().getPlayerInventory();
	auto& clickedStack = playerInventory.itemStacks[slotClicked];
	auto& clickedTab = m_itemSlotImages[slotClicked];

	// Highlight the selected item and unhighlight others
	setHighlightedSlot(slotClicked);

	// Open the rco menu
	Target target; // target info
	target.type = TargetType::TT_INV_ITEM;
	target.uid = (uint8_t)slotClicked;
	target._item.stack = clickedStack;
	//sf::Vector2f position = mousePos - sf::Vector2f(4, 4); // draw position
	sf::Vector2f position = clickedTab.getPosition() + clickedTab.getSize() / 2.f; // Open around the center of the icon

	// Build the list of options
	std::vector<RCOption> options;
	options.push_back(RCOption(target, RCO_USE));
	options.push_back(RCOption(target, RCO_INSPECT));
	options.push_back(RCOption(target, RCO_DROP));

	SceneManager::get().setRightClickOptions(position, options);
}

void InventoryTab::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 28; i++)
		if (inventory.itemStacks[i].count > 0)
		{
			auto itemTexture = ResourceLoader::get().getItemTexture(inventory.itemStacks[i]);
			m_itemSlotImages[i].setTexture(itemTexture);
			if (inventory.itemStacks[i].count > 1)
				m_itemCountText[i].setString(inventory.itemStacks[i].getFormattedCountString());
		}
}

void InventoryTab::draw(sf::RenderTarget& target) const
{
	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 28; i++)
		if (inventory.itemStacks[i].count > 0)
		{
			target.draw(m_itemSlotImages[i]);
			if (inventory.itemStacks[i].count > 1)
				target.draw(m_itemCountText[i]);
		}
}
