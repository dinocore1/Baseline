#ifndef ATOMIC_H_
#define ATOMIC_H_

namespace baseline {

/**
 * atomic fetch and increment 1
 * @return previous value
 */
int32_t atomic_inc( volatile int32_t* ptr );

/**
 * atomic fetch and decrement 1
 * @return previous value
 */
int32_t atomic_dec( volatile int32_t* ptr );

int32_t atomic_add( int32_t value, volatile int32_t* ptr );

int32_t atomic_and( int32_t value, volatile int32_t* ptr );

int32_t atomic_or( int32_t value, volatile int32_t* ptr );

int32_t atomic_cmpxchg( int32_t oldvalue, int32_t newvalue, volatile int32_t* ptr );

}


#endif // ATOMIC_H_