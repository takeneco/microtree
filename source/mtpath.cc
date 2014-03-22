/* NAME : mt_path.h
 * AUTH : T.Kato
 * VER  : 0.0.4
 * EDIT : 2008/08/24
 * COPY : (C) T.Kato
 * DESC : mt_pathクラスの定義
 */

#include "mttypes.h"


mt_path::‾mt_path()
{
}

void mt_path::grow_tail(mt_hop *hop)
{
	if (!hop) {
		return;
	}
	else if (!head) {
		head = tail = hop;
	}
	else {
		tail->next = hop;
		tail = hop;
	}

	depth++;
}
