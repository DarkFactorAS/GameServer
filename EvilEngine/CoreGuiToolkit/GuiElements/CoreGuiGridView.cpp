
#include "Precompile.h"
#include "CoreGuiGridView.h"

#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiFrame.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiImageView.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiTextfield.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiView.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiScrollbar.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"

#include <stdlib.h>

CoreGuiGridView::CoreGuiGridView( const Vector3& position, const Vector3& size ) : 
  CoreGuiBase( "CoreGuiGridView", position, size ),
  m_Initialized( false ),
  m_RowId(0),
  m_RowHeight( 0.1f ),
  m_ScrollStart( 0.0f ),
  m_SelectedObject( NULL )
{
}

CoreGuiGridView::CoreGuiGridView( const String& name, const Vector3& position, const Vector3& size ) : 
  CoreGuiBase( name, position, size ),
  m_RowId(0),
  m_Initialized( false ),
  m_RowHeight( 0.1f ),
  m_ScrollStart(0.0f),
  m_SelectedObject( NULL )
{
}

CoreGuiGridView::~CoreGuiGridView()
{
}

GridRowData* CoreGuiGridView::GetInternalRowData(uint32 rowId)
{
  // Remove everything
  for (std::vector< GridRowData* >::iterator itRowData = m_RowData.begin(); itRowData != m_RowData.end(); itRowData++)
  {
    GridRowData* rowData = *itRowData;
    if ( rowData->GetRowId() == rowId )
    {
      return rowData;
    }
  }
  return NULL;
}

const GridRowData* CoreGuiGridView::GetRowData(uint32 rowId)
{
  return GetInternalRowData( rowId );
}

void CoreGuiGridView::SetMember( const String& key, const String& value )
{
  if ( key.CompareWithCase("height" ) == 0 )
  {
    m_RowHeight = (float) atof( value.c_str() );
    m_Initialized = false;
  }
  else
  {
    CoreGuiBase::SetMember( key, value );
  }
}

void CoreGuiGridView::AddHeader( int widthPercentage, const String& name )
{
  m_ColumnWidth.push_back( widthPercentage );
  m_ColumnHeader.push_back( name );
}

void CoreGuiGridView::AddData( GridRowData* data )
{
  m_RowData.push_back( data );
}

void CoreGuiGridView::Clear()
{
  // Remove everything
  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    guiObject->Destroy();
  }
  m_GuiObjects.clear();

  for (std::vector<GridRowData*>::iterator itRow = m_RowData.begin(); itRow != m_RowData.end(); itRow++)
  {
    GridRowData* rowData = *itRow;
    delete rowData;
  }
  m_RowData.clear();

  m_Initialized = false;
}

void CoreGuiGridView::Init()
{
  Vector2 pos  = GetPosition();
  Vector2 size = GetSize();

  float startY    = pos.y - size.y + m_RowHeight;
  float startX    = pos.x - size.x;

  if ( m_ColumnWidth.empty() )
  {
    return;
  }

  AddDebugFrame( Color::Yellow );

  // Recalculate the width
  m_RowWidth = 0.0f;
  for ( size_t index = 0; index < m_ColumnWidth.size(); index++ )
  {
    m_RowWidth += ( size.x * m_ColumnWidth[ index ] * 0.009f );
  }

  // Add Scrollbar
  float scrollbarWidth = size.x * 0.1f;

  m_Scrollbar = new CoreGuiScrollbar( Vector2(pos.x + size.x - scrollbarWidth, pos.y ), Vector2( scrollbarWidth, size.y ), Color::DarkBlue, CoreGuiScrollbar::SCROLL_VERTICAL );
  m_Scrollbar->SignalScroll.Connect( this, &CoreGuiGridView::SlotScroll );
  AddChild( m_Scrollbar );
  
  // Initialize the header
  CoreGuiView* header = new CoreGuiView( m_Position, Vector2( m_RowWidth, m_RowHeight ) );
  for ( size_t index = 0; index < m_ColumnWidth.size(); index++ )
  {
    int hSize = m_ColumnWidth[ index ];
    float columnWidth = ( size.x * hSize * 0.009f );

    startX += columnWidth;

    Vector2 gridPos  = Vector2( startX, startY );
    Vector2 gridSize = Vector2( columnWidth, m_RowHeight );

    header->AddChild( new CoreGuiImageView( gridPos, gridSize, Color::Yellow ) );
    header->AddChild( new CoreGuiTextfield( m_ColumnHeader[index], gridPos, gridSize, TextStringRenderObject::ALIGNMENT_CENTER ) );    
    header->AddChild( new CoreGuiFrame( gridPos, gridSize, Color::Black ) );

    startX += columnWidth;
    //startX += 0.01f;
  }
  AddChild( header );

  // Initialize the data
  for ( size_t index = 0; index < m_RowData.size(); index++ )
  {
    startX    = pos.x - size.x;
    startY += ( m_RowHeight * 2.0f );

    // Do not add rows below the range
    if ( startY < m_Position.y - m_Size.y )
    {
      break;
    }

    Color rowColor = Color::Black;

    CoreGuiView* row = new CoreGuiView( Vector2( 0, startY ), Vector2( m_RowWidth, m_RowHeight ) );
    GridRowData* data = m_RowData[index];
    data->SetRowView(row);

    // Any of the rows contain the selected item ?
    for (size_t columnIndex = 0; columnIndex < data->GetNumColumns(); columnIndex++)
    {
      CoreGuiBase* obj = data->GetColumnData(columnIndex);
      if (obj != NULL && obj == m_SelectedObject )
      {
        rowColor = Color::Yellow;

        GridRowData* parent = data->GetParent();
        while ( parent != NULL )
        {
          parent->SetExpanded( true );
          parent = parent->GetParent();
        }

        break;
      }
    }

    for ( size_t columnIndex = 0; columnIndex < data->GetNumColumns(); columnIndex++ )
    {
      if ( columnIndex >= m_ColumnWidth.size() )
      {
        continue;
      }

      int hSize = m_ColumnWidth[ columnIndex ];
      float width = ( size.x * hSize * 0.009f );

      startX += width;

      Vector2 gridPos  = Vector2( startX, startY );
      Vector2 gridSize = Vector2( width, m_RowHeight );

      CoreGuiBase* obj = data->GetColumnData( columnIndex );
      if ( obj == NULL )
      {
        continue;
      }

      obj->SetPosition( gridPos );
      obj->SetSize( gridSize );

      row->AddChild( obj );    
      row->AddChild( new CoreGuiFrame( gridPos, gridSize, rowColor ) );

      startX += width;
    }

    AddChild( row );
  }

  RepositionRows();
}

