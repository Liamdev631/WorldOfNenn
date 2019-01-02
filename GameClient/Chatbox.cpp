#include "Chatbox.h"
#include "ResourceLoader.h"

Chatbox::Chatbox()
	: m_active(false)
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
	m_inputBuffer = "";
	m_inputBufferText.setFont(*font);
	m_inputBufferText.setFillColor(sf::Color::Black);
	m_inputBufferText.setCharacterSize(15);
	m_inputBufferText.setPosition((float)ChatboxPos.x + 6.f, ChatboxPos.y + ChatboxSize.y - 18.f);
	m_inputBufferText.setString(L"Chat: *");

	setScrollValue(0.0f);
}

Chatbox::~Chatbox()
{

}

void Chatbox::addText(const std::wstring& str)
{
	// Shift the text history and insert the new text at the bottom
	for (int i = 0; i < TextBufferLength - 2; i++)
		m_text[i + 1].setString(m_text[i].getString());
	m_text[0].setString(sf::String::fromUtf8(str.begin(), str.end()));
}

void Chatbox::setScrollValue(const float& value)
{
	// Move all the lines
	float firstLineYPos = ChatboxPos.y + ChatboxSize.y - 22.f;
	firstLineYPos += value * m_text[0].getCharacterSize();
	for (int i = 0; i < TextBufferLength; i++)
		m_text[i].setPosition((float)ChatboxPos.x + 6.f, firstLineYPos - (float)m_text[0].getCharacterSize() * (i + 1));
}

// UIComponent override

void Chatbox::onEvent(const sf::Event& ev, const sf::Vector2f& mousePos)
{
	switch (ev.type)
	{

	case sf::Event::MouseButtonPressed:
	{
		m_active = m_bounds.contains(mousePos);
		break;
	}

	case sf::Event::KeyPressed:
	{
		if (!m_active)
			break;
		if (ev.key.code == sf::Keyboard::Key::Enter)
		{
			// Press enter to print the typed text
			addText(L"Chat: " + m_inputBuffer);
			m_inputBuffer.clear();
			m_inputBufferText.setString(L"Chat: *");
			break;
		}
		break;
	}

	case sf::Event::TextEntered:
	{
		if (!m_active)
			break;
		m_inputBuffer += ev.text.unicode;
		m_inputBufferText.setString(L"Chat: " + m_inputBuffer + L"*");
		break;
	}

	default:
		break;
	}
	return;
}

void Chatbox::update(const sf::Vector2f& mousePos)
{

}

void Chatbox::draw(sf::RenderTarget& target)
{
	// Draw each line of text
	for (int i = 0; i < TextBufferLength; i++)
		target.draw(m_text[i]);
	target.draw(m_inputBufferText);
}
