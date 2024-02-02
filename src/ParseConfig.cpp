/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:18:12 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/02 17:46:48 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParseConfig.hpp"

std::vector<Server> ParseConfig::generate_servers(std::string file)
{
	(void)file;
	std::vector<Server> servers;
	ServConf temp_t;
	
	temp_t.host = "127.0.0.1";
	temp_t.port = 8080;
	temp_t.DefPage = "./Html_code/file.html";
	
	Server temp(temp_t);
	Route r("", _GET, "./Html_code/file.html");
	temp.addRoute(r);
	// temp.addRoute(Route("haha", _GET, "./Html_code/file.html"));
	servers.push_back(temp);
	return servers;
}
