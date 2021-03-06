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

#ifndef __CAESARIA_TILEOVERLAYFACTORY_H_INCLUDE_
#define __CAESARIA_TILEOVERLAYFACTORY_H_INCLUDE_

#include "core/scopedptr.hpp"
#include "predefinitions.hpp"
#include "objects/overlay.hpp"
#include "metadata.hpp"

class TileOverlayConstructor
{
public:
  virtual OverlayPtr create() = 0;
};

template< class T > class BaseCreator : public TileOverlayConstructor
{
public:
  virtual OverlayPtr create()
  {
    OverlayPtr ret( new T() );
    ret->initialize( object::Info::find( ret->type() ) );
    ret->drop();

    return ret;
  }
};

class TileOverlayFactory
{
public:
  static TileOverlayFactory& instance();
  OverlayPtr create( const object::Type type ) const;
  OverlayPtr create( const std::string& typeName ) const;

  bool canCreate( const object::Type type ) const;
  void addCreator( const object::Type type, const std::string& typeName, TileOverlayConstructor* ctor );
private:
  TileOverlayFactory();

  class Impl;
  ScopedPtr< Impl > _d;
};

#define REGISTER_CLASS_IN_OVERLAYFACTORY(type,a) \
namespace { \
struct Registrator_##a { Registrator_##a() { TileOverlayFactory::instance().addCreator( type, TEXT(a), new BaseCreator<a>() ); }}; \
static Registrator_##a rtor_##a; \
}


#endif  //__CAESARIA_TILEOVERLAYFACTORY_H_INCLUDE_
