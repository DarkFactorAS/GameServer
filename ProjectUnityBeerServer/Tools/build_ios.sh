/Applications/p4 -u thorh changes -m1 //depot/Multiplatform/Projects/ValyrianAdventures/... > ../EvilEngine/CoreVersion/version.txt
cat ../EvilEngine/CoreVersion/version.txt
python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt ../EvilEngine/CoreVersion/Templates/version_template.h ../EvilEngine/CoreVersion/version.h
python ../EvilEngine/CoreVersion/PerforceScripts/convert_p4version_to_version.py ../EvilEngine/CoreVersion/version.txt Client/iOS/Client/ValyrianAdventures_Template.plist Client/iOS/Client/ValyrianAdventures.plist

xcodebuild -scheme ValyrianAdventures.iOS -project Client/iOS/ValyrianAdventures.xcodeproj build -destination generic/platform=iOS clean
xcodebuild -scheme ValyrianAdventures.iOS -project Client/iOS/ValyrianAdventures.xcodeproj build -destination generic/platform=iOS build
