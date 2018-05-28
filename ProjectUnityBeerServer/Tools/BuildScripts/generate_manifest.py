#!/usr/bin/python
#*****************************************************************************
# Filename : generate_manifest.py
# Author   : Thor Richard Hansen
#*****************************************************************************
import sys, re, os
import common

def GetUpdatedTemplate(myName,srcFile):

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
if(len(sys.argv) != 3):
  print "#error: Wrong arguments. " + sys.argv[0] + " <srcFile> <destFile> <projectname> <version> <build>"
  sys.exit(0)

# For easier access to arguments
myName       = sys.argv[0]   # Name of this file
srcFile      = sys.argv[1]   # Name of the sourcefile ( the one to generate from )
dstFile      = sys.argv[2]   # Name of the destination file ( the one to actually build )

# Read the output and diff it to the output from the template to see if we have to rewrite it.
#oldFile = GetUpdatedTemplate( myName, outputFile, branchName, buildRevision )
#newFile = GetUpdatedTemplate( myName, templateFile, branchName, buildRevision )

# If they are different, recreate the file
#if oldFile != newFile:
#  common.WriteOutputFile( templateFile, outputFile, newFile, branchName, buildRevision)
  
