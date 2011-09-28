/***************************************************************************
**                        floatfield.h  -  description
**                           -------------------
**  begin                : lun dic 2 2002
**  email                : roberto@roccoangeloni.net
**
** @copyright &copy; 2011 by Roberto Rocco Angeloni <roberto@roccoangeloni.it>
** @license http://opensource.org/licenses/lgpl-3.0.html GNU Lesser General Public License, version 3.0 (LGPLv3)
** @version $Id: floatfield.h $
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

#ifndef DBLAYER_FLOATFIELD_H
#define DBLAYER_FLOATFIELD_H

#include "importedPackages.h"

#include "dbfield.h"

/**
 * @author Roberto Rocco Angeloni
 */

namespace DBLayer {

class DLLEXPORT FloatField : public DBField  {
public:
    FloatField(const string* nome);
    FloatField(const string* nome, float valore);
    virtual ~FloatField();
    float getValue();
    void setValue(float valore);
    string toString();
    virtual Field* createNewInstance(const char* aName=0);
};

}

#endif
