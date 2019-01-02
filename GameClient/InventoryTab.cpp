#include "InventoryTab.h"
#include "C_Client.h"
#include "ResourceLoader.h"
#include "Packets.h"
#include "C_WorldScene.h"

InventoryTab::InventoryTab()
{
	const auto tabSize = sf::Vector2f(32, 32);
	for (int i = 0; i < 28; i++)
	{
		// Calculate the bounds of the item sprite
		sf::Vector2f centerPos;
		int x = i % 4, y = i / 4;
		centerPos.x = m_menuBounds.left + (m_menuBounds.width / 5.f) * (x + 1);
		centerPos.y = m_menuBounds.top + (m_menuBounds.height / 8.f) * (y + 1);
		sf::Vector2f topLeft = centerPos - tabSize / 2.f;
		m_tabs[i].setPosition(topLeft);
		m_tabs[i].setSize(tabSize);

		// Set the border color
		m_tabs[i].setOutlineColor(sf::Color::White);
	}
}

InventoryTab::~InventoryTab()
{

}

void InventoryTab::setHighlightedSlot(const int& slotClicked)
{
	for (int i = 0; i < 28; i++)
		m_tabs[i].setOutlineThickness(0.0f);
	if (slotClicked > -1 && slotClicked < 28)
		m_tabs[slotClicked].setOutlineThickness(1.0f);
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
		if (m_tabs[i].getGlobalBounds().contains(mousePos))
			return i;
	}
	return -1;
}

void InventoryTab::drawItemStack(sf::RenderTarget& target, const ItemStack& item, const int& slot)
{
	// Grab the sprite and move it into position
	auto itemTexture = ResourceLoader::get().getItemSprite(item).getTexture();
	m_tabs[slot].setTexture(itemTexture);
	target.draw(m_tabs[slot]);
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
		auto& tabClicked = m_tabs[slotClicked];

		switch (ev.mouseButton.button)
		{
		case sf::Mouse::Button::Left:
		{
			// Shift + Left Click to drop items
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				C_Client::get().getPacket().write(CP_ItemDropped(clickedStack, (uint8_t)slotClicked));
			}
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
	auto& clickedTab = m_tabs[slotClicked];

	// Highlight the selected item and unhighlight others
	setHighlightedSlot(slotClicked);

	// Open the rco menu
	Target target; // target info
	target.type = TargetType::TT_INV_ITEM;
	target.uid = (uint8_t)slotClicked;
	target._item.stack = clickedStack;
	//sf::Vector2f position = mousePos - sf::Vector2f(4, 4); // draw position
	sf::Vector2f position = clickedTab.getPosition() + clickedTab.getSize();

	// Build the list of options
	std::vector<RCOption> options;
	options.push_back(RCOption(target, RCO_USE));
	options.push_back(RCOption(target, RCO_INSPECT));
	options.push_back(RCOption(target, RCO_DROP));

	C_WorldScene::get().setRightClickOptions(position, options);
}

void InventoryTab::update(const sf::Vector2f& mousePos)
{

}

void InventoryTab::draw(sf::RenderTarget& target)
{
	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 28; i++)
		if (inventory.itemStacks[i].count > 0)
			drawItemStack(target, inventory.itemStacks[i], i);
}
