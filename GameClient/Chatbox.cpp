#include "Chatbox.h"
#include "ResourceLoader.h"

Chatbox::Chatbox()
{
	// Load the chat font
	auto font = ResourceLoader::get().getFont("assets/fonts/Candara.ttf");
	if (!font)
		printf("Failed to load Chatbox font!\n");
	
	// Initialize the text buffer
	for (int i = 0; i < TextBufferLength; i++)
	{
		auto& t = m_text[i];
		t.setFont(*font);
		t.setFillColor(sf::Color::Black);
		t.setCharacterSize(15);
	}

	setScrollValue(0.0f);
}

Chatbox::~Chatbox()
{

}

void Chatbox::addText(const std::wstring& str)
{
	// Shift the text history and insert the new text at the bottom
	for (int i = TextBufferLength - 1; i >= 0; i--)
		m_text[i + 1].setString(m_text[i].getString());
	string newText = sf::String::fromUtf8(str.begin(), str.end());
	m_text[0].setString(sf::String(newText));
}

void Chatbox::setScrollValue(const float& value)
{
	// Move all the lines
	float firstLineYPos = ChatboxPos.y + ChatboxSize.y - 6.f;
	firstLineYPos += value * m_text[0].getCharacterSize();
	for (int i = 0; i < TextBufferLength; i++)
		m_text[i].setPosition((float)ChatboxPos.x + 6.f, firstLineYPos - (float)m_text[0].getCharacterSize() * (i + 1));
}

void Chatbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw each line of text
	for (int i = 0; i < TextBufferLength; i++)
		target.draw(m_text[i]);
}
