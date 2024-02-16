/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 13:20:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/16 11:19:53 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include "data.hpp"

class Request
{
	public:
		Request(const int client_Fd, int &error);

		// Add << operator;
	protected:
		m_type 						_method;
		m_dataType					_type;
		std::string					_hostPort;
		std::string					_path; // route with filename.
		std::string					_fileName;
		std::vector<std::string>	_Query;
		std::string					_body;
		std::string					_extension;
		std::string					_bodyContent; //Data in post methode.
		std::string					_usr;
		std::string					_pwd;
		std::string					_bodyFileName;
		std::string					_CookieId;
		int							_totaLength; // total request length of download file
		int							_length; // actual request length
		int							_clientFd;
		bool						_login;
		
		int				setServeur(void); // Read heder content, set Query // WEBSERVER
		int				setBody(int body); // Read body content (file / login) //SERVER
		int				setUrlFile(std::string route_path); //set html or CGI file to return
	
	private:
		std::string	_boundary;
		int 	receiveHTTPRequest(const int client_fd, const int length);
		bool	getMethode(void);
		bool	getHostPort(void);
		bool	getPath(void);
		bool	getExtension(void);
		bool	getType(void);
		int		getTotalLength(int error);
		void	getQuery(void);
		bool	getBoundary(int &error);
		int		getUploadName(int error);
		int		getLogin(int error);
		int		getDelete(int error);
		int		getBodyContent(int error);

};
