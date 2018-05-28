/*************************************************************************************
* Project     : Evil Engine 2012
* File        : DFCallstack
* Description : Generates a callstack and returns it in string form. The code is
*               fetched mainly from here:
*               http://stackoverflow.com/questions/6205981/windows-c-stack-trace-from-a-running-app
* 
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "DFCallstack.h"

DFCallstack* DFCallstack::s_Instance = NULL;

#if defined PLATFORM_WINDOWS && defined DEBUG

#include <windows.h>
#include <winnt.h>

#include <string>
#include <vector>
#include <Psapi.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <iterator>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

// Some versions of imagehlp.dll lack the proper packing directives themselves
// so we need to do it.
#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )

struct module_data 
{
  std::string image_name;
  std::string module_name;
  void *base_address;
  DWORD load_size;
};
typedef std::vector<module_data> ModuleList;

class SymHandler 
{
  HANDLE p;
public:
  SymHandler(HANDLE process, char const *path=NULL, bool intrude = false) : p(process) 
  { 
    if (!SymInitialize(p, path, intrude)) 
    {
      throw(std::logic_error("Unable to initialize symbol handler"));
    }
  }
  ~SymHandler() 
  {
    SymCleanup(p); 
  }
};

class symbol 
{ 
  typedef IMAGEHLP_SYMBOL64 sym_type;
  sym_type *sym;
  static const int max_name_len = 1024;
public:
  symbol(HANDLE process, DWORD64 address) : sym((sym_type *)::operator new(sizeof(*sym) + max_name_len)) 
  {
    memset(sym, '\0', sizeof(*sym) + max_name_len);
    sym->SizeOfStruct = sizeof(*sym);
    sym->MaxNameLength = max_name_len;
    DWORD64 displacement;

    if (!SymGetSymFromAddr64(process, address, &displacement, sym))
    {
      throw(std::logic_error("Bad symbol"));
    }
  }

  std::string name() { return std::string(sym->Name); }
  std::string undecorated_name() { 
    std::vector<char> und_name(max_name_len);
    UnDecorateSymbolName(sym->Name, &und_name[0], max_name_len, UNDNAME_COMPLETE);
    return std::string(&und_name[0], strlen(&und_name[0]));
  }
};

class get_mod_info {
  HANDLE process;
  static const int buffer_length = 4096;
public:
  get_mod_info(HANDLE h) : process(h) {}

  module_data operator()(HMODULE module) { 
    module_data ret;
    char temp[buffer_length];
    MODULEINFO mi;

    GetModuleInformation(process, module, &mi, sizeof(mi));
    ret.base_address = mi.lpBaseOfDll;
    ret.load_size = mi.SizeOfImage;

    GetModuleFileNameEx(process, module, LPWSTR(temp), sizeof(temp));
    ret.image_name = temp;
    GetModuleBaseName(process, module, LPWSTR(temp), sizeof(temp));
    ret.module_name = temp;
    std::vector<char> img(ret.image_name.begin(), ret.image_name.end());
    std::vector<char> mod(ret.module_name.begin(), ret.module_name.end());
    SymLoadModule64(process, 0, &img[0], &mod[0], (DWORD64)ret.base_address, ret.load_size);
    return ret;
  }
};

void* load_modules_symbols(void* process, DWORD /*pid*/) 
{
  ModuleList modules;

  DWORD cbNeeded;
  std::vector<HMODULE> module_handles(1);

  EnumProcessModules(process, &module_handles[0], module_handles.size() * sizeof(HMODULE), &cbNeeded);
  module_handles.resize(cbNeeded/sizeof(HMODULE));
  EnumProcessModules(process, &module_handles[0], module_handles.size() * sizeof(HMODULE), &cbNeeded);

  std::transform(module_handles.begin(), module_handles.end(), std::back_inserter(modules), get_mod_info(process));
  return modules[0].base_address;
}

