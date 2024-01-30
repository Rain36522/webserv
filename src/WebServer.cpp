/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:26:28 by pudry             #+#    #+#             */
/*   Updated: 2024/01/30 09:35:35 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

WebServer::~WebServer(void) {}

void	WebServer::AddServer(ServConf Conf)
{
	Server	*Serv = new Server(Conf);
	
}