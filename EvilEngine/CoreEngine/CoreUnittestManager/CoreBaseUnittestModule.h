#ifndef EVILENGINE_COREUNITTEST_COREBASEUNITTESTMODULE
#define EVILENGINE_COREUNITTEST_COREBASEUNITTESTMODULE 1

class CoreUnittest;

class CoreBaseUnittestModule
{

private:

  static uint32                 s_Id;
  String                        m_ModuleName;
  int                           m_Status;
  std::vector< CoreUnittest* >  m_UnitTestList;
  uint32                        m_NumFailed;
  CoreUnittest*                 m_RunningUnitTest;

protected:

  void Add(CoreUnittest* unittest);
  CoreUnittest*                 GetNextAsyncTest();

public:

  enum TESTSTATUS
  {
    STATUS_NONE,
    STATUS_RUNNING,
    STATUS_COMPLETED,
    STATUS_FAILED,
  };

  CoreBaseUnittestModule(const String& moduleName);

  void          SetStatus(int testStatus) { m_Status = testStatus; }
  int           GetStatus() { return m_Status; }
  const String& GetName() { return m_ModuleName; }
  uint32        GetNumFailed() const { return m_NumFailed; }
  uint32        GetNumTests() const;

  virtual bool Start();
  virtual bool Cleanup();
  virtual void RunAsync(float deltaTime);
  virtual int  RunSync();
};

#endif