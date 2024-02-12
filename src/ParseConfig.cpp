/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:18:12 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/12 14:02:25 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParseConfig.hpp"



void ParseConfig::validServer(Server server)
{
	if (server._name.empty() || server._host.empty() || server._defaultError.empty()
		|| server._port == -1 || server._bodyLimit == -1)
	{
		std::cout << "Error: Invalid server" << std::endl;
		exit(1);
	}
}

void ParseConfig::setState(std::string key, state &state, Server &curS, Route &curR, std::vector<Server> &servers)
{
	if (key == "server:")
	{
		if (state != OUT)
		{
			validServer(curS);
			servers.push_back(curS);
		}
		if (state == ROUTE)
			curS._routes.push_back(curR);
		state = SERVER;
		curS = Server();
		curR = Route();
	}
	if (key == "route:")
	{
		if (state == ROUTE)
			curS._routes.push_back(curR);
		curR = Route();
		state = ROUTE;
	}
}

void setIfNot(std::string val, std::string &ref, bool allowEmpty)
{
	if (!allowEmpty && val.empty())
	{
		std::cout << "error: value can not be empty" << std::endl;
		exit(1);
	}
	if (!ref.empty())
	{
		std::cout << "error: duplicate key" << std::endl;
		exit(1);
	}
	ref = val;
}

void setIfNot(int val,int &ref)
{
	if (ref != -1)
	{
		std::cout << "error: duplicate key" << std::endl;
		exit(1);
	}
	ref = val;
}

bool isNum(std::string s)
{
    std::string::iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
		++it;
	if (it != s.end() && *it == ':')
		++it;
    return s.size() > 1 && it == s.end();
}

void ParseConfig::setRoute(std::string key, std::string value, std::string line, Route &route)
{
	if (key == "path:")
		setIfNot(value, route._path, true);
	else if (key == "dir:")
		setIfNot(value, route._dir, true);
	else if (key == "default:")
		setIfNot(value, route._default, false);
	else if (key == "redirPath:")
		setIfNot(value, route._redirPath, false);
	else if (key == "uploadPath:")
		setIfNot(value, route._uploadPath, true);
	else if (key == "listDir:")
	{
		if (route._listDir)
		{
			std::cout << "Error: duplicate key" << std::endl;
			exit(1);
		}
		if (value == "true")
			route._listDir = true;
		else if (value == "false")
			route._listDir = false;
	}
	else if (key == "allowUpload:")
	{
		if (route._allowUpload)
		{
			std::cout << "Error: duplicate key" << std::endl;
			exit(1);
		}
		if (value == "true")
			route._allowUpload = true;
		else if (value == "false")
			route._allowUpload = false;
	}
	else if (key == "methods:")
	{
		if (route._methods.size() > 0)
		{
			std::cout << "Error: duplicate method" << std::endl;
		}
		if (line.find("GET") != std::string::npos)
			route._methods.insert(_GET);
		if (line.find("POST") != std::string::npos)
			route._methods.insert(_POST);
		if (line.find("DELETE") != std::string::npos)
			route._methods.insert(_DEL);
	}
	else if (key == "CGIs:")
	{
		std::istringstream iss(line);
		std::string cgi;
		iss >> cgi;
		while (iss >> cgi)
			route._CGIs.push_back(cgi);
	}
	else
	{
		std::cout << "Error: Unknown key " << key << std::endl;
		exit(1);
	}
}

void ParseConfig::setServer(std::string key, std::string value, Server &server)
{
	if (key == "name:")
		setIfNot(value, server._name, false);
	else if (key == "host:")
		setIfNot(value, server._host, false);
	else if (key == "defaultError:")
		setIfNot(value, server._defaultError, false);
	else if (key == "port:" || key == "bodyLimit:")
	{
		if (!isNum(value)){
			std::cout << "Error: expected number for " << key << std::endl;
			exit(1);
		}
		if (key == "port:")
			setIfNot(std::stoi(value), server._port);
		else
			setIfNot(std::stoi(value), server._bodyLimit);
	}
	else if (isNum(key.substr(0, key.length() - 1)))
	{
		if (key.size() > 4){
			std::cout << "Error: Invalid html error code " << key << std::endl;
			exit(1);
		}
		if (value.empty()){
			std::cout << "Error: No html page given for code " << key << std::endl;
			exit(1);
		}
		if (!server._errPages.insert(std::pair<int, std::string>(std::stoi(key), value)).second)
		{
			std::cout << "Error: Duplicate error code" << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << "Error: Unknown key " << key << std::endl;
		exit(1);
	}
}


void ParseConfig::validServers(std::vector<Server> &servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		for (size_t j = i + 1; j < servers.size(); j++)
			if (servers[i]._host == servers[j]._host && servers[i]._port == servers[j]._port)
			{
				std::vector<Route> r = servers[j]._routes;
				servers[i]._routes.insert(servers[i]._routes.end(), r.begin(), r.end());
				servers.erase(servers.begin() + j);
			}
		size_t len = servers[i]._routes.size();
		for (size_t j = 0; j < len; j++)
		{
			for (size_t k = j + 1; k < len; k++)
			{
				if (servers[i]._routes[j]._path == servers[i]._routes[k]._path)
				{
					std::cout << "Error: Duplicate path" << std::endl;
					exit(1);
				}
			}
		}
	}
}

std::vector<Server> ParseConfig::generate_servers(std::string file)
{
	std::ifstream infile(file);
	std::string line;
	if (infile.fail())
	{
		std::cout << "error: Unable to open config file" << std::endl;
		exit(1);
	}

	Server curS;
	Route curR;
	state state = OUT;
	std::vector<Server> servers;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string key, value;
		iss >> key >> value;
		if (key.empty() || key[0] == '#')
			continue;
		if (state == OUT && key != "server:")
		{
			std::cout << "error: key not assigned to server" << std::endl;
			exit(1);
		}
		setState(key, state, curS, curR, servers);
		if (key == "server:" || key == "route:")
			continue;
		if (state == SERVER)
			setServer(key, value, curS);
		if (state == ROUTE)
			setRoute(key, value, line, curR);
	}
	if (state == ROUTE)
	{
		curS.addRoute(curR);
	}
	if (state != OUT)
	{
		validServer(curS);
		servers.push_back(curS);
	}
	validServers(servers);
	return servers;
}


// port: 8080


// rules:
// 

// errors:
// duplicate routes
// non numerical port
// non existant files


// For every line, put into a vector of std::string pairs. Verify that keys are allowed

// Read each server

// read into pairs
// read into server
// read into routes

// validate servers
// validate routes
