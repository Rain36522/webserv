/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:30 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/05 15:05:28 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"
#include "structs.hpp"
#include "ParseConfig.hpp"

class ParseConfig;
class Route;

class Server
{
	private:
		std::string			_name; // required, nonempty
		std::string			_host; // required, nonempty
		int					_port; // required, nonempty
		int					_bodyLimit; // required, nonempty
		// error code => error page
		// we could perhaps use something like -1 or 0 for default page
		std::map<int, std::string>	_errPages; // just default error required
		std::vector<Route>	_routes;
	public:
		Server();
		Server(ServConf Conf);
		~Server(void);
		Route *matchRoute(const HttpRequest & req);
		void addRoute(Route r);
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
		friend class ParseConfig;
};
