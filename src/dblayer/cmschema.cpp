/**
 * dbschema.cpp
 */

#include "authschema.h"
using namespace AuthSchema;
#include "cmschema.h"
using namespace CMSchema;

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <boost/filesystem.hpp>

//*********************** DBELog: start.
StringVector DBELog::chiavi = {"ip","data"};
ColumnDefinitions DBELog::_columns;
StringVector DBELog::_column_order = {"ip","data","ora","count","url","note","note2"};
ColumnDefinitions DBELog::getColumns() const { return _columns; }
StringVector& DBELog::getColumnNames() const { return _column_order; }
DBELog::DBELog() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,StringVector > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["ip"] = StringVector {"uuid","not null"};
        _columns["data"] = StringVector {"date","not null default '0000-00-00'"};
        _columns["ora"] = StringVector {"time","not null default '00:00:00'"};
        _columns["count"] = StringVector {"int","not null default 0"};
        _columns["url"] = StringVector {"varchar(255)","default null"};
        _columns["note"] = StringVector {"varchar(255)","not null default ''"};
        _columns["note2"] = StringVector {"text","not null"};
    }
}
DBELog::~DBELog() {}
string& DBELog::name() const { static string ret("DBELog"); return ret; }
string DBELog::getTableName() const { return "log"; }
StringVector& DBELog::getKeys() const { return chiavi; }
DBELog* DBELog::createNewInstance() const { return new DBELog(); }
DBLayer::StringVector DBELog::getOrderBy() const {
    static DBLayer::StringVector ret({"data desc", "ora desc"});
    return ret;
}
//*********************** DBELog: end.

