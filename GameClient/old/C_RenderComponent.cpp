#include "C_RenderComponent.h"
#include "C_Client.h"

using namespace Render;

C_RenderComponent::C_RenderComponent()
	: enabled(true)
{
	//g_client->getGUIManager().addRenderComponent(this);
}

C_RenderComponent::~C_RenderComponent()
{
	//g_client->getGUIManager().removeRenderComponent(this);
}

static const std::string testString = "@Test String@";
const std::string& C_RenderComponent::getDescription() const
{
	return testString;
}
