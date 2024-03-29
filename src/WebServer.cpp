/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.json                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:09:58 by pudry             #+#    #+#             */
/*   Updated: 2024/02/21 10:11:54 by pudry            ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
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
	std::vector<struct kevent> change(servers.size() * 2);
	if (_kfd == -1) {
		perror("Error creating kqueue");
		exit(EXIT_FAILURE);
	}
	for (i = servers.begin(); i != servers.end(); i++)
	{
		std::string host_port = (*i).get_host() + ":" + std::to_string((*i).get_port());
		std::cout << BLUE << "ADDED " << host_port RESETN;
		_servers.insert(std::map<std::string, Server>::value_type(host_port, *i));
		if (ports.insert((*i).get_port()).second)
		{
			int server_fd = getServerSocket(*i);
			_serverFds.push_back(server_fd);
			EV_SET(&change[2 * ports.size() - 2], server_fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
			EV_SET(&change[2 * ports.size() - 1], server_fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, NULL);
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
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, 4)) {
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
	return sockfd;
}

void WebServer::run(void)
{
	struct sockaddr_in client_addr;
	std::vector<int>reads(5000);
	std::vector<int>writes(5000);
	
	if (_servers.empty())
	{
		std::cout << RED << "No servers, stopping" RESETN;
		return ;
	}
	while (true)
	{
		int n_events = kevent(_kfd, nullptr, 0, events, MAX_EVENTS, NULL);
		if (n_events == -1) {
			std::cerr << RED << "Error in kevent" RESETN;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < n_events; ++i) {
			std::cout << "================================================" RESETN;
			
			// DEBUGOUT << GREEN << "INCOMING FD: " << events[i].ident RESETN;
			if (std::find(_serverFds.begin(), _serverFds.end(), events[i].ident) != _serverFds.end()) {
				if (events[i].flags & EV_EOF) {
					std::cout << GREEN << "Closed client" RESETN;

					struct kevent event[2];
					EV_SET(&event[0], events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
					EV_SET(&event[0], events[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
					kevent(_kfd, event, 2, nullptr, 0, nullptr);
					close(events[i].ident);
					continue;
				}
				socklen_t client_len = sizeof(client_addr);
				int client_fd = accept(events[i].ident, (struct sockaddr*)&client_addr, &client_len);
				if (client_fd == -1) {
					std::cerr << RED << "Error accepting connection." RESETN;
					continue ;
				}
				 std::cout << GREEN << "Accepted connection from client" RESETN;
                struct kevent client_event[3];
                EV_SET(&client_event[0], client_fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
				EV_SET(&client_event[1], client_fd, EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, NULL);

                if (kevent(_kfd, client_event, 2, NULL, 0, NULL) == -1) {
                    std::cerr << RED << "Error adding client socket to kqueue" RESETN;
                    close(client_fd);
                }
			}
			else
			{
				// DEBUGOUT << "EVENT FILTER TYPE: " << BLUE << events[i].filter << RESET << "fd:" << GREEN << events[i].ident RESETN;
				int client_fd = events[i].ident;
				
				if (events[i].filter == -1)
					reads[events[i].ident] = 1;
				if (events[i].filter == -2)
					writes[events[i].ident] = 1;
				if (reads[events[i].ident] == 1 && writes[events[i].ident] == 1)
				{
					reads[events[i].ident] = 0;
					writes[events[i].ident] = 0;
				}
				else
					continue;
				Response response(client_fd);
				Request request(client_fd, response._errorCode);
				if (response._errorCode == 500)
				{
					struct kevent event[2];
					EV_SET(&event[0], client_fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
					EV_SET(&event[0], client_fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
					kevent(_kfd, event, 2, nullptr, 0, nullptr);
					std::cerr << ORANGE << "Invalid request, deleted event" RESETN;
					continue;
				}
				response._clientFd = client_fd;
				if (response._errorCode != 500 && _servers.find(request._hostPort) != _servers.end())
				{
					_servers[request._hostPort].genReponse(request, response);
					response.sendResponse();
				}
				else
					std::cout << ORANGE << "Request did not match a server" RESETN;
				// DEBUGOUT << "Closed client" << std::endl;
				close(client_fd);
			}
			
		}
	}
}
