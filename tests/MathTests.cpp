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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/MathUtils.h>

using namespace baseline;

TEST_CASE( "compute first moment", "[math]" )
{
  FirstMomentF mean;

  REQUIRE( mean.getN() == 0 );

  mean.increment( 5.0 );

  REQUIRE( mean.getN() == 1 );
  REQUIRE( mean.getResult() == Approx( 5.0 ) );

  mean.increment( 0.0 );

  REQUIRE( mean.getN() == 2 );
  REQUIRE( mean.getResult() == Approx( 2.5 ) );

  mean.clear();

  REQUIRE( mean.getN() == 0 );
  mean.increment( 10.0 );
  REQUIRE( mean.getN() == 1 );
  REQUIRE( mean.getResult() == Approx( 10.0 ) );
}

TEST_CASE( "compute sample variance", "[math]" )
{

  VarianceF var;

  REQUIRE( var.getN() == 0 );
  REQUIRE( var.isBiasCorrected() == true );

  var.increment( 1.0 );
  REQUIRE( var.getResult() == Approx( 0.0 ) );
  var.increment( 3.0 );
  var.increment( 5.0 );
  var.increment( 5.0 );

  REQUIRE( var.getN() == 4 );
  REQUIRE( var.getResult() == Approx( 3.66667 ) );

}

TEST_CASE( "compute population variance", "[math]" )
{

  VarianceF var( false );

  REQUIRE( var.getN() == 0 );
  REQUIRE( var.isBiasCorrected() == false );

  var.increment( 1.0 );
  var.increment( 3.0 );
  var.increment( 5.0 );
  var.increment( 5.0 );

  REQUIRE( var.getN() == 4 );
  REQUIRE( var.getResult() == Approx( 2.75 ) );

}

TEST_CASE( "compute sample std div", "[math]" )
{

  StandardDiviationF var;

  REQUIRE( var.getN() == 0 );
  REQUIRE( var.isBiasCorrected() == true );

  var.increment( 1.0 );
  REQUIRE( var.getResult() == Approx( 0.0 ) );
  var.increment( 3.0 );
  var.increment( 5.0 );
  var.increment( 5.0 );

  REQUIRE( var.getN() == 4 );
  REQUIRE( var.getResult() == Approx( 1.914854 ) );

}

TEST_CASE( "compute population std div", "[math]" )
{

  StandardDiviationF var( false );

  REQUIRE( var.getN() == 0 );
  REQUIRE( var.isBiasCorrected() == false );

  var.increment( 1.0 );
  REQUIRE( var.getResult() == Approx( 0.0 ) );
  var.increment( 3.0 );
  var.increment( 5.0 );
  var.increment( 5.0 );

  REQUIRE( var.getN() == 4 );
  REQUIRE( var.getResult() == Approx( 1.65831 ) );

}

TEST_CASE( "storeless stat", "[math]" )
{
  StorelessStatsF var( false );

  REQUIRE( var.getN() == 0 );
  REQUIRE( var.isBiasCorrected() == false );

  var.increment( 1.0f );
  var.increment( 3.0 );
  var.increment( 5.0 );
  var.increment( 5.0 );

  REQUIRE( var.getN() == 4 );
  REQUIRE( var.getMean() == Approx( 3.5f ) );
  REQUIRE( var.getStdDiv() == Approx( 1.65831f ) );

}

TEST_CASE( "linear function", "[math]" )
{
  LinearFunctionf f = LinearFunctionf::createFromPoints( {2, 2}, {4, 7} );

  REQUIRE( f.slope() == Approx( 2.5f ) );
  REQUIRE( f.offset() == Approx( -3.0f ) );
  REQUIRE( f( 3.0f ) == Approx( 4.5f ) );
}