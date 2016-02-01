// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#include "trainee.hpp"
#include "gfx/tile.hpp"
#include "core/variant.hpp"
#include "pathway/path_finding.hpp"
#include "city/statistic.hpp"
#include "name_generator.hpp"
#include "objects/constants.hpp"
#include "core/priorities.hpp"
#include "core/logger.hpp"
#include "core/variant_map.hpp"
#include "objects/building.hpp"
#include "gfx/tilemap.hpp"
#include "core/common.hpp"
#include "pathway/pathway_helper.hpp"
#include "walkers_factory.hpp"

using namespace gfx;

REGISTER_NAMED_CLASS_IN_WALKERFACTORY(walker::trainee,TraineeWalker,trainee)

class TraineeWalker::Impl
{
public:
  object::TypeSet necBuildings;  // list of buildings needing this trainee
  TilePos baseLocation;
  TilePos destLocation;
  unsigned int maxDistance;
  float maxNeed;  // evaluates the need for that trainee
};

TraineeWalker::TraineeWalker(PlayerCityPtr city, walker::Type traineeType)
  : Human( city, traineeType ), _d( new Impl )
{
  _d->maxDistance = 30;
  _init( traineeType );
}

void TraineeWalker::_init(walker::Type traineeType)
{
  switch( traineeType )
  {
  case walker::actor:      _d->necBuildings << object::theater
                                            << object::amphitheater;  break;
  case walker::gladiator:  _d->necBuildings << object::amphitheater
                                            << object::colloseum;  break;
  case walker::lionTamer:  _d->necBuildings << object::colloseum;  break;
  case walker::soldier:    _d->necBuildings << object::military_academy
                                            << object::fort_legionaries
                                            << object::fort_horse
                                            << object::fort_javelin
                                            << object::tower;  break;
  case walker::charioteer: _d->necBuildings << object::hippodrome;  break;
  default: break;
  }

  setName( NameGenerator::rand( NameGenerator::plebMale ) );
}

void TraineeWalker::_cancelPath()
{
  BuildingPtr destination = receiver();
  Logger::warningIf( destination.isNull(), "!!! WARNING: Trainee _cancelPath destination is null" );

  if( destination.isValid() )
  {
    destination->cancelTrainee( type() );
  }
}

void TraineeWalker::setBase(BuildingPtr originBuilding)
{
  _d->baseLocation = utils::objPosOrDefault( originBuilding );
}

BuildingPtr TraineeWalker::base() const
{
  return _map().overlay<Building>( _d->baseLocation );
}

BuildingPtr TraineeWalker::receiver() const
{
  return _map().overlay<Building>( _d->destLocation );
}

