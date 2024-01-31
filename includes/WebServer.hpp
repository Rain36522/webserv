/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:05:55 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/30 09:31:50 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#define MAX_EVENTS 100

class WebServer
{
	private:
		// map from server fd to Server
		std::map<int, Server>	_servers;
		int getServerSocket(Server s);
		int						_kfd;
		struct kevent events[MAX_EVENTS];

	public:
		WebServer(std::string file);
		void run(void);
		std::vector<Server>	getServer(int i) const;
				void	AddServer(ServConf Conf);
		~WebServer();
};