//*********************** DBECountry: start.
StringVector DBECountry::chiavi = {"id"};
ColumnDefinitions DBECountry::_columns;
StringVector DBECountry::_column_order = {
    "id","Common_Name","Formal_Name","Type","Sub_Type","Sovereignty","Capital"
    ,"ISO_4217_Currency_Code","ISO_4217_Currency_Name","ITU_T_Telephone_Code"
    ,"ISO_3166_1_2_Letter_Code","ISO_3166_1_3_Letter_Code","ISO_3166_1_Number"
    ,"IANA_Country_Code_TLD"
};
ColumnDefinitions DBECountry::getColumns() const { return _columns; }
StringVector& DBECountry::getColumnNames() const { return _column_order; }
DBECountry::DBECountry() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,StringVector > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["id"] = StringVector {"uuid","not null"};
        _columns["Common_Name"] = StringVector {"varchar(255)","default null"};
        _columns["Formal_Name"] = StringVector {"varchar(255)","default null"};
        _columns["Type"] = StringVector {"varchar(255)","default null"};
        _columns["Sub_Type"] = StringVector {"varchar(255)","default null"};
        _columns["Sovereignty"] = StringVector {"varchar(255)","default null"};
        _columns["Capital"] = StringVector {"varchar(255)","default null"};
        _columns["ISO_4217_Currency_Code"] = StringVector {"varchar(255)","default null"};
        _columns["ISO_4217_Currency_Name"] = StringVector {"varchar(255)","default null"};
        _columns["ITU_T_Telephone_Code"] = StringVector {"varchar(255)","default null"};
        _columns["ISO_3166_1_2_Letter_Code"] = StringVector {"varchar(255)","default null"};
        _columns["ISO_3166_1_3_Letter_Code"] = StringVector {"varchar(255)","default null"};
        _columns["ISO_3166_1_Number"] = StringVector {"varchar(255)","default null"};
        _columns["IANA_Country_Code_TLD"] = StringVector {"varchar(255)","default null"};
    }
}
DBECountry::~DBECountry() {}
string& DBECountry::name() const { static string ret("DBECountry"); return ret; }
string DBECountry::getTableName() const { return "countrylist"; }
StringVector& DBECountry::getKeys() const { return DBECountry::chiavi; }
DBECountry* DBECountry::createNewInstance() const { return new DBECountry(); }
DBLayer::StringVector DBECountry::getOrderBy() const {
    static DBLayer::StringVector ret({"id"});
    return ret;
}
bool DBECountry::init_table(DBMgr* dbmgr, bool verbose) {
    static StringVector queries = {
        "insert into schema_countrylist values ( 1,'Afghanistan','Islamic State of Afghanistan','Independent State','','','Kabul','AFN','Afghani','+93','AF','AFG','004','.af' )",
        "insert into schema_countrylist values ( 2,'Albania','Republic of Albania','Independent State','','','Tirana','ALL','Lek','+355','AL','ALB','008','.al' )",
        "insert into schema_countrylist values ( 3,'Algeria','People''s Democratic Republic of Algeria','Independent State','','','Algiers','DZD','Dinar','+213','DZ','DZA','012','.dz' )",
        "insert into schema_countrylist values ( 4,'Andorra','Principality of Andorra','Independent State','','','Andorra la Vella','EUR','Euro','+376','AD','AND','020','.ad' )",
        "insert into schema_countrylist values ( 5,'Angola','Republic of Angola','Independent State','','','Luanda','AOA','Kwanza','+244','AO','AGO','024','.ao' )",
        "insert into schema_countrylist values ( 6,'Antigua and Barbuda','','Independent State','','','Saint John''s','XCD','Dollar','+1-268','AG','ATG','028','.ag' )",
        "insert into schema_countrylist values ( 7,'Argentina','Argentine Republic','Independent State','','','Buenos Aires','ARS','Peso','+54','AR','ARG','032','.ar' )",
        "insert into schema_countrylist values ( 8,'Armenia','Republic of Armenia','Independent State','','','Yerevan','AMD','Dram','+374','AM','ARM','051','.am' )",
        "insert into schema_countrylist values ( 9,'Australia','Commonwealth of Australia','Independent State','','','Canberra','AUD','Dollar','+61','AU','AUS','036','.au' )",
        "insert into schema_countrylist values ( 10,'Austria','Republic of Austria','Independent State','','','Vienna','EUR','Euro','+43','AT','AUT','040','.at' )",
        "insert into schema_countrylist values ( 11,'Azerbaijan','Republic of Azerbaijan','Independent State','','','Baku','AZN','Manat','+994','AZ','AZE','031','.az' )",
        "insert into schema_countrylist values ( 12,'Bahamas, The','Commonwealth of The Bahamas','Independent State','','','Nassau','BSD','Dollar','+1-242','BS','BHS','044','.bs' )",
        "insert into schema_countrylist values ( 13,'Bahrain','Kingdom of Bahrain','Independent State','','','Manama','BHD','Dinar','+973','BH','BHR','048','.bh' )",
        "insert into schema_countrylist values ( 14,'Bangladesh','People''s Republic of Bangladesh','Independent State','','','Dhaka','BDT','Taka','+880','BD','BGD','050','.bd' )",
        "insert into schema_countrylist values ( 15,'Barbados','','Independent State','','','Bridgetown','BBD','Dollar','+1-246','BB','BRB','052','.bb' )",
        "insert into schema_countrylist values ( 16,'Belarus','Republic of Belarus','Independent State','','','Minsk','BYR','Ruble','+375','BY','BLR','112','.by' )",
        "insert into schema_countrylist values ( 17,'Belgium','Kingdom of Belgium','Independent State','','','Brussels','EUR','Euro','+32','BE','BEL','056','.be' )",
        "insert into schema_countrylist values ( 18,'Belize','','Independent State','','','Belmopan','BZD','Dollar','+501','BZ','BLZ','084','.bz' )",
        "insert into schema_countrylist values ( 19,'Benin','Republic of Benin','Independent State','','','Porto-Novo','XOF','Franc','+229','BJ','BEN','204','.bj' )",
        "insert into schema_countrylist values ( 20,'Bhutan','Kingdom of Bhutan','Independent State','','','Thimphu','BTN','Ngultrum','+975','BT','BTN','064','.bt' )",
        "insert into schema_countrylist values ( 21,'Bolivia','Republic of Bolivia','Independent State','','','La Paz (administrative/legislative) and Sucre (judical)','BOB','Boliviano','+591','BO','BOL','068','.bo' )",
        "insert into schema_countrylist values ( 22,'Bosnia and Herzegovina','','Independent State','','','Sarajevo','BAM','Marka','+387','BA','BIH','070','.ba' )",
        "insert into schema_countrylist values ( 23,'Botswana','Republic of Botswana','Independent State','','','Gaborone','BWP','Pula','+267','BW','BWA','072','.bw' )",
        "insert into schema_countrylist values ( 24,'Brazil','Federative Republic of Brazil','Independent State','','','Brasilia','BRL','Real','+55','BR','BRA','076','.br' )",
        "insert into schema_countrylist values ( 25,'Brunei','Negara Brunei Darussalam','Independent State','','','Bandar Seri Begawan','BND','Dollar','+673','BN','BRN','096','.bn' )",
        "insert into schema_countrylist values ( 26,'Bulgaria','Republic of Bulgaria','Independent State','','','Sofia','BGN','Lev','+359','BG','BGR','100','.bg' )",
        "insert into schema_countrylist values ( 27,'Burkina Faso','','Independent State','','','Ouagadougou','XOF','Franc','+226','BF','BFA','854','.bf' )",
        "insert into schema_countrylist values ( 28,'Burundi','Republic of Burundi','Independent State','','','Bujumbura','BIF','Franc','+257','BI','BDI','108','.bi' )",
        "insert into schema_countrylist values ( 29,'Cambodia','Kingdom of Cambodia','Independent State','','','Phnom Penh','KHR','Riels','+855','KH','KHM','116','.kh' )",
        "insert into schema_countrylist values ( 30,'Cameroon','Republic of Cameroon','Independent State','','','Yaounde','XAF','Franc','+237','CM','CMR','120','.cm' )",
        "insert into schema_countrylist values ( 31,'Canada','','Independent State','','','Ottawa','CAD','Dollar','+1','CA','CAN','124','.ca' )",
        "insert into schema_countrylist values ( 32,'Cape Verde','Republic of Cape Verde','Independent State','','','Praia','CVE','Escudo','+238','CV','CPV','132','.cv' )",
        "insert into schema_countrylist values ( 33,'Central African Republic','','Independent State','','','Bangui','XAF','Franc','+236','CF','CAF','140','.cf' )",
        "insert into schema_countrylist values ( 34,'Chad','Republic of Chad','Independent State','','','N''Djamena','XAF','Franc','+235','TD','TCD','148','.td' )",
        "insert into schema_countrylist values ( 35,'Chile','Republic of Chile','Independent State','','','Santiago (administrative/judical) and Valparaiso (legislative)','CLP','Peso','+56','CL','CHL','152','.cl' )",
        "insert into schema_countrylist values ( 36,'China, People''s Republic of','People''s Republic of China','Independent State','','','Beijing','CNY','Yuan Renminbi','+86','CN','CHN','156','.cn' )",
        "insert into schema_countrylist values ( 37,'Colombia','Republic of Colombia','Independent State','','','Bogota','COP','Peso','+57','CO','COL','170','.co' )",
        "insert into schema_countrylist values ( 38,'Comoros','Union of Comoros','Independent State','','','Moroni','KMF','Franc','+269','KM','COM','174','.km' )",
        "insert into schema_countrylist values ( 39,'Congo, Democratic Republic of the (Congo  Kinshasa)','Democratic Republic of the Congo','Independent State','','','Kinshasa','CDF','Franc','+243','CD','COD','180','.cd' )",
        "insert into schema_countrylist values ( 40,'Congo, Republic of the (Congo  Brazzaville)','Republic of the Congo','Independent State','','','Brazzaville','XAF','Franc','+242','CG','COG','178','.cg' )",
        "insert into schema_countrylist values ( 41,'Costa Rica','Republic of Costa Rica','Independent State','','','San Jose','CRC','Colon','+506','CR','CRI','188','.cr' )",
        "insert into schema_countrylist values ( 42,'Cote d''Ivoire (Ivory Coast)','Republic of Cote d''Ivoire','Independent State','','','Yamoussoukro','XOF','Franc','+225','CI','CIV','384','.ci' )",
        "insert into schema_countrylist values ( 43,'Croatia','Republic of Croatia','Independent State','','','Zagreb','HRK','Kuna','+385','HR','HRV','191','.hr' )",
        "insert into schema_countrylist values ( 44,'Cuba','Republic of Cuba','Independent State','','','Havana','CUP','Peso','+53','CU','CUB','192','.cu' )",
        "insert into schema_countrylist values ( 45,'Cyprus','Republic of Cyprus','Independent State','','','Nicosia','CYP','Pound','+357','CY','CYP','196','.cy' )",
        "insert into schema_countrylist values ( 46,'Czech Republic','','Independent State','','','Prague','CZK','Koruna','+420','CZ','CZE','203','.cz' )",
        "insert into schema_countrylist values ( 47,'Denmark','Kingdom of Denmark','Independent State','','','Copenhagen','DKK','Krone','+45','DK','DNK','208','.dk' )",
        "insert into schema_countrylist values ( 48,'Djibouti','Republic of Djibouti','Independent State','','','Djibouti','DJF','Franc','+253','DJ','DJI','262','.dj' )",
        "insert into schema_countrylist values ( 49,'Dominica','Commonwealth of Dominica','Independent State','','','Roseau','XCD','Dollar','+1-767','DM','DMA','212','.dm' )",
        "insert into schema_countrylist values ( 50,'Dominican Republic','','Independent State','','','Santo Domingo','DOP','Peso','+1-809 and 1-829','DO','DOM','214','.do' )",
        "insert into schema_countrylist values ( 51,'Ecuador','Republic of Ecuador','Independent State','','','Quito','USD','Dollar','+593','EC','ECU','218','.ec' )",
        "insert into schema_countrylist values ( 52,'Egypt','Arab Republic of Egypt','Independent State','','','Cairo','EGP','Pound','+20','EG','EGY','818','.eg' )",
        "insert into schema_countrylist values ( 53,'El Salvador','Republic of El Salvador','Independent State','','','San Salvador','USD','Dollar','+503','SV','SLV','222','.sv' )",
        "insert into schema_countrylist values ( 54,'Equatorial Guinea','Republic of Equatorial Guinea','Independent State','','','Malabo','XAF','Franc','+240','GQ','GNQ','226','.gq' )",
        "insert into schema_countrylist values ( 55,'Eritrea','State of Eritrea','Independent State','','','Asmara','ERN','Nakfa','+291','ER','ERI','232','.er' )",
        "insert into schema_countrylist values ( 56,'Estonia','Republic of Estonia','Independent State','','','Tallinn','EEK','Kroon','+372','EE','EST','233','.ee' )",
        "insert into schema_countrylist values ( 57,'Ethiopia','Federal Democratic Republic of Ethiopia','Independent State','','','Addis Ababa','ETB','Birr','+251','ET','ETH','231','.et' )",
        "insert into schema_countrylist values ( 58,'Fiji','Republic of the Fiji Islands','Independent State','','','Suva','FJD','Dollar','+679','FJ','FJI','242','.fj' )",
        "insert into schema_countrylist values ( 59,'Finland','Republic of Finland','Independent State','','','Helsinki','EUR','Euro','+358','FI','FIN','246','.fi' )",
        "insert into schema_countrylist values ( 60,'France','French Republic','Independent State','','','Paris','EUR','Euro','+33','FR','FRA','250','.fr' )",
        "insert into schema_countrylist values ( 61,'Gabon','Gabonese Republic','Independent State','','','Libreville','XAF','Franc','+241','GA','GAB','266','.ga' )",
        "insert into schema_countrylist values ( 62,'Gambia, The','Republic of The Gambia','Independent State','','','Banjul','GMD','Dalasi','+220','GM','GMB','270','.gm' )",
        "insert into schema_countrylist values ( 63,'Georgia','Republic of Georgia','Independent State','','','Tbilisi','GEL','Lari','+995','GE','GEO','268','.ge' )",
        "insert into schema_countrylist values ( 64,'Germany','Federal Republic of Germany','Independent State','','','Berlin','EUR','Euro','+49','DE','DEU','276','.de' )",
        "insert into schema_countrylist values ( 65,'Ghana','Republic of Ghana','Independent State','','','Accra','GHC','Cedi','+233','GH','GHA','288','.gh' )",
        "insert into schema_countrylist values ( 66,'Greece','Hellenic Republic','Independent State','','','Athens','EUR','Euro','+30','GR','GRC','300','.gr' )",
        "insert into schema_countrylist values ( 67,'Grenada','','Independent State','','','Saint George''s','XCD','Dollar','+1-473','GD','GRD','308','.gd' )",
        "insert into schema_countrylist values ( 68,'Guatemala','Republic of Guatemala','Independent State','','','Guatemala','GTQ','Quetzal','+502','GT','GTM','320','.gt' )",
        "insert into schema_countrylist values ( 69,'Guinea','Republic of Guinea','Independent State','','','Conakry','GNF','Franc','+224','GN','GIN','324','.gn' )",
        "insert into schema_countrylist values ( 70,'Guinea-Bissau','Republic of Guinea-Bissau','Independent State','','','Bissau','XOF','Franc','+245','GW','GNB','624','.gw' )",
        "insert into schema_countrylist values ( 71,'Guyana','Co-operative Republic of Guyana','Independent State','','','Georgetown','GYD','Dollar','+592','GY','GUY','328','.gy' )",
        "insert into schema_countrylist values ( 72,'Haiti','Republic of Haiti','Independent State','','','Port-au-Prince','HTG','Gourde','+509','HT','HTI','332','.ht' )",
        "insert into schema_countrylist values ( 73,'Honduras','Republic of Honduras','Independent State','','','Tegucigalpa','HNL','Lempira','+504','HN','HND','340','.hn' )",
        "insert into schema_countrylist values ( 74,'Hungary','Republic of Hungary','Independent State','','','Budapest','HUF','Forint','+36','HU','HUN','348','.hu' )",
        "insert into schema_countrylist values ( 75,'Iceland','Republic of Iceland','Independent State','','','Reykjavik','ISK','Krona','+354','IS','ISL','352','.is' )",
        "insert into schema_countrylist values ( 76,'India','Republic of India','Independent State','','','New Delhi','INR','Rupee','+91','IN','IND','356','.in' )",
        "insert into schema_countrylist values ( 77,'Indonesia','Republic of Indonesia','Independent State','','','Jakarta','IDR','Rupiah','+62','ID','IDN','360','.id' )",
        "insert into schema_countrylist values ( 78,'Iran','Islamic Republic of Iran','Independent State','','','Tehran','IRR','Rial','+98','IR','IRN','364','.ir' )",
        "insert into schema_countrylist values ( 79,'Iraq','Republic of Iraq','Independent State','','','Baghdad','IQD','Dinar','+964','IQ','IRQ','368','.iq' )",
        "insert into schema_countrylist values ( 80,'Ireland','','Independent State','','','Dublin','EUR','Euro','+353','IE','IRL','372','.ie' )",
        "insert into schema_countrylist values ( 81,'Israel','State of Israel','Independent State','','','Jerusalem','ILS','Shekel','+972','IL','ISR','376','.il' )",
        "insert into schema_countrylist values ( 82,'Italy','Italian Republic','Independent State','','','Rome','EUR','Euro','+39','IT','ITA','380','.it' )",
        "insert into schema_countrylist values ( 83,'Jamaica','','Independent State','','','Kingston','JMD','Dollar','+1-876','JM','JAM','388','.jm' )",
        "insert into schema_countrylist values ( 84,'Japan','','Independent State','','','Tokyo','JPY','Yen','+81','JP','JPN','392','.jp' )",
        "insert into schema_countrylist values ( 85,'Jordan','Hashemite Kingdom of Jordan','Independent State','','','Amman','JOD','Dinar','+962','JO','JOR','400','.jo' )",
        "insert into schema_countrylist values ( 86,'Kazakhstan','Republic of Kazakhstan','Independent State','','','Astana','KZT','Tenge','+7','KZ','KAZ','398','.kz' )",
        "insert into schema_countrylist values ( 87,'Kenya','Republic of Kenya','Independent State','','','Nairobi','KES','Shilling','+254','KE','KEN','404','.ke' )",
        "insert into schema_countrylist values ( 88,'Kiribati','Republic of Kiribati','Independent State','','','Tarawa','AUD','Dollar','+686','KI','KIR','296','.ki' )",
        "insert into schema_countrylist values ( 89,'Korea, Democratic People''s Republic of (North Korea)','Democratic People''s Republic of Korea','Independent State','','','Pyongyang','KPW','Won','+850','KP','PRK','408','.kp' )",
        "insert into schema_countrylist values ( 90,'Korea, Republic of  (South Korea)','Republic of Korea','Independent State','','','Seoul','KRW','Won','+82','KR','KOR','410','.kr' )",
        "insert into schema_countrylist values ( 91,'Kuwait','State of Kuwait','Independent State','','','Kuwait','KWD','Dinar','+965','KW','KWT','414','.kw' )",
        "insert into schema_countrylist values ( 92,'Kyrgyzstan','Kyrgyz Republic','Independent State','','','Bishkek','KGS','Som','+996','KG','KGZ','417','.kg' )",
        "insert into schema_countrylist values ( 93,'Laos','Lao People''s Democratic Republic','Independent State','','','Vientiane','LAK','Kip','+856','LA','LAO','418','.la' )",
        "insert into schema_countrylist values ( 94,'Latvia','Republic of Latvia','Independent State','','','Riga','LVL','Lat','+371','LV','LVA','428','.lv' )",
        "insert into schema_countrylist values ( 95,'Lebanon','Lebanese Republic','Independent State','','','Beirut','LBP','Pound','+961','LB','LBN','422','.lb' )",
        "insert into schema_countrylist values ( 96,'Lesotho','Kingdom of Lesotho','Independent State','','','Maseru','LSL','Loti','+266','LS','LSO','426','.ls' )",
        "insert into schema_countrylist values ( 97,'Liberia','Republic of Liberia','Independent State','','','Monrovia','LRD','Dollar','+231','LR','LBR','430','.lr' )",
        "insert into schema_countrylist values ( 98,'Libya','Great Socialist People''s Libyan Arab Jamahiriya','Independent State','','','Tripoli','LYD','Dinar','+218','LY','LBY','434','.ly' )",
        "insert into schema_countrylist values ( 99,'Liechtenstein','Principality of Liechtenstein','Independent State','','','Vaduz','CHF','Franc','+423','LI','LIE','438','.li' )",
        "insert into schema_countrylist values ( 100,'Lithuania','Republic of Lithuania','Independent State','','','Vilnius','LTL','Litas','+370','LT','LTU','440','.lt' )",
        "insert into schema_countrylist values ( 101,'Luxembourg','Grand Duchy of Luxembourg','Independent State','','','Luxembourg','EUR','Euro','+352','LU','LUX','442','.lu' )",
        "insert into schema_countrylist values ( 102,'Macedonia','Republic of Macedonia','Independent State','','','Skopje','MKD','Denar','+389','MK','MKD','807','.mk' )",
        "insert into schema_countrylist values ( 103,'Madagascar','Republic of Madagascar','Independent State','','','Antananarivo','MGA','Ariary','+261','MG','MDG','450','.mg' )",
        "insert into schema_countrylist values ( 104,'Malawi','Republic of Malawi','Independent State','','','Lilongwe','MWK','Kwacha','+265','MW','MWI','454','.mw' )",
        "insert into schema_countrylist values ( 105,'Malaysia','','Independent State','','','Kuala Lumpur (legislative/judical) and Putrajaya (administrative)','MYR','Ringgit','+60','MY','MYS','458','.my' )",
        "insert into schema_countrylist values ( 106,'Maldives','Republic of Maldives','Independent State','','','Male','MVR','Rufiyaa','+960','MV','MDV','462','.mv' )",
        "insert into schema_countrylist values ( 107,'Mali','Republic of Mali','Independent State','','','Bamako','XOF','Franc','+223','ML','MLI','466','.ml' )",
        "insert into schema_countrylist values ( 108,'Malta','Republic of Malta','Independent State','','','Valletta','MTL','Lira','+356','MT','MLT','470','.mt' )",
        "insert into schema_countrylist values ( 109,'Marshall Islands','Republic of the Marshall Islands','Independent State','','','Majuro','USD','Dollar','+692','MH','MHL','584','.mh' )",
        "insert into schema_countrylist values ( 110,'Mauritania','Islamic Republic of Mauritania','Independent State','','','Nouakchott','MRO','Ouguiya','+222','MR','MRT','478','.mr' )",
        "insert into schema_countrylist values ( 111,'Mauritius','Republic of Mauritius','Independent State','','','Port Louis','MUR','Rupee','+230','MU','MUS','480','.mu' )",
        "insert into schema_countrylist values ( 112,'Mexico','United Mexican States','Independent State','','','Mexico','MXN','Peso','+52','MX','MEX','484','.mx' )",
        "insert into schema_countrylist values ( 113,'Micronesia','Federated States of Micronesia','Independent State','','','Palikir','USD','Dollar','+691','FM','FSM','583','.fm' )",
        "insert into schema_countrylist values ( 114,'Moldova','Republic of Moldova','Independent State','','','Chisinau','MDL','Leu','+373','MD','MDA','498','.md' )",
        "insert into schema_countrylist values ( 115,'Monaco','Principality of Monaco','Independent State','','','Monaco','EUR','Euro','+377','MC','MCO','492','.mc' )",
        "insert into schema_countrylist values ( 116,'Mongolia','','Independent State','','','Ulaanbaatar','MNT','Tugrik','+976','MN','MNG','496','.mn' )",
        "insert into schema_countrylist values ( 117,'Montenegro','Republic of Montenegro','Independent State','','','Podgorica','EUR','Euro','+382','ME','MNE','499','.me and .yu' )",
        "insert into schema_countrylist values ( 118,'Morocco','Kingdom of Morocco','Independent State','','','Rabat','MAD','Dirham','+212','MA','MAR','504','.ma' )",
        "insert into schema_countrylist values ( 119,'Mozambique','Republic of Mozambique','Independent State','','','Maputo','MZM','Meticail','+258','MZ','MOZ','508','.mz' )",
        "insert into schema_countrylist values ( 120,'Myanmar (Burma)','Union of Myanmar','Independent State','','','Naypyidaw','MMK','Kyat','+95','MM','MMR','104','.mm' )",
        "insert into schema_countrylist values ( 121,'Namibia','Republic of Namibia','Independent State','','','Windhoek','NAD','Dollar','+264','NA','NAM','516','.na' )",
        "insert into schema_countrylist values ( 122,'Nauru','Republic of Nauru','Independent State','','','Yaren','AUD','Dollar','+674','NR','NRU','520','.nr' )",
        "insert into schema_countrylist values ( 123,'Nepal','','Independent State','','','Kathmandu','NPR','Rupee','+977','NP','NPL','524','.np' )",
        "insert into schema_countrylist values ( 124,'Netherlands','Kingdom of the Netherlands','Independent State','','','Amsterdam (administrative) and The Hague (legislative/judical)','EUR','Euro','+31','NL','NLD','528','.nl' )",
        "insert into schema_countrylist values ( 125,'New Zealand','','Independent State','','','Wellington','NZD','Dollar','+64','NZ','NZL','554','.nz' )",
        "insert into schema_countrylist values ( 126,'Nicaragua','Republic of Nicaragua','Independent State','','','Managua','NIO','Cordoba','+505','NI','NIC','558','.ni' )",
        "insert into schema_countrylist values ( 127,'Niger','Republic of Niger','Independent State','','','Niamey','XOF','Franc','+227','NE','NER','562','.ne' )",
        "insert into schema_countrylist values ( 128,'Nigeria','Federal Republic of Nigeria','Independent State','','','Abuja','NGN','Naira','+234','NG','NGA','566','.ng' )",
        "insert into schema_countrylist values ( 129,'Norway','Kingdom of Norway','Independent State','','','Oslo','NOK','Krone','+47','NO','NOR','578','.no' )",
        "insert into schema_countrylist values ( 130,'Oman','Sultanate of Oman','Independent State','','','Muscat','OMR','Rial','+968','OM','OMN','512','.om' )",
        "insert into schema_countrylist values ( 131,'Pakistan','Islamic Republic of Pakistan','Independent State','','','Islamabad','PKR','Rupee','+92','PK','PAK','586','.pk' )",
        "insert into schema_countrylist values ( 132,'Palau','Republic of Palau','Independent State','','','Melekeok','USD','Dollar','+680','PW','PLW','585','.pw' )",
        "insert into schema_countrylist values ( 133,'Panama','Republic of Panama','Independent State','','','Panama','PAB','Balboa','+507','PA','PAN','591','.pa' )",
        "insert into schema_countrylist values ( 134,'Papua New Guinea','Independent State of Papua New Guinea','Independent State','','','Port Moresby','PGK','Kina','+675','PG','PNG','598','.pg' )",
        "insert into schema_countrylist values ( 135,'Paraguay','Republic of Paraguay','Independent State','','','Asuncion','PYG','Guarani','+595','PY','PRY','600','.py' )",
        "insert into schema_countrylist values ( 136,'Peru','Republic of Peru','Independent State','','','Lima','PEN','Sol','+51','PE','PER','604','.pe' )",
        "insert into schema_countrylist values ( 137,'Philippines','Republic of the Philippines','Independent State','','','Manila','PHP','Peso','+63','PH','PHL','608','.ph' )",
        "insert into schema_countrylist values ( 138,'Poland','Republic of Poland','Independent State','','','Warsaw','PLN','Zloty','+48','PL','POL','616','.pl' )",
        "insert into schema_countrylist values ( 139,'Portugal','Portuguese Republic','Independent State','','','Lisbon','EUR','Euro','+351','PT','PRT','620','.pt' )",
        "insert into schema_countrylist values ( 140,'Qatar','State of Qatar','Independent State','','','Doha','QAR','Rial','+974','QA','QAT','634','.qa' )",
        "insert into schema_countrylist values ( 141,'Romania','','Independent State','','','Bucharest','RON','Leu','+40','RO','ROU','642','.ro' )",
        "insert into schema_countrylist values ( 142,'Russia','Russian Federation','Independent State','','','Moscow','RUB','Ruble','+7','RU','RUS','643','.ru and .su' )",
        "insert into schema_countrylist values ( 143,'Rwanda','Republic of Rwanda','Independent State','','','Kigali','RWF','Franc','+250','RW','RWA','646','.rw' )",
        "insert into schema_countrylist values ( 144,'Saint Kitts and Nevis','Federation of Saint Kitts and Nevis','Independent State','','','Basseterre','XCD','Dollar','+1-869','KN','KNA','659','.kn' )",
        "insert into schema_countrylist values ( 145,'Saint Lucia','','Independent State','','','Castries','XCD','Dollar','+1-758','LC','LCA','662','.lc' )",
        "insert into schema_countrylist values ( 146,'Saint Vincent and the Grenadines','','Independent State','','','Kingstown','XCD','Dollar','+1-784','VC','VCT','670','.vc' )",
        "insert into schema_countrylist values ( 147,'Samoa','Independent State of Samoa','Independent State','','','Apia','WST','Tala','+685','WS','WSM','882','.ws' )",
        "insert into schema_countrylist values ( 148,'San Marino','Republic of San Marino','Independent State','','','San Marino','EUR','Euro','+378','SM','SMR','674','.sm' )",
        "insert into schema_countrylist values ( 149,'Sao Tome and Principe','Democratic Republic of Sao Tome and Principe','Independent State','','','Sao Tome','STD','Dobra','+239','ST','STP','678','.st' )",
        "insert into schema_countrylist values ( 150,'Saudi Arabia','Kingdom of Saudi Arabia','Independent State','','','Riyadh','SAR','Rial','+966','SA','SAU','682','.sa' )",
        "insert into schema_countrylist values ( 151,'Senegal','Republic of Senegal','Independent State','','','Dakar','XOF','Franc','+221','SN','SEN','686','.sn' )",
        "insert into schema_countrylist values ( 152,'Serbia','Republic of Serbia','Independent State','','','Belgrade','RSD','Dinar','+381','RS','SRB','688','.rs and .yu' )",
        "insert into schema_countrylist values ( 153,'Seychelles','Republic of Seychelles','Independent State','','','Victoria','SCR','Rupee','+248','SC','SYC','690','.sc' )",
        "insert into schema_countrylist values ( 154,'Sierra Leone','Republic of Sierra Leone','Independent State','','','Freetown','SLL','Leone','+232','SL','SLE','694','.sl' )",
        "insert into schema_countrylist values ( 155,'Singapore','Republic of Singapore','Independent State','','','Singapore','SGD','Dollar','+65','SG','SGP','702','.sg' )",
        "insert into schema_countrylist values ( 156,'Slovakia','Slovak Republic','Independent State','','','Bratislava','SKK','Koruna','+421','SK','SVK','703','.sk' )",
        "insert into schema_countrylist values ( 157,'Slovenia','Republic of Slovenia','Independent State','','','Ljubljana','EUR','Euro','+386','SI','SVN','705','.si' )",
        "insert into schema_countrylist values ( 158,'Solomon Islands','','Independent State','','','Honiara','SBD','Dollar','+677','SB','SLB','090','.sb' )",
        "insert into schema_countrylist values ( 159,'Somalia','','Independent State','','','Mogadishu','SOS','Shilling','+252','SO','SOM','706','.so' )",
        "insert into schema_countrylist values ( 160,'South Africa','Republic of South Africa','Independent State','','','Pretoria (administrative), Cape Town (legislative), and Bloemfontein (judical)','ZAR','Rand','+27','ZA','ZAF','710','.za' )",
        "insert into schema_countrylist values ( 161,'Spain','Kingdom of Spain','Independent State','','','Madrid','EUR','Euro','+34','ES','ESP','724','.es' )",
        "insert into schema_countrylist values ( 162,'Sri Lanka','Democratic Socialist Republic of Sri Lanka','Independent State','','','Colombo (administrative/judical) and Sri Jayewardenepura Kotte (legislative)','LKR','Rupee','+94','LK','LKA','144','.lk' )",
        "insert into schema_countrylist values ( 163,'Sudan','Republic of the Sudan','Independent State','','','Khartoum','SDD','Dinar','+249','SD','SDN','736','.sd' )",
        "insert into schema_countrylist values ( 164,'Suriname','Republic of Suriname','Independent State','','','Paramaribo','SRD','Dollar','+597','SR','SUR','740','.sr' )",
        "insert into schema_countrylist values ( 165,'Swaziland','Kingdom of Swaziland','Independent State','','','Mbabane (administrative) and Lobamba (legislative)','SZL','Lilangeni','+268','SZ','SWZ','748','.sz' )",
        "insert into schema_countrylist values ( 166,'Sweden','Kingdom of Sweden','Independent State','','','Stockholm','SEK','Kronoa','+46','SE','SWE','752','.se' )",
        "insert into schema_countrylist values ( 167,'Switzerland','Swiss Confederation','Independent State','','','Bern','CHF','Franc','+41','CH','CHE','756','.ch' )",
        "insert into schema_countrylist values ( 168,'Syria','Syrian Arab Republic','Independent State','','','Damascus','SYP','Pound','+963','SY','SYR','760','.sy' )",
        "insert into schema_countrylist values ( 169,'Tajikistan','Republic of Tajikistan','Independent State','','','Dushanbe','TJS','Somoni','+992','TJ','TJK','762','.tj' )",
        "insert into schema_countrylist values ( 170,'Tanzania','United Republic of Tanzania','Independent State','','','Dar es Salaam (administrative/judical) and Dodoma (legislative)','TZS','Shilling','+255','TZ','TZA','834','.tz' )",
        "insert into schema_countrylist values ( 171,'Thailand','Kingdom of Thailand','Independent State','','','Bangkok','THB','Baht','+66','TH','THA','764','.th' )",
        "insert into schema_countrylist values ( 172,'Timor-Leste (East Timor)','Democratic Republic of Timor-Leste','Independent State','','','Dili','USD','Dollar','+670','TL','TLS','626','.tp and .tl' )",
        "insert into schema_countrylist values ( 173,'Togo','Togolese Republic','Independent State','','','Lome','XOF','Franc','+228','TG','TGO','768','.tg' )",
        "insert into schema_countrylist values ( 174,'Tonga','Kingdom of Tonga','Independent State','','','Nuku''alofa','TOP','Pa''anga','+676','TO','TON','776','.to' )",
        "insert into schema_countrylist values ( 175,'Trinidad and Tobago','Republic of Trinidad and Tobago','Independent State','','','Port-of-Spain','TTD','Dollar','+1-868','TT','TTO','780','.tt' )",
        "insert into schema_countrylist values ( 176,'Tunisia','Tunisian Republic','Independent State','','','Tunis','TND','Dinar','+216','TN','TUN','788','.tn' )",
        "insert into schema_countrylist values ( 177,'Turkey','Republic of Turkey','Independent State','','','Ankara','TRY','Lira','+90','TR','TUR','792','.tr' )",
        "insert into schema_countrylist values ( 178,'Turkmenistan','','Independent State','','','Ashgabat','TMM','Manat','+993','TM','TKM','795','.tm' )",
        "insert into schema_countrylist values ( 179,'Tuvalu','','Independent State','','','Funafuti','AUD','Dollar','+688','TV','TUV','798','.tv' )",
        "insert into schema_countrylist values ( 180,'Uganda','Republic of Uganda','Independent State','','','Kampala','UGX','Shilling','+256','UG','UGA','800','.ug' )",
        "insert into schema_countrylist values ( 181,'Ukraine','','Independent State','','','Kiev','UAH','Hryvnia','+380','UA','UKR','804','.ua' )",
        "insert into schema_countrylist values ( 182,'United Arab Emirates','United Arab Emirates','Independent State','','','Abu Dhabi','AED','Dirham','+971','AE','ARE','784','.ae' )",
        "insert into schema_countrylist values ( 183,'United Kingdom','United Kingdom of Great Britain and Northern Ireland','Independent State','','','London','GBP','Pound','+44','GB','GBR','826','.uk' )",
        "insert into schema_countrylist values ( 184,'United States','United States of America','Independent State','','','Washington','USD','Dollar','+1','US','USA','840','.us' )",
        "insert into schema_countrylist values ( 185,'Uruguay','Oriental Republic of Uruguay','Independent State','','','Montevideo','UYU','Peso','+598','UY','URY','858','.uy' )",
        "insert into schema_countrylist values ( 186,'Uzbekistan','Republic of Uzbekistan','Independent State','','','Tashkent','UZS','Som','+998','UZ','UZB','860','.uz' )",
        "insert into schema_countrylist values ( 187,'Vanuatu','Republic of Vanuatu','Independent State','','','Port-Vila','VUV','Vatu','+678','VU','VUT','548','.vu' )",
        "insert into schema_countrylist values ( 188,'Vatican City','State of the Vatican City','Independent State','','','Vatican City','EUR','Euro','+379','VA','VAT','336','.va' )",
        "insert into schema_countrylist values ( 189,'Venezuela','Bolivarian Republic of Venezuela','Independent State','','','Caracas','VEB','Bolivar','+58','VE','VEN','862','.ve' )",
        "insert into schema_countrylist values ( 190,'Vietnam','Socialist Republic of Vietnam','Independent State','','','Hanoi','VND','Dong','+84','VN','VNM','704','.vn' )",
        "insert into schema_countrylist values ( 191,'Yemen','Republic of Yemen','Independent State','','','Sanaa','YER','Rial','+967','YE','YEM','887','.ye' )",
        "insert into schema_countrylist values ( 192,'Zambia','Republic of Zambia','Independent State','','','Lusaka','ZMK','Kwacha','+260','ZM','ZMB','894','.zm' )",
        "insert into schema_countrylist values ( 193,'Zimbabwe','Republic of Zimbabwe','Independent State','','','Harare','ZWD','Dollar','+263','ZW','ZWE','716','.zw' )",
        "insert into schema_countrylist values ( 194,'Abkhazia','Republic of Abkhazia','Proto Independent State','','','Sokhumi','RUB','Ruble','+995','GE','GEO','268','.ge' )",
        "insert into schema_countrylist values ( 195,'China, Republic of (Taiwan)','Republic of China','Proto Independent State','','','Taipei','TWD','Dollar','+886','TW','TWN','158','.tw' )",
        "insert into schema_countrylist values ( 196,'Nagorno-Karabakh','Nagorno-Karabakh Republic','Proto Independent State','','','Stepanakert','AMD','Dram','+374-97','AZ','AZE','031','.az' )",
        "insert into schema_countrylist values ( 197,'Northern Cyprus','Turkish Republic of Northern Cyprus','Proto Independent State','','','Nicosia','TRY','Lira','+90-392','CY','CYP','196','.nc.tr' )",
        "insert into schema_countrylist values ( 198,'Pridnestrovie (Transnistria)','Pridnestrovian Moldavian Republic','Proto Independent State','','','Tiraspol','','Ruple','+373-533','MD','MDA','498','.md' )",
        "insert into schema_countrylist values ( 199,'Somaliland','Republic of Somaliland','Proto Independent State','','','Hargeisa','','Shilling','+252','SO','SOM','706','.so' )",
        "insert into schema_countrylist values ( 200,'South Ossetia','Republic of South Ossetia','Proto Independent State','','','Tskhinvali','RUB and GEL','Ruble and Lari','+995','GE','GEO','268','.ge' )",
        "insert into schema_countrylist values ( 201,'Ashmore and Cartier Islands','Territory of Ashmore and Cartier Islands','Dependency','External Territory','Australia','','','','','AU','AUS','036','.au' )",
        "insert into schema_countrylist values ( 202,'Christmas Island','Territory of Christmas Island','Dependency','External Territory','Australia','The Settlement (Flying Fish Cove)','AUD','Dollar','+61','CX','CXR','162','.cx' )",
        "insert into schema_countrylist values ( 203,'Cocos (Keeling) Islands','Territory of Cocos (Keeling) Islands','Dependency','External Territory','Australia','West Island','AUD','Dollar','+61','CC','CCK','166','.cc' )",
        "insert into schema_countrylist values ( 204,'Coral Sea Islands','Coral Sea Islands Territory','Dependency','External Territory','Australia','','','','','AU','AUS','036','.au' )",
        "insert into schema_countrylist values ( 205,'Heard Island and McDonald Islands','Territory of Heard Island and McDonald Islands','Dependency','External Territory','Australia','','','','','HM','HMD','334','.hm' )",
        "insert into schema_countrylist values ( 206,'Norfolk Island','Territory of Norfolk Island','Dependency','External Territory','Australia','Kingston','AUD','Dollar','+672','NF','NFK','574','.nf' )",
        "insert into schema_countrylist values ( 207,'New Caledonia','','Dependency','Sui generis Collectivity','France','Noumea','XPF','Franc','+687','NC','NCL','540','.nc' )",
        "insert into schema_countrylist values ( 208,'French Polynesia','Overseas Country of French Polynesia','Dependency','Overseas Collectivity','France','Papeete','XPF','Franc','+689','PF','PYF','258','.pf' )",
        "insert into schema_countrylist values ( 209,'Mayotte','Departmental Collectivity of Mayotte','Dependency','Overseas Collectivity','France','Mamoudzou','EUR','Euro','+262','YT','MYT','175','.yt' )",
        "insert into schema_countrylist values ( 210,'Saint Barthelemy','Collectivity of Saint Barthelemy','Dependency','Overseas Collectivity','France','Gustavia','EUR','Euro','+590','GP','GLP','312','.gp' )",
        "insert into schema_countrylist values ( 211,'Saint Martin','Collectivity of Saint Martin','Dependency','Overseas Collectivity','France','Marigot','EUR','Euro','+590','GP','GLP','312','.gp' )",
        "insert into schema_countrylist values ( 212,'Saint Pierre and Miquelon','Territorial Collectivity of Saint Pierre and Miquelon','Dependency','Overseas Collectivity','France','Saint-Pierre','EUR','Euro','+508','PM','SPM','666','.pm' )",
        "insert into schema_countrylist values ( 213,'Wallis and Futuna','Collectivity of the Wallis and Futuna Islands','Dependency','Overseas Collectivity','France','Mata''utu','XPF','Franc','+681','WF','WLF','876','.wf' )",
        "insert into schema_countrylist values ( 214,'French Southern and Antarctic Lands','Territory of the French Southern and Antarctic Lands','Dependency','Overseas Territory','France','Martin-de-Viviès','','','','TF','ATF','260','.tf' )",
        "insert into schema_countrylist values ( 215,'Clipperton Island','','Dependency','Possession','France','','','','','PF','PYF','258','.pf' )",
        "insert into schema_countrylist values ( 216,'Bouvet Island','','Dependency','Territory','Norway','','','','','BV','BVT','074','.bv' )",
        "insert into schema_countrylist values ( 217,'Cook Islands','','Dependency','Self-Governing in Free Association','New Zealand','Avarua','NZD','Dollar','+682','CK','COK','184','.ck' )",
        "insert into schema_countrylist values ( 218,'Niue','','Dependency','Self-Governing in Free Association','New Zealand','Alofi','NZD','Dollar','+683','NU','NIU','570','.nu' )",
        "insert into schema_countrylist values ( 219,'Tokelau','','Dependency','Territory','New Zealand','','NZD','Dollar','+690','TK','TKL','772','.tk' )",
        "insert into schema_countrylist values ( 220,'Guernsey','Bailiwick of Guernsey','Dependency','Crown Dependency','United Kingdom','Saint Peter Port','GGP','Pound','+44','GG','GGY','831','.gg' )",
        "insert into schema_countrylist values ( 221,'Isle of Man','','Dependency','Crown Dependency','United Kingdom','Douglas','IMP','Pound','+44','IM','IMN','833','.im' )",
        "insert into schema_countrylist values ( 222,'Jersey','Bailiwick of Jersey','Dependency','Crown Dependency','United Kingdom','Saint Helier','JEP','Pound','+44','JE','JEY','832','.je' )",
        "insert into schema_countrylist values ( 223,'Anguilla','','Dependency','Overseas Territory','United Kingdom','The Valley','XCD','Dollar','+1-264','AI','AIA','660','.ai' )",
        "insert into schema_countrylist values ( 224,'Bermuda','','Dependency','Overseas Territory','United Kingdom','Hamilton','BMD','Dollar','+1-441','BM','BMU','060','.bm' )",
        "insert into schema_countrylist values ( 225,'British Indian Ocean Territory','','Dependency','Overseas Territory','United Kingdom','','','','+246','IO','IOT','086','.io' )",
        "insert into schema_countrylist values ( 226,'British Sovereign Base Areas','','Dependency','Overseas Territory','United Kingdom','Episkopi','CYP','Pound','+357','','','','' )",
        "insert into schema_countrylist values ( 227,'British Virgin Islands','','Dependency','Overseas Territory','United Kingdom','Road Town','USD','Dollar','+1-284','VG','VGB','092','.vg' )",
        "insert into schema_countrylist values ( 228,'Cayman Islands','','Dependency','Overseas Territory','United Kingdom','George Town','KYD','Dollar','+1-345','KY','CYM','136','.ky' )",
        "insert into schema_countrylist values ( 229,'Falkland Islands (Islas Malvinas)','','Dependency','Overseas Territory','United Kingdom','Stanley','FKP','Pound','+500','FK','FLK','238','.fk' )",
        "insert into schema_countrylist values ( 230,'Gibraltar','','Dependency','Overseas Territory','United Kingdom','Gibraltar','GIP','Pound','+350','GI','GIB','292','.gi' )",
        "insert into schema_countrylist values ( 231,'Montserrat','','Dependency','Overseas Territory','United Kingdom','Plymouth','XCD','Dollar','+1-664','MS','MSR','500','.ms' )",
        "insert into schema_countrylist values ( 232,'Pitcairn Islands','','Dependency','Overseas Territory','United Kingdom','Adamstown','NZD','Dollar','','PN','PCN','612','.pn' )",
        "insert into schema_countrylist values ( 233,'Saint Helena','','Dependency','Overseas Territory','United Kingdom','Jamestown','SHP','Pound','+290','SH','SHN','654','.sh' )",
        "insert into schema_countrylist values ( 234,'South Georgia and the South Sandwich Islands','','Dependency','Overseas Territory','United Kingdom','','','','','GS','SGS','239','.gs' )",
        "insert into schema_countrylist values ( 235,'Turks and Caicos Islands','','Dependency','Overseas Territory','United Kingdom','Grand Turk','USD','Dollar','+1-649','TC','TCA','796','.tc' )",
        "insert into schema_countrylist values ( 236,'Northern Mariana Islands','Commonwealth of The Northern Mariana Islands','Dependency','Commonwealth','United States','Saipan','USD','Dollar','+1-670','MP','MNP','580','.mp' )",
        "insert into schema_countrylist values ( 237,'Puerto Rico','Commonwealth of Puerto Rico','Dependency','Commonwealth','United States','San Juan','USD','Dollar','+1-787 and 1-939','PR','PRI','630','.pr' )",
        "insert into schema_countrylist values ( 238,'American Samoa','Territory of American Samoa','Dependency','Territory','United States','Pago Pago','USD','Dollar','+1-684','AS','ASM','016','.as' )",
        "insert into schema_countrylist values ( 239,'Baker Island','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 240,'Guam','Territory of Guam','Dependency','Territory','United States','Hagatna','USD','Dollar','+1-671','GU','GUM','316','.gu' )",
        "insert into schema_countrylist values ( 241,'Howland Island','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 242,'Jarvis Island','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 243,'Johnston Atoll','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 244,'Kingman Reef','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 245,'Midway Islands','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 246,'Navassa Island','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 247,'Palmyra Atoll','','Dependency','Territory','United States','','','','','UM','UMI','581','' )",
        "insert into schema_countrylist values ( 248,'U.S. Virgin Islands','United States Virgin Islands','Dependency','Territory','United States','Charlotte Amalie','USD','Dollar','+1-340','VI','VIR','850','.vi' )",
        "insert into schema_countrylist values ( 249,'Wake Island','','Dependency','Territory','United States','','','','','UM','UMI','850','' )",
        "insert into schema_countrylist values ( 250,'Hong Kong','Hong Kong Special Administrative Region','Proto Dependency','Special Administrative Region','China','','HKD','Dollar','+852','HK','HKG','344','.hk' )",
        "insert into schema_countrylist values ( 251,'Macau','Macau Special Administrative Region','Proto Dependency','Special Administrative Region','China','Macau','MOP','Pataca','+853','MO','MAC','446','.mo' )",
        "insert into schema_countrylist values ( 252,'Faroe Islands','','Proto Dependency','','Denmark','Torshavn','DKK','Krone','+298','FO','FRO','234','.fo' )",
        "insert into schema_countrylist values ( 253,'Greenland','','Proto Dependency','','Denmark','Nuuk (Godthab)','DKK','Krone','+299','GL','GRL','304','.gl' )",
        "insert into schema_countrylist values ( 254,'French Guiana','Overseas Region of Guiana','Proto Dependency','Overseas Region','France','Cayenne','EUR','Euro','+594','GF','GUF','254','.gf' )",
        "insert into schema_countrylist values ( 255,'Guadeloupe','Overseas Region of Guadeloupe','Proto Dependency','Overseas Region','France','Basse-Terre','EUR','Euro','+590','GP','GLP','312','.gp' )",
        "insert into schema_countrylist values ( 256,'Martinique','Overseas Region of Martinique','Proto Dependency','Overseas Region','France','Fort-de-France','EUR','Euro','+596','MQ','MTQ','474','.mq' )",
        "insert into schema_countrylist values ( 257,'Reunion','Overseas Region of Reunion','Proto Dependency','Overseas Region','France','Saint-Denis','EUR','Euro','+262','RE','REU','638','.re' )",
        "insert into schema_countrylist values ( 258,'Aland','','Proto Dependency','','Finland','Mariehamn','EUR','Euro','+358-18','AX','ALA','248','.ax' )",
        "insert into schema_countrylist values ( 259,'Aruba','','Proto Dependency','','Netherlands','Oranjestad','AWG','Guilder','+297','AW','ABW','533','.aw' )",
        "insert into schema_countrylist values ( 260,'Netherlands Antilles','','Proto Dependency','','Netherlands','Willemstad','ANG','Guilder','+599','AN','ANT','530','.an' )",
        "insert into schema_countrylist values ( 261,'Svalbard','','Proto Dependency','','Norway','Longyearbyen','NOK','Krone','+47','SJ','SJM','744','.sj' )",
        "insert into schema_countrylist values ( 262,'Ascension','','Proto Dependency','Dependency of Saint Helena','United Kingdom','Georgetown','SHP','Pound','+247','AC','ASC','','.ac' )",
        "insert into schema_countrylist values ( 263,'Tristan da Cunha','','Proto Dependency','Dependency of Saint Helena','United Kingdom','Edinburgh','SHP','Pound','+290','TA','TAA','','' )",
        "insert into schema_countrylist values ( 264,'Antarctica','','Disputed Territory','','Undetermined','','','','','AQ','ATA','010','.aq' )",
        "insert into schema_countrylist values ( 265,'Kosovo','','Disputed Territory','','Administrated by the UN','Pristina','CSD and EUR','Dinar and Euro','+381','CS','SCG','891','.cs and .yu' )",
        "insert into schema_countrylist values ( 266,'Palestinian Territories (Gaza Strip and West Bank)','','Disputed Territory','','Administrated by Israel','Gaza City (Gaza Strip) and Ramallah (West Bank)','ILS','Shekel','+970','PS','PSE','275','.ps' )",
        "insert into schema_countrylist values ( 267,'Western Sahara','','Disputed Territory','','Administrated by Morocco','El-Aaiun','MAD','Dirham','+212','EH','ESH','732','.eh' )",
        "insert into schema_countrylist values ( 268,'Australian Antarctic Territory','','Antarctic Territory','External Territory','Australia','','','','','AQ','ATA','010','.aq' )",
        "insert into schema_countrylist values ( 269,'Ross Dependency','','Antarctic Territory','Territory','New Zealand','','','','','AQ','ATA','010','.aq' )",
        "insert into schema_countrylist values ( 270,'Peter I Island','','Antarctic Territory','Territory','Norway','','','','','AQ','ATA','010','.aq' )",
        "insert into schema_countrylist values ( 271,'Queen Maud Land','','Antarctic Territory','Territory','Norway','','','','','AQ','ATA','010','.aq' )",
        "insert into schema_countrylist values ( 272,'British Antarctic Territory','','Antarctic Territory','Overseas Territory','United Kingdom','','','','','AQ','ATA','010','.aq' )"
    };
    for(const string& q : queries) {
        if(verbose) cout << "DBECountry::init_table: q=" << q << endl;
        string q1 = replaceAll(q,"schema_",this->schemaName+"_");
        if(verbose) cout << "DBECountry::init_table: q1=" << q1 << endl;
        ResultSet* rs = dbmgr->getConnection()->exec(q1);
        if(rs!=0) {
            if(verbose) cout << "DBECountry::init_table: rs=" << rs->toString() << endl;
            delete rs;
        }
    }
    return true;
}
//*********************** DBECountry: end.

