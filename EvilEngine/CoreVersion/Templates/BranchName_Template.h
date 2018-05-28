#ifndef BranchName_H
#define BranchName_H

//! The version string used in the client
//! If you update the versionnumber, you should also update the 
//! number in version.txt file (for the installer)
#define CC_VERSION 1,0,0
#define VER_NAME(ver) VER_NAME2(ver)
#define VER_NAME2(ver) #ver
static const QString    s_BranchName  = "$BranchName";

#endif // BranchName_H
