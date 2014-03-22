/* NAME : mt_node.cpp
 * AUTH : T.Kato
 * VER  : 0.0.2
 * EDIT : 2008/08/24
 * COPY : (C) T.Kato
 * DESC : 
 */

#include <cstring>
#include "mttypes.h"


mt_node::‾mt_node()
{
	delete [] name;

	mt_key* key = keys_head;
	while (key) {
		mt_key* tmp = key;
		key = key->next;
		delete tmp;
	}

	mt_elem* elem = elems_head;
	while (elem) {
		mt_elem* tmp = elem;
		elem = elem->next;
		delete tmp;
	}
}

// キーリストの最後に1キーを追加する
void mt_node::grow_keytail(mt_key* key)
{
	if (!key) {
	}
	else if (!keys_head) {
		keys_head = keys_tail = key;
	}
	else {
		keys_tail->next = key;
		keys_tail = key;
	}
}

// 要素リストの最後に1要素を追加する
void mt_node::grow_elemtail(mt_elem* elem)
{
	if (!elem) {
	}
	else if (!elems_head) {
		elems_head = elems_tail = elem;
	}
	else {
		elems_tail->next = elem;
		elems_tail = elem;
	}
}

mt_node* mt_node::open(const mt_path* rpath)
{
	if (!rpath || !rpath->is_relative()) {
		return 0;
	}

	mt_elem* elem = elems_head;
	const mt_hop* hop = rpath->get_head();
	while (elem) {
		if (elem->is_node() && std::strcmp(elem->node->name, hop->match) == 0) {
			if (hop->next) {
				elem = elem->node->elems_head;
				if (hop->next)
					hop = hop->next;
				else
					break;
			}
			else {
				break;
			}
		}
		else {
			elem = elem->next;
		}
	}

	return elem->node;
}
