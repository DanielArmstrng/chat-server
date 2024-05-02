#ifndef UTIL_H
#define UTIL_H

#include "util.h"
#include <iostream>

void net_run(std::function<::Socket::Status(void)> f, std::string error_message)
{
	sf::Socket::Status status = f();
	if (status != sf::Socket::Done)
	{
		std::cerr << "Failed to " << error_message << ": " << status << std::endl;
		perror(error_message.cstr());
		exit(0);
	}
}

#endif