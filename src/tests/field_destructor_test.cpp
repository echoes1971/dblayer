#include <catch2/catch_all.hpp>
#include "../dblayer/dbfield.h"
#include "../dblayer/booleanfield.h"
#include "../dblayer/integerfield.h"
#include "../dblayer/stringfield.h"
#include "../dblayer/floatfield.h"
#include "../dblayer/datefield.h"
using namespace DBLayer;

TEST_CASE("Field destructors clean up stringValue", "[memory]") {
    SECTION("BooleanField destructor does not leak") {
        BooleanField* bf = new BooleanField("test");
        bf->setValue(true);
        // After scope exit, destructor should run without error
        delete bf;
        // If we get here without crash, test passes
    }
    
    SECTION("IntegerField destructor does not leak") {
        IntegerField* ifield = new IntegerField("test");
        ifield->setValue(42);
        delete ifield;
    }
    
    SECTION("StringField destructor properly cleans stringValue") {
        StringField* sf = new StringField("test");
        sf->setValue("hello world");
        // Verify value is set before delete
        REQUIRE(sf->getStringValue() != nullptr);
        REQUIRE(*sf->getStringValue() == "hello world");
        delete sf;
        // After delete, accessing would be dangling - but we just verify no crash
    }
    
    SECTION("FloatField destructor does not leak") {
        FloatField* ffl = new FloatField("test");
        ffl->setValue(3.14f);
        delete ffl;
    }
    
    SECTION("DateField destructor does not leak") {
        DateField* df = new DateField("test", "2023-01-01 12:00:00");
        df->setValue("2023-06-15 10:30:45");
        delete df;
    }
}

TEST_CASE("Field::getStringValue returns valid pointer before delete", "[memory]") {
    SECTION("StringField stringValue is valid while object alive") {
        StringField sf("test");
        sf.setValue("test value");
        const string* val = sf.getStringValue();
        REQUIRE(val != nullptr);
        REQUIRE(*val == "test value");
    }
    
    SECTION("IntegerField getIntegerValue works") {
        IntegerField ifield("test");
        ifield.setValue(99);
        REQUIRE(ifield.getIntegerValue() == 99);
    }
    
    SECTION("FloatField getFloatValue works") {
        FloatField ffl("test");
        ffl.setValue(2.71f);
        REQUIRE(ffl.getFloatValue() == 2.71f);
    }
}