/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "TestHelpers.h"
#include "threads/Atomics.h"

#include <boost/shared_array.hpp>
#include <boost/bind.hpp>
#include <iostream>

#define TESTNUM 100000l
#define NUMTHREADS 10l

void doIncrement(long* number)
{
  for (long i = 0; i<TESTNUM; i++)
    AtomicIncrement(number);
}

void doDecrement(long* number)
{
  for (long i = 0; i<TESTNUM; i++)
    AtomicDecrement(number);
}

void doAdd(long* number, long toAdd)
{
  for (long i = 0; i<TESTNUM; i++)
    AtomicAdd(number,toAdd);
}

void doSubtract(long* number, long toAdd)
{
  for (long i = 0; i<TESTNUM; i++)
    AtomicSubtract(number,toAdd);
}

TEST(TestMassAtomic, Increment)
{
  long lNumber = 0;
  boost::shared_array<thread> t;
  t.reset(new thread[NUMTHREADS]);
  for(size_t i=0; i<NUMTHREADS; i++)
    t[i] = thread(boost::bind(&doIncrement,&lNumber));

  for(size_t i=0; i<NUMTHREADS; i++)
    t[i].join();

  EXPECT_EQ((NUMTHREADS * TESTNUM), lNumber);
 }

TEST(TestMassAtomic, Decrement)
{
  long lNumber = (NUMTHREADS * TESTNUM);
  boost::shared_array<thread> t;
  t.reset(new thread[NUMTHREADS]);
  for(size_t i=0; i<NUMTHREADS; i++)
    t[i] = thread(boost::bind(&doDecrement,&lNumber));

  for(size_t i=0; i<NUMTHREADS; i++)
    t[i].join();

  EXPECT_EQ(0, lNumber);
 }

TEST(TestMassAtomic, Add)
{
  long lNumber = 0;
  long toAdd = 10;
  boost::shared_array<thread> t;
  t.reset(new thread[NUMTHREADS]);
  for(size_t i=0; i<NUMTHREADS; i++)
    t[i] = thread(boost::bind(&doAdd,&lNumber,toAdd));

  for(size_t i=0; i<NUMTHREADS; i++)
    t[i].join();

  EXPECT_EQ((NUMTHREADS * TESTNUM) * toAdd, lNumber);
 }

TEST(TestMassAtomic, Subtract)
{
  long toSubtract = 10;
  long lNumber = (NUMTHREADS * TESTNUM) * toSubtract;
  boost::shared_array<thread> t;
  t.reset(new thread[NUMTHREADS]);
  for(size_t i=0; i<NUMTHREADS; i++)
    t[i] = thread(boost::bind(&doSubtract,&lNumber,toSubtract));

  for(size_t i=0; i<NUMTHREADS; i++)
    t[i].join();

  EXPECT_EQ(0, lNumber);
 }

#define STARTVAL 767856l

TEST(TestAtomic, Increment)
{
  long check = STARTVAL;
  EXPECT_EQ(STARTVAL + 1l, AtomicIncrement(&check));
  EXPECT_EQ(STARTVAL + 1l,check);
}

TEST(TestAtomic, Decrement)
{
  long check = STARTVAL;
  EXPECT_EQ(STARTVAL - 1l, AtomicDecrement(&check));
  EXPECT_EQ(STARTVAL - 1l,check);
}

TEST(TestAtomic, Add)
{
  long check = STARTVAL;
  EXPECT_EQ(STARTVAL + 123l, AtomicAdd(&check,123l));
  EXPECT_EQ(STARTVAL + 123l,check);
}

TEST(TestAtomic, Subtract)
{
  long check = STARTVAL;
  EXPECT_EQ(STARTVAL - 123l, AtomicSubtract(&check,123l));
  EXPECT_EQ(STARTVAL - 123l, check);
}