//*********************** DBEObject: start.
StringVector DBEObject::chiavi = {"id"};
ForeignKeyVector DBEObject::_fkv;
ColumnDefinitions DBEObject::_columns;
StringVector DBEObject::_column_order = {
    "id","owner","group_id","permissions"
    ,"creator","creation_date"
    ,"last_modify","last_modify_date"
    ,"deleted_by","deleted_date"
    ,"father_id"
    ,"name","description"
};
DBEObject::DBEObject() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        for(const pair<string,StringVector > pair: DBEntity::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["id"] = StringVector {"uuid","not null"};
        _columns["owner"] = StringVector {"uuid","not null"};
        _columns["group_id"] = StringVector {"uuid","not null"};

        _columns["permissions"] = StringVector {"char(9)","not null default 'rwx------'"};
        _columns["creator"] = StringVector {"uuid","not null"};
        _columns["creation_date"] = StringVector {"datetime","default null"};
        _columns["last_modify"] = StringVector {"uuid","not null"};
        _columns["last_modify_date"] = StringVector {"datetime","default null"};
        _columns["deleted_by"] = StringVector {"uuid","default null"};
        _columns["deleted_date"] = StringVector {"datetime","not null default '0000-00-00 00:00:00'"};
        _columns["father_id"] = StringVector {"uuid","default null"};
        _columns["name"] = StringVector {"varchar(255)","not null"};
        _columns["description"] = StringVector {"text","default null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEntity::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("owner","users","id"));
        _fkv.push_back(ForeignKey("group_id","groups","id"));
        _fkv.push_back(ForeignKey("creator","users","id"));
        _fkv.push_back(ForeignKey("last_modify","users","id"));
        _fkv.push_back(ForeignKey("deleted_by","users","id"));
        _fkv.push_back(ForeignKey("father_id",DBEObject::getTableName(),"id"));
    }
}
DBEObject::~DBEObject() {}
string& DBEObject::name() const { static string ret("DBEObject"); return ret; }
string DBEObject::getTableName() const { return "objects"; }
DBEObject* DBEObject::createNewInstance() const { return new DBEObject(); }
StringVector& DBEObject::getKeys() const { return chiavi; }
ForeignKeyVector& DBEObject::getFK() const { return _fkv; }
ColumnDefinitions DBEObject::getColumns() const { return _columns; }
StringVector& DBEObject::getColumnNames() const { return _column_order; }
DBLayer::StringVector DBEObject::getOrderBy() const {
    static DBLayer::StringVector ret({"name"});
    return ret;
}

