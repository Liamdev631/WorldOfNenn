#include "Global.h"

enum MouseButton : u8
{
	MB_Left = 0,
	MB_Right = 1,
};

struct MouseState
{
public:
	bool m_buttonStates[2];
	bool m_buttonPressed[2];
	bool m_buttonReleased[2];
	vec2<u16> m_position;

	MouseState()
	{
		m_buttonStates[0] = false;
		m_buttonStates[1] = false;
		m_buttonPressed[0] = false;
		m_buttonPressed[1] = false;
		m_buttonReleased[0] = false;
		m_buttonReleased[1] = false;
	}

	bool isButtonDown(const MouseButton button) const
	{
		return m_buttonStates[(u8)button];
	}
	bool isButtonUp(const MouseButton button) const
	{
		return !m_buttonStates[(u8)button];
	}
	bool isButtonPressed(const MouseButton button) const
	{
		return m_buttonPressed[(u8)button];
	}
	bool isButtonReleased(const MouseButton button) const
	{
		return m_buttonReleased[(u8)button];
	}
	vec2<u16> getm_mousePos() const
	{
		return m_position;
	}
};