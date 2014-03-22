/* NAME : basetypes.h
 * AUTH : T.Kato
 * VER  : 0.0.2
 * COPY : (C) T.Kato
 * DESC : 共通で使う型・関数など
 */

#ifndef _BASETYPES_H_
#define _BASETYPES_H_

typedef unsigned short u16_t;
typedef unsigned long  u32_t;
typedef unsigned char  __u8;
typedef unsigned short __u16;
typedef unsigned long  __u32;

/// for little endian
static inline __u16 _swap16(__u16 x) {
	return (x >> 8) | (x << 8);
}
static inline __u16 cpu_to_be16(__u16 x) {
	return _swap16(x);
}
static inline __u16 be16_to_cpu(__u16 x) {
	return _swap16(x);
}
static inline __u16 cpu_to_le16(__u16 x) {
	return x;
}
static inline __u16 le16_to_cpu(__u16 x) {
	return x;
}
static inline __u32 _swap32(__u32 x) {
	return x << 24 | x >> 24 |
		(x & 0x0000FF00UL) << 8 |
		(x & 0x00FF0000UL) >> 8;
}
static inline __u32 cpu_to_be32(__u32 x) {
	return _swap32(x);
}

/// エラー番号
namespace result {
	// 0以上なら成功
	// 0未満なら失敗
	enum result_t {
		OK = 0,
		FAIL = -1,
		NO_MEMORY = -2,
		INVALID_PARAMS = -3,
		INVALID_OPERATION = -4,
		UNKNOWN = -6,
	};

	typedef int type;
	inline bool isok(type t) { return t >= 0; }
	inline bool isfail(type t) { return t < 0; }
}

#endif // _BASETYPES_H_
