#!/bin/bash
# This is the installation script for the DSIMU_Sim package
###########################################################
red=$'\e[1;31m'
grn=$'\e[1;32m'
yel=$'\e[1;33m'
blu=$'\e[1;34m'
mag=$'\e[1;35m'
cyn=$'\e[1;36m'
end=$'\e[0m'
# First, remove previous build attempts
rm -rf build
# Second, create a build directory
mkdir build
# Third, move into the build directory
cd build
# Fourth, make the makefiles with cmake
cmake ..
# Fifth, make the package
make
# Sixth, move back to the main package folder
cd ..
# Seventh, check that we are back in the right folder
pwd
printf "%s\n" "${grn}--------------------------------------------------------------${end}"
printf "%s\n" "       ${grn}INSTALLATION COMPLETE. NOW RUNNING TEST MODULE.${end}"
printf "%s\n" "${grn}--------------------------------------------------------------${end}"
# Now let's run the test code
cd test_module
# First, remove previous build attempts
rm -rf build
# Second, create a build directory
mkdir build
# Third, move into the build directory
cd build
# Fourth, make the makefiles with cmake
cmake ..
# Fifth, make the package
make
# Run the test case
./test_time_sync
# Move back to the main package folder 
cd ../..
# Report that everything is good.
printf "%s\n" "${grn}--------------------------------------------------------------${end}"
printf "%s\n" "                       ${grn}TEST COMPLETE!${end}"
printf "%s\n" "${grn}--------------------------------------------------------------${end}"
