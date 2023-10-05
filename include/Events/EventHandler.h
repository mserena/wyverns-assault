/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __EVENT_HANDLER_H_
#define __EVENT_HANDLER_H_

#include <boost/shared_ptr.hpp>

#include "Event.h"

namespace WyvernsAssault
{
	/// Abstract class to encapsulate an event handler's functionality
	class BaseEventHandler
	{
	public:

		virtual ~BaseEventHandler(){}
		/// Handle the given event by performing double dispatch
		/// @param evt event to handle
		void handleEvent(const EventPtr evt)
		{
			invoke(evt);
		}
		/// Equality operator: It'll return true if
		/// their types are the same and they're deemed equal
		/// @param other instance to compare
		/// @return true if 'other' has the same type as this, 
		///				 and their values are equal
		bool operator== (const BaseEventHandler& other) const
		{
			if (typeid(*this)!=typeid(other)) return false;
			return equal(other);
		}
		/// Return true if other is the same object as this
		/// @param other instance to compare
		/// @return true if 'other' is the same object as this
		virtual bool equal(const BaseEventHandler& other) const=0;
	protected:
		/// Process the event
		/// @param evt event to handle
		virtual void invoke(const EventPtr evt)=0;
	};

	typedef boost::shared_ptr<BaseEventHandler> EventHandlerPtr;

	/// Compares two event handler shared pointers
	class EventHandlerComparisonFunctor
	{
	private:
		/// instance against which other handlers will be compared
		EventHandlerPtr mInstance;
	public:
		/// Constructor
		EventHandlerComparisonFunctor(EventHandlerPtr instance):mInstance(instance){}
		/// Functor operator: it'll retrieve the raw pointers and then
		/// will test their contents for equality
		/// @param other handler to compare
		/// @return true if this instance is equal as 'other'
		bool operator()(EventHandlerPtr& other) const
		{
			return *(mInstance.get())==*(other.get());
		}
	};
	/// Generic class to implement event handlers as a callback, that'll be 
	/// a member function
	/// T is the class that will handle the events, and eventT is the class of the
	/// event type that will be handled
	template <class T, class TEvent>
	class EventHandler: public BaseEventHandler
	{
	protected:
		/// Invokes the callback function for the event pointer passed as a parameter
		/// @param evt	event to process
		void invoke(const EventPtr evt)
		{
			(mInstance.get()->*mCallback)(boost::dynamic_pointer_cast<TEvent>(evt));
		}
	public:
		//DO NOT TAKE THESE ALIASES AWAY FROM HERE, AS THEY HAVE CLASS SCOPE
		typedef void (T::*TMemberFunction)(boost::shared_ptr<TEvent>);
		typedef boost::shared_ptr<T> TPtr;
		typedef boost::shared_ptr<TEvent> TEventPtr;

		/// Construct a handler from an instance to an object of class T, and a member function
		/// @param instance instance of the object that'll respond to the event
		/// @param memFn	callback function that will process the event
		EventHandler(TPtr instance, TMemberFunction memFn) : mInstance(instance), mCallback(memFn) {}

		/// Return the instance
		/// @return the instance
		TPtr getInstance() const {return mInstance;}
		
		/// Return the callback function
		/// @return the callback function
		TMemberFunction getCallback() const{return mCallback;}

		/// Test an instance of a baseEventHandler for equality:
		/// To do that, it has to be downcast to an instance of the generic class,
		/// and then it gets compared.
		/// @param other instance to compare
		/// @return true	if 'other' is considered the same handler as this
		///					(i.e, the instance and callback method are the same)
		bool equal(const BaseEventHandler& other) const
		{
			EventHandler<T,TEvent>& derivedOther = static_cast<EventHandler<T,TEvent>&>(const_cast<BaseEventHandler&>(other));
			return derivedOther.getInstance() == this->getInstance() && derivedOther.getCallback()==this->getCallback();
		}

	private:
		/// Instance to invoke the callback function
		TPtr mInstance;
		/// Callback function
		TMemberFunction mCallback;
	};
}

#endif // __EVENT_HANDLER_H_