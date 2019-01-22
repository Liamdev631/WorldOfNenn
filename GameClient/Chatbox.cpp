#include "Chatbox.h"
#include "ResourceLoader.h"
#include "C_Client.h"
#include "ClientPackets.h"

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
	m_inputBufferText.setString(L"?: *");

	setScrollValue(0.0f);
}

Chatbox::~Chatbox()
{

}

void Chatbox::addText(const std::wstring& str)
{
	// Shift the text history and insert the new text at the bottom
	for (int i = TextBufferLength - 1; i > 0; i--)
		m_text[i].setString(m_text[i - 1].getString());
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
			// Send the text to the server
			C_Client::get().getPacket().write(CP_ChatText(m_inputBuffer.toWideString()));

			// Reset
			m_inputBuffer.clear();
			auto thisPlayer = C_WorldManager::get().getThisEntity();
			if (thisPlayer != nullptr)
				m_inputBufferText.setString(thisPlayer->username + L": *");
			break;
		}
		break;
	}

	case sf::Event::TextEntered:
	{
		if (!m_active)
			break;
		m_inputBuffer += ev.text.unicode;
		auto thisPlayer = C_WorldManager::get().getThisEntity();
		if (thisPlayer != nullptr)
			m_inputBufferText.setString(thisPlayer->username + L": " + m_inputBuffer + L"*");
		break;
	}

	default:
		break;
	}
	return;
}

void Chatbox::update(const GameTime& time, const sf::Vector2f& mousePos)
{
	auto thisPlayer = C_WorldManager::get().getThisEntity();
	if (thisPlayer != nullptr)
		m_inputBufferText.setString(thisPlayer->username + L": " + m_inputBuffer + L"*");
}

void Chatbox::draw(sf::RenderTarget& target) const
{
	// Draw each line of text
	for (int i = 0; i < TextBufferLength; i++)
		target.draw(m_text[i]);
	target.draw(m_inputBufferText);
}
