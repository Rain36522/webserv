/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:08:42 by pudry             #+#    #+#             */
/*   Updated: 2024/02/21 10:08:42 by pudry            ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/data.hpp"

char **env;

void	doHeader(void)
{
	std::cout << BLUE << BOLD;
	std::cout << "__          __  ______   ____     _____   ______   _____   __      __" << std::endl;
	std::cout << "\\ \\        / / |  ____| |  _ \\   / ____| |  ____| |  __ \\  \\ \\    / /" << std::endl;
	std::cout << " \\ \\  /\\  / /  | |__    | |_) | | (___   | |__    | |__) |  \\ \\  / / " << std::endl;
	std::cout << "  \\ \\/  \\/ /   |  __|   |  _ <   \\___ \\  |  __|   |  _  /    \\ \\/ /  " << std::endl;
	std::cout << "   \\  /\\  /    | |____  | |_) |  ____) | | |____  | | \\ \\     \\  /   " << std::endl;
	std::cout << "    \\/  \\/     |______| |____/  |_____/  |______| |_|  \\_\\     \\/    " << NOBOLD RESETN;
	std::cout << std::endl << std::endl;
	std::cout << GREEN << "Starting Webserv..." RESETN;
}


int main(int ac, char*av[], char **env_input)
{
	if (ac != 2)
	{
		perror("Invalid number of parameters");
		return (1);
	}
	doHeader();
	env = env_input;
	WebServer MasterServer((std::string(av[1])));
	std::cout << GREEN << BOLD "Server is runing" << NOBOLD RESETN;
	MasterServer.run();
	return (0);
}
