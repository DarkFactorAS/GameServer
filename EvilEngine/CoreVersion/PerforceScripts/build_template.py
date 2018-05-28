#!/usr/bin/python
#*****************************************************************************
# Filename : create_config
# Author   : Thor Richard Hansen
#*****************************************************************************
import sys, re, os
import common

def GetUpdatedTemplate(myName,templateFile,branchName,branchRevision):

  outfile = ""
     	
  # Go through the template file and replace the $ fields
  if os.path.exists(templateFile):
    file = open(templateFile)
    for line in file.readlines():
      line = line.replace("$BranchName",branchName)
      line = line.replace("$BranchRevision",branchRevision)
#      line = line.replace("\n","")
      outfile = outfile + line  
	
  return outfile
	
# Must specify all arguments    
if(len(sys.argv) != 4):
  print "#error: Wrong arguments. " + sys.argv[0] + " <branchfile> <templatefile> <outputfile>"
  for a in sys.argv:
    print "- Argument:" + a

  sys.exit(0)

# For easier access to arguments
myName       = sys.argv[0]   # Name of this file
branchFile   = sys.argv[1]   # The file hg generates ( heads.txt )
templateFile = sys.argv[2]   # The template we want to generate from. F.ex config_win32_template.xml
outputFile   = sys.argv[3]   # The end result. F.ex : config_win32.xml

# Fetch the revision
branchName, buildRevision = common.GetBranchVersion( myName, branchFile )

# Read the output and diff it to the output from the template to see if we have to rewrite it.
oldFile = GetUpdatedTemplate( myName, outputFile, branchName, buildRevision )
newFile = GetUpdatedTemplate( myName, templateFile, branchName, buildRevision )

if ( newFile == "" ):
  print "Could not open template file : " + templateFile
  sys.exit(0)

# If they are different, recreate the file
if oldFile != newFile:
  common.WriteOutputFile( templateFile, outputFile, newFile, branchName, buildRevision)
else:
  print "No changes to the file " + outputFile
  
