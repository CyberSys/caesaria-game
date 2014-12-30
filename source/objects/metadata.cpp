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
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com
// Copyright 2012-2014 dalerank, dalerankn8@gmail.com

#include "metadata.hpp"

#include <map>
#include "core/gettext.hpp"
#include "core/saveadapter.hpp"
#include "core/utils.hpp"
#include "core/enumerator.hpp"
#include "core/foreach.hpp"
#include "core/logger.hpp"
#include "constants.hpp"
#include "gfx/picture_info_bank.hpp"

using namespace constants;
using namespace gfx;

const char* MetaDataOptions::cost = "cost";
const char* MetaDataOptions::requestDestroy = "requestDestroy";
const char* MetaDataOptions::employers = "employers";

MetaData MetaData::invalid = MetaData( objects::unknown, "unknown" );

class BuildingTypeHelper : public EnumsHelper<TileOverlay::Type>
{
public:
  BuildingTypeHelper() : EnumsHelper<TileOverlay::Type>( objects::unknown )
  {
#define __REG_TOTYPE(a) append(objects::a, CAESARIA_STR_EXT(a) );
#define __REG_ALTTYPE(a, b) append(objects::a, b );
    __REG_TOTYPE( amphitheater )
    __REG_TOTYPE( theater )
    __REG_TOTYPE( hippodrome )
    __REG_TOTYPE( colloseum )
    __REG_TOTYPE( actorColony )
    __REG_TOTYPE( gladiatorSchool ) __REG_ALTTYPE( gladiatorSchool, "gladiator_pit" )
    __REG_TOTYPE( lionsNursery )    __REG_ALTTYPE( lionsNursery,   "lion_pit" )
    __REG_TOTYPE( chariotSchool )   __REG_ALTTYPE( chariotSchool,  "chatioteer_school" )
    __REG_TOTYPE( house )
    append( objects::road,       "road" );
    append( objects::plaza,      "plaza" );
    append( objects::garden,     "garden" );
    append( objects::senate,         "senate_1" );
    append( objects::forum,          "forum_1" );
    append( objects::governorHouse,  "governor_palace_1" );
    append( objects::governorVilla,  "governor_palace_2" );
    append( objects::governorPalace, "governor_palace_3" );
    append( objects::fortLegionaire, "fort_legionaries" );
    append( objects::fortJavelin,    "fort_javelin" );
    append( objects::fortMounted,    "fort_horse" );
    __REG_TOTYPE( prefecture )
    __REG_TOTYPE( barracks )
    append( objects::militaryAcademy,"military_academy" );
    append( objects::doctor,         "clinic" );
    __REG_TOTYPE( hospital )
    __REG_TOTYPE( baths )
    __REG_TOTYPE( barber )
    __REG_TOTYPE( school )
    __REG_TOTYPE( academy );
    __REG_TOTYPE( library )
    __REG_TOTYPE( missionaryPost )
    append( objects::templeCeres,    "small_ceres_temple" );
    append( objects::templeNeptune,  "small_neptune_temple" );
    append( objects::templeMars,     "small_mars_temple" );
    append( objects::templeMercury,  "small_mercury_temple" );
    append( objects::templeVenus,    "small_venus_temple" );
    append( objects::cathedralCeres,  "big_ceres_temple" );
    append( objects::cathedralNeptune,"big_neptune_temple" );
    append( objects::cathedralMars,   "big_mars_temple");
    append( objects::cathedralMercury,"big_mercury_temple");
    append( objects::cathedralVenus,  "big_venus_temple");
    __REG_TOTYPE( oracle )
    __REG_TOTYPE( market )
    append( objects::granary,        "granery");
    __REG_TOTYPE( warehouse )
    append( objects::wheatFarm,      "wheat_farm");
    append( objects::fruitFarm,      "fig_farm");
    append( objects::vegetableFarm,  "vegetable_farm");
    append( objects::oliveFarm,      "olive_farm");
    append( objects::grapeFarm,      "vinard");
    append( objects::pigFarm,        "meat_farm");
    append( objects::marbleQuarry,   "quarry");
    append( objects::ironMine,       "iron_mine");
    append( objects::timberLogger,   "lumber_mill");
    append( objects::clayPit,        "clay_pit");
    append( objects::winery,         "wine_workshop");
    append( objects::creamery,       "oil_workshop");
    append( objects::weaponsWorkshop,"weapons_workshop");
    append( objects::furnitureWorkshop, "furniture_workshop");
    append( objects::pottery,        "pottery_workshop");
    append( objects::engineerPost,   "engineering_post");
    append( objects::smallStatue,    "statue_small");
    append( objects::middleStatue,   "statue_middle");
    append( objects::bigStatue,      "statue_big");
    append( objects::lowBridge,      "low_bridge");
    append( objects::highBridge,     "high_bridge");
    __REG_TOTYPE( dock )
    __REG_TOTYPE( shipyard )
    __REG_TOTYPE( wharf )
    append( objects::triumphalArch,  "triumphal_arch");
    __REG_TOTYPE( well )
    __REG_TOTYPE( fountain )
    __REG_TOTYPE( aqueduct )
    __REG_TOTYPE( reservoir )
    append( objects::nativeHut,      "native_hut");
    append( objects::nativeCenter,   "native_center");
    append( objects::nativeField,    "native_field");
    append( objects::burningRuins,   "burning_ruins");
    append( objects::burnedRuins,    "burned_ruins");
    append( objects::plagueRuins,    "plague_ruins");
    append( objects::collapsedRuins, "collapsed_ruins");
    append( objects::forum2,         "forum_2" );
    __REG_TOTYPE( gatehouse )
    append( objects::senate2,        "senate_2" );
    __REG_TOTYPE( tower )
    __REG_TOTYPE( wall )
    __REG_TOTYPE( fortification )
    __REG_TOTYPE( elevation )
    __REG_TOTYPE( rift )
    __REG_TOTYPE( river )
    __REG_TOTYPE( tree )
    __REG_TOTYPE( waymark )
    __REG_TOTYPE( terrain )
    __REG_TOTYPE( water )
    __REG_TOTYPE( meadow )

    append( objects::unknown,        "" );
#undef __REG_TOTYPE
 }
};

