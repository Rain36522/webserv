/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:50:01 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/02 17:48:56 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "data.hpp"

class Route
{
	//set_methode();
	private:
		std::string					_path; // part in the url after the port/hostname if I understand correctly
		std::string					_dir; // path on server
		std::string					_default; // default page (index.html)
		std::set<m_type>			_methods; // a set elements are unique and also has the contains() method
		bool						_listDir; // enable or disable directory listing
		std::string					_redirPath; // Redirect from this route to another?
		bool						_allowUpload;
		std::string					_uploadPath;
		std::vector<std::string>	_CGIs;
		
	public:
		Route(std::string const path, m_type type, std::string default_page);
		bool match(HttpRequest req);
		int execute(HttpRequest req);
		int runCGI(HttpRequest request, std::string &html);
		int getMethod(HttpRequest request, std::string &html);
		int postMethod(HttpRequest request, std::string &html);
		int delMethod(HttpRequest request);

};

// Helper class for creating Http Reponses?
