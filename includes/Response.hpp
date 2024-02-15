/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:08:22 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/15 18:35:50 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "structs.hpp"
# include "Server.hpp"
#include "WebServer.hpp"
#include "Route.hpp"
class WebServer;
class Server;
class Route;


class Response
{
	public:
		Response(void);
		void sendResponse();
		friend Server;
		friend WebServer;
		friend Route;
	protected:
		std::string _servName;
		int			_errorCode;
		std::string _htmlContent;
		int			_clientFd;
		std::string _redirLocation;
		std::string	getCodeText();
		void sendHTML();
		void sendRedirect();
};
