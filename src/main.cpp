/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:51:05 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/12 17:00:47 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

char **env;

int main(int ac, char*av[])
{
	if (ac != 2)
	{
		perror("Invalid number of parameters");
		return (1);
	}
	WebServer a((std::string(av[1])));
	a.run();
	env = av;
	return (0);
}
