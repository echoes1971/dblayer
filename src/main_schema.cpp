#include <config.h>

#include <cstdio>

#include <iostream>
#include <typeinfo>
using namespace std;

#include "schema/field.h"
#include "schema/booleanfield.h"
#include "schema/datefield.h"
#include "schema/floatfield.h"
#include "schema/integerfield.h"
#include "schema/stringfield.h"
#include "schema/schema.h"
using namespace SchemaNS;

void testDateField() {
// 	DateField dateField = DateField( new string("cippa"),
    string* nome = new string("cippa");
    string* dataora = new string("1970-02-01 00:00:00");
    string* data2 = new string("1971-07-01");
    string* data3 = new string("2038-01-19 00:00:00");

    DateField dateField = DateField( nome,
                                     dataora );
    cout << "string: " << dateField.toString() << endl;
    cout << "seconds: " << dateField.to_seconds() << endl;
    cout << "days: " << dateField.to_days() << endl;

    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    // My birthday
    dateField.setValue( data2 );
    cout << "string: " << dateField.toString() << endl;
    cout << "seconds: " << dateField.to_seconds() << endl;
    cout << "days: " << dateField.to_days() << endl;
    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    dateField.setValue( data3 );
    cout << "date: " << dateField.toString() << endl;
    dateField.setValue( dateField.to_seconds() );
    cout << "seconds2date: " << dateField.toString() << endl;

    delete nome;
    delete dataora;
    delete data2;
    delete data3;
}

void testSchema() {
    string toString;

    string nomeSchema("Prova");
    Schema* schema = new Schema(&nomeSchema);

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    string nomeCampo("cippa");
    schema->setValue( &nomeCampo, true );

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    nomeCampo = "lippa";
    schema->setValue( &nomeCampo, 300L );

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    nomeCampo = "lippo";
    schema->setValue( &nomeCampo, (float) 245.678 );

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    nomeCampo = "lippi";
    string miaStringa("xxxxx");
    schema->setValue( &nomeCampo, &miaStringa );

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    nomeCampo = "cippa_data";
    string miaData("2006-01-14 00:00:00");
    schema->setDateValue( &nomeCampo, &miaData );

    toString = schema->toString("\n",false);
    printf("testSchema: schema=%s\n", toString.c_str() );

    Schema* copia=schema->clone();
    toString = copia->toString("\n",false);
    printf("testSchema: copia=%s\n", toString.c_str() );
    printf("testSchema: copia==schema?\n" );
    if( (*copia) == (*schema) )
        printf("testSchema: copia==schema\n" );
    else
        printf("testSchema: copia!=schema\n" );

    delete copia;
    delete schema;
}

int main() {

    bool cippa = true;
    long lippa = 7;
    float lippo = 5.3;

    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );

    printf("testSchema: cippa = %s\n", typeid(cippa).name() );
    printf("testSchema: lippa = %s\n", typeid(lippa).name() );
    printf("testSchema: lippo = %s\n", typeid(lippo).name() );

    cout << "--------------------------> testSchema" << endl;
    for(int i=0; i<5; i++) {
            printf("--------------------------> testSchema[%d]\n",i);
            testSchema();
    }
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );

    cout << "--------------------------> DateField" << endl;
    testDateField();
    printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
    printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );

    return 0;
}
