#!/bin/bash

astyle -n --options=astyle.cfg --recursive "src/*.cpp"
astyle -n --options=astyle.cfg "src/Baseline.h.in"
astyle -n --options=astyle.cfg --recursive "src/*.h"
astyle -n --options=astyle.cfg --recursive "tests/*.cpp"