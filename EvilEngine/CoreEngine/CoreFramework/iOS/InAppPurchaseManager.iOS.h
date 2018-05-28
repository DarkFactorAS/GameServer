//
//  InAppPurchaseManager.h
//
//  Created by Thor Richard Hanssen on 26.08.14.
//  Copyright (c) 2014 Thor Hansen. All rights reserved.
//

#include "EvilEngine/CoreLib/Signal/Signal.h"

namespace Client
{
  extern Signal2< void, const char*, const char* >  SignalItemShopPurchase;
  
  extern void InitializeItemShop();
  extern void ShutdownItemShop();
  extern void ItemShopPurchaseItem( const char* itemShopId );
  
} // namespace Client
