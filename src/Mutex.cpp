#include <baseline/Baseline.h>
#include <baseline/Mutex.h>

namespace baseline {

Mutex::Mutex()
{}

Mutex::~Mutex()
{}

status_t Mutex::lock()
{
  return UNKNOWN_ERROR;
}

void Mutex::unlock()
{}


}