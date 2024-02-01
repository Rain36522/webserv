/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/01 17:34:17 by dvandenb         ###   ########.fr       */
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
	for (std::vector<Route>::iterator i = _routes.begin(); i != _routes.end(); i++)
	{
		if ((*i).match(request))
		{
			(*i).execute(request);
			return true;
		}
	}
	return true;
}

