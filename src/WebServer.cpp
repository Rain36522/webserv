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
#include "../includes/ParseConfig.hpp"

WebServer::~WebServer(void) {}

WebServer::WebServer(std::string file)
{
	std::vector<Server> servers = ParseConfig::generate_servers(file);
	std::vector<Server>::iterator i;

	_kfd = kqueue();
	if (_kfd == -1) {
		perror("Error creating kqueue");
		exit(EXIT_FAILURE);
	}
	for (i = servers.begin(); i != servers.end(); i++)
	{
		int server_fd = getServerSocket(*i);
		struct kevent change;
		EV_SET(&change, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(_kfd, &change, 1, nullptr, 0, nullptr) == -1) {
			perror("Error adding server socket to kqueue");
			exit(EXIT_FAILURE);
		}
		_servers.insert({server_fd, *i});
	}
}

void	WebServer::AddServer(ServConf Conf)
{
	Server	*Serv = new Server(&Conf);
}

int WebServer::getServerSocket(Server s) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(s.get_port());

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, SOMAXCONN) == -1) {
		perror("Error listening on socket");
		exit(EXIT_FAILURE);
	}
	return sockfd;
}

void WebServer::run(void)
{
	while (true)
	{
		int n_events = kevent(_kfd, nullptr, 0, events, MAX_EVENTS, nullptr);
		if (n_events == -1) {
			perror("Error in kevent");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < n_events; ++i) {
			if (_servers.find(events[i].ident) != _servers.end()) {
				
			}
	}
}
