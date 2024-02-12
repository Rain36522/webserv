/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:51:05 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/12 18:19:20 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

char **env;

int main(int ac, char*av[], char **env_input)
{
	if (ac != 2)
	{
		perror("Invalid number of parameters");
		return (1);
	}
	env = env_input;
	WebServer a((std::string(av[1])));
	a.run();
	return (0);
}
