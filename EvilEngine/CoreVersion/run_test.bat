
p4 changes -m1 > version.txt
echo "P4 gave us:"
echo version.txt

python.exe PerforceScripts\convert_p4version_to_version.py version.txt Templates\version_template.h

pause