string DBEObject::_getTodayString() {
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string ret("");
//    $oggi = $oggi_array['year'] . "/"
//     . ( strlen($oggi_array['mon'])<2 ? "0" : "" ) . $oggi_array['mon'] . "/"
//     . ( strlen($oggi_array['mday'])<2 ? "0" : "" ) . $oggi_array['mday'] . " "
//     . ( strlen($oggi_array['hours'])<2 ? "0" : "" ) . $oggi_array['hours'] . ":"
//     . ( strlen($oggi_array['minutes'])<2 ? "0" : "" ) . $oggi_array['minutes'];
    string month = SchemaNS::integer2string((long) now->tm_mon+1); if(month.length()<2) month="0"+month;
    string day = SchemaNS::integer2string((long) now->tm_mday+1); if(day.length()<2) day="0"+day;
    string hour = SchemaNS::integer2string((long) now->tm_hour); if(hour.length()<2) hour="0"+hour;
    string minute = SchemaNS::integer2string((long) now->tm_min); if(minute.length()<2) minute="0"+minute;
    string secs = SchemaNS::integer2string((long) now->tm_sec); if(secs.length()<2) secs="0"+secs;
    ret.append(SchemaNS::integer2string((long) now->tm_year + 1900))
        .append("-").append(month)
        .append("-").append(SchemaNS::integer2string((long) now->tm_mday))
        .append(" ")
        .append(hour).append(":").append(minute).append(":").append(secs);
    return ret;
}
string DBEObject::getId() const { return this->getField("id")==0 || this->getField("id")->isNull() ? "" : *(this->getField("id")->getStringValue()); }
DBEObject* DBEObject::setId(const string s) { this->setValue("id",s); return this; }
string DBEObject::getName() const { return this->getField("name")==0 || this->getField("name")->isNull() ? "" : *(this->getField("name")->getStringValue()); }
DBEObject* DBEObject::setName(const string s) { this->setValue("name",s); return this; }
string DBEObject::getDescription() const { return this->getField("description")==0 || this->getField("description")->isNull() ? "" : *(this->getField("description")->getStringValue()); }
DBEObject* DBEObject::setDescription(const string s) { this->setValue("description",s); return this; }
string DBEObject::getOwnerId() const { return this->getField("owner")==0 || this->getField("owner")->isNull() ? "" : *(this->getField("owner")->getStringValue()); }
DBEObject* DBEObject::setOwnerId(const string s) { this->setValue("owner",s); return this; }
string DBEObject::getGroupId() const { return this->getField("group_id")==0 || this->getField("group_id")->isNull() ? "" : *(this->getField("group_id")->getStringValue()); }
DBEObject* DBEObject::setGroupId(const string s) { this->setValue("group_id",s); return this; }
bool DBEObject::isDeleted() const { return this->getField("deleted_date")==0 || this->getField("deleted_date")->isNull() || *(this->getField("deleted_date")->getStringValue())=="0000-00-00 00:00:00" ? false : true; }
bool DBEObject::canRead(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(0+0)=='r';
    } else if(kind=="G") {
        return perms.at(0+3)=='r';
    } else {
        return perms.at(0+6)=='r';
    }
}
bool DBEObject::canWrite(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(1+0)=='w';
    } else if(kind=="G") {
        return perms.at(1+3)=='w';
    } else {
        return perms.at(1+6)=='w';
    }
}
bool DBEObject::canExecute(const string kind) const {
    string perms = this->getField("permissions")==0 || this->getField("permissions")->isNull() ? "" : *(this->getField("permissions")->getStringValue());
    if(!(perms.length()>0)) return true;
    if(kind=="U") {
        return perms.at(2+0)=='x';
    } else if(kind=="G") {
        return perms.at(2+3)=='x';
    } else {
        return perms.at(2+6)=='x';
    }
}
DBEObject* DBEObject::setDefaultValues(ObjectMgr* dbmgr) {
    cout << "DBEObject::setDefaultValues: start." << endl;
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        if(this->getField("owner")==0 || this->getField("owner")->isNull() || this->getField("owner")->getStringValue()->length()==0)
            this->setValue("owner",myuser->getId());
        if(this->getField("group_id")==0 || this->getField("group_id")->isNull() || this->getField("group_id")->getStringValue()->length()==0)
            this->setValue("group_id",myuser->getStringValue("group_id"));
        this->setValue("creator",myuser->getId());
        this->setValue("last_modify",myuser->getId());
    }
    string today = this->_getTodayString();
    this->setValue("creation_date",today);
    this->setValue("last_modify_date",today);
    this->setValue("deleted_date","0000-00-00 00:00:00");

    if(this->getField("father_id")==0 || this->getField("father_id")->isNull()) {
        this->setValue("father_id","0"); // TODO verify this

        if(this->getField("fk_obj_id")!=0 && !this->getField("fk_obj_id")->isNull()) {
            DBEObject* fkobj = dbmgr->objectById(this->getStringValue("id"));
            if(fkobj!=0) {
                this->setValue("group_id",fkobj->getStringValue("group_id"));
                this->setValue("permissions",fkobj->getStringValue("permissions"));
                this->setValue("father_id",this->getStringValue("fk_obj_id"));
                delete fkobj;
            }
        }
    } else {
        DBEObject* father = dbmgr->objectById(this->getStringValue("id"));
        if(father!=0) {
            this->setValue("group_id",father->getStringValue("group_id"));
            this->setValue("permissions",father->getStringValue("permissions"));
            delete father;
        }
    }
    cout << "DBEObject::setDefaultValues: end." << endl;
    return this;
}
void DBEObject::_before_insert(DBMgr* dbmgr) {
    string myid = dbmgr->getNextUuid(this);
    this->setValue("id",myid);
    this->setDefaultValues((ObjectMgr*) dbmgr);
}
void DBEObject::_before_update(DBMgr* dbmgr) {
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        this->setValue("last_modify",myuser->getId());
    }
    this->setValue("last_modify_date",this->_getTodayString());
}
void DBEObject::_before_delete(DBMgr* dbmgr) {
    if(this->isDeleted()) return;
    DBEUser* myuser = (DBEUser*) dbmgr->getDBEUser();
    if(myuser!=0) {
        this->setValue("deleted_by",myuser->getId());
    }
    this->setValue("deleted_date",this->_getTodayString());
}
//*********************** DBEObject: end.


