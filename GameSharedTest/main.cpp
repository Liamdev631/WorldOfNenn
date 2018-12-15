#include <vector>
#include <algorithm>
#include <stdio.h>
#include "Test.h"
#include "Test_Packets.h"

std::vector<Test*> tests;
void addTest(Test* test) {
	tests.push_back(test);
}

void runTests() {
	for (Test* test : tests)
	{
		printf("Test (%s) %s.\n", test->name.c_str(), test->failed ? "failed" : "passed");
	}
}

int main()
{
	// Select the tests to use
	addTest(new Test_Packets());

	if (tests.size() == 0)
	{
		printf("No tests have been added. Quitting.\n");
		return 0;
	}

	// Run all the tests
	runTests();

	// Print overall test results
	printf("\n");
	int numTestsPassed = std::count_if(tests.begin(), tests.end(), [](const Test* test) { return !test->failed; });
	if (numTestsPassed > 0)
		printf("Tests Passed: %u\n", numTestsPassed);
	if (tests.size() - numTestsPassed > 0)
		printf("Tests Failed : %u\n", tests.size() - numTestsPassed);

	// release the tests
	for (Test* test : tests)
		delete test;

	// Wait 
	std::getchar();
}