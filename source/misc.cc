
#include <iostream>
#include "mttypes.h"


class tab {
	int n;
	friend std::ostream& operator << (std::ostream& os, tab t) {
		for (int i = 0; i < t.n; i++)
			os << "\t";
		return os;
	}
public:
	tab(int _n) : n(_n) {}
};

void dump_node(const mt_node* node, int tabs)
{
	if (node == 0)
		return;

	if (node->name) {
		std::cout << tab(tabs) << "/" << node->name;

		for (const mt_key* key = node->keys_head; key; key = key->next) {
			std::cout
				<< '\n'
				<< tab(tabs)
				<< "  "
				<< key->name << '=' << key->value;
		}
	}

	const mt_elem* elem = node->elems_head;
	if (elem) {
		if (node->name) {
			if (node->keys_head) {
				std::cout << "\n" << tab(tabs) << "{\n";
			}
			else {
				std::cout << " {\n";
			}
		}

		while (elem) {
			if (elem->is_text()) {
				std::cout << tab(node->name ? tabs + 1 : tabs) << elem->text << "\n";
			}
			else {
				dump_node(elem->node, node->name ? tabs + 1 : tabs);
			}

			elem = elem->next;
		}

		if (node->name) {
			std::cout << tab(tabs) << "}" << std::endl;
		}
	}
	else {
		std::cout << ";" << std::endl;
	}
}
