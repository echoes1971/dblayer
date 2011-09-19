/***************************************************************************
**	foreignkey  v0.0.1 - gio apr 6 2006
**	---------------------------------------------------
**
**	Author:		Roberto Rocco Angeloni.
**	E-mail:			roberto@roccoangeloni.it
**	Comment:
**	To Do:
**	Future:
**	History:
**		v1.0 - 2006.03.31	...
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: foreignkey.cpp $
** @package rproject::dblayer
**
** Permission to use, copy, modify, and distribute this software for any
** purpose with or without fee is hereby granted, provided that the above
** copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
** ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
** OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
****************************************************************************/

#include "foreignkey.h"
using namespace DBLayer;

ForeignKey::ForeignKey(string colonna_fk, string tabella_riferita, string colonna_riferita) {
	this->colonna_fk = colonna_fk;
	this->tabella_riferita = tabella_riferita;
	this->colonna_riferita = colonna_riferita;
}

ForeignKey::~ForeignKey() {
//	delete this->colonna_fk;
//	delete this->tabella_riferita;
//	delete this->colonna_riferita;
}
