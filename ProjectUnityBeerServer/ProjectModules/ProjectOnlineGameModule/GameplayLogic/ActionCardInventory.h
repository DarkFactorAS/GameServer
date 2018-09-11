#ifndef PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDINVENTORY
#define PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDINVENTORY 1

class ActionCard;

class ActionCardInventory
{
public:

  ActionCardInventory();

  void                        AddItems( std::vector< ActionCard* > itemList );

private:

  std::vector<ActionCard*>    m_Inventory;
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDINVENTORY
