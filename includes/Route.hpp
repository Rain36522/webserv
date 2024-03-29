/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/20 14:58:08 by dvandenb         ###   ########.fr       */
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
		std::string					_path;
		std::string					_dir;
		std::string					_default; // default page (index.html)
		std::set<m_type>			_methods; // a set elements are unique and also has the contains() method
		bool						_listDir; // enable or disable directory listing
		std::string					_redirPath; // Redirect from this route to another?
		bool						_allowUpload;
		std::string					_uploadPath;
		std::vector<std::string>	_CGIs;
		std::string					_server_name; // Contain server name for send in request.
		std::set<std::pair<std::string, std::string> > _logins;
		
	public:
		Route();

		/**
		 * @brief Returns the size of the matching path to the request
		 * 
		 * @param req The http request
		 * @return size_t: the size of the path if matching, otherwise -1
		 */
		int match(Request req);

		void 	execute(Request req, Response &response);
		void	login(Request request, Response &response);

		void 	runCGI(Request request, Response &response);
		void 	delMethod(Request request, Response &response);
		int		addListBox(std::string &html);
		int		doListDir(std::string &html, std::string pathDir) const;
		void	setHtml(Request file, std::string dir, Response &response);
		friend class ParseConfig;
};
