#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDROW
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDROW

#include "../BaseGuiElements/CoreGuiBase.h"

class GridRowData
{
public:
  GridRowData( uint32 rowId, GridRowData* parent, bool isExpanded );
  GridRowData*                GetParent(){ return m_Parent;}
  void                        SetRowId(uint32 rowId){ m_RowId = rowId; }
  uint32                      GetRowId() const { return m_RowId; }
  uint32                      GetNumColumns() const{ return (uint32)m_RowData.size(); }
  bool                        IsExpanded() const{ return m_IsExpanded; }
  void                        SetExpanded( bool isExpanded ) const { m_IsExpanded = isExpanded; }
  void                        SetRowView(CoreGuiBase* rowView) { m_RowObject = rowView; }
  CoreGuiBase*                GetRowView() const{ return m_RowObject; }
  void AddRowData(CoreGuiBase* guiBase);
  CoreGuiBase* GetColumnData( uint32 columnIndex ) const;
  
private:
  GridRowData*                m_Parent;
  CoreGuiBase*                m_RowObject;
  uint32                      m_RowId;
  std::vector< CoreGuiBase* > m_RowData;
  mutable bool                m_IsExpanded;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIGRIDROW
