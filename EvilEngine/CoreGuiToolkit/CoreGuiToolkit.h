#ifndef EVILENGINE_COREGUITOOLKIT_COREGUITOOLKIT
#define EVILENGINE_COREGUITOOLKIT_COREGUITOOLKIT

#include "EvilEngine/CoreRendering/RenderModule.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilEngine/CoreLib/BasicTypes/Mutex/Mutex.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"

class CoreGuiBase;
class CoreOpenGLRender;
class CoreBaseObject;
class CoreGuiDialog;

class CoreGuiToolkit : public RenderModule, public EngineModule, public SignalTarget
{
public:

  enum ModuleType
  {
    RenderModule_GuiToolkit = 10
  };

  enum GuiObjectType
  {
    GuiObject_Dialog
  };

  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("CoreGuiToolkit"); }

  static CoreGuiToolkit*      GetInstance();
  static void                 Destroy();
  static CoreGuiToolkit*      CreateInstance();

  virtual ~CoreGuiToolkit();

  typedef CoreGuiBase*        InstantiateGui_fp();
  static CoreGuiBase*         InstantiateGui( const String& );

  virtual CoreGuiBase*        CreateGuiObject( GuiObjectType objectType );

  /// From RenderModule
#ifdef CLIENT
  virtual void                OnDraw( const Matrix& projection )                          DF_OVERRIDE;
  virtual void                ResizeScreen( int width, int height )                       DF_OVERRIDE;
#endif

  /// From EngineModule
  virtual void                FrameProcess(float deltaTime)                             DF_OVERRIDE;
#ifdef CLIENT
  virtual bool                OnMouseButtonDown( int buttonId, float xPos, float yPos )   DF_OVERRIDE;
  virtual bool                OnMouseButtonUp( int buttonId, float xPos, float yPos )     DF_OVERRIDE;
  virtual bool                OnMouseButtonWheel( float delta, const Vector2& mousePos )  DF_OVERRIDE;
  virtual void                OnMouseMove( float xPos, float yPos )                       DF_OVERRIDE;
  virtual bool                OnKeyDown( int keyCode )                                    DF_OVERRIDE;
  virtual bool                OnKeyUp( int keyCode )                                      DF_OVERRIDE;
#endif

  CoreGuiBase*                GetWireGuiUnderMouse( float xPos, float yPos );
  CoreGuiBase*                GetSolidGuiUnderMouse( float xPos, float yPos );

  void                        AddGuiObject( CoreGuiBase* guiObject );
  void                        RemoveGuiObject( CoreGuiBase* guiObject );
  CoreGuiBase*                GetChildWithName( const String& guiName );
  bool                        HasGuiObject( CoreGuiBase* guiObject );
  void                        RemoveAllGuiObjects();


  bool                        SetNextFocus();
  void                        GainFocus( CoreGuiBase* focusObject );
  void                        LooseFocus();
  void                        SlotLooseFocus( CoreGuiBase* focusObject );
  CoreGuiBase*                GetFocusObject(){ return m_FocusObject; }

  static CoreGuiDialog*       CreateDefaultDialog( const String& title, const String& message, int buttonGroupId );
  static CoreGuiDialog*       GenericErrorDialog( const String& title, const String& message );

  static void                 SetDragObject( CoreBaseObject* dragObject ){ s_DragObject = dragObject; }
  static CoreBaseObject*      GetDragObject(){ return s_DragObject; }

  static float                s_Left;
  static float                s_Right;
  static float                s_Top;
  static float                s_Bottom;
  static float                s_Width;
  static float                s_Height;

  static bool                 s_MoveGui;
  static CoreBaseObject*      s_DragObject;

  const Matrix&               GetProjection(){ return m_Projection; }

#ifdef CLIENT
  Signal1< void, const Vector2& > SignalMouseLongTouch;
#endif

  std::vector< CoreGuiBase* > GetGuiObjects(){ return m_GuiObjects; }

protected:
  CoreGuiToolkit();

  static CoreGuiToolkit*      s_Instance;

  void                        AddGuiSpawner( const char* name, InstantiateGui_fp* fp );

  void                        OnGuiObjectDeath( const CoreBaseObject* guiObject );
  void                        OnFocusObjectDeath( const CoreBaseObject* guiObject );

  CoreGuiBase*                m_FocusObject;
  Matrix                      m_Projection;

  std::vector< CoreGuiBase* > m_GuiObjects;
  std::vector< CoreGuiBase* > m_NewGuiObjects;
  Mutex                       m_GuiObjectMutex;

  float                       m_MouseHoldTimer;
  Vector2                     m_MouseHoldPos;

  static std::map< const String, InstantiateGui_fp* >  s_GuiSpawnList;
};

#endif /// EVILENGINE_COREGUITOOLKIT_COREGUITOOLKIT
