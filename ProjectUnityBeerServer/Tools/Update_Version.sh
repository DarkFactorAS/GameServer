#!/bin/bash
#
# Set up the external libraries
#
#export DF_EXTERNALLIBS="/Users/thorh/Develop/External_Libraries"

if [ "$DF_EXTERNALLIBS" != "" ]; then

  echo "DF_EXTERNALLIBS=${DF_EXTERNALLIBS}"

  export P4CONFIG=.p4config

  /Applications/p4 -u thorh changes -m1 //depot/Multiplatform/Projects/ValyrianAdventures/... > ../EvilEngine/CoreVersion/version.txt
  cat ../EvilEngine/CoreVersion/version.txt
  python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt ../EvilEngine/CoreVersion/Templates/version_template.h ../EvilEngine/CoreVersion/version.h
  python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt Client/iOS/Client/ValyrianAdventures_Template.plist Client/iOS/Client/ValyrianAdventures.plist

else

  echo You must set DF_EXTERNALLIBS to point to the externallibraries folder!

fi

