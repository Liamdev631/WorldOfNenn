#pragma once
#include <enetpp\global_state.h>
#include "C_Client.h"

class Main
{
public:
	Main()
	{

	}

	~Main()
	{
		forceStop();
	}

	void start()
	{
		enetpp::global_state::get().initialize();
		C_Client::get().start();
	}

	bool update()
	{
		return C_Client::get().update();
	}

	void forceStop()
	{
		C_Client::get().stop();
		enetpp::global_state::get().deinitialize();
	}
};

int main()
{
	srand((unsigned int)time(NULL));
	Sleep(100);

	Main m;
	m.start();
	while (m.update()) {  }
	m.forceStop();

	return 0;
}
