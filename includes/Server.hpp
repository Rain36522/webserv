/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:30 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/30 09:47:39 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"
#include "structs.hpp"

class Server
{
	private:
		std::string			_name;
		int					_host;
		int					_port;
		int					_bodyLimit;
		// error code => error page
		// we could perhaps use something like -1 or 0 for default page
		std::map<int, std::string>	_ErrPages;

		std::vector<Route>	_routes;
	public:
		Server(ServConf *Conf);
		~Server(void);
		Route *matchRoute(const HttpRequest & req)
		{
			for (size_t i = 0; i < _routes.size(); i++)
				if (_routes[i].match(req))
					return &_routes[i];
			return 0;
		};
		int get_port()
		{
			return _port;
		}
};
