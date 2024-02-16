/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:05:55 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/16 13:33:49 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#define MAX_EVENTS 100

class Server;

class WebServer
{
	private:
		// map from hostname to Server
		std::map<std::string, Server>	_servers;
		std::vector<int> 				_serverFds;
		int getServerSocket(Server s);
		int						_kfd;
		struct kevent events[MAX_EVENTS];

	public:
		WebServer(std::string file);
		void run(void);
		std::vector<Server>	getServer(int i) const;
		~WebServer();
};
