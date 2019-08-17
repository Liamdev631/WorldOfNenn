#include "InventoryTab.h"
#include "C_Client.h"
#include "ResourceLoader.h"
#include "Packets.h"
#include "C_WorldManager.h"
#include "RCOption.h"
#include "SceneManager.h"

constexpr float ItemDragDistanceCutoff = 4.0f;

InventoryTab::InventoryTab()
	: MenuTab()
{
	// Initialize the dragable item
	GridSettings settings;
	settings.bounds = { 519, 205, 241, 261 };
	settings.rowSize = 5;
	settings.cellSize = { 32, 32 };
	m_grid.setSettings(settings);
	m_grid.setContainer(&C_Client::get().getPlayerInventory().itemStacks[0], 30);
}

InventoryTab::~InventoryTab()
{

}

void InventoryTab::setHighlightedSlot(const int& slotClicked)
{
	for (int i = 0; i < 30; i++)
		m_grid.itemBorders[i].setOutlineColor(sf::Color::Black);
	if (slotClicked > -1 && slotClicked < 30)
		m_grid.itemBorders[slotClicked].setOutlineColor(sf::Color::Yellow);
}

// UIComponent override

void InventoryTab::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	switch (ev.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			//draggedItem.exists = false;
			setHighlightedSlot(-1);

			if (!m_menuBounds.contains(mousePos))
				return;

			int slotClicked = m_grid.getSlotUnderMouse(mousePos);
			if (slotClicked < 0 || slotClicked >= 28) // Mouse did not click a slot
				return;

			switch (ev.mouseButton.button)
			{
				case sf::Mouse::Button::Left:
				{
					auto& playerInventory = C_Client::get().getPlayerInventory();
					auto& clickedStack = playerInventory.itemStacks[slotClicked];
					if (clickedStack.count == 0)
						return;

					// Shift + Left Click to drop items
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					{
						C_Client::get().getPacket().write(CP_ItemDropped(clickedStack, (uint8_t)slotClicked));
						return;
					}

					if (clickedStack.count == 0)
						return;

					DraggableItem draggedItem;
					draggedItem.exists = true;
					draggedItem.slot = slotClicked;
					draggedItem.shape.setTexture(ResourceLoader::get().getItemTexture(clickedStack));
					draggedItem.startPos = mousePos;
					SceneManager::get().setDraggedItem(draggedItem);

					return;
				}

				case sf::Mouse::Button::Right:
				{
					openRCOMenuForSlot(slotClicked);
					return;
				}
			}
		}

		case sf::Event::MouseButtonReleased:
		{
			if (ev.mouseButton.button == sf::Mouse::Button::Left)
			{
				/*if (!draggedItem.exists)
					return;
				draggedItem.exists = false;
				if (!m_menuBounds.contains(mousePos))
					return;

				auto& playerInventory = C_Client::get().getPlayerInventory();
				auto& clickedStack = playerInventory.itemStacks[draggedItem.slot];

				if (draggedItem.draggedDistance < ItemDragDistanceCutoff)
				{
					// If the mouse hasn't moved much since the drag started, just use the item.
					C_Client::get().getPacket().write(CP_UseItem(clickedStack.type, draggedItem.slot));
					return;
				}
				else
				{
					// The item was actually dragged. Try to move it to a new slot.
					auto toSlot = m_grid.getSlotUnderMouse(mousePos);
					if (toSlot == -1)
						return;
					C_Client::get().getPacket().write(CP_SubmitItem(clickedStack, MoveItemOptions(draggedItem.slot, toSlot)));
					return;
				}*/
			}
		}
	}
}

void InventoryTab::openRCOMenuForSlot(const int &slotClicked)
{
	auto& playerInventory = C_Client::get().getPlayerInventory();
	auto& clickedStack = playerInventory.itemStacks[slotClicked];
	if (clickedStack.count == 0)
		return;
	auto& clickedTab = m_grid.itemSlotImages[slotClicked];

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
	printf("");
	// Highlight the slot hovered by the mouse
	int slotUnderMouse = m_grid.getSlotUnderMouse(mousePos);
	setHighlightedSlot(slotUnderMouse);

	// Move the dragged item if it exists
	/*if (draggedItem.exists)
	{
		draggedItem.shape.setPosition(mousePos);
		auto dragOffset = draggedItem.startPos - mousePos;
		draggedItem.draggedDistance = sqrtf(dragOffset.x * dragOffset.x + dragOffset.y * dragOffset.y);
	}*/

	auto& inventory = C_Client::get().getPlayerInventory();
	for (int i = 0; i < 30; i++)
		if (inventory.itemStacks[i].count > 0)
		{
			// Update item textures
			m_grid.itemSlotImages[i].setTexture(ResourceLoader::get().getItemTexture(inventory.itemStacks[i]));

			// Update item count strings
			if (inventory.itemStacks[i].count > 1)
				m_grid.itemCountText[i].setString(inventory.itemStacks[i].getFormattedCountString());
		}
}

void InventoryTab::draw(sf::RenderTarget& target) const
{
	auto& inventory = C_Client::get().getPlayerInventory();

	for (int i = 0; i < 30; i++)
	{
		if (inventory.itemStacks[i].count > 0)
		{
			// Draw the item icon
			target.draw(m_grid.itemSlotImages[i]);
			
			// Draw the item count
			if (inventory.itemStacks[i].count > 1)
				target.draw(m_grid.itemCountText[i]);
		}

		// Draw the item border
		target.draw(m_grid.itemBorders[i]);
	}

	// Draw the dragged item if it exists
	//if (draggedItem.exists && draggedItem.draggedDistance > ItemDragDistanceCutoff)
	//	target.draw(draggedItem.shape);
}
