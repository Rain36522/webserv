/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/19 16:44:40 by dvandenb         ###   ########.fr       */
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

bool	Server::genReponse(Request request, Response &res)
{
	std::vector<Route>::iterator i;
	Route bestRoute;
	int	maxMatch = -1;

	request.setBody(_bodyLimit);
	res._servName = _name;
	res._errorCode = 404;
	for (i = _routes.begin(); i != _routes.end(); i++)
	{
		int curMatch = (*i).match(request);
		if (curMatch > maxMatch)
		{
			bestRoute = *i;
			maxMatch = curMatch;
		}
	}
	if (maxMatch != -1)
		bestRoute.execute(request, res);
	if (res._errorCode >= 400)
		handleError(res._errorCode, res._htmlContent);
	return true;
}

void	Server::handleError(int code, std::string &html)
{
	if (_errPages.find(code) != _errPages.end())
		html = getErrorHtml(_errPages[code], code);
	else
		html = getErrorHtml(_defaultError, code);
	if (html == "")
		html = "Error 500 : No file found for error: " + std::to_string(code);
}

Route *Server::matchRoute(Request & req)
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
