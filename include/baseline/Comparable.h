/*
 * Copyright (C) 2018 Baseline
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BASELINE_COMPARABLE_H_
#define BASELINE_COMPARABLE_H_

namespace baseline {

template<typename T>
class Comparable
{
public:

  /**
   * @return <0 if this is less than rhs,
   * 0 if this is equal to rhs,
   * >0 if this is less than rhs
   */
  virtual int compare( const T& rhs ) const = 0;

  bool operator== ( const T& ) const;
  bool operator< ( const T& ) const;
  bool operator> ( const T& ) const;
  bool operator<= ( const T& ) const;
  bool operator>= ( const T& ) const;
  bool operator!= ( const T& ) const;
};

#define OP_IMPL(op_)                                    \
  template<typename T>                                    \
  bool Comparable<T>::operator op_ (const T& rhs) const {   \
    return compare(rhs) op_ 0;                           \
  }

OP_IMPL( == )
OP_IMPL( < )
OP_IMPL( > )
OP_IMPL( <= )
OP_IMPL( >= )
OP_IMPL( != )

#undef OP_IMPL

} // namespace

#endif // BASELINE_COMPARABLE_H_