//*********************** ObjectManager: start.
ObjectMgr::ObjectMgr() : DBMgr::DBMgr() {}
ObjectMgr::ObjectMgr(Connection* con, bool verbose) : DBMgr::DBMgr(con,verbose) {}
ObjectMgr::~ObjectMgr() {
    if( this->verbose ) { cout << "ObjectMgr::~ObjectMgr: start." << endl; }
    if( this->verbose ) { cout << "ObjectMgr::~ObjectMgr: end." << endl; }
}
bool ObjectMgr::canRead(const DBEObject& obj) const {
    bool ret = false;
    if(obj.canRead())
        ret = true;
    else if(obj.canRead("G") && this->hasGroup( obj.getGroupId() ) )
        ret = true;
    else if(obj.canRead("U")) {
        DBEUser* myuser = (DBEUser*) this->getDBEUser();
        if(myuser!=0 && myuser->getId()==obj.getOwnerId()) {
            ret = true;
        }
    }
    return ret;
}
bool ObjectMgr::canWrite(const DBEObject& obj) const {
    bool ret = false;
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->getId()==obj.getStringValue("creator")) {
        ret = true;
    } else {
        if(obj.canWrite())
            ret = true;
        else if(obj.canWrite("G") && this->hasGroup( obj.getGroupId() ) )
            ret = true;
        else if(obj.canWrite("U") && myuser!=0 && myuser->getId()==obj.getOwnerId())
            ret = true;
    }
    return ret;
}
bool ObjectMgr::canExecute(const DBEObject& obj) const {
    bool ret = false;
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->getId()==obj.getStringValue("creator")) {
        ret = true;
    } else {
        if(obj.canExecute())
            ret = true;
        else if(obj.canExecute("G") && this->hasGroup( obj.getGroupId() ) )
            ret = true;
        else if(obj.canExecute("U") && myuser!=0 && myuser->getId()==obj.getOwnerId())
            ret = true;
    }
    return ret;
}
DBEntityVector* ObjectMgr::Select(const string &tableName, const string &searchString) {
    DBEntityVector* tmp = DBMgr::Select(tableName, searchString);
    if( this->verbose ) cout << "ObjectMgr::Select: tmp.size=" << tmp->size() << endl;
    DBEUser* myuser = (DBEUser*) this->getDBEUser();
    if(myuser!=0 && myuser->isRoot())
        return tmp;

    DBEntityVector* ret = new DBEntityVector();
    for(DBEntity* dbe : (*tmp) ) {
        bool append = false;
        DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
        if( obj!=0 ) {
            if( this->verbose ) cout << "ObjectMgr::Select: obj=" << obj->toString() << endl;
            if(myuser!=0 && myuser->getId()==obj->getStringValue("creator")) {
                append = true;
            } else if( obj->isDeleted() ) {
                append = false;
            } else if(this->canRead(*obj)) {
                append = true;
            }
        } else {
            append = true;
        }
        if(append)
            ret->push_back(dbe);
        else
            delete dbe;
    }
    delete tmp;
    return ret;
}
DBEntity* ObjectMgr::Insert(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if( obj!=0 ) {
        have_permission = this->canWrite(*obj);
    }
    return have_permission ? DBMgr::Insert(dbe) : 0;
}
DBEntity* ObjectMgr::Update(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if( obj!=0 ) {
        have_permission = this->canWrite(*obj);
    }
    return have_permission ? DBMgr::Update(dbe) : 0;
}
DBEntity* ObjectMgr::Delete(DBEntity *dbe) {
    bool have_permission = true;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    DBEObject* full_obj = 0;
    if( obj!=0 ) {
        // FIXME do we really need to retrieve again the object?
        full_obj = this->fullObjectById( obj->getId(), false );
        have_permission = this->canWrite(*full_obj);
        delete full_obj;
    }
    if( have_permission ) {
        if( obj==0 || obj->isDeleted() ) {
            return DBMgr::Delete(dbe);
        } else {
            //this->connect();
            obj->_before_delete( this );
            string sqlString = this->_buildUpdateString( obj );
            if( this->verbose ) cout << "ObjectMgr.delete: sqlString = " << sqlString << endl;
            ResultSet* rs = this->getConnection()->exec( sqlString );
            delete rs;
            obj->_after_delete( this );
            return obj;
        }
    } else {
        return dbe;
    }
}
string ObjectMgr::_buildSelectString(DBEntity *dbe, bool uselike, bool caseSensitive) {
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: start." << endl;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if(obj==0 || obj->name()!="DBEObject") {
        if( this->verbose ) cout << "ObjectMgr::_buildSelectString: dbe is not a DBEObject" << endl;
        if( this->verbose ) cout << "ObjectMgr::_buildSelectString: end." << endl;
        return DBMgr::_buildSelectString(dbe, uselike, caseSensitive);
    }
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: TODO" << endl;
    //string searchString = DBMgr::_buildSelectString(dbe, uselike, caseSensitive);
    DBEntityVector types = this->getRegisteredTypes();
    StringVector q;
    for(const DBEntity* mytype : types) {
        DBEntity* mydbe = mytype->createNewInstance();
        DBEObject* myobj = dynamic_cast<DBEObject*>(mydbe);
        if(myobj==0 || myobj->name()=="DBEObject") {
            delete mydbe;
            continue;
        }
        myobj->setValuesDictionary( dbe->getValuesDictionary() );
        q.push_back( replaceAll( DBMgr::_buildSelectString(myobj, uselike, caseSensitive),
                                "select * ",
                                "select '"+myobj->name()+"' as classname,id,owner,group_id,permissions,creator,creation_date,last_modify,last_modify_date,deleted_by,deleted_date,father_id,name,description "
                                )
                     );
        delete mydbe;
    }
    string searchString = joinString(&q,string(" union "));
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: searchString=" << searchString << endl;
    if( this->verbose ) cout << "ObjectMgr::_buildSelectString: end." << endl;
    return searchString;
}