void sym_options(DWORD add, DWORD remove=0) 
{
  DWORD symOptions = SymGetOptions();
  symOptions |= add;
  symOptions &= ~remove;
  SymSetOptions(symOptions);
}

/************************************************************************************************
* InitStackFrame:
* Sets the argument for StackWalker64 for the addresses to check on the stack.
*
* @author Thor Richard Hansen
*************************************************************************************************/
STACKFRAME64 InitStackFrame(CONTEXT c) 
{
  STACKFRAME64 s;

#ifdef _M_X64
  s.AddrPC.Offset = c.Rip;
  s.AddrPC.Mode = AddrModeFlat;
  s.AddrStack.Offset = c.Rsp;
  s.AddrStack.Mode = AddrModeFlat;    
  s.AddrFrame.Offset = c.Rbp;
  s.AddrFrame.Mode = AddrModeFlat;
#else
  s.AddrPC.Offset = c.Eip;
  s.AddrPC.Mode = AddrModeFlat;
  s.AddrStack.Offset = c.Esp;
  s.AddrStack.Mode = AddrModeFlat;    
  s.AddrFrame.Offset = c.Ebp;
  s.AddrFrame.Mode = AddrModeFlat;
#endif

  return s;
}

bool get_callstack(String &os, void* hThread, uint32 imageType, void* process, CONTEXT& c) 
{
  int             frame_number        = 0;
  DWORD           offset_from_symbol  = 0;
  IMAGEHLP_LINE64 line                = {0};

  STACKFRAME64 s = InitStackFrame(c);
  line.SizeOfStruct = sizeof(line);

  do 
  {
    if (!StackWalk64(imageType, process, hThread, &s, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
    {
      return false;
    }

    if ( s.AddrPC.Offset != 0 ) 
    {
      if (SymGetLineFromAddr64( process, s.AddrPC.Offset, &offset_from_symbol, &line ) ) 
      {
        os += String::FormatString( "[%d] %s:%d \r\n", frame_number, line.FileName, line.LineNumber );
      }
    }

    ++frame_number;
  } while (s.AddrReturn.Offset != 0);

  return true;
}

DFCallstack::DFCallstack()
{
  m_Process     = GetCurrentProcess();
  m_SymHandler  = new SymHandler( m_Process );

  sym_options(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
  void *base = load_modules_symbols(m_Process, GetCurrentProcessId());
  IMAGE_NT_HEADERS *h = ImageNtHeader(base);
  m_ImageType = h->FileHeader.Machine;
}

bool DFCallstack::GetCallStack(String &os) 
{
  HANDLE hThread = GetCurrentThread();

  CONTEXT context;
  memset(&context, 0, sizeof(context));
  context.ContextFlags = CONTEXT_ALL;
  RtlCaptureContext(&context);

  return get_callstack( os, hThread, m_ImageType, m_Process, context );
}

bool DFCallstack::GetCallStack(String &os, void* hThread) 
{
  CONTEXT context;
  memset(&context, 0, sizeof(context));
  context.ContextFlags = CONTEXT_ALL;
  RtlCaptureContext(&context);

  return get_callstack( os, hThread, m_ImageType, m_Process, context );
}

#else 

bool DFCallstack::GetCallStack(String &/*os*/){ return false; }
bool DFCallstack::GetCallStack(String &/*os*/, void* /*hThread*/) { return false; }
DFCallstack::DFCallstack(){}

#endif

/************************************************************************************************
* GetInstance:
* Singleton for the assert class. This is a singleton because we want to cache the symbols for
* this program to make it faster to fetch multiple callstacks.
*
* @author Thor Richard Hansen
*************************************************************************************************/
DFCallstack* DFCallstack::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new DFCallstack();
  }
  return s_Instance;
}

/************************************************************************************************
* GetInstanceIfAny:
* Get the instance of the class if it is still alive
*
* @author Thor Richard Hansen
*************************************************************************************************/
DFCallstack* DFCallstack::GetInstanceIfAny()
{
  return s_Instance;
}
