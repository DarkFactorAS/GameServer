#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUITILEVIEW
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUITILEVIEW

#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/FrameBuffer.h"
#include "CoreGuiWindow.h"

class CoreGuiButton;
class QuadRenderObject;

class CoreGuiTileView : public CoreGuiBase, public SignalTarget
{
public:

  enum LAYOUT
  {
    LAYOUT_HORIZONTAL,
    LAYOUT_VERTICAL,
  };

  CoreGuiTileView( const Vector2& position, const Vector2& size, const Vector2& tileSize, int layout );
  CoreGuiTileView( const String& objectName, const Vector2& position, const Vector2& size, const Vector2& tileSize, int layout );
  ~CoreGuiTileView();

  static CoreGuiBase*         Initialize(){ return new CoreGuiTileView( Vector2::s_Vec2Zero, Vector2::s_Vec2One, Vector2::s_Vec2One, LAYOUT_HORIZONTAL ); }
  void                        SetMember( const String& key, const String& value ) DF_OVERRIDE;

  void                        Init();
  void                        ReInit();
  void                        Clear();

  virtual bool                OnDraw( const Matrix& projection ) DF_OVERRIDE;
  CoreGuiBase*                OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  //virtual CoreGuiBase*        GetGuiUnderMouse( float xPos, float yPos, bool isWireframe );
  virtual bool                OnMouseButtonWheel( float delta, const Vector2& mousePos ) DF_OVERRIDE;

  virtual CoreGuiBase*        GetWireGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;
  virtual CoreGuiBase*        GetSolidGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;

  virtual void                SetPosition( float x, float y ) DF_OVERRIDE;
  virtual void                SetSize( float x, float y ) DF_OVERRIDE;

  CoreGuiBase*                AddTile( CoreGuiBase* tile );
  void                        SetSelected( CoreGuiBase* tile );
  CoreGuiBase*                GetSelected() const { return m_SelectedTile; }
  CoreGuiBase*                GetTileWithName( const String& tileName );
  bool                        RemoveTile( CoreGuiBase* tile );

  void                        SetSelectedColor( const Color& color );
  void                        SetNormalColor( const Color& color );

  const Vector2&              GetTileSize(){ return m_TileSize; }
  virtual bool                RunDelete() DF_OVERRIDE;

  Signal1< void, CoreGuiBase* > SignalItemClicked;

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiTileView"; }
#endif

private:

  void                        CreateFrameBuffer();

  float                       GetVisibleTop();
  void                        Reposition();
  void                        ScrollLeft( int btnId );
  void                        ScrollRight( int btnId );
  void                        ScrollStop();

  std::vector< CoreGuiBase* > m_Tiles;
  CoreGuiButton*              m_Arrow1;
  CoreGuiButton*              m_Arrow2;
  Vector2                     m_TileSize;
  Vector2                     m_TilePosition;
  int                         m_Layout;
  bool                        m_NeedsUpdate;
  float                       m_Offset;
  float                       m_ScrollSpeed;
  CoreGuiBase*                m_SelectedTile;

  Color                       m_SelectedColor;
  Color                       m_NormalColor;

  FrameBuffer*                m_Framebuffer;
  QuadRenderObject*           m_FramebufferView;
  int                         m_OffsetIndex;
  int                         m_VisibleTiles;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIDIALOG
