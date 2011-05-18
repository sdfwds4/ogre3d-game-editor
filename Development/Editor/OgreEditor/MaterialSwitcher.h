

#pragma once

using namespace Ogre;

class SelectionBuffer;

struct cmp_ColourValue
{
	bool operator()(const ColourValue &a, const ColourValue &b) const
	{
		return a.getAsBGRA() < b.getAsBGRA();
	}
};

class MaterialSwitcher : public MaterialManager::Listener
{
private:
	typedef std::map<ColourValue, String, cmp_ColourValue> ColorMap;
	typedef std::map<ColourValue, String, cmp_ColourValue>::const_iterator ColorMapConstIter;

	String       mEmptyString;
	ColourValue  mCurrentColor;
	String       mLastEntity;
	Technique   *mLastTechnique;
	MaterialSwitcher::ColorMap mColorDict;

	void getNextColor();

	friend SelectionBuffer;
public:
	MaterialSwitcher();
	~MaterialSwitcher();

	virtual Technique *handleSchemeNotFound(unsigned short schemeIndex, const String &schemeName, 
		Material *originalMaterial, unsigned short lodIndex, 
		const Renderable *rend);
	const String &getEntityName(const ColourValue &color) const;
	void reset();
};


