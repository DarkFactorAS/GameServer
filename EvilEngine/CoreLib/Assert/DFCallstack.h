#ifndef EVILENGINE_CORELIB_ASSERT_DFCALLSTACK
#define EVILENGINE_CORELIB_ASSERT_DFCALLSTACK 1

//#include <windows.h>

class SymHandler;

class DFCallstack
{
public:

  DFCallstack();

  static DFCallstack*     GetInstance();
  static DFCallstack*     GetInstanceIfAny();

  bool                    GetCallStack(String& os);
  bool                    GetCallStack(String& os, void* hThread);
  bool                    GetCallStack(String& os, int addressLength, void* context);

private:

  static DFCallstack*     s_Instance;

  SymHandler*             m_SymHandler;
  void*                   m_Process;
  uint32                  m_ImageType;
};

#endif