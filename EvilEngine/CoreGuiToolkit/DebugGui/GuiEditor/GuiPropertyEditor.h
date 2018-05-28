
#include "../../BaseGuiElements/CoreGuiXMLView.h"

class CoreGuiGridView;

class GuiPropertyEditor : public CoreGuiXMLView
{
public:

  GuiPropertyEditor( CoreGuiBase* guiObject, bool isMinimized, const Vector2& position );

  virtual void              OnDataLoaded(Resource* /*resource*/) DF_OVERRIDE;
  void                      SlotChangedNameInputfield(int actionId,CoreGuiBase* guiBase);
  void                      SlotChangedValueInputfield( int /*btnId*/, CoreGuiBase* guiBase );
  void                      SlotSelf(int btnId);
  void                      SlotAddField( int /*btnId*/ );
  void                      SlotUndo( int /*btnId*/ );
  void                      SlotCloseWindow(int btnId);
  void                      SlotToggleMinimize( int /*btnId*/ );

private:

  void                      ShowGridList(std::vector< KeyValue > memberList);

  void                      CloseWindow();

  CoreGuiBase*              m_GuiObject;

  std::vector< KeyValue >   m_MemberList;
  
  bool                      m_HasChanges;
  bool                      m_IsMinimized;
  Vector2                   m_ParentPosition;
};
