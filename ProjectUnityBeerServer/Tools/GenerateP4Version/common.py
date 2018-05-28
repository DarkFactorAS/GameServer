import sys, re, os

def GetBranchVersion(myName,branchFile):

  # Define all args
  buildRevision = ""
  branchName = ""

  #([\w]+) match the tweak name
  #(\s*=\s*) matches the whitespace = whitespace
  #([+-]?[\d]*[.]?[\d]+[fF;]?) match the number in int, float format with '+' or '-' before and/or ';', 'f' or 'F' after
  #(\s*) match any whitespace 
  #(//.*)?" match any comment

  # [\w]+ Match any branch names 
  # @     Match the at
  branchline   = re.compile("([\w]+)@([\w]+)*")

  # File must exist
  if not os.path.exists(branchFile):
    print "#error " + myName + " Could not find the branchfile : " + branchFile
    sys.exit(1)

  # Read the heads.txt file
  infile = open(branchFile)
  for inline in infile.readlines():

    if branchline.match(inline):
      split = branchline.split(inline) #this is a define line
      buildRevision = split[1]
      num = 0
      for n in split:
        branchName = split[num]
        num = num + 1
        
      branchName = branchName.replace(" ","")
      print "Using:" + buildRevision + "@" + branchName

  # Verify that we got the branchname
  if branchName == "":
    print "#error " + myName + " Could not fetch branchname"
    sys.exit(2)

  if buildRevision == "":
    print "#error " + myName + " Could not fetch revision"
    sys.exit(2)
    
  return branchName,buildRevision

def WriteOutputFile( templatefile, outputfile, content, branchName, buildRevision ):
  print "Python: Generating from template: " + templatefile + " --> " + outputfile + " (Branch:" + branchName + ", Build:" + buildRevision + ")"
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