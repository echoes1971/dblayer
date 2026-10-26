#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../dblayer/dbfield.h"
using namespace DBLayer;

TEST_CASE("Field::isFloat returns correct type", "[field]") {
    SECTION("Boolean field is not float") {
        BooleanField bf("test");
        REQUIRE_FALSE(bf.isFloat());
    }
    
    SECTION("Integer field is not float") {
        IntegerField ifield("test");
        REQUIRE_FALSE(ifield.isFloat());
    }
    
    SECTION("Float field IS float") {
        FloatField ffl("test");
        REQUIRE(ffl.isFloat());
    }
    
    SECTION("String field is not float") {
        StringField sf("test");
        REQUIRE_FALSE(sf.isFloat());
    }
    
    SECTION("Date field is not float") {
        DateField df("test", "2023-01-01");
        REQUIRE_FALSE(df.isFloat());
    }
    
    SECTION("Default Field type ANY is not float") {
        Field f("test");
        f.setType(Field::ANY);
        REQUIRE_FALSE(f.isFloat());
    }
    
    SECTION("isInteger works correctly alongside isFloat") {
        FloatField ffl("test");
        REQUIRE_FALSE(ffl.isInteger());
        REQUIRE(ffl.isFloat());
    }
}

TEST_CASE("Field type introspection is consistent", "[field]") {
    SECTION("isBoolean returns true only for boolean type") {
        BooleanField bf("test");
        REQUIRE(bf.isBoolean());
        IntegerField ifield("test");
        REQUIRE_FALSE(ifield.isBoolean());
    }
    
    SECTION("isInteger returns true only for integer type") {
        IntegerField ifield("test");
        REQUIRE(ifield.isInteger());
        FloatField ffl("test");
        REQUIRE_FALSE(ffl.isInteger());
    }
    
    SECTION("isString returns true only for string type") {
        StringField sf("test");
        REQUIRE(sf.isString());
        IntegerField ifield("test");
        REQUIRE_FALSE(ifield.isString());
    }
    
    SECTION("isDate returns true only for date type") {
        DateField df("test", "2023-01-01");
        REQUIRE(df.isDate());
        IntegerField ifield("test");
        REQUIRE_FALSE(ifield.isDate());
    }
}