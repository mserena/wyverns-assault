/*
TecnoFreak Animation System
http://sourceforge.net/projects/tecnofreakanima/

Copyright (c) 2008 Pau Novau Lebrato

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#ifndef __TECNOFREAK__TRIGGER__H__
#define __TECNOFREAK__TRIGGER__H__

#include "ITrigger.h"
#include "ITriggerSubscriber.h"

#include <string>
#include <vector>

namespace tecnofreak
{

class TecnoFreakDllPrivate Trigger
	: public ITrigger
{
public:
	Trigger( const std::string& name, const float relativeTime, const float minWeightThreshold );
	virtual ~Trigger();

	virtual const std::string& getName() const;

	virtual float getRelativeTime() const;
	virtual float getMinWeightThreshold() const;

	void notify( const float currentWeight ) const;

	void addSubscriber( ITriggerSubscriber* subscriber );
	void removeSubscriber( ITriggerSubscriber* subscriber );
	void clearSubscribers();
private:
	std::string m_name;
	float m_relativeTime;
	float m_minWeightThreshold;

	std::vector< ITriggerSubscriber* > m_subscribers;
};

}

#endif