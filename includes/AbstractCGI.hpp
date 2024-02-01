/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AbstractCGI.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 16:01:17 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/01 15:17:25 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "data.hpp"

class AbstractCGI
{
	private:
		std::string const &_exec_path;
		
	public:
		AbstractCGI(std::string const &path): _exec_path(path){};
		virtual HttpResponse execute(HttpRequest req) = 0;
};
