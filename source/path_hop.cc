/* NAME : mt_hop.cpp
 * AUTH : T.Kato
 * VER  : 0.0.2
 * EDIT : 2008/08/24
 * COPY : (C) T.Kato
 * DESC : 
 */

#include "mttypes.h"


mt_hop::~mt_hop()
{
	delete [] match;
}
