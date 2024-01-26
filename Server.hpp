/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:30 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/26 16:07:57 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"

class Server
{
	private:
		std::string _host;
		int _port;
		std::vector<Route> _routes;
	public:
		Route *matchRoute(const HttpRequest & req)
		{
			for (size_t i = 0; i < _routes.size(); i++)
				if (_routes[i].match(req))
					return &_routes[i];
			return 0;
		};
};
