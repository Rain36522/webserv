/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 13:45:21 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::~Server(void) {}

Server::Server()
{

}

Server::Server(ServConf Conf)
{
	_name = Conf.name;
	_host = Conf.host;
	_port = Conf.port;
	_bodyLimit = Conf.bodyLimit;
	_defaultPage = Conf.DefPage;
	// _idirectorylListing = Conf->idirectorylListing;
	// _bdirectorylListing = Conf->bdirectoryListing;
	// _ErrPage = Conf->ErrPage;
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
		sendHTMLResponse(fd, getHtmlPage(_errPages[code]));
}

