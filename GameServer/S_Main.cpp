#pragma once
#include <enetpp\global_state.h>
#include <ctime>
#include <cstdlib>
#include "S_Server.h"
#include "Loader.h"

using namespace std;

S_Server* g_server;

class Main
{
public:
	Main()
	{
		srand((unsigned int)time(0));
		Loader::get();
		g_server = new S_Server();
	}
	~Main() = default;

	void start()
	{
		enetpp::global_state::get().initialize();
		g_server->start();
	}

	void update()
	{
		g_server->update();
	}

	void forceStop()
	{
		g_server->stop();
		enetpp::global_state::get().deinitialize();
	}
};

int main()
{
	Main m;
	m.start();
	u8 tickTimer = 0;
	while (true)
	{

		auto start = std::chrono::high_resolution_clock::now();

		m.update();

		std::this_thread::sleep_until(start + std::chrono::milliseconds(1000 / S_TICKS_PER_SECOND));

	}
	//m.forceStop(); Not needed. The deconstructor handles everything
	return 0;
}
