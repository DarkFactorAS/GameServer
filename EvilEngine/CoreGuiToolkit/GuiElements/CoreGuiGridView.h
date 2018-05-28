#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDVIEW
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDVIEW

#include "../BaseGuiElements/CoreGuiBase.h"
#include "GridRowData.h"

class CoreGuiScrollbar;

class CoreGuiGridView : public CoreGuiBase, public SignalTarget
{
public:


  CoreGuiGridView( const Vector3& position, const Vector3& size );
  CoreGuiGridView( const String& name, const Vector3& position, const Vector3& size );
  ~CoreGuiGridView();

  static CoreGuiBase*         Initialize(){ return new CoreGuiGridView( Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }

  void                        Init();
  virtual void                SetMember( const String& key, const String& value ) DF_OVERRIDE;
  void                        AddHeader( int widthPercentage, const String& name );
  void                        AddData( GridRowData* data );
  void                        Clear();
  void                        SetSelected( GridRowData* selectedRow );
  void                        SetSelectedItem( CoreGuiBase* selectedItem );
  const GridRowData*          GetRowData( uint32 rowId );
  bool                        ToggleExpandRow(uint32 rowId);
  void                        RepositionRows();
  bool                        AllParentsAreExpanded(GridRowData* rowData);

  virtual bool                OnMouseButtonWheel(float delta, const Vector2& mousePos);
  virtual bool                OnDraw( const Matrix& projection ) DF_OVERRIDE;

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiGridView"; }
#endif

private:

  void                        SlotScroll(int scrollPercent);
  void                        SetRowColor( GridRowData* selectedRow, const Color& col );
  GridRowData*                GetInternalRowData(uint32 rowId);

  bool                        m_Initialized;
  uint32                      m_RowId;
  float                       m_RowWidth;
  float                       m_RowHeight;
  float                       m_ScrollStart;
  CoreGuiBase*                m_SelectedObject;
  CoreGuiScrollbar*           m_Scrollbar;

  std::vector< int >          m_ColumnWidth;
  std::vector< String >       m_ColumnHeader;
  std::vector< GridRowData* > m_RowData;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDVIEW
