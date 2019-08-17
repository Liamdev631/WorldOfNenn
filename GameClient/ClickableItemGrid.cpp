#include "ClickableItemGrid.h"
	
ClickableItemGrid::ClickableItemGrid(const GridSettings& settings)
{
	setSettings(settings);
}

void ClickableItemGrid::setSettings(const GridSettings& settings)
{
	this->settings = settings;
	recalcDrawables();
}

void ClickableItemGrid::recalcDrawables()
{
	if (!container)
		return;
	if (itemSlotImages)
	{
		delete[] itemSlotImages;
		delete[] itemBorders;
		delete[] itemCountText;
	}
	itemSlotImages = new sf::RectangleShape[m_containerSize];
	itemBorders = new sf::RectangleShape[m_containerSize];
	itemCountText = new sf::Text[m_containerSize];

	auto itemCountFont = ResourceLoader::get().getFont("assets/fonts/Candarab.ttf");
	int columnSize = m_containerSize / settings.rowSize;
	for (int i = 0; i < m_containerSize; i++)
	{
		// Calculate the bounds of the item spriteS
		sf::Vector2f centerPos;
		int x = i % 5, y = i / 5;
		centerPos.x = (float)int(settings.bounds.left + (settings.bounds.width / 6.f) * (x + 1));
		centerPos.y = (float)int(settings.bounds.top + (settings.bounds.height / 7.f) * (y + 1));
		sf::Vector2f topLeft = centerPos - settings.cellSize / 2.f;

		auto& itemImage = itemSlotImages[i];
		itemImage.setPosition(topLeft);
		itemImage.setSize(settings.cellSize);

		auto& itemBorder = itemBorders[i];
		itemBorder.setPosition(topLeft);
		itemBorder.setSize(settings.cellSize);
		itemBorder.setFillColor(sf::Color::Transparent);
		itemBorder.setOutlineColor(sf::Color::Black);
		itemBorder.setOutlineThickness(1.0f);

		// Place the item count text
		auto& itemCount = itemCountText[i];
		itemCount.setFont(*itemCountFont);
		itemCount.setPosition(topLeft + sf::Vector2f(0, -10.f));
		itemCount.setString(L"");
		itemCount.setFillColor(sf::Color::Black);
		itemCount.setCharacterSize(13);
	}
}

void ClickableItemGrid::setContainer(ItemStack const* firstItem, u16 size)
{
	this->m_containerSize = size;
	this->container = firstItem;
	recalcDrawables();
}

int ClickableItemGrid::getSlotUnderMouse(const sf::Vector2f& mousePos) const
{
	
	for (int i = 0; i < m_containerSize; i++)
		if (itemSlotImages[i].getGlobalBounds().contains(mousePos))
			return i;
	return -1;
}

void ClickableItemGrid::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{

}

void ClickableItemGrid::update(const GameTime& time, const sf::Vector2f& mousePos)
{

}

void ClickableItemGrid::draw(sf::RenderTarget& target) const
{

}