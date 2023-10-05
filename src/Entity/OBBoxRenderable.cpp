#include "Ogre.h"

using namespace Ogre;

#include "..\..\include\Entity\OBBoxRenderable.h"

using namespace WyvernsAssault;

OBBoxRenderable::OBBoxRenderable()
{
	initialize("OBBoxManualMaterial");
}

OBBoxRenderable::OBBoxRenderable(Ogre::String material)
{
	initialize(material);
}
	
OBBoxRenderable::~OBBoxRenderable()
{
	delete mRenderOp.vertexData;
}

void OBBoxRenderable::initialize(Ogre::String material)
{
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.indexData = 0;
	mRenderOp.vertexData->vertexCount = 24;
	mRenderOp.vertexData->vertexStart = 0;
	mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;
	mRenderOp.useIndexes = false;
	Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;
	decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		decl->getVertexSize(0), mRenderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	// Bind buffer
	bind->setBinding(0, vbuf);
	// setup material
	if(Ogre::MaterialManager::getSingleton().getByName(material).isNull())
	{
		Ogre::MaterialPtr matptrOBBoxManualMaterial = Ogre::MaterialManager::getSingleton().create(material, "General");
		matptrOBBoxManualMaterial->setReceiveShadows(false);
		matptrOBBoxManualMaterial->getTechnique(0)->setLightingEnabled(true);

		if(material=="OBBoxManualMaterial_Enemy")
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1, 0, 0);
		}
		else if(material=="OBBoxManualMaterial_Player")
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(0, 1, 0, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(0, 1, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0, 1, 0);
		}
		else if(material=="OBBoxManualMaterial_Item")
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(0, 0, 1, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(0, 0, 1);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0, 0, 1);
		}
		else if(material=="OBBoxManualMaterial_Object")
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(1, 1, 0, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(1, 1, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1, 1, 0);
		}
		else if(material=="OBBoxManualMaterial_Attack")
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(0.9, 0, 0, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(0.9, 0.0, 0.0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0.9, 0, 0);
		}
		else 
		{
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 1, 0);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setAmbient(1, 0, 1);
			matptrOBBoxManualMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1, 0, 1);
		}
	}
	this->setCastShadows(false);
	//this->setQueryFlags(QF_UNKNOWN); // set a query flag to exlude from queries (if necessary).
	this->setMaterial(material);
}
	
void OBBoxRenderable::setupVertices(const Ogre::AxisAlignedBox& aab)
{
	Ogre::Vector3 vmax = aab.getMaximum();
	Ogre::Vector3 vmin = aab.getMinimum();
	Ogre::Real maxdistground = vmax.y;
	Ogre::Real mindistground = vmin.y;
	Ogre::Real maxx = vmax.x;
	Ogre::Real maxy = vmax.y;
	Ogre::Real maxz = vmax.z;
	Ogre::Real minx = vmin.x;
	Ogre::Real miny = vmin.y;
	Ogre::Real minz = vmin.z;
	// fill in the Vertex buffer: 12 lines with 2 endpoints each make up a box
	Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
	float* pPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	// line 0
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	// line 1
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 2
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 3
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 4
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 5
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 6
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 7
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 8
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 9
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 10
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 11
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	vbuf->unlock();
	// setup the bounding box of this SimpleRenderable
	setBoundingBox(aab);
}
	
Ogre::Real OBBoxRenderable::getSquaredViewDepth(const Ogre::Camera* cam)const
{
	Ogre::Vector3 min, max, mid, dist;
	min = mBox.getMinimum();
	max = mBox.getMaximum();
	mid = ((max - min) * 0.5) + min;
	dist = cam->getDerivedPosition() - mid;
	return dist.squaredLength();
}
	
Ogre::Real OBBoxRenderable::getBoundingRadius()const
{
	return 0;
}
	
void OBBoxRenderable::getWorldTransforms (Ogre::Matrix4 *xform)const
{
	SimpleRenderable::getWorldTransforms (xform);
}
