
#include "Precompile.h"
#include "GridRowData.h"

#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiFrame.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiImageView.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiTextfield.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiView.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"

#include <stdlib.h>

GridRowData::GridRowData( uint32 rowId, GridRowData* parent, bool isExpanded )
{
  m_RowId = rowId; 
  m_IsExpanded = isExpanded; 
  m_Parent = parent; 
}

void GridRowData::AddRowData(CoreGuiBase* guiBase)
{
  m_RowData.push_back( guiBase );
}

CoreGuiBase* GridRowData::GetColumnData( uint32 columnIndex ) const
{
  if ( columnIndex < m_RowData.size() )
  {
    return m_RowData[ columnIndex ];
  }
  return NULL;
}
