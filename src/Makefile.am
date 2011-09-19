CPPFLAGS = -Wall -I$(top_srcdir)
LDFLAGS = -L../schema -lschema

# Binaries
bin_PROGRAMS = schema dblayer dblayer_test
schema_SOURCES = main_schema.cpp

LDFLAGS += -L../dblayer -ldblayer

dblayer_test_SOURCES = main_dblayer_test.cpp dbschema.cpp dbschema.h

dblayer_SOURCES = main_dblayer.cpp
