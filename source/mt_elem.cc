/* NAME : mt_key.cpp
 * AUTH : T.Kato
 * VER  : 0.0.1
 * COPY : (C) T.Kato
 * DESC : 
 */

#include "mttypes.h"


mt_elem::~mt_elem()
{
	is_text() ?
		delete [] text :
		delete node;
}
