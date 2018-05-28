
#include "Precompile.h"
#include "SSLNetworkClient.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

// Used to fetch content from https connections
//#include <boost/bind.hpp>
//#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp>

#if defined USE_OPENSSL
  #include <openssl/ssl.h>
  #include <openssl/err.h>
#endif

#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"

SSLNetworkClient::SSLNetworkClient() :
  NetworkClient("SSLClient")
{
}

//bool verify_certificate_cb(bool preverified, boost::asio::ssl::verify_context& ctx)
//{
  ////std::cout << "Function : " << __func__ << " ----------------- Line : " << __LINE__ << std::endl;
  //int8_t subject_name[256];
  //X509_STORE_CTX *cts = ctx.native_handle();
  ////int32_t length = 0;
  //X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
  ////std::cout << "CTX ERROR : " << cts->error << std::endl;

  ////int32_t depth = X509_STORE_CTX_get_error_depth(cts);
  ////std::cout << "CTX DEPTH : " << depth << std::endl;

  //switch (cts->error)
  //{
  //case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
  //  //Debug(PRIORITY_ERROR, "X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT");
  //  break;
  //case X509_V_ERR_CERT_NOT_YET_VALID:
  //case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
  //  //Debug(PRIORITY_ERROR, "Certificate not yet valid!!");
  //  break;
  //case X509_V_ERR_CERT_HAS_EXPIRED:
  //case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
  //  //Debug(PRIORITY_ERROR, "Certificate expired..");
  //  break;
  //case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
  //  //Debug(PRIORITY_WARN, "Self signed certificate in chain!!!\n");
  //  preverified = true;
  //  break;
  //default:
  //  break;
  //}
  //const int32_t name_length = 256;
  //X509_NAME_oneline(X509_get_subject_name(cert), reinterpret_cast<char*>(subject_name), name_length);
  ////Debug(PRIORITY_INFO, "Verifying %s", subject_name);
  ////Debug(PRIORITY_INFO, "Verification status : %d", preverified);

  //return preverified;
//  return true;
//}

/************************************************************************************************
* VerifyKeyList:
* Verify an encrypted SHA-1 data block using a list of keys/salt and a public key. This is 
* used to verify that a given datablock is genuine and issued by the sender using their public
* key and ever so changing list of keys. ( Current use : Game Center verification on iOS ).
*
* @param  (unsigned char*)  publicKey                 - Datablock with the publickey
* @param  (long)            publicKeyLength           - Length of the publickey datablock
* @param  (unsigned char*)  encryptedDataBlock        - The encrypted data we want to verify
* @param  (unsigned int)    encryptedDataBlockLength  - Length of the encrypted data
* @param  (StringList)      keyList                   - List of keys to use when verifying
* @return (int)                                       - Return 1 for success, 0 for fail
*
* @author Thor Richard Hansen
*************************************************************************************************/
int SSLNetworkClient::VerifyKeyList( 
  const unsigned char* publicKey, long publicKeyLength, 
  unsigned char* encryptedDataBlock, unsigned int encryptedDataBlockLength, 
  const StringList& keyList )
{
#if defined USE_OPENSSL

  // Extract the public key from the data block
  X509* cert = d2i_X509 ( NULL, (const unsigned char**) &publicKey, publicKeyLength );
  EVP_PKEY* rawPublicKey = X509_get_pubkey (cert);

  // Initalize the decryption method
  OpenSSL_add_all_digests();
  const EVP_MD* decryptType = EVP_get_digestbyname("SHA1");

  EVP_MD_CTX c;
  EVP_MD_CTX_init(&c);
  EVP_VerifyInit(&c, decryptType);

  // Encrypt the list of keys
  for ( StringList::const_iterator itKey = keyList.begin(); itKey != keyList.end(); ++itKey )
  {
    const String& msg = *itKey;
    EVP_VerifyUpdate(&c, msg.c_str(), (unsigned int) msg.length() );
  }

  // Run the check ( 1 == success, 0 == fail, other == unknown )
  int ret = EVP_VerifyFinal(&c, encryptedDataBlock, encryptedDataBlockLength, rawPublicKey );
  return ret;

#else

  VOLATILE_ARG( publicKey );
  VOLATILE_ARG( publicKeyLength );
  VOLATILE_ARG( encryptedDataBlock );
  VOLATILE_ARG( encryptedDataBlockLength );
  VOLATILE_ARG( keyList );

  LogFatal( "SSLNetworkClient", "OpenSSL is disabled in this build!" );

  return -1;

#endif
}

bool SSLNetworkClient::SendRequest( const String& /*url*/ )
{
  ////boost::asio::ssl::context context_(boost::asio::ssl::context::sslv23);
  ////ctx.load_verify_file("ca.pem");

  //boost::asio::ssl::context_base::method SSL_version = static_cast<boost::asio::ssl::context_base::method>(boost::asio::ssl::context::sslv23);

  ////// load certificate files
  //boost::shared_ptr<boost::asio::ssl::context> context_ = boost::shared_ptr<boost::asio::ssl::context>(
  //  new boost::asio::ssl::context(SSL_version)); // parasoft-suppress BD-RES-LEAKS "The memory is allocated via boost::shared_ptr, hence it'll be deallocated automatically"

  ////p_ctx = boost::static_pointer_cast<void>(context_);

  //context_->set_options(boost::asio::ssl::context::default_workarounds);

  //context_->use_certificate_chain_file(cert_chain_file);
  //context_->use_certificate_file(cert_file, boost::asio::ssl::context::pem);
  //context_->use_private_key_file(cert_file, boost::asio::ssl::context::pem);

  //context_->set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert );
  //context_->set_verify_callback(boost::bind(&verify_certificate_cb, _1, _2));
  return true;
}
