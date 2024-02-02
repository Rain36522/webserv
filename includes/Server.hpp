/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:30 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/02 13:42:05 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"
#include "structs.hpp"

class Server
{
	private:
		std::string			_name;
		std::string			_host;
		int					_port;
		int					_bodyLimit;
		std::string			_defaultPage;
		// error code => error page
		// we could perhaps use something like -1 or 0 for default page
		std::map<int, std::string>	_errPages;

		std::vector<Route>	_routes;
	public:
		Server();
		Server(ServConf Conf);
		~Server(void);
		Route *matchRoute(const HttpRequest & req)
		{
			for (size_t i = 0; i < _routes.size(); i++)
				if (_routes[i].match(req))
					return &_routes[i];
			return 0;
		};
		void addRoute(Route r)
		{
			_routes.push_back(r);
		}
		int get_port()
		{
			return _port;
		}
		std::string get_host()
		{
			return _host;
		}
		bool	makeRequest(HttpRequest request);
		void	handleError(int code, int fd);
};