DBEntityVector* ObjectMgr::Search(DBEntity* dbe, bool uselike, bool caseSensitive, const string& orderBy,bool ignore_deleted, bool full_object) {
    if( this->verbose ) cout << "ObjectMgr::Search: start." << endl;
    DBEObject* obj = dynamic_cast<DBEObject*>(dbe);
    if(obj==0 || obj->name()!="DBEObject") {
        if( this->verbose ) cout << "ObjectMgr::Search: dbe is not a DBEObject" << endl;
        if( this->verbose ) cout << "ObjectMgr::Search: end." << endl;
        return DBMgr::Search(dbe, uselike, caseSensitive,orderBy);
    }
    if( this->verbose ) cout << "ObjectMgr::Search: TODO" << endl;
    if( ignore_deleted==true ) obj->setDateValue("deleted_date","0000-00-00 00:00:00");
    DBEntityVector* tmp = DBMgr::Search(dbe,uselike,caseSensitive,orderBy);
    if(!full_object) {
        if( this->verbose ) cout << "ObjectMgr::Search: not full object." << endl;
        if( this->verbose ) cout << "ObjectMgr::Search: end." << endl;
        return tmp;
    }
    DBEntityVector* ret = new DBEntityVector();
    // FIXME optimize this
    for(const DBEntity* _obj : *tmp) {
        DBEntity* search = this->getClazzByTypeName(_obj->getStringValue("classname"));
        search->setValue("id",_obj->getStringValue("id"));
        DBEntityVector* tmplist = this->Search(search,false);
        delete search;
        if(tmplist->size()!=1) {
            this->Destroy(tmplist);
            continue;
        }
        ret->push_back(tmplist->at(0));
        delete tmplist;
    }
    return ret;
}
DBEObject* ObjectMgr::objectById(const string id, const bool ignore_deleted) {
    if( this->verbose ) cout << "ObjectMgr::objectById: start." << endl;
    DBEntityVector types = this->getRegisteredTypes();
    StringVector q;
    for(const DBEntity* mytype : types) {
        DBEntity* mydbe = mytype->createNewInstance();
        DBEObject* myobj = dynamic_cast<DBEObject*>(mydbe);
        if(myobj==0 || myobj->name()=="DBEObject") {
            delete mydbe;
            continue;
        }
        q.push_back( "select '"+myobj->name()+"' as classname,id,owner,group_id,permissions,creator,creation_date,last_modify,last_modify_date,deleted_by,deleted_date,father_id,name,description"
                   + "  from " + this->_buildTableName(myobj)
                   + " where id='"+DBEntity::hex2uuid(id)+"'"
                   +(ignore_deleted?" and deleted_date='0000-00-00 00:00:00'":"")
                     );
        delete mydbe;
    }
    string searchString = joinString(&q,string(" union "));
    if( this->verbose ) cout << "ObjectMgr::objectById: searchString=" << searchString << endl;
    DBEntityVector* mylist = this->Select("objects", searchString);
    DBEObject* ret = 0;
    if( this->verbose ) cout << "ObjectMgr::objectById: mylist=" << mylist->size() << endl;
    if(mylist->size()==1) {
        DBEntity* tmpret = mylist->at(0);
        ret = dynamic_cast<DBEObject*>(tmpret);
        if(ret==0) {
            delete tmpret;
        }
        if( this->verbose ) cout << "ObjectMgr::objectById: ret=" << ret->toString("\n") << endl;
        delete mylist;
    } else {
        this->Destroy(mylist);
    }
    if( this->verbose ) cout << "ObjectMgr::objectById: end." << endl;
    return ret;
}
DBEObject* ObjectMgr::fullObjectById(const string id, const bool ignore_deleted) {
//     cout << "ObjectMgr::fullObjectById: start." << endl;
    DBEObject* myobj = this->objectById(id,ignore_deleted);
    if(myobj==0) {
//         cout << "ObjectMgr::fullObjectById: end." << endl;
        return 0;
    }
    DBEntity* search = this->getClazzByTypeName(myobj->getStringValue("classname"));
    search->setValue("id",myobj->getId());
    // FIXME this looks like a bit redundant, maybe there is a smarter way to do it
    DBEntityVector* mylist = this->Search(search,false,false,"",ignore_deleted);
    delete search;
    delete myobj;
    DBEObject* ret = 0;
    if(mylist->size()==1) {
        DBEntity* tmpret = mylist->at(0);
        ret = dynamic_cast<DBEObject*>(tmpret);
        if(ret==0) {
            delete tmpret;
        }
        delete mylist;
    } else {
        this->Destroy(mylist);
    }
//     cout << "ObjectMgr::fullObjectById: end." << endl;
    return ret;
}
DBEObject* ObjectMgr::objectByName(const string name, const bool ignore_deleted) {
    if( this->verbose ) cout << "ObjectMgr::objectByName: start." << endl;
    DBEntityVector types = this->getRegisteredTypes();
    StringVector q;
    for(const DBEntity* mytype : types) {
        DBEntity* mydbe = mytype->createNewInstance();
        DBEObject* myobj = dynamic_cast<DBEObject*>(mydbe);
        if(myobj==0 || myobj->name()=="DBEObject") {
            delete mydbe;
            continue;
        }
        q.push_back( "select '"+myobj->name()+"' as classname,id,owner,group_id,permissions,creator,creation_date,last_modify,last_modify_date,deleted_by,deleted_date,father_id,name,description"
                   + "  from " + this->_buildTableName(myobj)
                   + " where name='"+name+"'"
                   +(ignore_deleted?" and deleted_date='0000-00-00 00:00:00'":"")
                     );
        delete mydbe;
    }
    string searchString = joinString(&q,string(" union "));
    if( this->verbose ) cout << "ObjectMgr::objectByName: searchString=" << searchString << endl;
    DBEntityVector* mylist = this->Select("objects", searchString);
    DBEObject* ret = 0;
    if(mylist->size()==1) {
        DBEntity* tmpret = mylist->at(0);
        ret = dynamic_cast<DBEObject*>(tmpret);
        if(ret==0) {
            delete tmpret;
        }
        delete mylist;
    } else {
        this->Destroy(mylist);
    }
    if( this->verbose ) cout << "ObjectMgr::objectByName: end." << endl;
    return ret;
}
DBEObject* ObjectMgr::fullObjectByName(const string name, const bool ignore_deleted) {
    cout << "ObjectMgr::fullObjectByName: start." << endl;
    DBEObject* myobj = this->objectByName(name,ignore_deleted);
    if(myobj==0) {
        cout << "ObjectMgr::fullObjectByName: end." << endl;
        return 0;
    }
    DBEntity* search = this->getClazzByTypeName(myobj->getStringValue("classname"));
    search->setValue("id",myobj->getId());
    // FIXME this looks like a bit redundant, maybe there is a smarter way to do it
    DBEntityVector* mylist = this->Search(search,false,false,"",ignore_deleted);
    delete search;
    delete myobj;
    DBEObject* ret = 0;
    if(mylist->size()==1) {
        DBEntity* tmpret = mylist->at(0);
        ret = dynamic_cast<DBEObject*>(tmpret);
        if(ret==0) {
            delete tmpret;
        }
        delete mylist;
    } else {
        this->Destroy(mylist);
    }
    cout << "ObjectMgr::fullObjectByName: end." << endl;
    return ret;
}
//*********************** ObjectManager: end.




//*********************** DBECompany: start.
ForeignKeyVector DBECompany::_fkv;
ColumnDefinitions DBECompany::_columns;
StringVector DBECompany::_column_order = {"street","zip","city","state","fk_countrylist_id","phone","fax","email","url","p_iva"};
DBECompany::DBECompany() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBECompany::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
//         for(const string& s : _column_order) column_order.push_back(s);
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["street"] = StringVector {"varchar(255)","default null"};
        _columns["zip"] = StringVector {"varchar(255)","default null"};
        _columns["city"] = StringVector {"varchar(255)","default null"};
        _columns["state"] = StringVector {"varchar(255)","default null"};
        _columns["fk_countrylist_id"] = StringVector {"uuid","default null"};
        _columns["phone"] = StringVector {"varchar(255)","default null"};
        _columns["fax"] = StringVector {"varchar(255)","default null"};
        _columns["email"] = StringVector {"varchar(255)","default null"};
        _columns["url"] = StringVector {"varchar(255)","default null"};
        _columns["p_iva"] = StringVector {"varchar(16)","default null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
//         _fkv.push_back(ForeignKey("father_id",DBECompany::getTableName(),"id"));
        _fkv.push_back(ForeignKey("fk_countrylist_id","countrylist","id"));
    }
}
DBECompany::~DBECompany() {}
string& DBECompany::name() const { static string ret("DBECompany"); return ret; }
string DBECompany::getTableName() const { return "companies"; }
ForeignKeyVector& DBECompany::getFK() const { return _fkv; }
ColumnDefinitions DBECompany::getColumns() const { return _columns; }
StringVector& DBECompany::getColumnNames() const { return _column_order; }
DBECompany* DBECompany::createNewInstance() const { return new DBECompany(); }
//*********************** DBECompany: end.

//*********************** DBEPeople: start.
ForeignKeyVector DBEPeople::_fkv;
ColumnDefinitions DBEPeople::_columns;
StringVector DBEPeople::_column_order = {"street","zip","city","state","fk_countrylist_id","fk_companies_id","fk_users_id","phone","office_phone","mobile","fax","email","url","codice_fiscale","p_iva"};
DBEPeople::DBEPeople() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBEPeople::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["street"] = StringVector {"varchar(255)","default null"};
        _columns["zip"] = StringVector {"varchar(255)","default null"};
        _columns["city"] = StringVector {"varchar(255)","default null"};
        _columns["state"] = StringVector {"varchar(255)","default null"};
        _columns["fk_countrylist_id"] = StringVector {"uuid","default null"};
        _columns["fk_companies_id"] = StringVector {"uuid","default null"};
        _columns["fk_users_id"] = StringVector {"uuid","default null"};
        _columns["phone"] = StringVector {"varchar(255)","default null"};
        _columns["office_phone"] = StringVector {"varchar(255)","default null"};
        _columns["mobile"] = StringVector {"varchar(255)","default null"};
        _columns["fax"] = StringVector {"varchar(255)","default null"};
        _columns["email"] = StringVector {"varchar(255)","default null"};
        _columns["url"] = StringVector {"varchar(255)","default null"};
        _columns["codice_fiscale"] = StringVector {"varchar(20)","default null"};
        _columns["p_iva"] = StringVector {"varchar(16)","default null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_countrylist_id","countrylist","id"));
        _fkv.push_back(ForeignKey("fk_companies_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_users_id","users","id"));
    }
}
DBEPeople::~DBEPeople() {}
string &DBEPeople::name() const { static string ret("DBEPeople"); return ret; }
string DBEPeople::getTableName() const { return "people"; }
ForeignKeyVector& DBEPeople::getFK() const { return _fkv; }
ColumnDefinitions DBEPeople::getColumns() const { return _columns; }
StringVector& DBEPeople::getColumnNames() const { return _column_order; }
DBEPeople* DBEPeople::createNewInstance() const { return new DBEPeople(); }
//*********************** DBEPeople: end.

