/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:51:05 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/05 13:11:07 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

int main(int ac, char*av[])
{
	if (ac != 2)
	{
		perror("Invalid number of parameters");
		return (1);
	}
	WebServer a((std::string(av[1])));
	a.run();
	return (0);
}
