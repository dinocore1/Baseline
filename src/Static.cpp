// All static variables go here, to control initialization and
// destruction order in the library.

#include "Static.h"

//#include <utils/BufferedTextOutput.h>
#include <baseline/Log.h>

namespace baseline {

class LibUtilsFirstStatics
{
public:
    LibUtilsFirstStatics()
    {
        initialize_string8();
        initialize_string16();
    }
    
    ~LibUtilsFirstStatics()
    {
        terminate_string16();
        terminate_string8();
    }
};

static LibUtilsFirstStatics gFirstStatics;
int gDarwinCantLoadAllObjects = 1;



}