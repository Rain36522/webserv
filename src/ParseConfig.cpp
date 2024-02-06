/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:18:12 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/05 18:10:55 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ParseConfig.hpp"



void ParseConfig::validServer(Server server)
{
	if (server._name.empty() || server._host.empty()
		|| server._port == -1 || server._bodyLimit == -1)
	{
		std::cout << "Error: Invalid server" << std::endl;
		std::cout << server._name << server._host << server._port << server._bodyLimit << std::endl;
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
			std::cout << "error: duplicate key" << std::endl;
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
			std::cout << "error: duplicate key" << std::endl;
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
			std::cout << "error: duplicate method" << std::endl;
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
	else if (key == "port:" || key == "bodyLimit:")
	{
		if (!isNum(value)){
			std::cout << "error: expected number for " << key << std::endl;
			exit(1);
		}
		if (key == "port:")
			setIfNot(std::stoi(value), server._port);
		else
			setIfNot(std::stoi(value), server._bodyLimit);
	}
	else if (isNum(key))
	{
		if (key.size() > 3){
			std::cout << "Invalid html error code " << key << std::endl;
			exit(1);
		}
		if (value.empty()){
			std::cout << "No html page given for code " << key << std::endl;
			exit(1);
		}
		if (!server._errPages.insert(std::pair<int, std::string>(std::stoi(key), value)).second)
		{
			std::cout << "Duplicate error code" << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << "Error: Unknown key " << key << std::endl;
		exit(1);
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
		std::cout << "Read <" << key << " " << value << std::endl;
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
	ServConf temp_t;
	
	temp_t.host = "127.0.0.1";
	temp_t.port = 8080;
	temp_t.DefPage = "./Html_code/file.html";
	Server temp(temp_t);
	Route r("", _GET, "file.html");
	r._dir = "./Html_code/";
	temp.addRoute(r);
	// temp.addRoute(Route("haha", _GET, "./Html_code/file.html"));
	servers.push_back(temp);
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