//*********************** DBEEvent: start.
ForeignKeyVector DBEEvent::_fkv;
ColumnDefinitions DBEEvent::_columns;
StringVector DBEEvent::_column_order = {
    "fk_obj_id"
    ,"start_date","end_date","all_day"
    ,"url"
    ,"alarm","alarm_minute","alarm_unit","before_event"
    ,"category"
    ,"recurrence","recurrence_type"
    ,"daily_every_x"
    ,"weekly_every_x","weekly_day_of_the_week"
    ,"monthly_every_x"
        ,"monthly_day_of_the_month"
        ,"monthly_week_number","monthly_week_day"

        ,"yearly_month_number","yearly_month_day"
        ,"yearly_week_number","yearly_week_day"
        ,"yearly_day_of_the_year"
    ,"recurrence_times","recurrence_end_date"
};
DBEEvent::DBEEvent() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBEEvent::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};

        _columns["start_date"] = StringVector {"datetime","not null default '0000-00-00 00:00:00'"};
        _columns["end_date"] = StringVector {"datetime","not null default '0000-00-00 00:00:00'"};
        _columns["all_day"] = StringVector {"char(1)","not null default '1'"}; // Bool - An all day event?

        _columns["url"] = StringVector {"varchar(255)","default null"}; // A url

        _columns["alarm"] = StringVector {"char(1)","default '0'"}; // Bool - Signal an alarm before?
        _columns["alarm_minute"] = StringVector {"int","default 0"}; // Num. time unit
        _columns["alarm_unit"] = StringVector {"char(1)","default '0'"}; // Time unit 0-2 => minutes, hours, days
        _columns["before_event"] = StringVector {"char(1)","default '0'"}; // 0=before event starts 1=after

        _columns["category"] = StringVector {"varchar(255)","default ''"}; // Event category

        // Recurrence
        _columns["recurrence"] = StringVector {"char(1)","default '0'"}; // Bool - Recurrence active?
        _columns["recurrence_type"] = StringVector {"char(1)","default '0'"}; // 0=Daily, 1=Weekly, 2=monthly, 3=yearly
        // 0: daily
        _columns["daily_every_x"] = StringVector {"int","default 0"}; // every_x_days
        // 1: weekly
        _columns["weekly_every_x"] = StringVector {"int","default 0"};
        _columns["weekly_day_of_the_week"] = StringVector {"char(1)","default '0'"}; // 0=monday ... 6=sunday
        // 2: monthly
        _columns["monthly_every_x"] = StringVector {"int","default 0"}; // every x months
        //  1) n-th day of the month
        _columns["monthly_day_of_the_month"] = StringVector {"int","default 0"}; // 0=do not, -5...-1,1 ... 31
        //  2) n-th week on monday
        _columns["monthly_week_number"] = StringVector {"int","default 0"}; // 0=do not, 1...5
        _columns["monthly_week_day"] = StringVector {"char(1)","default '0'"}; // 0=monday ... 6=sunday
        // 3: Yearly
        //  1) every day XX of month MM
        _columns["yearly_month_number"] = StringVector {"int","default 0"}; // 0=do not, 1...12
        _columns["yearly_month_day"] = StringVector {"int","default 0"}; // 0=do not, 1...31
        //  2) every first monday of june
        //_columns["yearly_month_number"] = StringVector {"int","default 0"}; // 0=do not, 1...12
        _columns["yearly_week_number"] = StringVector {"int","default 0"}; // 0=do not 1...5
        _columns["yearly_week_day"] = StringVector {"char(1)","default '0'"}; // 0=monday ... 6=sunday
        //  3) every n-th day of the year
        _columns["yearly_day_of_the_year"] = StringVector {"int","default 0"}; // 0=do not, 1...366
        // Recurrence range
        _columns["recurrence_times"] = StringVector {"int","default 0"}; // 0=always 1...N times
        // Recurrence until <date>
        _columns["recurrence_end_date"] = StringVector {"datetime","not null default '0000-00-00 00:00:00'"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_obj_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","folders","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","people","id"));
//         _fkv.push_back(ForeignKey("fk_obj_id","projects","id"));
    }
}
DBEEvent::~DBEEvent() {}
string &DBEEvent::name() const { static string ret("DBEEvent"); return ret; }
string DBEEvent::getTableName() const { return "events"; }
ForeignKeyVector& DBEEvent::getFK() const { return _fkv; }
ColumnDefinitions DBEEvent::getColumns() const { return _columns; }
StringVector& DBEEvent::getColumnNames() const { return _column_order; }
DBEEvent* DBEEvent::createNewInstance() const { return new DBEEvent(); }
//*********************** DBEEvent: end.




//*********************** DBEFile: start.
ForeignKeyVector DBEFile::_fkv;
ColumnDefinitions DBEFile::_columns;
StringVector DBEFile::_column_order = {"fk_obj_id","path","filename","checksum","mime","alt_link"};
DBEFile::DBEFile() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBEFile::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};
        _columns["path"] = StringVector {"text","default null"};
        _columns["filename"] = StringVector {"text","not null"};
        _columns["checksum"] = StringVector {"char(40)","default null"};
        _columns["mime"] = StringVector {"varchar(255)","default null"};
        _columns["alt_link"] = StringVector {"varchar(255)","not null default ''"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("father_id","folders","id"));
        _fkv.push_back(ForeignKey("father_id","pages","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","pages","id"));
    }
}
DBEFile::~DBEFile() {}
string &DBEFile::name() const { static string ret("DBEFile"); return ret; }
string DBEFile::getTableName() const { return "files"; }
ForeignKeyVector& DBEFile::getFK() const { return _fkv; }
ColumnDefinitions DBEFile::getColumns() const { return _columns; }
StringVector& DBEFile::getColumnNames() const { return _column_order; }
DBEFile* DBEFile::createNewInstance() const { return new DBEFile(); }

string DBEFile::_root_directory;
DBEFile* DBEFile::setRootDirectory(const string& dir) { _root_directory = dir; return this; }
string DBEFile::getRootDirectory() const { return _root_directory; }
DBEFile* DBEFile::setFilename(const string& f) {
    this->setValue("filename",f);
    if(this->getName().length()==0)
        this->setName(f);
}
string DBEFile::getFilename() const { return this->getField("filename")==0 || this->getField("filename")->isNull() ? "" : *(this->getField("filename")->getStringValue()); }
string DBEFile::createFilename(const string& aId, const string& aFilename) const {
    string filename = aFilename.size()==0 ? this->getFilename() : aFilename;
    string id = aId.size()==0 ? this->getId() : aId;
    string prefix = "r_" + id + "_";
    if(filename.find(prefix)!=filename.npos)
        filename = replaceAll(filename, prefix, "");
    return filename.insert(0,prefix);
}
string DBEFile::createObjectPath(DBEFile* an_obj) {
    DBEFile* obj = an_obj!=0 ? an_obj : this;
    string dest_path = obj->getStringValue("path");
    string father_id = obj->getStringValue("father_id");

    if(father_id.length()>0) {
        if(dest_path.length()>0) father_id.append("/");
        dest_path.insert(0,father_id);
    }
    return dest_path;
}
string DBEFile::getFullpath(DBEFile* an_obj) {
    DBEFile* obj = an_obj!=0 ? an_obj : this;
    string dest_path = obj->createObjectPath();
    string dest_dir(DBEFile::_root_directory.c_str());
    if(dest_path.length()>0)
        dest_dir.append("/").append(dest_path.c_str());
    dest_dir.append("/").append(obj->getFilename().c_str());
    return dest_dir;
}
bool DBEFile::readFile(const string& src_file, bool move) {
    bool ret = false;
    boost::filesystem::path src_path(src_file);
    if(!exists(src_path)) {
        cerr << "DBEFile::readFile: source does not exists " << src_path << endl;
        return ret;
    }
    this->setFilename(src_path.filename().string());
    string fullpath = this->getFullpath();
    boost::filesystem::path dst_path(fullpath);
    cout << "DBEFile::readFile: parent path '" << dst_path.parent_path() << "'" << endl;
    if(!boost::filesystem::create_directories( dst_path.parent_path() )) {
        cerr << "DBEFile::readFile: unable to create path " << dst_path.parent_path() << endl;
        return false;
    }
    if(move) {
        cout << "DBEFile::readFile: moving '" << src_file << "' to '" << fullpath << "'" << endl;
        boost::filesystem::rename(src_path, dst_path);
    } else {
        cout << "DBEFile::readFile: copying '" << src_file << "' to '" << fullpath << "'" << endl;
        boost::filesystem::copy(src_path, dst_path);
    }
    ret = true;
    return ret;
}
void DBEFile::_before_insert(DBMgr* dbmgr) {
    DBEObject::_before_insert(dbmgr);

    // Inherit the father's root
    this->_inherith_father_root(dynamic_cast<ObjectMgr*>(dbmgr));

//     // Aggiungo il prefisso al nome del file
//     if( $this->getValue('filename')>'' ) {
//         $dest_path = $this->generaObjectPath();
//         $from_dir=realpath($GLOBALS['root_directory'].'/'.$this->dest_directory);
//         $dest_dir=realpath($GLOBALS['root_directory'].'/'.$this->dest_directory);
//         if($dest_path>'') $dest_dir.="/$dest_path";
//         if(!file_exists($dest_dir)) mkdir($dest_dir, 0755 );
//         // con basename() ottengo solo il nome del file senza il path relativo nel quale e' stato caricato
//         $nuovo_filename = $this->generaFilename($this->getValue('id'), basename($this->getValue('filename')));
//         rename( $from_dir."/".$this->getValue('filename'), $dest_dir."/".$nuovo_filename );
//         if( !($this->getValue('name')>'') ) $this->setValue('name',basename($this->getValue('filename')) );
//         $this->setValue('filename', $nuovo_filename);
//     }
//     // Checksum
//     $_fullpath = $this->getFullpath();
//     if(file_exists($_fullpath)) {
//         $newchecksum = sha1_file( $_fullpath );
//         $this->setValue('checksum',$newchecksum);
//     } else {
//         $this->setValue('checksum',"File '".$this->getValue('filename')."' not found!");
//     }
//     // Mime type
//     if(file_exists($_fullpath)) {
//         if(function_exists('finfo_open')) {
//             $finfo = finfo_open(FILEINFO_MIME);
//             if (!$finfo) {
//                 if(function_exists('mime_content_type'))
//                     $this->setValue('mime',mime_content_type($_fullpath));
//                 else
//                     $this->setValue('mime','text/plain');
//                 return;
//             }
//             $this->setValue('mime',finfo_file($finfo,$_fullpath));
//             finfo_close($finfo);
//         } elseif(function_exists('mime_content_type'))
//             $this->setValue('mime',mime_content_type($_fullpath));
//         else
//             $this->setValue('mime','text/plain');
//     } else {
//         $this->setValue('mime','text/plain');
//     }
//     // Image
//     if($this->isImage())
//         $this->createThumbnail($_fullpath);
}
void DBEFile::_inherith_father_root(ObjectMgr* objmgr) {
//     cout << "DBEFile::_inherith_father_root: start." << endl;
    string father_id = this->getStringValue("father_id");
//     cout << "DBEFile::_inherith_father_root: father_id=" << father_id << endl;
    if(father_id.size()>0) {
        ForeignKeyVector fkv = this->getFKForColumn("father_id");
        for(const ForeignKey& fk : fkv) {
            DBEntity* clazz = objmgr->getClazz(fk.tabella_riferita);
            string mytablename = objmgr->buildTableName(clazz);
            delete clazz;
            string query = "select fk_obj_id from " + mytablename + " where id='" + father_id + "'";
//             cout << "DBEFile::_inherith_father_root: query=" << query << endl;
            DBEntityVector* tmp = objmgr->Select(fk.tabella_riferita,query);
            if(tmp->size()==1) {
                DBEntity* tmpret = tmp->at(0);
                this->setValue("fk_obj_id", tmpret->getStringValue("fk_obj_id"));
//                 cout << "DBEFile::_inherith_father_root: fk_obj_id=" << this->getStringValue("fk_obj_id") << endl;
            }
            objmgr->Destroy(tmp);
            if(tmp->size()==1)
                break;
        }
    }
//     cout << "DBEFile::_inherith_father_root: end." << endl;
}
//*********************** DBEFile: end.

//*********************** DBEFolder: start.
ForeignKeyVector DBEFolder::_fkv;
ColumnDefinitions DBEFolder::_columns;
StringVector DBEFolder::_column_order = {"fk_obj_id","childs_sort_order"};
DBEFolder::DBEFolder() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBEFolder::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};
        _columns["childs_sort_order"] = StringVector {"text","default null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_obj_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","people","id"));
