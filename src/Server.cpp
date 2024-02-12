/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/12 14:01:05 by dvandenb         ###   ########.fr       */
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
	if (_errPages.find(code) != _errPages.end())
		sendHTMLResponse(fd, getErrorHtml(_errPages[code], code));
	else
		sendHTMLResponse(fd,  getErrorHtml(_defaultError, code));;
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
