#ifndef EVILENGINE_COREGUITOOLKIT_GUICREATOR_GUIRESOURCE
#define EVILENGINE_COREGUITOOLKIT_GUICREATOR_GUIRESOURCE 1

#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"

class CoreGuiBase;
class XMLNode;
class CoreBaseObject;
class XMLParser;

class GuiResource : public Resource, public SignalTarget
{
public:

  GuiResource( CoreGuiBase* parent, const char * filename );
  GuiResource( CoreGuiBase* parent, uint32 resourceInstance, const char * filename );
  GuiResource( CoreGuiBase* parent, const char * filename, ResourceRef* memory );
  virtual ~GuiResource();

  virtual void                        Copy( const Resource* other ) DF_OVERRIDE;

  virtual bool                        LoadData()            DF_OVERRIDE;
  virtual bool                        ReloadData()          DF_OVERRIDE;
  virtual void                        OnDoneLoading()       DF_OVERRIDE;

  virtual bool                        UniqueByName() const  DF_OVERRIDE { return true; }
  void                                OnParentDeath( const CoreBaseObject* runObject );
  std::vector< CoreGuiBase* >         GetParents(){ return m_Parents; }
  bool                                SpawnGui(CoreGuiBase* parent);

  Signal2< void, GuiResource*, CoreGuiBase* >  m_SignalResourceReady;

private:

  CoreGuiBase*                        SpawnGui(CoreGuiBase* parent, XMLNode* root );

  XMLParser*                          m_ParsedData;
  std::vector< CoreGuiBase* >         m_Parents;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUICREATOR_GUIRESOURCE
