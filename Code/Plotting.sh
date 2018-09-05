#! /bin/bash

echo "start Iter Temp Fit"


DIR=${PWD##*/} # the directery where I am currently to make things a bit more
               # felxible in the code


# time root -l -b -q IterTempCreation.C++\(\"$DIR\",$4\)
