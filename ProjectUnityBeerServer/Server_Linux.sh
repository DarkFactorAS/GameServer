#!/bin/bash
#
# Set up the external libraries
#

if [ "$DF_EXTERNALLIBS" != "" ]; then

  export SOLUTION_TYPE="Server"
  export SOLUTION_NAME="ValyrianAdventuresServer"

  export EVILENGINEPATH="${PWD}/../EvilEngine"
  export ZLIB=$DF_EXTERNALLIBS/zlib-1.2.5
  export PNGLIB=$DF_EXTERNALLIBS/libpng-1.6.7
  export MYSQL=$DF_EXTERNALLIBS/mysql/mysql-connector-1.1.7
  export BOOST=$DF_EXTERNALLIBS/boost-1.59.0
  #export OPENSSL=$DF_EXTERNALLIBS/openssl-1.0.0.e

  p4client -u thorh changes -m1 //depot/Multiplatform/Projects/ValyrianAdventures/... > ../EvilEngine/CoreVersion/version.txt
  cat ../EvilEngine/CoreVersion/version.txt
  python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt ../EvilEngine/CoreVersion/Templates/version_template.h ../EvilEngine/CoreVersion/version.h
  python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt Tools/Deploy_Linux_Template.sh Deploy_Linux.sh keepFormat
  chmod u+x Deploy_Linux.sh

  ssh-agent bash

else

  echo You must set DF_EXTERNALLIBS to point to the externallibraries folder!

fi