class BuildingClassHelper : public EnumsHelper<TileOverlay::Group>
{
public:
  BuildingClassHelper() : EnumsHelper<TileOverlay::Group>( objects::unknownGroup )
  {
    append( objects::industryGroup, "industry" );
    append( objects::obtainGroup, "rawmaterial" );
    append( objects::foodGroup, "food" );
    append( objects::disasterGroup, "disaster" );
    append( objects::religionGroup, "religion" );
    append( objects::militaryGroup, "military" );
    append( objects::nativeGroup, "native" );
    append( objects::waterGroup, "water" );
    append( objects::administrationGroup, "administration" );
    append( objects::bridgeGroup, "bridge" );
    append( objects::engineeringGroup, "engineer" );
    append( objects::tradeGroup, "trade" );
    append( objects::tower, "tower" );
    append( objects::gateGroup, "gate" );
    append( objects::securityGroup, "security" );
    append( objects::educationGroup, "education" );
    append( objects::healthGroup, "health" );
    append( objects::sightGroup, "sight" );
    append( objects::gardenGroup, "garden" );
    append( objects::roadGroup, "road" );
    append( objects::entertainmentGroup, "entertainment" );
    append( objects::houseGroup, "house" );
    append( objects::wallGroup, "wall" );
    append( objects::unknown, "" );
  }
};

class MetaData::Impl
{
public:
  Desirability desirability;
  TileOverlay::Type tileovType;
  TileOverlay::Group group;
  std::string name;  // debug name  (english, ex:"iron")
  std::string sound;
  StringArray desc;
  VariantMap options;
  std::string prettyName;

  std::map< int, StringArray > pictures;
};

MetaData::MetaData(const gfx::TileOverlay::Type buildingType, const std::string& name )
  : _d( new Impl )
{
  _d->prettyName = "##" + name + "##";
  _d->tileovType = buildingType;
  _d->group = objects::unknownGroup;
  _d->name = name;  
}

MetaData::MetaData(const MetaData &a) : _d( new Impl )
{
  *this = a;
}

