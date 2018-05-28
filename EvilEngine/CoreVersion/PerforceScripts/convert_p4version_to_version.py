#!/usr/bin/python
#*****************************************************************************
# Filename    : convert_p4version_to_version.py
# Author      : Thor Richard Hansen
# Description : This python script takes the raw dump from 'p4 changes -m1' 
#               and formats it nicely into a pretty format
#*****************************************************************************
import sys, re, os
import common
import datetime

# Must specify all arguments    
if(len(sys.argv) < 4):
  print "#error: Wrong arguments. " + sys.argv[0] + " <p4version file> <templatefile> <outputfile>"
  sys.exit(0)

# For easier access to arguments
myName       = sys.argv[0]   # Name if this file
p4File       = sys.argv[1]   # The file perforce generates ( EvilEngine/CoreVersion/version.txt )
templateFile = sys.argv[2]   # The template file to fill in
outputFile   = sys.argv[3]   # The done output file

# Fetch the revision and date from the perforce file
lastCommitDate, lastCommitRevision = common.GetPerforceVersion( myName, p4File )

if ( lastCommitDate != "" and lastCommitRevision != "" ):

  if( len(sys.argv ) == 4 ):
    dt = datetime.datetime.strptime(lastCommitDate, "%Y/%m/%d")
    lastCommitDate = dt.strftime('%d.%m.%Y')

  # What should the file contain
  newFileContent = common.GetFromTemplate( myName, templateFile, lastCommitDate, lastCommitRevision )

  # Write the branchname if it has changed
  if common.IsFileEqual(outputFile,newFileContent) != 0:
    print "Python: Generating from template: " + templateFile + " --> " + outputFile + " (Date:" + lastCommitDate + ", Revision:" + lastCommitRevision + ")"
    common.WriteOutputFile( outputFile, newFileContent )
  else:
    print "Python: Re-using from template: " + templateFile + " --> " + outputFile + " (Date:" + lastCommitDate + ", Revision:" + lastCommitRevision + ")" 
else:
  print "Python: Could not fetch data from version file .. ignoring"
    