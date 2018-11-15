#!/bin/bash

astyle -n --options=astyle.cfg --recursive "src/*.cpp"
astyle -n --options=astyle.cfg --recursive "include/baseline/*.h"
astyle -n --options=astyle.cfg --recursive "src/*.h"
astyle -n --options=astyle.cfg --recursive "tests/*.cpp"