MetaData::~MetaData(){}
std::string MetaData::name() const{  return _d->name;}
std::string MetaData::sound() const{  return _d->sound;}
std::string MetaData::prettyName() const {  return _d->prettyName;}

std::string MetaData::description() const
{
  if( _d->desc.empty() )
    return "##" + _d->name + "_info##";

  return _d->desc[ rand() % _d->desc.size() ];
}

TileOverlay::Type MetaData::type() const {  return _d->tileovType;}
Desirability MetaData::desirability() const{  return _d->desirability;}

Picture MetaData::picture(int size) const
{
  StringArray& array = _d->pictures[ size ];
  return Picture::load( array.random() );
}

Variant MetaData::getOption(const std::string &name, Variant defaultVal ) const
{
  VariantMap::iterator it = _d->options.find( name );
  return it != _d->options.end() ? it->second : defaultVal;
}

MetaData& MetaData::operator=(const MetaData &a)
{
  _d->tileovType = a._d->tileovType;
  _d->name = a._d->name;
  _d->prettyName = a._d->prettyName;
  _d->sound = a._d->sound;
  _d->pictures = a._d->pictures;
  _d->group = a._d->group;
  _d->desirability = a._d->desirability;
  _d->desc = a._d->desc;
  _d->options = a._d->options;

  return *this;
}

TileOverlay::Group MetaData::group() const {  return _d->group; }

class MetaDataHolder::Impl
{
public:
  BuildingTypeHelper typeHelper;
  BuildingClassHelper classHelper;

  typedef std::map<TileOverlay::Type, MetaData> ObjectsMap;
  typedef std::map<good::Type, TileOverlay::Type> FactoryInMap;

  ObjectsMap objectsInfo;// key=building_type, value=data
  FactoryInMap mapBuildingByInGood;
};

MetaDataHolder& MetaDataHolder::instance()
{
  static MetaDataHolder inst;
  return inst;
}

TileOverlay::Type MetaDataHolder::getConsumerType(const good::Type inGoodType) const
{
  TileOverlay::Type res = objects::unknown;

  Impl::FactoryInMap::iterator mapIt;
  mapIt = _d->mapBuildingByInGood.find(inGoodType);
  if (mapIt != _d->mapBuildingByInGood.end())
  {
    res = mapIt->second;
  }
  return res;
}

const MetaData& MetaDataHolder::getData(const TileOverlay::Type buildingType)
{
  Impl::ObjectsMap::iterator mapIt;
  mapIt = instance()._d->objectsInfo.find(buildingType);
  if (mapIt == instance()._d->objectsInfo.end())
  {
    Logger::warning("MetaDataHolder::Unknown objects %d", buildingType );
    return MetaData::invalid;
  }
  return mapIt->second;
}

bool MetaDataHolder::hasData(const TileOverlay::Type buildingType) const
{
  bool res = true;
  Impl::ObjectsMap::iterator mapIt;
  mapIt = _d->objectsInfo.find(buildingType);
  if (mapIt == _d->objectsInfo.end())
  {
    res = false;
  }
  return res;
}

MetaDataHolder::OverlayTypes MetaDataHolder::availableTypes() const
{
  OverlayTypes ret;
  foreach( it, _d->objectsInfo )  { ret.push_back( it->first );  }
  return ret;
}

void MetaDataHolder::addData(const MetaData &data)
{
  TileOverlay::Type buildingType = data.type();

  if (hasData(buildingType))
  {
    Logger::warning( "MetaDataHolder: Info is already set for " + data.name() );
    return;
  }

  _d->objectsInfo.insert(std::make_pair(buildingType, data));
}


MetaDataHolder::MetaDataHolder() : _d( new Impl )
{
}

