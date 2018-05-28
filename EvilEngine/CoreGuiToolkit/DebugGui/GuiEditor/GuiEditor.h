
#include "../../BaseGuiElements/CoreGuiXMLView.h"

class CoreGuiGridView;
class GridRowData;
class EffectPackage;

class GuiEditor : public CoreGuiXMLView
{
public:
  
  enum RowIndexEnum
  {
    GameObjectIndex = 5
  };
  
  GuiEditor();
  virtual ~GuiEditor();

  void                      SetActiveParent( CoreGuiBase* parent );
  void                      SlotClear(int /*btnId*/);
  void                      SlotCloseWindow( int btnId );
  void                      SlotSaveGui( int btnId );
  void                      SlotToggleDebug( int btnId );
  void                      SlotApplySettings( int btnId );
  void                      SlotReloadThisGui(int /*btnId*/);
  void                      SlotReloadGui(int /*btnId*/ );
  void                      SlotTopGui(int btnId);

  virtual void              OnDataLoaded( Resource* /*resource*/ ) DF_OVERRIDE;

  Signal0< void >           SignalWindowClosed;
  
private:

  void                      StopEffect();
  void                      DeleteElement(int rowId, CoreGuiButton* button);
  void                      EditElement(int rowId, CoreGuiButton* button);
  void                      ChooseElement(int rowId, CoreGuiButton* /*button*/);
  void                      ToggleVisibility(int rowId, CoreGuiButton* /*button*/);
  void                      ExpandCollapseElement(int rowId, CoreGuiButton* button);
  CoreGuiBase*              GetObjectFromRow(uint32 rowId);
  uint32                    AddChildren( CoreGuiGridView* gridView, CoreGuiBase* xmlParent, GridRowData* parent, uint32 uniqueId );

  float                     m_EffectStartAlpha;
  EffectPackage*            m_EffectPackage;
  CoreGuiBase*              m_GuiObject;
};
