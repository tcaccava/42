#ifndef ISocketHandler_HPP
# define ISocketHandler_HPP
# include "Dispatcher.hpp"

class ISocketHandler {
	/*
	ISocketHandler interface
	*/

	public:
		virtual int handle(Dispatcher::EventType events) = 0;

		virtual ~ISocketHandler() {}

};

#endif