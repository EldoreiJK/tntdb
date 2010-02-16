/*
 * Copyright (C) 2010 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */


#include <cxxtools/unit/testsuite.h>
#include <cxxtools/unit/registertest.h>
#include <stdlib.h>
#include <tntdb/connect.h>
#include <tntdb/statement.h>
#include <tntdb/row.h>
#include <limits>

#define BEGIN_TEST(T, col)                           \
      const std::string colName = col;               \
      tntdb::Statement ins = conn.prepare(           \
        "insert into tntdbtest(" col  ") values(:" col ")"); \
      tntdb::Statement sel = conn.prepare(           \
        "select " col " from tntdbtest");            \
      tntdb::Value dbvalue;                          \
      T res;

#define TEST(val)                                    \
      del.execute();                                 \
      ins.set(colName, val).execute();               \
      dbvalue = sel.selectValue();                   \
      dbvalue.get(res);                              \
      CXXTOOLS_UNIT_ASSERT_EQUALS(val, res);

#define TESTEQ(val)                                  \
      del.execute();                                 \
      ins.set(colName, val).execute();               \
      dbvalue = sel.selectValue();                   \
      dbvalue.get(res);                              \
      CXXTOOLS_UNIT_ASSERT(val == res);

#define TESTFLOAT(val)                               \
      del.execute();                                 \
      ins.set(colName, val).execute();               \
      dbvalue = sel.selectValue();                   \
      dbvalue.get(res);                              \
      CXXTOOLS_UNIT_ASSERT(val / res >= .9999 && val / res <= 1.0001);

#define TESTDT(val)                                  \
      del.execute();                                 \
      ins.set(colName, val).execute();               \
      dbvalue = sel.selectValue();                   \
      dbvalue.get(res);                              \
      CXXTOOLS_UNIT_ASSERT_EQUALS(val.getIso(), res.getIso());

class TntdbTypesTest : public cxxtools::unit::TestSuite
{
    tntdb::Connection conn;
    tntdb::Statement del;

  public:
    TntdbTypesTest()
      : cxxtools::unit::TestSuite("tntdb-types-Test")
    {
      registerMethod("testAll", *this, &TntdbTypesTest::testAll);
      registerMethod("testLimits", *this, &TntdbTypesTest::testLimits);
      registerMethod("testNull", *this, &TntdbTypesTest::testNull);

      registerMethod("testBool", *this, &TntdbTypesTest::testBool);
      registerMethod("testInt", *this, &TntdbTypesTest::testInt);
      registerMethod("testLong", *this, &TntdbTypesTest::testLong);
      registerMethod("testUnsigned", *this, &TntdbTypesTest::testUnsigned);
      registerMethod("testUnsignedLong", *this, &TntdbTypesTest::testUnsignedLong);
      registerMethod("testInt32", *this, &TntdbTypesTest::testInt32);
      registerMethod("testUint32", *this, &TntdbTypesTest::testUint32);
      registerMethod("testInt64", *this, &TntdbTypesTest::testInt64);
      registerMethod("testUint64", *this, &TntdbTypesTest::testUint64);
      registerMethod("testDecimal", *this, &TntdbTypesTest::testDecimal);
      registerMethod("testFloat", *this, &TntdbTypesTest::testFloat);
      registerMethod("testDouble", *this, &TntdbTypesTest::testDouble);
      registerMethod("testChar", *this, &TntdbTypesTest::testChar);
      registerMethod("testString", *this, &TntdbTypesTest::testString);
      registerMethod("testBlob", *this, &TntdbTypesTest::testBlob);
      registerMethod("testDate", *this, &TntdbTypesTest::testDate);
      registerMethod("testTime", *this, &TntdbTypesTest::testTime);
      registerMethod("testDatetime", *this, &TntdbTypesTest::testDatetime);
      registerMethod("testSequence", *this, &TntdbTypesTest::testSequence);

      const char* dburl = getenv("TNTDBURL");
      if (!dburl)
        dburl = "sqlite:test.db";

      std::cout << "testing with dburl=" << dburl << std::endl;

      conn = tntdb::connect(dburl);
      del = conn.prepare("delete from tntdbtest");
    }

    void testBool()
    {
      BEGIN_TEST(bool, "boolcol");
      TEST(true);
      TEST(false);
    }

    void testInt()
    {
      BEGIN_TEST(int, "intcol");
      TEST(static_cast<int>(42));
      TEST(static_cast<int>(-123));
      TEST(std::numeric_limits<int>::max());
      TEST(std::numeric_limits<int>::min());
    }

