========================================================================
    DYNAMIC LINK LIBRARY : dblayer Project Overview
========================================================================


MYSQL Support
=============

1. Download the connector from: http://dev.mysql.com/downloads/connector/c/

2. In dblayer -> Properties -> Configuration Properties -> C/C++
	-> Preprocessor -> Preprocessor Definitions
	append ;USE_MYSQL

3. In dblayer -> Properties -> Configuration Properties -> C/C++
	-> General -> Additional Include Directories
	add C:\Program Files\MySQL\MySQL Connector C 6.0.2\include
