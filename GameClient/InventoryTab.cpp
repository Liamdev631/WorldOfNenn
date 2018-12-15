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
	}
}

InventoryTab::~InventoryTab()
{

}

void InventoryTab::draw(sf::RenderTarget& target)
{
	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 28; i++)
		if (inventory.itemStacks[i].count > 0)
			drawItemStack(target, inventory.itemStacks[i], i);
}

void InventoryTab::onMousePressed(const sf::Mouse::Button& button, const sf::Vector2f& mousePos)
{
	int slotClicked = getSlotUnderMouse(mousePos);
	if (slotClicked < 0 || slotClicked >= 28)
		return; // Mouse did not click a slot

	auto& playerInventory = C_Client::get().getPlayerInventory();
	auto& clickedStack = playerInventory.itemStacks[slotClicked];
	if (clickedStack.count == 0)
		return; // The slot clicked was empty

	switch (button)
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
		// Open the rco menu
		Target target; // target info
		target.type = TargetType::TT_INV_ITEM;
		target.uid = (uint8_t)slotClicked;
		target._item.stack = clickedStack;
		sf::Vector2f position = mousePos - sf::Vector2f(4, 4); // draw position

		// Build the list of options
		std::vector<RCOption> options;
		options.push_back(RCOption(target, RCO_INSPECT));
		options.push_back(RCOption(target, RCO_USE));
		options.push_back(RCOption(target, RCO_DROP));

		C_WorldScene::get().setRightClickOptions(position, options);
		return;
	}

	default:
		return;
	}
	return;
}

void InventoryTab::onMouseReleased(const sf::Mouse::Button& button, const sf::Vector2f& mousePos)
{

}

void InventoryTab::update(const sf::Vector2f& mousePos)
{

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
