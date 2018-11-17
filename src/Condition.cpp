#include <baseline/Baseline.h>
#include <baseline/Condition.h>

namespace baseline {

status_t Condition::wait( Mutex& mutex )
{
  return UNKNOWN_ERROR;
}

void Condition::signalOne()
{

}

void Condition::signalAll()
{

}

}