//         _fkv.push_back(ForeignKey("fk_obj_id","projects","id"));
    }
}
DBEFolder::~DBEFolder() {}
string &DBEFolder::name() const { static string ret("DBEFolder"); return ret; }
string DBEFolder::getTableName() const { return "folders"; }
ForeignKeyVector& DBEFolder::getFK() const { return _fkv; }
ColumnDefinitions DBEFolder::getColumns() const { return _columns; }
StringVector& DBEFolder::getColumnNames() const { return _column_order; }
DBEFolder* DBEFolder::createNewInstance() const { return new DBEFolder(); }
void DBEFolder::_inherith_father_root(ObjectMgr* objmgr) {
//     cout << "DBEFolder::_inherith_father_root: start." << endl;
    string father_id = this->getStringValue("father_id");
//     cout << "DBEFolder::_inherith_father_root: father_id=" << father_id << endl;
    if(father_id.size()>0) {
        ForeignKeyVector fkv = this->getFKForColumn("father_id");
        for(const ForeignKey& fk : fkv) {
            DBEntity* clazz = objmgr->getClazz(fk.tabella_riferita);
            string mytablename = objmgr->buildTableName(clazz);
            delete clazz;
            string query = "select fk_obj_id from " + mytablename + " where id='" + father_id + "'";
//             cout << "DBEFolder::_inherith_father_root: query=" << query << endl;
            DBEntityVector* tmp = objmgr->Select(fk.tabella_riferita,query);
            if(tmp->size()==1) {
                DBEntity* tmpret = tmp->at(0);
                this->setValue("fk_obj_id", tmpret->getStringValue("fk_obj_id"));
//                 cout << "DBEFolder::_inherith_father_root: fk_obj_id=" << this->getStringValue("fk_obj_id") << endl;
            }
            objmgr->Destroy(tmp);
            if(tmp->size()==1)
                break;
        }
    }
//     cout << "DBEFolder::_inherith_father_root: end." << endl;
}
DBEObject* DBEFolder::setDefaultValues(ObjectMgr* objmgr) {
//     cout << "DBEFolder::setDefaultValues: start." << endl;
    DBEObject::setDefaultValues(objmgr);
    this->_inherith_father_root(objmgr);
//     cout << "DBEFolder::setDefaultValues: end." << endl;
    return this;
}
// void DBEFolder::_before_insert(DBMgr* dbmgr) {
//     cout << "DBEFolder::_before_insert: start." << endl;
//     DBEObject::_before_insert(dbmgr);
//     cout << "DBEFolder::_before_insert: ===========================" << endl;
//     this->_inherith_father_root(dynamic_cast<ObjectMgr*>(dbmgr));
//     cout << "DBEFolder::_before_insert: end." << endl;
// }
void DBEFolder::_before_update(DBMgr* dbmgr) {
    cout << "DBEFolder::_before_update: start." << endl;
    DBEObject::_before_update(dbmgr);
    this->_inherith_father_root(dynamic_cast<ObjectMgr*>(dbmgr));
    cout << "DBEFolder::_before_update: end." << endl;
}
//*********************** DBEFolder: end.

//*********************** DBELink: start.
ForeignKeyVector DBELink::_fkv;
ColumnDefinitions DBELink::_columns;
StringVector DBELink::_column_order = {"fk_obj_id","href","target"};
DBELink::DBELink() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBELink::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};
        _columns["href"] = StringVector {"varchar(255)","not null"};
        _columns["target"] = StringVector {"varchar(255)","default '_blank'"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_obj_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","folders","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","pages","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","people","id"));

        _fkv.push_back(ForeignKey("father_id","pages","id"));
    }
}
DBELink::~DBELink() {}
string &DBELink::name() const { static string ret("DBELink"); return ret; }
string DBELink::getTableName() const { return "links"; }
DBELink* DBELink::createNewInstance() const { return new DBELink(); }
ForeignKeyVector& DBELink::getFK() const { return _fkv; }
ColumnDefinitions DBELink::getColumns() const { return _columns; }
StringVector& DBELink::getColumnNames() const { return _column_order; }
//*********************** DBELink: end.

//*********************** DBENote: start.
ForeignKeyVector DBENote::_fkv;
ColumnDefinitions DBENote::_columns;
StringVector DBENote::_column_order = {"fk_obj_id"};
DBENote::DBENote() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBENote::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_obj_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","folders","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","people","id"));
    }
}
DBENote::~DBENote() {}
string &DBENote::name() const { static string ret("DBENote"); return ret; }
string DBENote::getTableName() const { return "notes"; }
DBENote* DBENote::createNewInstance() const { return new DBENote(); }
ForeignKeyVector& DBENote::getFK() const { return _fkv; }
ColumnDefinitions DBENote::getColumns() const { return _columns; }
StringVector& DBENote::getColumnNames() const { return _column_order; }
//*********************** DBENote: end.

//*********************** DBEPage: start.
ForeignKeyVector DBEPage::_fkv;
ColumnDefinitions DBEPage::_columns;
StringVector DBEPage::_column_order = {"fk_obj_id","html","language"};
DBEPage::DBEPage() {
    this->tableName.clear();
    this->schemaName = CMSchema::getSchema();
    if(_columns.size()==0) {
        StringVector column_order = DBEPage::getColumnNames();
        StringVector parentColumns = DBEObject::getColumnNames();
        for(size_t i=(parentColumns.size()-1); i>=0 && i<parentColumns.size(); i--) {
            column_order.insert(column_order.begin(),parentColumns.at(i));
        }
        _column_order = column_order;
        for(const pair<string,StringVector > pair: DBEObject::getColumns()) {
            _columns[pair.first] = pair.second;
        }
        _columns["fk_obj_id"] = StringVector {"uuid","default null"};
        _columns["html"] = StringVector {"text","default null"};
        _columns["language"] = StringVector {"varchar(5)","default 'en_us'"};
    }
    if(_fkv.size()==0) {
        for(const DBLayer::ForeignKey& fk : DBEObject::getFK()) { _fkv.push_back(fk); }
        _fkv.push_back(ForeignKey("fk_obj_id","companies","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","folders","id"));
        _fkv.push_back(ForeignKey("fk_obj_id","people","id"));
    }
}
DBEPage::~DBEPage() {}
string &DBEPage::name() const { static string ret("DBEPage"); return ret; }
string DBEPage::getTableName() const { return "pages"; }
DBEPage* DBEPage::createNewInstance() const { return new DBEPage(); }
ForeignKeyVector& DBEPage::getFK() const { return _fkv; }
ColumnDefinitions DBEPage::getColumns() const { return _columns; }
StringVector& DBEPage::getColumnNames() const { return _column_order; }
//*********************** DBEPage: end.

void CMSchema::registerClasses(DBEFactory* dbeFactory) {
    dbeFactory->registerClass("log", new DBELog() );
    dbeFactory->registerClass("objects", new DBEObject() );
    dbeFactory->registerClass("countrylist", new DBECountry() );
    dbeFactory->registerClass("companies", new DBECompany() );
    dbeFactory->registerClass("people", new DBEPeople() );
    dbeFactory->registerClass("events", new DBEEvent() );
    dbeFactory->registerClass("files", new DBEFile() );
    dbeFactory->registerClass("folders", new DBEFolder() );
    dbeFactory->registerClass("links", new DBELink() );
    dbeFactory->registerClass("notes", new DBENote() );
    dbeFactory->registerClass("pages", new DBEPage() );
}

void CMSchema::checkDB(DBMgr& dbmgr, bool verbose) {
    if(verbose) cout << "CMSchema::checkDB: start." << endl;

    if(dbmgr.getConnection()->isProxy()) {
        cerr << "CMSchema::checkDB: WARNING - remote databases cannot be checked." << endl;
        return;
    }
    if(!dbmgr.connect()) {
        cerr << "CMSchema::checkDB: ERROR - UNABLE TO CONNECT TO DB!!!" << endl;
        cerr << dbmgr.getConnection()->getErrorMessage() << endl;
        return;
    }


    // 1. Check app version
    long current_db_version = -1;
    DBEDBVersion* dbecurrentversion = 0;
    DBEDBVersion* cerca = new DBEDBVersion();
    cerca->setValue("model_name", CMSchema::getSchema());
    DBEntityVector* res = dbmgr.Search(cerca,false);
    if(verbose) cout << "CMSchema::checkDB: res.size=" << res->size() << endl;
    if(dbmgr.getErrorMessage().length()==0 && res->size()>0) {
        if(verbose) {
            cout << "Results (" << typeid(res).name() << "):" << endl;
            for(const auto& elem : (*res)) {
                cout << "- " << elem->toString() << endl;
            }
        }
        dbecurrentversion = (DBEDBVersion*) res->at(0);
        res->clear();
        if(verbose) cout << "CMSchema::checkDB: " << dbecurrentversion->toString("\n") << endl;
        current_db_version = dbecurrentversion->version();
        delete dbecurrentversion; dbecurrentversion = 0;
        if(verbose) cout << "CMSchema::checkDB: current_db_version=" << current_db_version << endl;
    } else if(dbmgr.getErrorMessage().length()>0) {
        cerr << "CMSchema::checkDB: " << dbmgr.getErrorMessage() << endl;
    }
    dbmgr.Destroy(res);
    delete cerca;

//     printf("\n");
//     printf("==================================================\n");
//     printf("Field Creati: %d\n",   SchemaNS::getFieldCreati() );
//     printf("Field Distrutti: %d\n",SchemaNS::getFieldDistrutti() );
//     printf("Schemi Creati: %d\n",   SchemaNS::getSchemiCreati() );
//     printf("Schemi Distrutti: %d\n",SchemaNS::getSchemiDistrutti() );
//     printf("==================================================\n");
//     printf("\n");

    if(verbose) cout << "CMSchema::checkDB: current_db_version=" << current_db_version << endl;

    // Lambda :-)
    std::function<string(const string&)> lambda_dbeType2dbType = [&dbmgr] (const string& s) mutable -> string { return dbmgr.getConnection()->dbeType2dbType(s); };
    std::function<string(const string&)> lambda_getClazzSchema = [&dbmgr] (const string& s) mutable -> string {
        DBEntity* dbe = dbmgr.getClazz(s);
        string ret = dbe->getSchemaName();
        delete dbe;
        return ret;
    };

    // drop table dblayer_dbversion,auth_groups,auth_users,auth_users_groups;

    // 2. Do the DB migration
    long current_migration = -1;
    if(current_db_version<0) {
        string sql;
        bool use_fk = dbmgr.getConnection()->getDBType()!="MYSQL";// && dbmgr.getConnection()->getDBType()!="SQLite";
        DBEDBVersion* dbversion = new DBEDBVersion();
        sql = dbversion->toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
        delete dbversion;
        dbmgr.getConnection()->exec(sql);
        if(verbose) cout << sql << endl;

        StringVector classes = {"log","objects","countrylist","companies","people"
                                ,"events"
                                ,"files","folders","links"
                                ,"notes","pages"};
        for(const string clazz : classes) {
            DBEntity* dbe = dbmgr.getClazz(clazz);
            sql = dbe->toSql(lambda_dbeType2dbType,lambda_getClazzSchema,"\n",use_fk);
            dbmgr.getConnection()->exec(sql);
            if(verbose) cout << sql << endl;
            //dbmgr.Delete(dbe);
            if(dbmgr.getConnection()->hasErrors()) {
                cerr << "CMSchema::checkDB: ERROR=" << dbmgr.getConnection()->getErrorMessage() << endl;
                cerr << "   sql = " << sql << endl;
                cerr << "   dbe = " << dbe->toString("   \n") << endl;
            }
            delete dbe;
            if(dbmgr.getConnection()->hasErrors()) {
                break;
            }
        }

        if(verbose) cout << "CMSchema::checkDB: init countries..." << endl;
        DBECountry* country = new DBECountry();
//         country->init_table(&dbmgr);
        delete country;
        if(verbose) cout << "CMSchema::checkDB: init countries... done." << endl;
//        dbegroup.setValue("id","-1")->setValue("name","Admin")->setValue("description","System admins"); dbmgr.Insert(&dbegroup);
//        dbegroup.setValue("id","-2")->setValue("name","Users")->setValue("description","System users");  dbmgr.Insert(&dbegroup);
//        dbegroup.setValue("id","-3")->setValue("name","Guests")->setValue("description","System guests (read only)"); dbmgr.Insert(&dbegroup);

    }
    current_migration++;

    if(verbose) cout << "CMSchema::checkDB: current_migration=" << current_migration << endl;
    if(verbose) cout << "CMSchema::checkDB: current_db_version=" << current_db_version << endl;

    // 3. Write version to DB
    if(current_db_version<current_migration) {
        dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.getClazzByTypeName("DBEDBVersion");
        dbecurrentversion->setValue("model_name", CMSchema::getSchema())
                        ->setValue("version",current_migration);
        if(!dbmgr.exists(dbecurrentversion))
            dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.Insert(dbecurrentversion);
        else
            dbecurrentversion = (AuthSchema::DBEDBVersion*) dbmgr.Update(dbecurrentversion);
        if(verbose) cout << dbecurrentversion->toString("\n") << endl;
//         dbecurrentversion->setValue("version",current_migration);
//         dbmgr.Update(dbecurrentversion);
        delete dbecurrentversion;
    }
    if(verbose) cout << "CMSchema::checkDB: end." << endl;
}
