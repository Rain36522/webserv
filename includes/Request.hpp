/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 13:20:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/15 15:52:37 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include "data.hpp"

class Request
{
	public:
		Request(int client_Fd);

		// Add << operator;
	protected:
		m_type 						method;
		m_dataType					type;
		std::string					hostPort;
		std::string					path; // route with filename.
		std::string					fileName;
		std::vector<std::string>	Query;
		std::string					body; // look for remove TODO
		std::string					extension;
		std::string					bodyContent; //Data in post methode.
		std::string					usr;
		std::string					pwd;
		std::string					bodyFileName;
		std::string					CookieId;
		int							totaLength; // total request length of download file
		int							length; // actual request length
		int							clientFd;
		bool						Login;
		
		int				setServeur(void); // Read heder content, set Query // WEBSERVER
		int				setBody(int body); // Read body content (file / login) //SERVER
		int				setUrlFile(std::string route_path); //set html or CGI file to return
};		