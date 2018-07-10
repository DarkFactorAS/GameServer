#ifndef PROJECT_PROJECTGAMEMANAGEMENTMODULE_SERVERMODULE
#define PROJECT_PROJECTGAMEMANAGEMENTMODULE_SERVERMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

//#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"

class Playfield;
class BaseNetworkPacket;

class ProjectGameManagementServerModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_GAMEMANAGEMENT = 102,
  };

  ProjectGameManagementServerModule();
  ~ProjectGameManagementServerModule();

  virtual void                          FrameProcess(float /*deltaTime*/) {};
  virtual String                        GetModuleName() DF_OVERRIDE { return StaticStr("ProjectServerLobbyGameModule"); }
  static ProjectGameManagementServerModule*    GetModule(CoreEngine* gameEngine);

  virtual void                          OnAccountDisconnected(uint32 accountId) DF_OVERRIDE;

  virtual std::vector< LobbyGameData* > GetOpenGameList();
  virtual LobbyGameData*                CreateNewGame(uint32 accountId, const String& playerName, Playfield* playfield, uint32 robotId);

  virtual LobbyGameData*                GetLobbyGameWithGameId(uint32 gameId);
  virtual LobbyGameData*                GetLobbyGameWithAccountId(uint32 accountId);
  virtual LobbyGameData*                JoinLobbyGame(uint32 gameId, uint32 accountId, const String& playerName, uint32 robotId);
  virtual LobbyGameData*                LeaveLobbyGame(uint32 gameId, uint32 accountId);

  virtual void                          JoinQuickGame(uint32 accountId);
  virtual void                          LeaveQuickGame(uint32 accountId);

  const String&                         GetGameError(uint32 errorCode);
  uint32                                GetLastError(){ return m_LastError; }
  std::vector< LobbyGameData* >         GetLobbyGames() { return m_LobbyGames; }

protected:

  void                                  AddGameError(GameEnginePacketData::PacketError errorCode, const String& errorMessage);
  void                                  AddGameError(uint32 errorCode, const String& errorDescription);

  std::map<uint32, String>              m_ErrorMessages;
  uint32                                m_LastError;

private:

  static uint32                         s_LobbyGameId;

  std::vector< LobbyGameData* >         m_LobbyGames;
  /// AccountId, LobbyGameId
  std::map< uint32, uint32 >            m_AccountLobbyGameMap;

  uint32                                m_QuickGameAccountId;

  bool                                  RemoveLobbyGameWithId(uint32 gameId);
  bool                                  RemoveAccountFromLobbyGame(uint32 accountId, uint32 lobbyGameId);
  
  // Move this to a seperate class
  bool                                  SendPacketToClientAccount(uint32 accountId, BaseNetworkPacket* packet);
  bool                                  SendPacketToLobbyGamePlayers(LobbyGameData* lobbyGame, BaseNetworkPacket* packet);

};

#endif /// PROJECT_PROJECTGAMEMANAGEMENTMODULE_SERVERMODULE
