@echo off

if exist p4config (
  set P4CONFIG=p4config
  p4 changes -m1 //depot/Multiplatform/Games/ValyrianAdventures/... > last_perforce_version.txt
) else (
  echo You must create the p4config file. Check p4config_template.txt as an example
  pause
)