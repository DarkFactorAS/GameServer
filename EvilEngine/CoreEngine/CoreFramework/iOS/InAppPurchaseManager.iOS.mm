
//
//  InAppPurchaseManager.mm
//
//  Created by Thor Richard Hanssen on 26.08.14.
//  Copyright (c) 2014 Thor Hansen. All rights reserved.
//
//#include "PreCompile.h"

//#include "Core/Utils/MacTypesWorkaround.iOS.h" //< Required before any iOS system headers.

#import "InAppPurchaseManager.iOS.h"
#import <StoreKit/StoreKit.h>

//#include "CoreLibraries/Core/Signals/Signal.h"

// TODO : Change NSLog to use Funcom logging
//      : Give proper feedback on failed transcations
//      : Release the object properly
//      : Proper call to the server to send the receipt to our itemshop backend

/************************************************************************************************
 * @interface InAppPurchaseManager
 * Define the interface for the InAppPurchaseManager
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
  NSString*            m_ProductId;
  SKProductsRequest*   m_ProductsRequest;
}
- (void) LoadStore;
- (void) PurchaseItem:(NSString*)itemId;
- (void) RequestProductData: (const char*)itemId;
@end

/************************************************************************************************
 * Client::InitializeLoginBrowserView:
 * C++ bridge between itemshop and iOS implementation
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
InAppPurchaseManager* g_InAppPurchaseManager = nullptr;

namespace Client
{
  Signal2< void, const char*, const char* >  SignalItemShopPurchase;

  void InitializeItemShop()
  {
    if (g_InAppPurchaseManager != nullptr)
    {
      return;
    }
    
    g_InAppPurchaseManager = [[ InAppPurchaseManager alloc ] init ] ;
    [ g_InAppPurchaseManager LoadStore ];
  }
  
  void ShutdownItemShop()
  {
    if (g_InAppPurchaseManager == nullptr)
    {
      return;
    }
    
    //InAppPurchaseManager* const viewController = g_InAppPurchaseManager;
    g_InAppPurchaseManager = nullptr;
    
    // TODO : Fix this leak
    //[viewController willMoveToParentViewController:nil];
    //[viewController.view removeFromSuperview];
    //[viewController removeFromParentViewController];
    //[viewController release];
  }
  
  void ItemShopPurchaseItem( const char* itemShopId )
  {
    InitializeItemShop();
    [ g_InAppPurchaseManager RequestProductData:itemShopId ];
    //NSString *itemId = [NSString stringWithCString:itemShopId ];
    //[ g_InAppPurchaseManager PurchaseItem:itemId ];
  }
}

@implementation InAppPurchaseManager

/************************************************************************************************
 * LoadStore:
 * This function will set up the delegate function to handle the AppStore transaction and fetches
 * the products from the AppStore.
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) LoadStore
{
  m_ProductId       = nullptr;
  m_ProductsRequest = nullptr;
  
  // restarts any purchases if they were interrupted last time the app was open
  [ [SKPaymentQueue defaultQueue] addTransactionObserver:self ];
}

/************************************************************************************************
 * CanMakePurchases:
 *
 * Returns true if this device can make purchases from AppStore. The user can turn off in-app
 * purchases on his/her iPad/iPhone in the settings.
 *
 * @return  (BOOL)     - Return true if the user can purchase items
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (BOOL) CanMakePurchases
{
  return [SKPaymentQueue canMakePayments];
}

/************************************************************************************************
 * RequestProUpgradeProductData:
 *
 * Send a request to AppStore to get the product handle for each of the items we want to purchase.
 * The response from the request is handled in the delegated method productsRequest
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) RequestProductData:(const char*)itemId
{
  if ( itemId != nullptr )
  {
    NSSet *productIdentifiers = [NSSet setWithObjects:[NSString stringWithCString:itemId],nil ];
    m_ProductsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    m_ProductsRequest.delegate = self;
    [m_ProductsRequest start];
  }
}

/************************************************************************************************
 * RequestProUpgradeProductData:
 *
 * Send a request to AppStore to get the product handle for each of the items we want to purchase.
 * The response from the request is handled in the delegated method productsRequest
 *
 * @return  (BOOL)     - Return true if the user can purchase items
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
  for (SKProduct *validProduct in response.products)
  {
    NSLog(@"--------");
    NSLog(@"Product title: %@" ,        validProduct.localizedTitle);
    NSLog(@"Product description: %@" ,  validProduct.localizedDescription);
    NSLog(@"Product price: %@" ,        validProduct.price);
    NSLog(@"Product id: %@" ,           validProduct.productIdentifier);
    
    // It should only be 1 reply, regardless, buy all of them
    [ self PurchaseItem:validProduct.productIdentifier ];
  }
  
  for (NSString *invalidProductId in response.invalidProductIdentifiers)
  {
    NSLog(@"Invalid product id: %@" , invalidProductId);
  }
}

/************************************************************************************************
 * PurchaseItem:
 *
 * Send a request to purchase an item from AppStore
 *
 * @return  (BOOL)     - Return true if the user can purchase items
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void) PurchaseItem:(NSString*)itemId
{
  if ( itemId != nullptr )
  {
    m_ProductId = itemId;

#ifdef IN_APP_PURCHASE
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:m_ProductId];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
#endif
  }
}

/************************************************************************************************
 * RecordTransaction:
 * Callback for when I get the transaction receipt for the itemshop item.
 *
 * @param   (SKPaymentTransaction*)    transaction - The successful transaction
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)recordTransaction:(SKPaymentTransaction *)transaction itemId:(NSString*)itemId
{
  const char* cItemId   = [ itemId UTF8String ];

  NSURL*  receiptURL    = [ [ NSBundle mainBundle ] appStoreReceiptURL ];
  NSData* receipt       = [ NSData dataWithContentsOfURL:receiptURL ];
  NSString* receptStr   = [ receipt base64EncodedStringWithOptions:0 ];
  const char* cReceipt  = [ receptStr UTF8String ];
  
  NSLog(@"-[ Transactipn Receipt ]-------");
  NSLog(@"Product id: %@" ,           itemId);
  NSLog(@"Product desc: %@" ,         transaction.payment.debugDescription);
  NSLog(@"Product receiptURL: %@" ,   receiptURL.path );
  NSLog(@"Product receipt: %@" ,      receptStr);
  
  Client::SignalItemShopPurchase( cItemId , cReceipt );
}

/************************************************************************************************
 * finishTransaction:
 * Callback for when the transaction has completed. This will remove the remove the transaction
 * from the SKPaymentQueue so it is not triggered again.
 *
 * @param   (SKPaymentTransaction*)    transaction  - The transaction that finished
 * @param   (BOOL)                     wasSucessful - True if the transaction was successful
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
  // remove the transaction from the payment queue.
  [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
  
  if (wasSuccessful)
  {
    NSLog(@"finishTransaction [SUCCEEDED] : %@", transaction.description );
  }
  else
  {
    NSLog(@"finishTransaction [FAILED] : %@", transaction.description );
  }
}

/************************************************************************************************
 * completeTransaction:
 * Callback for when the transaction has successfully completed.
 *
 * @param   (SKPaymentTransaction*)    transaction  - The transaction that finished
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
  NSLog(@"InAppPurchaseManager.completeTransaction [Succeeded] %@", transaction.description );
  
  [self recordTransaction:transaction itemId:transaction.payment.productIdentifier];
  [self finishTransaction:transaction wasSuccessful:YES];
}

/************************************************************************************************
 * completeTransaction:
 * Callback for when the transaction was restored from a previous session and has successfully completed.
 *
 * @param   (SKPaymentTransaction*)    transaction  - The transaction that finished
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
  NSLog(@"InAppPurchaseManager.restoreTransaction [Succeeded] : %@", transaction.description );
  
  [self recordTransaction:transaction.originalTransaction itemId:transaction.originalTransaction.payment.productIdentifier];
  [self finishTransaction:transaction wasSuccessful:YES];
}

/************************************************************************************************
 * failedTransaction:
 * Callback for when the transaction was failed.
 *
 * @param   (SKPaymentTransaction*)    transaction  - The transaction that finished
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
  if ( transaction.error.code != SKErrorPaymentCancelled)
  {
    NSLog(@"InAppPurchaseManager.failedTransaction [error] : %ld", (long)transaction.error.code );
    [self finishTransaction:transaction wasSuccessful:NO];
  }
  else
  {
    // this is fine, the user just cancelled, so don’t notify
    NSLog(@"InAppPurchaseManager.failedTransaction [PaymentCancelled] : %@", transaction.description );
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
  }
}

/************************************************************************************************
 * failedTransaction:
 * Callback for when a transaction is updating the state.
 *
 * @param   (SKPaymentQueue*)    queue        - The transaction queue
 * @param   (NSArray*)           transactions - List of items in this transaction
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
  for (SKPaymentTransaction *transaction in transactions)
  {
    switch (transaction.transactionState)
    {
      case SKPaymentTransactionStatePurchased:
        [self completeTransaction:transaction];
        break;
      case SKPaymentTransactionStateFailed:
        [self failedTransaction:transaction];
        break;
      case SKPaymentTransactionStateRestored:
        [self restoreTransaction:transaction];
        break;
      default:
        break;
    }
  }
}


@end