    void testLong()
    {
      BEGIN_TEST(long, "longcol");
      TEST(static_cast<long>(42));
      TEST(static_cast<long>(-123));
      TEST(std::numeric_limits<long>::max());
      TEST(std::numeric_limits<long>::min());
    }

    void testUnsigned()
    {
      BEGIN_TEST(unsigned, "unsignedcol");
      TEST(static_cast<unsigned>(42));
      TEST(std::numeric_limits<unsigned>::max());
      TEST(std::numeric_limits<unsigned>::min());
    }

    void testUnsignedLong()
    {
      BEGIN_TEST(unsigned long, "unsignedlongcol");
      TEST(static_cast<unsigned long>(42));
      TEST(std::numeric_limits<unsigned long>::max());
      TEST(std::numeric_limits<unsigned long>::min());
    }

    void testInt32()
    {
      BEGIN_TEST(int32_t, "int32col");
      TEST(static_cast<int32_t>(42));
      TEST(static_cast<int32_t>(-123));
      TEST(std::numeric_limits<int32_t>::max());
      TEST(std::numeric_limits<int32_t>::min());
    }

    void testUint32()
    {
      BEGIN_TEST(uint32_t, "uint32col");
      TEST(static_cast<uint32_t>(42));
      TEST(std::numeric_limits<uint32_t>::max());
      TEST(std::numeric_limits<uint32_t>::min());
    }

    void testInt64()
    {
      BEGIN_TEST(int64_t, "int64col");
      TEST(static_cast<int64_t>(42));
      TEST(static_cast<int64_t>(-123));
      TEST(std::numeric_limits<int64_t>::max());
      TEST(std::numeric_limits<int64_t>::min());
    }

    void testUint64()
    {
      BEGIN_TEST(uint64_t, "uint64col");
      TEST(static_cast<uint64_t>(42));
      TEST(std::numeric_limits<uint64_t>::max());
      TEST(std::numeric_limits<uint64_t>::min());
    }

    void testDecimal()
    {
      BEGIN_TEST(tntdb::Decimal, "decimalcol");
      TEST(tntdb::Decimal(456, 8));
      TEST(tntdb::Decimal(456, -4));
      TEST(tntdb::Decimal(-456, 8));
    }

    void testFloat()
    {
      BEGIN_TEST(float, "floatcol");
      TESTFLOAT(static_cast<float>(42));
      TESTFLOAT(static_cast<float>(-123));
      TESTFLOAT(std::numeric_limits<float>::max() * .999999);
      TESTFLOAT(std::numeric_limits<float>::min() * 1.00001);
    }

    void testDouble()
    {
      BEGIN_TEST(double, "doublecol");
      TESTFLOAT(static_cast<double>(42));
      TESTFLOAT(static_cast<double>(-123));
      TESTFLOAT(std::numeric_limits<double>::max() * .999999);
      TESTFLOAT(std::numeric_limits<double>::min() * 1.00001);
    }

    void testChar()
    {
      BEGIN_TEST(char, "charcol");
      TEST('A');
      TEST('Z');
      TEST('\n');
    }

    void testString()
    {
      BEGIN_TEST(std::string, "stringcol");
      TEST(std::string("Hello\n"));
      TEST(std::string("M\xc3\xa4kitalo"));
    }

    void testBlob()
    {
      BEGIN_TEST(tntdb::Blob, "blobcol");
      tntdb::Blob blobval = tntdb::Blob("\0\1\2\3\0xff", 5);
      TESTEQ(blobval);
    }

    void testDate()
    {
      BEGIN_TEST(tntdb::Date, "datecol");
      TESTDT(tntdb::Date());
      TESTDT(tntdb::Date(2010, 2, 15));
    }

    void testTime()
    {
      BEGIN_TEST(tntdb::Time, "timecol");
      TESTDT(tntdb::Time());
      TESTDT(tntdb::Time(20, 9, 31, 12));
    }

    void testDatetime()
    {
      BEGIN_TEST(tntdb::Datetime, "datetimecol");
      TESTDT(tntdb::Datetime());
      TESTDT(tntdb::Datetime(2010, 2, 15, 20, 9, 31, 12));
    }

