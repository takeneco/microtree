/* NAME : mttypes.h
 * AUTH : T.Kato
 * VER  : 0.0.5
 * EDIT : 2008/08/24
 * COPY : (C) T.Kato
 * DESC : 
 */

#ifndef _MTTYPES_H_
#define _MTTYPES_H_

#ifndef _BASETYPES_H_
#include "basetypes.h"
#endif // _BASETYPES_H_

class mt_node;
class mt_key;
class mt_elem;
class mt_hop;
class mt_path;

/// ノードを格納する
/* 
 */
class mt_node
{
public:
	mt_node() :
		name(0),
		keys_head(0),
		elems_head(0)
//		,next(0)
	{}
	mt_node(char* n) :
		name(n),
		keys_head(0),
		elems_head(0)
//		,next(0)
	{}

	‾mt_node();

	// キーリストの最後に1キーを追加する
	void grow_keytail(mt_key* key);

	// 要素リストの最後に1要素を追加する
	void grow_elemtail(mt_elem* elem);

	// 子ノードを開く
	mt_node* open(const mt_path* rpath);

	// ノード名
	char* name;

	// キーリスト
	mt_key* keys_head;
	mt_key* keys_tail;

	// 要素リスト
	mt_elem* elems_head;
	mt_elem* elems_tail;

//	mt_node* next;
};

/// キーを格納する
/*
 */
class mt_key
{
public:
	mt_key() :
		name(0),
		value(0),
		next(0)
	{}

	mt_key(char* n, char* v) :
		name(n),
		value(v),
		next(0)
	{}

	‾mt_key();

	char* name;
	char* value;
	mt_key* next;
};

/// ノードかテキストのポインタを格納する
class mt_elem
{
	enum type_t {
		unknown_elem,
		text_elem,
		node_elem
	};
public:
	mt_elem() :
		type(unknown_elem),
		text(0),
		next(0)
	{}
	mt_elem(char* t) :
		type(text_elem),
		text(t),
		next(0)
	{}
	mt_elem(mt_node* n) :
		type(node_elem),
		node(n),
		next(0)
	{}

	‾mt_elem();

	type_t type;
	bool is_text() const { return type == text_elem; }
	bool is_node() const { return type == node_elem; }
	union {
		char* text;
		mt_node* node;
	};
	mt_elem* next;
};

/// パスの１ノード分
class mt_hop
{
public:
	mt_hop() :
		match(0),
		next(0)
	{}
	mt_hop(char* m) :
		match(m),
		next(0)
	{}

	‾mt_hop();

	char* match;

	mt_hop* next;
};

/// パス
class mt_path
{
	mt_hop* head;
	mt_hop* tail;

	// パスの長さ
	// ホップの数に等しい
	int depth;

	// relative = true なら相対パス
	// relative = false なら絶対パス
	bool relative;

public:
	mt_path() :
		head(0),
		tail(0),
		depth(0),
		relative(false)
	{}

	‾mt_path();

	// パスの最後にノードを追加する
	void grow_tail(mt_hop* hop);

	// 相対パスか絶対パスに設定する
	void set_relative(bool rel) { relative = rel; }
	// 相対パスならtrue
	// 絶対パスならfalse
	bool is_relative() const { return relative; }

	// パスの長さを返す
	int get_depth() const { return depth; }

	// 先頭のホップを返す
	mt_hop* get_head() { return head; }
	const mt_hop* get_head() const { return head; }
};


#endif // _MTTYES_H_
