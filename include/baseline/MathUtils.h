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

#ifndef BASELINE_MATHUTILS_H_
#define BASELINE_MATHUTILS_H_

namespace baseline {

template<typename T>
class FirstMoment
{
public:
  FirstMoment();
  void increment( T v );
  void clear();
  int getN() const;
  T getResult() const;

protected:
  T m1;
  T mDev;
  T mNDev;
  int mN;

};

template<typename T>
class SecondMoment : public FirstMoment<T>
{
public:
  SecondMoment();
  void increment( T v );
  void clear();
  int getN() const;
  T getResult() const;

protected:
  T m2;
};

/**
 *
 * Computes the variance of the available values. By default,
 * the unbiased "sample variance" definitional formula is used:
 *
 * variance = sum((x_i - mean)^2) / (n - 1)
 *
 * The definitional formula does not have good numerical properties,
 * so this implementation computes the variance using updating formulas based on West's algorithm, as described in
 * http://doi.acm.org/10.1145/359146.359152 Chan, T. F. and
 * J. G. Lewis 1979, Communications of the ACM, vol. 22 no. 9, pp. 526-531.
 *
 * The "population variance":
 *
 * variance = ( sum((x_i - mean)^2) / n )
 *
 * can also be computed by setting the isBiasCorrected propery to false.
 * The isBiasCorrected property determines whether the "population" or "sample" value is
 * returned by the getResult function. To compute the population variance,
 * set this property to false.
 *
 *
 */
template<typename T>
class Variance
{
public:
  Variance( bool biasCorrected = true );

  bool isBiasCorrected() const;

  void increment( T v );
  void clear();
  int getN() const;
  T getResult() const;

private:
  SecondMoment<T> moment;
  bool mIsBiasCorrected;
};

template<typename T>
class StandardDiviation : public Variance<T>
{
public:
  StandardDiviation( bool biasCorrected = true );

  T getResult() const;

};

template<typename T>
class StorelessStats
{
public:
  StorelessStats( bool biasCorrected = true );

  bool isBiasCorrected() const;
  void increment( T v );
  void clear();
  int getN() const;

  T getMean() const;
  T getStdDiv() const;

private:
  FirstMoment<T> mMean;
  StandardDiviation<T> mStdDiv;

};

typedef FirstMoment<float> FirstMomentF;
typedef FirstMoment<double> FirstMomentD;
typedef FirstMoment<float> MeanF;
typedef FirstMoment<double> MeanD;

typedef SecondMoment<float> SecondMomentF;
typedef SecondMoment<double> SecondMomentD;

typedef Variance<float> VarianceF;
typedef Variance<double> VarianceD;

typedef StandardDiviation<float> StandardDiviationF;
typedef StandardDiviation<double> StandardDiviationD;

typedef StorelessStats<float> StorelessStatsF;
typedef StorelessStats<double> StorelessStatsD;


template<typename T, size_t Size>
struct Vec {
  T data[Size];

  T& operator[]( int i ) {
    return data[i];
  }

  const T& operator[]( int i ) const {
    return data[i];
  }
};

template<typename T>
using Vec2 = Vec<T, 2>;

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;

template<typename T>
class UnivariantFunction
{
public:
  T operator()( const T& );
};

template<typename T>
class LinearFunction : public UnivariantFunction<T>
{
public:

  static
  LinearFunction createFromPoints( const Vec2<T>& a, const Vec2<T>& b );

  T operator()( const T& );
  const T& slope() const;
  const T& offset() const;

private:
  T mM;
  T mB;

};

typedef LinearFunction<float> LinearFunctionf;
typedef LinearFunction<double> LinearFunctiond;

template<typename T>
const T& clamp( const T& v, const T& lo, const T& hi );

} // namespace dev


#endif // BASELINE_MATHUTILS_H_