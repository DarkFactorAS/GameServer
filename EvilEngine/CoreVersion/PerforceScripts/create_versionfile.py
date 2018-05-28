#!/usr/bin/python
#*****************************************************************************
# Filename    : create_config
# Author      : Thor Richard Hansen
# Description : This python script takes the raw dump from 'p4 changes -m1' 
#               and formats it nicely into a file with version and date
#               ( fex : VERSION_NUMBER = 1292 and VERSION_DATE = 24.12.2015 )
#*****************************************************************************
import sys, re, os
import common

# Must specify all arguments    
if(len(sys.argv) != 3):
  print "#error: Wrong arguments. " + sys.argv[0] + " <perforce versionfile> <outputfile>"
  sys.exit(0)

# For easier access to arguments
myName       = sys.argv[0]   # Name if this file
p4File       = sys.argv[1]   # The file perforce generates ( EvilEngine/CoreVersion/version.txt )
outputFile   = sys.argv[2]   # The end result. ( version.h )

# Fetch the revision and date from the perforce file
branchDate, buildRevision = common.GetPerforceVersion( myName, p4File )

# What should the file contain   	
newFileContent = branchVersion + "@" + buildDate

# Write the branchname if it has changed
#if common.IsFileEqual(outputFile,newFileContent) != 0:
print "Python: Generating from template: " + templatefile + " --> " + outputfile + " (Branch:" + branchName + ", Build:" + buildRevision + ")"
#common.WriteOutputFile( hgFile, outputFile, newFileContent, branchName, buildRevision)
  