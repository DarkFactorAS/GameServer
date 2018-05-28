#ifndef EVILENGINE_COREUNITTEST_COREASYNCUNITTEST
#define EVILENGINE_COREUNITTEST_COREASYNCUNITTEST 1

class CoreUnittest;

class CoreAsyncUnittest : public CoreUnittest
{
protected:

  virtual void          Failed(const String& errorMessage) DF_OVERRIDE;

public:

  CoreAsyncUnittest(String testName);

  virtual bool           Execute() DF_OVERRIDE;

  virtual CoreUnittest::UnitTestType GetUnittestType() DF_OVERRIDE DF_FINAL
  {
    return CoreUnittest::e_AsyncUnitTest;
  }
};

#endif /// EVILENGINE_COREUNITTEST_COREASYNCUNITTEST