void TraineeWalker::_computeWalkerPath( bool roadOnly )
{
  if( _d->baseLocation == TilePos::invalid() )
  {
    Logger::warning( "!!! WARNING: trainee walker baselocation is unaccessible at [{},{}]", _d->baseLocation.i(), _d->baseLocation.j() );
    deleteLater();
    return;
  }

  BuildingPtr base = _map().overlay<Building>( _d->baseLocation );
  if( !base.isValid() )
  {
    Logger::warning( "!!! WARNING: trainee walker base is null at [{},{}]", _d->baseLocation.i(), _d->baseLocation.j() );
    deleteLater();
    return;
  }

  _d->maxNeed = 0;  // need of this trainee in buildings
 
  Pathway finalPath;

  BuildingList buildings = _city()->statistic().objects.find<Building>( _d->necBuildings );
  TilesArray startArea = roadOnly ? base->roadside() : base->enterArea();

  DirectRoute droute;
  _d->maxNeed = 0;
  unsigned int minDistance = _d->maxDistance;

  bool isNeedTrainee = false;
  for( auto bld : buildings )
  {
    float howMuchNeedMyService = bld->evaluateTrainee( type() );
    if( howMuchNeedMyService > 0 )
    {
      isNeedTrainee = true;
      break;
    }
  }

  if( !isNeedTrainee )
  {
    Logger::warning( "!!! WARNING: not need trainee walker from [{},{}]", base->pos().i(), base->pos().j() );
    deleteLater();
    return;
  }

  std::set<BuildingPtr> checkedBuilding;

  for( auto itile : startArea )
  {
    TilePos startPos = itile->pos();

    for( auto bld : buildings )
    {
      bool buildingAlsoServicing = checkedBuilding.count( bld ) > 0;
      if( buildingAlsoServicing )
        continue;

      checkedBuilding.insert( bld );

      float curNeed = bld->evaluateTrainee( type() );
      if( _d->maxNeed < curNeed )
      {
        Pathway way = PathwayHelper::create( startPos, bld,
                                             roadOnly ? PathwayHelper::roadOnly : PathwayHelper::allTerrain );
        if( way.isValid() && way.length() < minDistance )
        {
          _d->maxNeed = curNeed;
          droute = DirectRoute( bld.object(), way );
        }
      }
    }
  }

  if( droute.first.isValid() )
  {
    finalPath = droute.second;
    _d->destLocation = droute.first->pos();
  }

  if( finalPath.isValid() )
  {
    // some building needs that trainee!!!
    setPos( finalPath.startPos() );
    setPathway( finalPath );
  }
  else
  {
    // nobody needs him...
    deleteLater();
  }
}

void TraineeWalker::_checkDestination(const object::Type buildingType, Propagator &pathPropagator)
{
  DirectPRoutes pathWayList = pathPropagator.getRoutes( buildingType );

  for( auto item : pathWayList )
  {
    // for every building within range
    BuildingPtr building = item.first.as<Building>();

    float need = building->evaluateTrainee( type() );
    if (need > _d->maxNeed)
    {
      _d->maxNeed = need;
      _d->destLocation = building->pos();
    }
  }
}

int TraineeWalker::value() const{ return 100; }

void TraineeWalker::send2City(BuildingPtr base, bool roadOnly )
{
  if( !base.isValid() )
  {
    Logger::warning( "!!! WARNING: trainee walker base is null" );
    deleteLater();
    return;
  }

  _d->baseLocation = base->pos();
  _computeWalkerPath( roadOnly );

  if( !isDeleted() && config::tilemap.isValidLocation( _d->destLocation ) )
  {
    BuildingPtr dest = receiver();
    dest->reserveTrainee( type() );
    attach();
  }
}

void TraineeWalker::_reachedPathway()
{
  Walker::_reachedPathway();
  deleteLater();

  auto destidnationBuilding = _map().overlay( _d->destLocation ).as<Building>();
  if( destidnationBuilding.isValid() )
  {
    destidnationBuilding->updateTrainee( this );
  }
}

void TraineeWalker::save( VariantMap& stream ) const
{
  Walker::save( stream );
  VARIANT_SAVE_ANY_D( stream, _d, baseLocation );
  VARIANT_SAVE_ANY_D( stream, _d, destLocation );

  VARIANT_SAVE_ANY_D( stream, _d, maxDistance )
  stream[ "traineeType" ] = type();
  stream[ "type" ] = (int)walker::trainee;
}

void TraineeWalker::load( const VariantMap& stream )
{
  Walker::load(stream);

  VARIANT_LOAD_ANY_D( _d, baseLocation, stream )
  VARIANT_LOAD_ANY_D( _d, destLocation, stream )
  VARIANT_LOAD_ANY_D( _d, maxDistance, stream )
  walker::Type wtype = (walker::Type)stream.get( "traineeType" ).toInt();

  _setType( wtype );
  _init( wtype );
}

TilePos TraineeWalker::places(Walker::Place type) const
{
  switch( type )
  {
  case plOrigin: return _d->baseLocation;
  case plDestination: return _d->destLocation;
  default: break;
  }

  return Human::places( type );
}


TraineeWalker::~TraineeWalker(){}
