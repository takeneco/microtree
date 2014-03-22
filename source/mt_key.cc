/* NAME : mt_key.cpp
 * AUTH : T.Kato
 * VER  : 0.0.1
 * COPY : (C) T.Kato
 * DESC : 
 */

#include "mttypes.h"


mt_key::~mt_key()
{
	delete [] name;
	delete [] value;
}
