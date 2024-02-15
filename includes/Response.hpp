#ifndef __RESPONSE__HPP
# define __RESPONSE__HPP

# include <iostream>

class Response
{
	public:
		Response(void);
		void sendResponse();
	protected:
		std::string servName;
		int			errorCode;
		std::string htmlContent;
		int			clientFd;
	
		void sendHTMLResponse();
		void sendRedirectResponse();
};

#endif
