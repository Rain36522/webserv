/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/15 18:57:29 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "data.hpp"
#include "ParseConfig.hpp"

class ParseConfig;

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
		int match(HttpRequest req);

		int execute(HttpRequest req, Response &response);
		void runCGI(HttpRequest request, Response &response);
		int getMethod(HttpRequest request, std::string &html);
		int postMethod(HttpRequest request, std::string &html);
		void delMethod(HttpRequest request, Response &response);
		void	uploadClientFile(HttpRequest request, Response &response);
		int	addListBox(std::string &html);
		int	doListDir(std::string &html) const;
		void setHtml(std::string file, std::string dir, Response response);
		friend class ParseConfig;
};

// Helper class for creating Http Reponses?
