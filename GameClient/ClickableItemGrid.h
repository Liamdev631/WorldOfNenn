#pragma once
#include "Types.h"
#include "ItemContainer.h"
#include "UIComponent.h"

struct DraggableItem
{
	bool exists;
	u8 slot;
	sf::RectangleShape shape;
	sf::Vector2f startPos;
	float draggedDistance;

	DraggableItem()
		: shape(sf::RectangleShape({ 32, 32 }))
	{

	}
};

struct GridSettings
{
	sf::Vector2f cellSize;
	sf::FloatRect bounds;
	int rowSize;
};

class ClickableItemGrid : public UIComponent
{
public:
	GridSettings settings;

	sf::RectangleShape* itemSlotImages;
	sf::RectangleShape* itemBorders;
	sf::Text* itemCountText;
	u16 m_containerSize = 0;

public:
	ClickableItemGrid() = default;
	ClickableItemGrid(const GridSettings& settings);

	void setSettings(const GridSettings& settings);

	void recalcDrawables();

	void setContainer(const ItemStack* firstItem, u16 size);

	// Returns which slot of the inventory is being pointed at by the mouse, or -1 if none are
	int getSlotUnderMouse(const sf::Vector2f& mousePos) const;

private:
	ItemStack const* container;

	// UIComponent override
	void onEvent(const sf::Event& ev, const sf::Vector2f& mousePos) override;
	void update(const GameTime& time, const sf::Vector2f& mousePos) override;
	void draw(sf::RenderTarget& target) const override;
};
