/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:26:28 by pudry             #+#    #+#             */
/*   Updated: 2024/02/14 10:37:02 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/ParseConfig.hpp"

WebServer::~WebServer(void) {
	if (_kfd != -1)
		close(_kfd);
}

WebServer::WebServer(std::string file)
{
	std::vector<Server> servers = ParseConfig::generate_servers(file);
	std::vector<Server>::iterator i;
	std::set<int> ports;
	_kfd = kqueue();
	std::vector<struct kevent> change(servers.size());
	if (_kfd == -1) {
		perror("Error creating kqueue");
		exit(EXIT_FAILURE);
	}
	for (i = servers.begin(); i != servers.end(); i++)
	{
		std::string host_port = (*i).get_host() + ":" + std::to_string((*i).get_port());
		_servers.insert(std::map<std::string, Server>::value_type(host_port, *i));
		if (ports.insert((*i).get_port()).second)
		{
			int server_fd = getServerSocket(*i);
			_serverFds.push_back(server_fd);
			EV_SET(&change[2 * ports.size() - 2], server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			EV_SET(&change[2 * ports.size() - 1], server_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		}
	}
	if (kevent(_kfd, &change[0], 2 * ports.size(), nullptr, 0, nullptr) == -1) {
		perror("Error adding server socket to kqueue");
		exit(EXIT_FAILURE);
	}
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
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		perror("Error setting socket option SO_REUSEADDR");
		exit(EXIT_FAILURE);
	}
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}
	if (listen(sockfd, SOMAXCONN) == -1) {
		perror("Error listening on socket");
		exit(EXIT_FAILURE);
	}
	std::cout << "listening on port: " << s.get_port() << std::endl;
	return sockfd;
}

void WebServer::run(void)
{
	HttpRequest	request;
	struct sockaddr_in client_addr;

	while (true)
	{
		int n_events = kevent(_kfd, nullptr, 0, events, MAX_EVENTS, nullptr);
		if (n_events == -1) {
			perror("Error in kevent");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < n_events; ++i) {
			if (std::find(_serverFds.begin(), _serverFds.end(), events[i].ident) != _serverFds.end()) {
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(events[i].ident, (struct sockaddr*)&client_addr, &client_len);
				if (client_fd == -1) {
					perror("Error accepting connection");
				}
				request = requestToStruct(client_fd);
				if (request.errorCode == 500)
				{
					std::string	html = "Error 500 : Get error while reading request";
					sendHTMLResponse(client_fd, html, 500, "master");
				}
				if (_servers.find(request.hostPort) != _servers.end())
					_servers[request.hostPort].makeRequest(request);
				else
					std::cout << "request to " << request.hostPort << "did not match server" << std::endl;
			}
		}
	}
}