void MetaDataHolder::initialize( vfs::Path filename )
{
  // populate _mapBuildingByInGood
  _d->mapBuildingByInGood[good::iron  ] = objects::weaponsWorkshop;
  _d->mapBuildingByInGood[good::timber] = objects::furnitureWorkshop;
  _d->mapBuildingByInGood[good::clay  ] = objects::pottery;
  _d->mapBuildingByInGood[good::olive ] = objects::creamery;
  _d->mapBuildingByInGood[good::grape ] = objects::winery;

  VariantMap constructions = SaveAdapter::load( filename );

  foreach( mapItem, constructions )
  {
    VariantMap options = mapItem->second.toMap();

    const TileOverlay::Type btype = findType( mapItem->first );
    if( btype == objects::unknown )
    {
      Logger::warning( "!!!Warning: can't associate type with %s", mapItem->first.c_str() );
      continue;
    }

    Impl::ObjectsMap::const_iterator bdataIt = _d->objectsInfo.find( btype );
    if( bdataIt != _d->objectsInfo.end() )
    {
      Logger::warning( "!!!Warning: type %s also initialized", mapItem->first.c_str() );
      continue;
    }

    MetaData bData( btype, mapItem->first );

    bData._d->options = options;
    VariantMap desMap = options[ "desirability" ].toMap();
    bData._d->desirability.VARIANT_LOAD_ANY(base, desMap );
    bData._d->desirability.VARIANT_LOAD_ANY(range, desMap);
    bData._d->desirability.VARIANT_LOAD_ANY(step, desMap );

    bData._d->desc = options.get( "desc" ).toStringArray();
    bData._d->prettyName = options.get( "prettyName", Variant( bData._d->prettyName ) ).toString();

    bData._d->group = findGroup( options[ "class" ].toString() );

    VariantList basePic = options[ "image" ].toList();
    if( !basePic.empty() )
    {
      std::string groupName = basePic.get( 0 ).toString();
      int imageIndex = basePic.get( 1 ).toInt();
      Variant vOffset = options[ "image.offset" ];
      if( vOffset.isValid() )
      {
        PictureInfoBank::instance().setOffset( groupName, imageIndex, vOffset.toPoint() );
      }

      Picture pic = Picture::load( groupName, imageIndex );
      bData._d->pictures[ 0 ] << pic.name();
    }

    VariantMap extPics = options[ "image.ext" ].toMap();
    foreach( it, extPics )
    {
      VariantMap info = it->second.toMap();
      VARIANT_INIT_ANY( int, size, info )
      VARIANT_INIT_ANY( int, start, info );
      VARIANT_INIT_ANY( int, count, info );
      VARIANT_INIT_STR( rc, info );
      for( int i=0; i < count; i++ )
      {
        Picture pic = Picture::load( rc, start + i );
        if( pic.isValid() )
        {
          bData._d->pictures[ size ] << pic.name();
        }
      }
    }

    VariantList soundVl = options[ "sound" ].toList();
    if( !soundVl.empty() )
    {
      bData._d->sound = utils::format( 0xff, "%s_%05d",
                                              soundVl.get( 0 ).toString().c_str(), soundVl.get( 1 ).toInt() );
    }

    addData( bData );
  }
}

MetaDataHolder::~MetaDataHolder() {}

TileOverlay::Type MetaDataHolder::findType( const std::string& name )
{
  TileOverlay::Type type = instance()._d->typeHelper.findType( name );

  if( type == instance()._d->typeHelper.getInvalid() )
  {
    Logger::warning( "MetaDataHolder: can't find type for typeName " + ( name.empty() ? "null" : name) );
    return objects::unknown;
  }

  return type;
}

std::string MetaDataHolder::findTypename(TileOverlay::Type type)
{
  return instance()._d->typeHelper.findName( type );
}

TileOverlay::Group MetaDataHolder::findGroup( const std::string& name )
{
  TileOverlay::Group type = instance()._d->classHelper.findType( name );

  if( type == instance()._d->classHelper.getInvalid() )
  {
    Logger::warning( "MetaDataHolder: can't find object class for className %s", name.c_str() );
    return objects::unknownGroup;
  }

  return type;
}

std::string MetaDataHolder::findPrettyName(TileOverlay::Type type)
{
  return instance().getData( type ).prettyName();
}

std::string MetaDataHolder::findDescription(TileOverlay::Type type)
{
  return instance().getData( type ).description();
}

Picture MetaDataHolder::randomPicture(TileOverlay::Type type, Size size)
{
  const MetaData& md = getData( type );
  return md.picture( size.width() );
}
