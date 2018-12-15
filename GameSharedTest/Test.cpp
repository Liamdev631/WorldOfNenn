#include "Test.h"

Test::Test(const std::string& name)
	: name(name), failed(false)
{

}

Test::~Test()
{

}

void Test::reportFailure(const std::string& failMessage) 
{
	printf("%s\n", failMessage.c_str());
	failed = true;
}

void Test::run() 
{
	printf("\n----- Test '%s'-----\n\n", name.c_str());
	runAllSubtests();
}
