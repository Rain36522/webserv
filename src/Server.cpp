/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:36:10 by pudry             #+#    #+#             */
/*   Updated: 2024/01/30 09:47:12 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::~Server(void) {}

Server::Server(ServConf *Conf)
{
	_name = Conf->name;
	_host = Conf->host;
	_port = Conf->port;
	_bodyLimit = Conf->bodyLimit;
	// _idirectorylListing = Conf->idirectorylListing;
	// _bdirectorylListing = Conf->bdirectoryListing;
	// _ErrPage = Conf->ErrPage;
}