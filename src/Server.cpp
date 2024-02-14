/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/14 10:56:25 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::~Server(void) {}

Server::Server() {
	_port = -1;
	_bodyLimit = -1;
}

Server::Server(ServConf Conf)
{
	_name = Conf.name;
	_host = Conf.host;
	_port = Conf.port;
	_bodyLimit = Conf.bodyLimit;
	
}

bool	Server::makeRequest(HttpRequest request)
{
	int code = 404;
	request.servName = _name;
	std::vector<Route>::iterator i;
	for (i = _routes.begin(); i != _routes.end(); i++)
	{
		if ((*i).match(request))
		{
			code = (*i).execute(request);
			break;
		}
	}
	if (code != 200)
		handleError(code, request.clientFd);
	return true;
}

void	Server::handleError(int code, int fd)
{
	std::string		html;


	if (_errPages.find(code) != _errPages.end())
		html = getErrorHtml(_errPages[code], code);
	else
		html = getErrorHtml(_defaultError, code);
	if (html == "")
	{
		html = "Error 500 : No file found for error :" + std::to_string(code);
		code = 500;
	}
	sendHTMLResponse(fd, html, code, _name);
}

Route *Server::matchRoute(const HttpRequest & req)
{
	for (size_t i = 0; i < _routes.size(); i++)
		if (_routes[i].match(req))
			return &_routes[i];
	return 0;
}

void Server::addRoute(Route r)
{
	_routes.push_back(r);
}
