/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:23:14 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/05 15:45:09 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"

class Server;
class Route;

enum state
{
	OUT,
	SERVER,
	ROUTE
};

class ParseConfig
{
	private:
		static void setState(std::string key, state &state, Server &curS, Route &curR, std::vector<Server> &servers);
		static void setServer(std::string key, std::string value, Server &server);
		static void validServer(Server server);
		static void setRoute(std::string key, std::string value, std::string line, Route &route);

	public:
		static std::vector<Server> generate_servers(std::string file);
};
