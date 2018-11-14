#!/bin/bash

astyle -n --options=astyle.cfg --recursive "src/*.cpp" "src/*.h" "include/baseline/*.h"