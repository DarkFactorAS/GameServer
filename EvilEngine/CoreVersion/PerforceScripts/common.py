import sys, re, os

#################################################################################################
#
# GetPerforceVersion:
# Reads the output from 'p4 changes -m1' from a file to get the date 
# and version of the last commit to a branch.
#
# @param  (String) myName     - The name of the file that called this function
# @param  (String) textFile   - The file that contains the data
# @return (String)            - The date of the last commit
# @return (String)            - The version of the last commit
#
#################################################################################################
def GetPerforceVersion(myName,textFile):
  
  # Define all args
  lastCommitDate = ""
  lastCommitVersion = ""

  #([\w]+) match the tweak name
  #(\s*=\s*) matches the whitespace = whitespace
  #([+-]?[\d]*[.]?[\d]+[fF;]?) match the number in int, float format with '+' or '-' before and/or ';', 'f' or 'F' after
  #(\s*) match any whitespace 
  #(//.*)?" match any comment

  # [\s]+ Split the words between the spaces
  regexpLine = re.compile("\s*")

  # File must exist
  if not os.path.exists(textFile):
    print "#error " + myName + " Could not find the perforce textfile : " + textFile
    sys.exit(1)

  # Read the heads.txt file
  infile = open(textFile)
  for inline in infile.readlines():

    if regexpLine.match(inline):

      split             = regexpLine.split(inline) #this is a define line
      lastCommitDate    = split[3]
      lastCommitVersion = split[1]

      #print "Using:" + lastCommitVersion + " @ " + lastCommitDate
    
  return lastCommitDate,lastCommitVersion

#
# Read the template file and replace the variables with proper values
#
def GetFromTemplate( myName, templateFile, lastCommitDate, lastCommitRevision ):

  # File must exist
  if not os.path.exists(templateFile):
    print "#error " + myName + " Could not find the perforce textfile : " + templateFile
    sys.exit(1)

  fullLine = ""
  file = open(templateFile)
  for line in file.readlines():
    line = line.replace("$BuildDate",lastCommitDate)
    line = line.replace("$BuildRevision",lastCommitRevision)
    fullLine = fullLine + line

  return fullLine 

def WriteOutputFile( outputfile, content ):
  outfile = open(outputfile,'w')
  outfile.write( content )

# Go through the old outputfile and read it
def IsFileEqual( fileToCheck, content ):
  oldFileContent = ""
  if os.path.exists(fileToCheck):
    file = open(fileToCheck)
    for line in file.readlines():
      oldFileContent = oldFileContent + line  

  if oldFileContent == content:
    return 0
  return 1