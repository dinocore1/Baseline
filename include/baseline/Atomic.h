#ifndef ATOMIC_H_
#define ATOMIC_H_

namespace baseline {

/**
 * atomic fetch and increment 1
 * @return previous value
 */
int32_t atomic_inc( int32_t* ptr );

/**
 * atomic fetch and decrement 1
 * @return previous value
 */
int32_t atomic_dec( int32_t* ptr );


}


#endif // ATOMIC_H_