#! /bin/bash

echo "Starte Plotting Makro fuer die Bacherlor Arbeit"


DIR="GammaCalo-All_503_normal_and_extra" # the directery where I am currently to make things a bit more
               # felxible in the code


time root -l -b -q Plotting.C++\(\"$DIR\"\)