bool CoreGuiGridView::ToggleExpandRow(uint32 rowId)
{
  GridRowData* rowData = GetInternalRowData(rowId);
  if ( rowData != NULL )
  {
    bool isExpanded = !rowData->IsExpanded();
    rowData->SetExpanded( isExpanded );
    RepositionRows();
    return isExpanded;
  }
  return false;
}

bool CoreGuiGridView::AllParentsAreExpanded( GridRowData* rowData )
{
  if ( rowData == NULL )
  {
    return true;
  }
  else if ( !rowData->IsExpanded() )
  {
    return false;
  }
  return AllParentsAreExpanded( rowData->GetParent() );
}

void CoreGuiGridView::RepositionRows()
{
  Vector2 pos = GetPosition();
  Vector2 size = GetSize();
  float top    = pos.y - size.y;
  float bottom = pos.y + size.y;
  float startY = top + ( m_RowHeight * 2 );
  float visibleSpace = 0.0f;
  float realStartY = startY;
  for (size_t index = 0; index < m_RowData.size(); index++)
  {
    GridRowData* rowData = m_RowData[index];

    CoreGuiBase* rowView = rowData->GetRowView();
    if (rowView != NULL && AllParentsAreExpanded( rowData->GetParent() ) )
    {
      float startX = rowView->GetPosition().x;
      float height = rowView->GetHeight();
      visibleSpace += height * 0.5f;
      startY += height * 0.5f;
      
      if ( ( startY >= top + height + m_ScrollStart ) && ( startY < bottom - height + m_ScrollStart ) )
      {
        realStartY += height * 0.5f;
        rowView->SetVisible(true);
        rowView->SetPosition(startX, realStartY);
        startY += rowView->GetHeight() * 0.5f;
        realStartY += height * 0.5f;

        continue;
      }
    }
    
    rowView->SetVisible(false);
  }

  // Calculate slider
  if ( visibleSpace <= size.y )
  {
    visibleSpace = size.y;
  }

  // update scrollbar
  if ( m_Scrollbar != NULL )
  {
    m_Scrollbar->SetTotalSize(visibleSpace);
  }
}

bool CoreGuiGridView::OnDraw( const Matrix& projection )
{
  if ( !m_Initialized )
  {
    m_Initialized = true;
    Init();
  }
  return CoreGuiBase::OnDraw( projection );
}

void CoreGuiGridView::SetSelectedItem( CoreGuiBase* selectedItem )
{
  m_SelectedObject = selectedItem;
}

void CoreGuiGridView::SetSelected( GridRowData* /*selectedRow*/ )
{
}

bool CoreGuiGridView::OnMouseButtonWheel(float delta, const Vector2& mousePos)
{
  // Only forward scroll if we are scrolling inside the grid
  if (!IsIntersecting(mousePos.x, mousePos.y))
  {
    return false;
  }
  return CoreGuiBase::OnMouseButtonWheel( delta, mousePos );
}

void CoreGuiGridView::SlotScroll(int scrollPercent)
{
  Vector2 size = GetSize();
  m_ScrollStart = scrollPercent * 0.01f * size.y;
  RepositionRows();
}

void CoreGuiGridView::SetRowColor( GridRowData* selectedRow, const Color& col )
{
  if ( selectedRow != NULL )
  {
    for ( size_t columnIndex = 0; columnIndex < selectedRow->GetNumColumns(); columnIndex++ )
    {
      CoreGuiBase* columnData = selectedRow->GetColumnData( columnIndex );
      if (columnData != NULL)
      {
        CoreGuiBase* parent = columnData->GetParent();
        if (parent != NULL)
        {
          parent->SetColor(col);
          return;
        }
      }
    }
  }
}
