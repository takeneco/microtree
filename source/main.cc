/* NAME : main.cpp
 * AUTH : T.Kato
 * VER  : 0.0.5
 * EDIT : 2008/08/24
 * COPY : (C) T.Kato
 * DESC : main関数の定義
 */

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include "basetypes.h"
#include "mttypes.h"

//#undef yyFlexLexer
//#define yyFlexLexer mtFlexLexer
//#define yyFlexLexer mtpathFlexLexer
//#include <FlexLexer.h>

// MT解析機
int mt_parse(mt_node**, std::istream* instr = 0);
// MTPath解析機
int mtpath_parse(mt_path**, std::istream* instr = 0);

#include "mttypes.h"

int yyparse(void*);
void dump_node(const mt_node*, int tabs);

char* dupli_text(const char* src, size_t length)
{
	char* dest = new char[length + 1];
	std::strcpy(dest, src);

	return dest;
}

char* concat_text(char* text1, char* text2)
{
	size_t len1 = std::strlen(text1);
	size_t len2 = std::strlen(text2);
	char* result = new char[len1 + len2 + 1];

	memcpy(result, text1, len1);
	memcpy(result + len1, text2, len2);
	result[len1 + len2] = '¥0';

	return result;
}
/*
void bin_save()
{
	dbt_block block;

	block.initialize(512, 10);

	std::ofstream ofs("test.db");

	ofs << block;

	ofs.close();
}
*/
int main(int argc, char* argv[])
{
	std::ifstream src;

	if (argc < 2) {
		abort();
	}

	src.open(argv[1]);
	if (src.fail()) {
		std::cout << "Could not open." << std::endl;
		abort();
	}

	// Test code for MT
	mt_node* rootnode;

	if (result::isfail(mt_parse(&rootnode, &src))) {
		std::cerr << "error occured." << std::endl;
		return 1;
	}

	src.close();

	dump_node(rootnode, 0);


	char buf[1024];
	std::cin.getline(buf, sizeof buf);

	std::istringstream path_str(buf);

	// Test code for MTPath
	mt_path* path;

	if (result::isfail(mtpath_parse(&path, &path_str))) {
		std::cerr << "error occured."
			<< std::endl;
		return 1;
	}

	std::cout << "relative : " << path->is_relative()
		<< "¥ndepth : " << path->get_depth()
		<< std::endl;
	for (mt_hop* hop = path->get_head(); hop; hop = hop->next) {
		std::cout << " ¥"" << hop->match << "¥"" << std::endl;
	}

	mt_node* child = rootnode->open(path);
	if (child) {
		std::cout << "seleted:" << std::endl;
		dump_node(child, 0);
	} else {
		std::cout << "seleted: null." << std::endl;
	}

/*
	extern FlexLexer* mtpathlexer;
	mtpathlexer = new mtpathFlexLexer;

	path* p;
	if (mtpathparse(&p)) {
		fprintf(stderr, "error occued.¥n");
		return 1;
	}
*/

	return EXIT_SUCCESS;
}
