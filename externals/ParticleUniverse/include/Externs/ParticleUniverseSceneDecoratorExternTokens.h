/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2010 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_SCENE_DECORATOR_EXTERN_TOKENS_H__
#define __PU_SCENE_DECORATOR_EXTERN_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExternTokens.h"
#include "ParticleUniverseAttachableTokens.h"

namespace ParticleUniverse
{
	/** The SceneDecoratorExternTranslator parses 'SceneDecoratorExtern' tokens
	*/
	class _ParticleUniverseExport SceneDecoratorExternTranslator : public ScriptTranslator
	{
		public:
			SceneDecoratorExternTranslator(void){};
			~SceneDecoratorExternTranslator(void){};
			virtual bool translateChildProperty(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
			virtual bool translateChildObject(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
	};
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	/** 
    */
	class _ParticleUniverseExport SceneDecoratorExternWriter : public AttachableWriter, ExternWriter
	{
		public:

			SceneDecoratorExternWriter(void) {};
			virtual ~SceneDecoratorExternWriter(void) {};

			/** @see
				ScriptWriter::write
			*/
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element);
	};

}
#endif
