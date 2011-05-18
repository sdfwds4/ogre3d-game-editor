

#include "stdafx.h"
#include "LevelSerializer.h"
//#include "xml/WXXMLHandler.h"
//#include "xml/WXXMLAttributes.h"


/*!
\brief
	Handler class used to parse the Scene XML files using SAX2
*/
class SceneXMLHandler : public WX::XMLHandler
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/

	/** 
		Constructor for SceneXMLHandler objects
	*/
	SceneXMLHandler();

	/** Destructor for SceneXMLHandler objects */
	~SceneXMLHandler();

    /** Overridden from XMLHandler */
	virtual void characters(const String& chars);

    /** Overridden from XMLHandler */
	virtual void startElement(const String& element, const WX::XMLAttributes& attributes);

    /** Overridden from XMLHandler */
    virtual void endElement(const String& element);
};


//////////////////////////////////////////////////////////////////////////

SceneXMLHandler::SceneXMLHandler()
{
}

SceneXMLHandler::~SceneXMLHandler()
{
}

//////////////////////////////////////////////////////////////////////////
void SceneXMLHandler::characters(const String& chars)
{
	//if (mString)
	//{
	//	mString->append(chars);
	//}
}


//////////////////////////////////////////////////////////////////////////
void SceneXMLHandler::startElement(const String& element, const WX::XMLAttributes& attributes)
{
	//if (element == "Property")
	//{
	//	assert(mObject);

	//	const String* name = &attributes.getValue("name");
	//	const String& value = attributes.getValue("value");
	//	if (mPropertyNameMap)
	//	{
	//		NameMap::const_iterator it = mPropertyNameMap->find(*name);
	//		if (it != mPropertyNameMap->end())
	//			name = &it->second;
	//	}

	//	mObject->setPropertyAsString(*name, value);
	//}
	//else if (element == "Object")
	//{
	//	assert(!mObject);

	//	const String& type = attributes.getValue("type");

	//	mObject = ObjectFactoryManager::getSingleton().createInstance(type);
	//	if (attributes.exists("name"))
	//		mObject->setName(attributes.getValue("name"));
	//	else
	//		mObject->setName(mScene->generateAutoName(mObject));
	//	mScene->addObject(mObject);

	//	NameCollectionMap::const_iterator it = mPropertyNameCollectionMap.find(type);
	//	if (it != mPropertyNameCollectionMap.end())
	//	{
	//		mPropertyNameMap = &it->second;
	//	}
	//}
	//else if (element == "Attribute")
	//{
	//	assert(!mString);
	//	mAttributeName = attributes.getValue("name");
	//	mAttributeValue.clear();
	//	mString = &mAttributeValue;
	//}
	//else if (element == "Terrain")
	//{
	//	mScene->mTerrainFilename = attributes.getValue("filename");
	//}
	//else if (element == "Scene")
	//{
	//	String formatVersion = attributes.getValue("formatVersion");
	//	if (formatVersion != CURRENT_FORMAT_VERSION &&
	//		formatVersion != "0.1.0")
	//	{
	//		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
	//			"Unexpected format version was found while parsing the scene file."
	//			"\nformat version require: " + CURRENT_FORMAT_VERSION +
	//			"\nformat version in scene file: " + formatVersion,
	//			"SceneXMLHandler::startElement");
	//	}

	//	mScene->mName = attributes.getValueAs<String>("name", Ogre::StringUtil::BLANK);

	//	if (formatVersion == "0.1.0")
	//	{
	//		mPropertyNameCollectionMap.clear();

	//		NameMap& enviroment = mPropertyNameCollectionMap["Enviroment"];
	//		enviroment["fog mode"] = "fog.mode";
	//		enviroment["fog colour"] = "fog.colour";
	//		enviroment["fog exp density"] = "fog.exp density";
	//		enviroment["fog linear start"] = "fog.linear start";
	//		enviroment["fog linear end"] = "fog.linear end";
	//	}
	//}
	//else if (element == "Author")
	//{
	//	assert(!mString);
	//	mString = &mScene->mAuthor;
	//}
	//else if (element == "Copyright")
	//{
	//	assert(!mString);
	//	mString = &mScene->mCopyright;
	//}
	//else if (element == "Description")
	//{
	//	assert(!mString);
	//	mString = &mScene->mDescription;
	//}
	//// anything else is an error which *should* have already been caught by XML validation
	//else
	//{
	//	OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
	//		"Unexpected data was found while parsing the scene file: '" + element + "' is unknown.",
	//		"SceneXMLHandler::startElement");
	//}
}

void SceneXMLHandler::endElement(const String& element)
{
	//if (element == "Object")
	//{
	//	assert(mObject);
	//	mObject.reset();
	//	mPropertyNameMap = NULL;
	//}
	//else if (element == "Attribute")
	//{
	//	assert(mString);
	//	mString = NULL;
	//	mScene->setAttribute(mAttributeName, mAttributeValue);
	//}
	//else if (element == "Author")
	//{
	//	assert(mString);
	//	mString = NULL;
	//}
	//else if (element == "Copyright")
	//{
	//	assert(mString);
	//	mString = NULL;
	//}
	//else if (element == "Description")
	//{
	//	assert(mString);
	//	mString = NULL;
	//}
}


CLevelSerializer::CLevelSerializer()
{

}

CLevelSerializer::~CLevelSerializer()
{

}

void CLevelSerializer::Load()
{

}

void CLevelSerializer::Save()
{

}