#include <catch2/catch_all.hpp>
#include "../dblayer/dblayer.h"
#include "../schema/schema.h"
using namespace DBLayer;
using namespace SchemaNS;

TEST_CASE("integer2string produces correct output", "[utility]") {
    SECTION("positive small number") {
        string result = DBLayer::integer2string(5);
        REQUIRE(result == "5");
    }
    
    SECTION("positive medium number") {
        string result = DBLayer::integer2string(123);
        REQUIRE(result == "123");
    }
    
    SECTION("positive large number") {
        string result = DBLayer::integer2string(99999);
        REQUIRE(result == "99999");
    }
    
    SECTION("zero") {
        string result = DBLayer::integer2string(0);
        REQUIRE(result == "0");
    }
    
    SECTION("negative number") {
        string result = DBLayer::integer2string(-42);
        REQUIRE(result == "-42");
    }
    
    SECTION("large negative number") {
        string result = DBLayer::integer2string(-12345);
        REQUIRE(result == "-12345");
    }
    
    SECTION("number at boundary of 10") {
        string result = DBLayer::integer2string(10);
        REQUIRE(result == "10");
    }
}

TEST_CASE("integer2string from schema namespace works", "[utility][schema]") {
    SECTION("SchemaNS::integer2string(42) == \"42\"") {
        string result = SchemaNS::integer2string(42);
        REQUIRE(result == "42");
    }
    
    SECTION("SchemaNS::integer2string(-1) == \"-1\"") {
        string result = SchemaNS::integer2string(-1);
        REQUIRE(result == "-1");
    }
    
    SECTION("SchemaNS::integer2string(0) == \"0\"") {
        string result = SchemaNS::integer2string(0);
        REQUIRE(result == "0");
    }
}