/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:18:12 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/01 17:41:52 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParseConfig.hpp"

std::vector<Server> ParseConfig::generate_servers(std::string file)
{
	(void)file;
	std::vector<Server> servers;
	ServConf temp_t = (ServConf)
	{
		.host = "127.0.0.1",
		.port = 80,
		.DefPage = "./Html_code/main2.html"
	};
	Server temp(temp_t);
	
	temp.addRoute(Route("", _GET, "./Html_code/main2.html"));
	servers.push_back(temp);
	ServConf temp_t2 = (ServConf)
	{
		.host = "127.0.0.1",
		.port = 8080,
		.DefPage = "./Html_code/file.html"
	};
	Server temp2(temp_t2);
	temp2.addRoute(Route("", _GET, "./Html_code/file.html"));
	servers.push_back(temp2);
	return servers;
}