/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:59:30 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/20 12:36:17 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"
#include "Response.hpp"
#include "ParseConfig.hpp"
#include "Request.hpp"

class ParseConfig;
class Route;

class Server
{
	private:
		std::string			_name; // required, nonempty
		std::string			_host; // required, nonempty
		int					_port; // required, nonempty
		int					_bodyLimit; // required, nonempty
		std::map<int, std::string>	_errPages;
		std::string					_defaultError;
		std::vector<Route>	_routes;
	public:
		Server();
		Server(ServConf Conf);
		~Server(void);
		Route *matchRoute(Request & req);
		void addRoute(Route r);
		int get_port()
		{
			return _port;
		}
		std::string get_host()
		{
			return _host;
		}
		bool	genReponse(Request request, Response &res);
		void	handleError(int code, std::string &html);
		friend class ParseConfig;
};
