#ifndef EVILENGINE_COREUNITTEST_COREUNITTEST
#define EVILENGINE_COREUNITTEST_COREUNITTEST 1

class CoreUnittest
{
protected:

  uint32            m_ID;
  String            m_Testname;
  String            m_ErrorMessage;
  int               m_Status;
  unsigned long     m_StartTime;
  float             m_RunTime;
  int               m_RunCycle;

  virtual void      Failed(const String& errorMessage);
 
public:

  enum TESTSTATUS
  {
    STATUS_NONE,
    STATUS_RUNNING,
    STATUS_SUCCEEDED,
    STATUS_FAILED,
  };

  enum UnitTestType
  {
    e_SyncUnitTest,
    e_AsyncUnitTest,
  };

  CoreUnittest( String testName );

  void                  SetID( uint32 uniqueId){ m_ID = uniqueId; }
  uint32                GetID(){ return m_ID; }
  const String&         GetName(){ return m_Testname; }
  const String&         GetErrorMessage() { return m_ErrorMessage; }
  void                  SetStatus(int testStatus) { m_Status = testStatus; }
  int                   GetStatus() { return m_Status; }
  virtual UnitTestType  GetUnittestType() = 0;

  void                  SetStartTime(unsigned long startTime){ m_StartTime = startTime;}
  unsigned long         GetStartTime(){ return m_StartTime; }

  virtual bool          Execute() = 0;
  virtual bool          Cleanup();
  virtual void          Succeeded();

  float                 GetRunTime() const { return m_RunTime; }
  void                  AddRunTime(float deltaTime) { m_RunTime += deltaTime; }
  virtual float         GetMaxRunTime() const{ return 10.0f; }

  void                  AddRunCycle(){ m_RunCycle++;}
  int                   GetRunCycles() const{ return m_RunCycle; }
};

#endif