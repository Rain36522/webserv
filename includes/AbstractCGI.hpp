/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AbstractCGI.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:01:17 by dvandenb          #+#    #+#             */
/*   Updated: 2024/01/26 16:07:53 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "structs.hpp"

class AbstractCGI
{
	private:
		std::string const &_exec_path;
		
	public:
		AbstractCGI(std::string const &path): _exec_path(path){};
		virtual HttpResponse execute(HttpRequest req) = 0;
};
