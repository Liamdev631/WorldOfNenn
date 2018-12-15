#pragma once
#include "Global.h"
#include <string>
#include <vector>
#include <iostream>
#include <functional>

typedef bool (*TestMemFn)(void);

class Test
{
public:
	const std::string name;
	bool failed;

public:
	Test(const std::string& name);
	~Test();

	// Runs all the tests and returns.
	void run();
	 
protected:
	// Prints the bytes of an object to the console.
	// @object: The object to print.
	template<typename T>
	void printBytes(const T& object) const;

	// Prints a list of byte data to the console.
	// @object: The buffer to print from.
	// @size: The number of bytes to print.
	// @offset: The byte to start reading from. 0 by default.
	void printBytes(const void* object, const size_t size, const size_t offset = 0) const;

	void reportFailure(const std::string& failMessage);

	virtual void runAllSubtests() = 0;
};

