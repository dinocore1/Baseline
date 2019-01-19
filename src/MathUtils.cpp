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

#include <baseline/Baseline.h>
#include <baseline/MathUtils.h>
#include <limits>
#include <cmath>

namespace baseline {


///////////// FirstMoment ///////////////////

template<typename T>
FirstMoment<T>::FirstMoment()
{
  clear();
}

template<typename T>
void FirstMoment<T>::clear()
{
  mN = 0;
  m1 = std::numeric_limits<T>::quiet_NaN();
  mDev = std::numeric_limits<T>::quiet_NaN();
  mNDev = std::numeric_limits<T>::quiet_NaN();

}

template<typename T>
void FirstMoment<T>::increment( T v )
{

  if( mN == 0 ) {
    m1 = 0.0;
  }
  mN++;
  mDev = v - m1;
  mNDev = mDev / mN;
  m1 += mNDev;

}

template<typename T>
int FirstMoment<T>::getN() const
{
  return mN;
}

template<typename T>
T FirstMoment<T>::getResult() const
{
  return m1;
}

template
class FirstMoment<float>;

template
class FirstMoment<double>;


///////////// SecondMoment ///////////////////

template<typename T>
SecondMoment<T>::SecondMoment()
{
  clear();
}

template<typename T>
void SecondMoment<T>::clear()
{
  FirstMoment<T>::clear();
  m2 = std::numeric_limits<T>::quiet_NaN();
}

template<typename T>
int SecondMoment<T>::getN() const
{
  return FirstMoment<T>::mN;
}

template<typename T>
void SecondMoment<T>::increment( T v )
{
  if( FirstMoment<T>::mN < 1 ) {
    FirstMoment<T>::m1 = m2 = 0.0;
  }

  FirstMoment<T>::increment( v );
  m2 += ( FirstMoment<T>::mN - 1 ) * FirstMoment<T>::mDev * FirstMoment<T>::mNDev;
}

template<typename T>
T SecondMoment<T>::getResult() const
{
  return m2;
}

template
class SecondMoment<float>;

template
class SecondMoment<double>;


//////////////// Variance /////////////////

template<typename T>
Variance<T>::Variance( bool biasCorrected )
  : mIsBiasCorrected( biasCorrected )
{
}

template<typename T>
bool Variance<T>::isBiasCorrected() const
{
  return mIsBiasCorrected;
}

template<typename T>
void Variance<T>::increment( T v )
{
  moment.increment( v );
}

template<typename T>
void Variance<T>::clear()
{
  moment.clear();
}

template<typename T>
int Variance<T>::getN() const
{
  return moment.getN();
}

template<typename T>
T Variance<T>::getResult() const
{
  const int n = moment.getN();
  if( n == 0 ) {
    return std::numeric_limits<T>::quiet_NaN();
  } else if( n == 1 ) {
    return 0.0;
  } else {
    T m2 = moment.getResult();
    if( mIsBiasCorrected ) {
      return m2 / ( n - 1 );
    } else {
      return m2 / ( n );
    }
  }
}


template
class Variance<float>;

template
class Variance<double>;

//////////////// StandardDiviation //////////////////

template<typename T>
StandardDiviation<T>::StandardDiviation( bool biasCorrected )
  : Variance<T>( biasCorrected )
{
}

template<typename T>
T StandardDiviation<T>::getResult() const
{
  T value = Variance<T>::getResult();
  return std::sqrt( value );
}


template
class StandardDiviation<float>;

template
class StandardDiviation<double>;


/////////// StorelessStats ///////////////////

template<typename T>
StorelessStats<T>::StorelessStats( bool biasCorrected )
  : mStdDiv( biasCorrected )
{
}

template<typename T>
bool StorelessStats<T>::isBiasCorrected() const
{
  return mStdDiv.isBiasCorrected();
}

template<typename T>
void StorelessStats<T>::increment( T v )
{
  mMean.increment( v );
  mStdDiv.increment( v );
}

template<typename T>
void StorelessStats<T>::clear()
{
  mMean.clear();
  mStdDiv.clear();
}

template<typename T>
int StorelessStats<T>::getN() const
{
  return mMean.getN();
}

template<typename T>
T StorelessStats<T>::getMean() const
{
  return mMean.getResult();
}

template<typename T>
T StorelessStats<T>::getStdDiv() const
{
  return mStdDiv.getResult();
}

template
class StorelessStats<float>;

template
class StorelessStats<double>;

template<typename T>
LinearFunction<T> LinearFunction<T>::createFromPoints( const Vec2<T>& a, const Vec2<T>& b )
{
  LinearFunction<T> retval;

  retval.mM = ( a[1] - b[1] ) / ( a[0] - b[0] );
  retval.mB = a[1] - a[0] * retval.mM;

  return retval;
}

template<typename T>
T LinearFunction<T>::operator()( const T& input )
{
  return input * mM + mB;
}

template<typename T>
const T& LinearFunction<T>::slope() const
{
  return mM;
}

template<typename T>
const T& LinearFunction<T>::offset() const
{
  return mB;
}

template
class LinearFunction<float>;

template
class LinearFunction<double>;

template<typename T>
const T& clamp( const T& v, const T& lo, const T& hi )
{
  if( v < lo ) {
    return lo;
  } else if( v > hi ) {
    return hi;
  } else {
    return v;
  }
}

template
const float& clamp( const float&, const float&, const float& );

template
const double& clamp( const double&, const double&, const double& );

template
const int& clamp( const int&, const int&, const int& );

template
const uint16_t& clamp( const uint16_t&, const uint16_t&, const uint16_t& );

} // namespace