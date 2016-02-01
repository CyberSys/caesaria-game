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

#ifndef __CAESARIA_GOODSTORE_SIMPLE_H_INCLUDED__
#define __CAESARIA_GOODSTORE_SIMPLE_H_INCLUDED__

#include "store.hpp"
#include "core/scopedptr.hpp"

namespace good
{

class Storage : public Store
{
public:
  Storage();
  virtual ~Storage();

  virtual void setCapacity(const good::Product& goodType, const int maxQty);
  virtual void setCapacity(const int maxQty);

  virtual int  capacity() const;
  virtual int  capacity(const good::Product& goodType) const;

  virtual int  qty(const good::Product& goodType) const;
  virtual int  qty() const;

  void resize( const Store& other );

  good::Stock& getStock(const good::Product &goodType);
  virtual ProductMap details() const;

  void setQty(const good::Product& goodType, const int currentQty);

  // returns the max quantity that can be stored now
  int getMaxStore(const good::Product goodType);

  // store/retrieve
  virtual bool applyStorageReservation(good::Stock &stock, const int reservationID);
  virtual bool applyRetrieveReservation(good::Stock &stock, const int reservationID);
  virtual void confirmDeliver(Product type, int qty, unsigned int tag, const DateTime& time );

  virtual VariantMap save() const;
  virtual void load( const VariantMap& stream );

private:
  class Impl;
  ScopedPtr< Impl > _gsd;
};

}//end namespace good

#endif //__CAESARIA_GOODSTORE_SIMPLE_H_INCLUDED__