    void testSequence()
    {
      del.execute();

      tntdb::Statement ins = conn.prepare(
        "insert into tntdbtest(intcol) values(:intcol)");

      ins.set("intcol", 42)
         .execute();

      long serialval = conn.lastInsertId("tntdbtest_seq");

      tntdb::Statement sel = conn.prepare(
        "select id from tntdbtest");

      tntdb::Value dbvalue = sel.selectValue();

      long serialres = -1;
      dbvalue.get(serialres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(serialval, serialres);
    }

    void testAll()
    {
      del.execute();

      tntdb::Statement ins = conn.prepare(
        "insert into tntdbtest("
        "    boolcol, intcol, longcol, unsignedcol, unsignedlongcol,"
        "    int32col, uint32col, int64col, uint64col,"
        "    decimalcol, floatcol, doublecol,"
        "    charcol, stringcol, blobcol,"
        "    datecol, timecol, datetimecol)"
        " values("
        "    :boolcol, :intcol, :longcol, :unsignedcol, :unsignedlongcol,"
        "    :int32col, :uint32col, :int64col, :uint64col,"
        "    :decimalcol, :floatcol, :doublecol,"
        "    :charcol, :stringcol, :blobcol,"
        "    :datecol, :timecol, :datetimecol)");

      bool boolval                = true;
      int intval                  = 42;
      long longval                = 43;
      unsigned uval               = 44;
      unsigned long ulongval      = 45;
      int32_t int32val            = 47;
      uint32_t uint32val          = 50;
      int64_t int64val            = 52;
      uint64_t uint64val          = 55;
      tntdb::Decimal decimalval   = tntdb::Decimal(456, 8);
      float floatval              = 42.195;
      double doubleval            = 3.1415926535;
      char charval                = 'q';
      std::string stringval       = "tntdb test string\n";
      tntdb::Blob blobval         = tntdb::Blob("\0\1\2\3\0xff", 5);
      tntdb::Date dateval         = tntdb::Date(2010, 2, 15);
      tntdb::Time timeval         = tntdb::Time(20, 9, 31, 12);
      tntdb::Datetime datetimeval = tntdb::Datetime(2010, 2, 15, 20, 9, 31, 12);

      ins.set("boolcol", boolval)
         .set("intcol", intval)
         .set("longcol", longval)
         .set("unsignedcol", uval)
         .set("unsignedlongcol", ulongval)
         .set("int32col", int32val)
         .set("uint32col", uint32val)
         .set("int64col", int64val)
         .set("uint64col", uint64val)
         .set("decimalcol", decimalval)
         .set("floatcol", floatval)
         .set("doublecol", doubleval)
         .set("charcol", charval)
         .set("stringcol", stringval)
         .set("blobcol", blobval)
         .set("datecol", dateval)
         .set("timecol", timeval)
         .set("datetimecol", datetimeval)
         .execute();

      long serialval = conn.lastInsertId("tntdbtest_seq");

      long serialres;
      bool boolres;
      int intres;
      long longres;
      unsigned ures;
      unsigned long ulongres;
      int32_t int32res;
      uint32_t uint32res;
      int64_t int64res;
      uint64_t uint64res;
      tntdb::Decimal decimalres;
      float floatres;
      double doubleres;
      char charres;
      std::string stringres;
      tntdb::Blob blobres;
      tntdb::Date dateres;
      tntdb::Time timeres;
      tntdb::Datetime datetimeres;

      tntdb::Statement sel = conn.prepare(
        "select"
        "    id,"
        "    boolcol, intcol, longcol, unsignedcol, unsignedlongcol,"
        "    int32col, uint32col, int64col, uint64col,"
        "    decimalcol, floatcol, doublecol,"
        "    charcol, stringcol, blobcol,"
        "    datecol, timecol, datetimecol"
        " from tntdbtest");

      tntdb::Row row = sel.selectRow();
      row[0].get(serialres);
      row[1].get(boolres);
      row[2].get(intres);
      row[3].get(longres);
      row[4].get(ures);
      row[5].get(ulongres);
      row[6].get(int32res);
      row[7].get(uint32res);
      row[8].get(int64res);
      row[9].get(uint64res);
      row[10].get(decimalres);
      row[11].get(floatres);
      row[12].get(doubleres);
      row[13].get(charres);
      row[14].get(stringres);
      row[15].get(blobres);
      row[16].get(dateres);
      row[17].get(timeres);
      row[18].get(datetimeres);

      float fq = floatval / floatres;
      double dq = doubleval / doubleres;

      CXXTOOLS_UNIT_ASSERT_EQUALS(serialval, serialres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(boolval, boolres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(intval, intres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(longval, longres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uval, ures);
      CXXTOOLS_UNIT_ASSERT_EQUALS(ulongval, ulongres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(int32val, int32res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uint32val, uint32res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(int64val, int64res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uint64val, uint64res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(decimalval, decimalres);
      CXXTOOLS_UNIT_ASSERT(fq >= .9999 && fq <= 1.0001);
      CXXTOOLS_UNIT_ASSERT(dq >= .9999 && dq <= 1.0001);
      CXXTOOLS_UNIT_ASSERT_EQUALS(charval, charres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(stringval, stringres);
      CXXTOOLS_UNIT_ASSERT(blobval == blobres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(dateval.getIso(), dateres.getIso());
      CXXTOOLS_UNIT_ASSERT_EQUALS(timeval.getIso(), timeres.getIso());
      CXXTOOLS_UNIT_ASSERT_EQUALS(datetimeval.getIso(), datetimeres.getIso());
    }

    void testLimits()
    {
      del.execute();

      tntdb::Statement ins = conn.prepare(
        "insert into tntdbtest("
        "    intcol, longcol, unsignedcol, unsignedlongcol,"
        "    int32col, uint32col, int64col, uint64col,"
        "    floatcol, doublecol)"
        " values("
        "    :intcol, :longcol, :unsignedcol, :unsignedlongcol,"
        "    :int32col, :uint32col, :int64col, :uint64col,"
        "    :floatcol, :doublecol)");

      int intval                  = std::numeric_limits<int>::max();
      long longval                = std::numeric_limits<long>::max();
      unsigned uval               = std::numeric_limits<unsigned>::max();
      unsigned long ulongval      = std::numeric_limits<unsigned long>::max();
      int32_t int32val            = std::numeric_limits<int32_t>::max();
      uint32_t uint32val          = std::numeric_limits<uint32_t>::max();
      int64_t int64val            = std::numeric_limits<int64_t>::max();
      uint64_t uint64val          = std::numeric_limits<uint64_t>::max();
      float floatval              = std::numeric_limits<float>::max() * .999999;
      double doubleval            = std::numeric_limits<double>::max() * .999999;

      ins.set("intcol", intval)
         .set("longcol", longval)
         .set("unsignedcol", uval)
         .set("unsignedlongcol", ulongval)
         .set("int32col", int32val)
         .set("uint32col", uint32val)
         .set("int64col", int64val)
         .set("uint64col", uint64val)
         .set("floatcol", floatval)
         .set("doublecol", doubleval)
         .execute();

      int intres;
      long longres;
      unsigned ures;
      unsigned long ulongres;
      int32_t int32res;
      uint32_t uint32res;
      int64_t int64res;
      uint64_t uint64res;
      float floatres;
      double doubleres;

      tntdb::Statement sel = conn.prepare(
        "select"
        "    intcol, longcol, unsignedcol, unsignedlongcol,"
        "    int32col, uint32col, int64col, uint64col,"
        "    floatcol, doublecol"
        " from tntdbtest");

      tntdb::Row row = sel.selectRow();
      row[0].get(intres);
      row[1].get(longres);
      row[2].get(ures);
      row[3].get(ulongres);
      row[4].get(int32res);
      row[5].get(uint32res);
      row[6].get(int64res);
      row[7].get(uint64res);
      row[8].get(floatres);
      row[9].get(doubleres);

      float fq = floatval / floatres;
      double dq = doubleval / doubleres;

      CXXTOOLS_UNIT_ASSERT_EQUALS(intval, intres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(longval, longres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uval, ures);
      CXXTOOLS_UNIT_ASSERT_EQUALS(ulongval, ulongres);
      CXXTOOLS_UNIT_ASSERT_EQUALS(int32val, int32res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uint32val, uint32res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(int64val, int64res);
      CXXTOOLS_UNIT_ASSERT_EQUALS(uint64val, uint64res);
      CXXTOOLS_UNIT_ASSERT(fq >= .9999 && fq <= 1.0001);
      CXXTOOLS_UNIT_ASSERT(dq >= .9999 && dq <= 1.0001);
    }

    void testNull()
    {
      tntdb::Statement ins = conn.prepare(
        "insert into tntdbtest(intcol, longcol)"
        " values(:intcol, :longcol)");

      long longval = 43;

      ins.setNull("intcol")
         .set("longcol", longval)
         .execute();

      int intres;
      long longres;

      tntdb::Statement sel = conn.prepare(
        "select intcol, longcol"
        " from tntdbtest");

      tntdb::Row row = sel.selectRow();
      bool intnotnull = row[0].get(intres);
      bool longnotnull = row[1].get(longres);
      CXXTOOLS_UNIT_ASSERT(!intnotnull);
      CXXTOOLS_UNIT_ASSERT(longnotnull);
    }

};

cxxtools::unit::RegisterTest<TntdbTypesTest> register_TntdbTypesTest;