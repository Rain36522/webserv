/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/16 13:51:24 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "data.hpp"
#include "ParseConfig.hpp"
#include "Response.hpp"
#include "Request.hpp"

class ParseConfig;
class Response;

class Route
{
	//set_methode();
	private:
		std::string					_path; // part in the url after the port/hostname if I understand correctly
		// required, can be empty
		std::string					_dir; // path on server
		// required, can be empty
		std::string					_default; // default page (index.html)
		std::set<m_type>			_methods; // a set elements are unique and also has the contains() method
		bool						_listDir; // enable or disable directory listing
		std::string					_redirPath; // Redirect from this route to another?
		bool						_allowUpload;
		std::string					_uploadPath;
		std::vector<std::string>	_CGIs;
		std::string					_server_name; // Contain server name for send in request.
		
	public:
		Route();

		/**
		 * @brief Returns the size of the matching path to the request
		 * 
		 * @param req The http request
		 * @return size_t: the size of the path if matching, otherwise -1
		 */
		int match(Request req);

		void execute(Request req, Response &response);
		void runCGI(Request request, Response &response);
		int getMethod(Request request, std::string &html);
		int postMethod(Request request, std::string &html);
		void delMethod(Request request, Response &response);
		void	uploadClientFile(Request request, Response &response);
		int	addListBox(std::string &html);
		int	doListDir(std::string &html) const;
		void setHtml(std::string file, std::string dir, Response &response);
		friend class ParseConfig;
};

// Helper class for creating Http Reponses?
