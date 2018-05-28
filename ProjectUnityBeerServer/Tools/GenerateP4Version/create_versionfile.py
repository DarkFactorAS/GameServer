#!/usr/bin/python
#*****************************************************************************
# Filename : create_config
# Author   : Thor Richard Hansen
#*****************************************************************************
import sys, re, os
import common


# Must specify all arguments    
if(len(sys.argv) != 3):
  print "#error: Wrong arguments. " + sys.argv[0] + " <branchfile> <outputfile>"
  sys.exit(0)

# For easier access to arguments
myName       = sys.argv[0]   # Name if this file
p4File       = sys.argv[1]   # The file perforce generates ( PerforceScripts/version.txt )
outputFile   = sys.argv[2]   # The end result. ( version.txt )

# Fetch the revision
branchName, buildRevision = common.GetBranchVersion( myName, p4File )

# What should the file contain   	
newFileContent = branchName + "@" + buildRevision

# Write the branchname if it has changed
if common.IsFileEqual(outputFile,newFileContent) != 0:
  common.WriteOutputFile( hgFile, outputFile, newFileContent, branchName, buildRevision)
  