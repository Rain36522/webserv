/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:51:21 by pudry             #+#    #+#             */
/*   Updated: 2024/02/21 10:51:21 by pudry            ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::~Server(void) {}

Server::Server() {
	_port = -1;
	_bodyLimit = -1;
}

bool	Server::genReponse(Request request, Response &res)
{
	std::vector<Route>::iterator i;
	Route bestRoute;
	int	maxMatch = -1;

	res._errorCode = request.setBody(_bodyLimit);
	if (res._errorCode > 400)
	{
		if (res._errorCode == 413)
			std::cerr << RED << "Body size too large." << res._errorCode RESETN;
		else
			std::cerr << RED << "Missing data on request." << res._errorCode RESETN;
		handleError(res._errorCode, res._htmlContent);
		return false;
	}
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
