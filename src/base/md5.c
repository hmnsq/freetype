/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * (This is a heavily cut-down "BSD license".)
 *
 * This differs from Colin Plumb's older public domain implementation in that
 * no exactly 32-bit integer data type is required (any 32-bit or wider
 * unsigned integer data type will do), there's no compile-time endianness
 * configuration, and the function prototypes match OpenSSL's.  No code from
 * Colin Plumb's implementation has been reused; this comment merely compares
 * the properties of the two independent implementations.
 *
 * The primary goals of this implementation are portability and ease of use.
 * It is meant to be fast, but not as fast as possible.  Some known
 * optimizations are not included to reduce source code size and avoid
 * compile-time configuration.
 */

#که_نه_تعریف_شوی HAVE_OPENSSL

#ضمن <string.h>

#ضمن "md5.h"

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#تعریف_کړه F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#تعریف_کړه G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#تعریف_کړه H(x, y, z)			(((x) ^ (y)) ^ (z))
#تعریف_کړه H2(x, y, z)			((x) ^ ((y) ^ (z)))
#تعریف_کړه I(x, y, z)			((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#تعریف_کړه ګام(f, a, b, c, d, x, t, s) \
	(a) += f((b), (c), (d)) + (x) + (t); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
	(a) += (b);

/*
 * SET reads 4 input bytes in little-endian byte order and stores them in a
 * properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned memory
 * accesses is just an optimization.  Nothing will break if it fails to detect
 * a suitable architecture.
 *
 * Unfortunately, this optimization may be a C strict aliasing rules violation
 * if the caller's data buffer has effective type that cannot be aliased by
 * MD5_u32plus.  In practice, this problem may occur if these MD5 routines are
 * inlined into a calling function, or with future and dangerously advanced
 * link-time optimizations.  For the time being, keeping these MD5 routines in
 * their own translation unit avoids the problem.
 */
#إذا defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#تعریف_کړه سېټ_2(n) \
	(*(MD5_u32plus *)&ptr[(n) * 4])
#تعریف_کړه GET(n) \
	سېټ_2(n)
#وإلا
#تعریف_کړه سېټ_2(n) \
	(ctx->بلاک[(n)] = \
	(MD5_u32plus)ptr[(n) * 4] | \
	((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
#تعریف_کړه GET(n) \
	(ctx->بلاک[(n)])
#د_که_پای

/*
 * This processes one or more 64-byte data blocks, but does NOT update the bit
 * counters.  There are no alignment requirements.
 */
ساكن ثابت فراغ *body(MD5_CTX *ctx, ثابت فراغ *معلومات, بینښه اوږد اندازه)
{
	ثابت بینښه حرف *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = (ثابت بینښه حرف *)معلومات;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	افعل {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

/* Round 1 */
		ګام(F, a, b, c, d, سېټ_2(0), 0xd76aa478, 7)
		ګام(F, d, a, b, c, سېټ_2(1), 0xe8c7b756, 12)
		ګام(F, c, d, a, b, سېټ_2(2), 0x242070db, 17)
		ګام(F, b, c, d, a, سېټ_2(3), 0xc1bdceee, 22)
		ګام(F, a, b, c, d, سېټ_2(4), 0xf57c0faf, 7)
		ګام(F, d, a, b, c, سېټ_2(5), 0x4787c62a, 12)
		ګام(F, c, d, a, b, سېټ_2(6), 0xa8304613, 17)
		ګام(F, b, c, d, a, سېټ_2(7), 0xfd469501, 22)
		ګام(F, a, b, c, d, سېټ_2(8), 0x698098d8, 7)
		ګام(F, d, a, b, c, سېټ_2(9), 0x8b44f7af, 12)
		ګام(F, c, d, a, b, سېټ_2(10), 0xffff5bb1, 17)
		ګام(F, b, c, d, a, سېټ_2(11), 0x895cd7be, 22)
		ګام(F, a, b, c, d, سېټ_2(12), 0x6b901122, 7)
		ګام(F, d, a, b, c, سېټ_2(13), 0xfd987193, 12)
		ګام(F, c, d, a, b, سېټ_2(14), 0xa679438e, 17)
		ګام(F, b, c, d, a, سېټ_2(15), 0x49b40821, 22)

/* Round 2 */
		ګام(G, a, b, c, d, GET(1), 0xf61e2562, 5)
		ګام(G, d, a, b, c, GET(6), 0xc040b340, 9)
		ګام(G, c, d, a, b, GET(11), 0x265e5a51, 14)
		ګام(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
		ګام(G, a, b, c, d, GET(5), 0xd62f105d, 5)
		ګام(G, d, a, b, c, GET(10), 0x02441453, 9)
		ګام(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
		ګام(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
		ګام(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
		ګام(G, d, a, b, c, GET(14), 0xc33707d6, 9)
		ګام(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
		ګام(G, b, c, d, a, GET(8), 0x455a14ed, 20)
		ګام(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
		ګام(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
		ګام(G, c, d, a, b, GET(7), 0x676f02d9, 14)
		ګام(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
		ګام(H, a, b, c, d, GET(5), 0xfffa3942, 4)
		ګام(H2, d, a, b, c, GET(8), 0x8771f681, 11)
		ګام(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
		ګام(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
		ګام(H, a, b, c, d, GET(1), 0xa4beea44, 4)
		ګام(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
		ګام(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
		ګام(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
		ګام(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
		ګام(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
		ګام(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
		ګام(H2, b, c, d, a, GET(6), 0x04881d05, 23)
		ګام(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
		ګام(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
		ګام(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
		ګام(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

/* Round 4 */
		ګام(I, a, b, c, d, GET(0), 0xf4292244, 6)
		ګام(I, d, a, b, c, GET(7), 0x432aff97, 10)
		ګام(I, c, d, a, b, GET(14), 0xab9423a7, 15)
		ګام(I, b, c, d, a, GET(5), 0xfc93a039, 21)
		ګام(I, a, b, c, d, GET(12), 0x655b59c3, 6)
		ګام(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
		ګام(I, c, d, a, b, GET(10), 0xffeff47d, 15)
		ګام(I, b, c, d, a, GET(1), 0x85845dd1, 21)
		ګام(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
		ګام(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
		ګام(I, c, d, a, b, GET(6), 0xa3014314, 15)
		ګام(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
		ګام(I, a, b, c, d, GET(4), 0xf7537e82, 6)
		ګام(I, d, a, b, c, GET(11), 0xbd3af235, 10)
		ګام(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
		ګام(I, b, c, d, a, GET(9), 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} طالما (اندازه -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	أرجع ptr;
}

فراغ MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

فراغ MD5_Update(MD5_CTX *ctx, ثابت فراغ *معلومات, بینښه اوږد اندازه)
{
	MD5_u32plus saved_lo;
	بینښه اوږد کارول_شوی, شته;

	saved_lo = ctx->lo;
	إذا ((ctx->lo = (saved_lo + اندازه) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += اندازه >> 29;

	کارول_شوی = saved_lo & 0x3f;

	إذا (کارول_شوی) {
		شته = 64 - کارول_شوی;

		إذا (اندازه < شته) {
			حافظه_نقل(&ctx->بفر[کارول_شوی], معلومات, اندازه);
			أرجع;
		}

		حافظه_نقل(&ctx->بفر[کارول_شوی], معلومات, شته);
		معلومات = (ثابت بینښه حرف *)معلومات + شته;
		اندازه -= شته;
		body(ctx, ctx->بفر, 64);
	}

	إذا (اندازه >= 64) {
		معلومات = body(ctx, معلومات, اندازه & ~(بینښه اوږد)0x3f);
		اندازه &= 0x3f;
	}

	حافظه_نقل(ctx->بفر, معلومات, اندازه);
}

#تعریف_کړه بهر_2(dst, src) \
	(dst)[0] = (بینښه حرف)(src); \
	(dst)[1] = (بینښه حرف)((src) >> 8); \
	(dst)[2] = (بینښه حرف)((src) >> 16); \
	(dst)[3] = (بینښه حرف)((src) >> 24);

فراغ MD5_Final(بینښه حرف *پایله, MD5_CTX *ctx)
{
	بینښه اوږد کارول_شوی, شته;

	کارول_شوی = ctx->lo & 0x3f;

	ctx->بفر[کارول_شوی++] = 0x80;

	شته = 64 - کارول_شوی;

	إذا (شته < 8) {
		memset(&ctx->بفر[کارول_شوی], 0, شته);
		body(ctx, ctx->بفر, 64);
		کارول_شوی = 0;
		شته = 64;
	}

	memset(&ctx->بفر[کارول_شوی], 0, شته - 8);

	ctx->lo <<= 3;
	بهر_2(&ctx->بفر[56], ctx->lo)
	بهر_2(&ctx->بفر[60], ctx->hi)

	body(ctx, ctx->بفر, 64);

	بهر_2(&پایله[0], ctx->a)
	بهر_2(&پایله[4], ctx->b)
	بهر_2(&پایله[8], ctx->c)
	بهر_2(&پایله[12], ctx->d)

	memset(ctx, 0, حجم(*ctx));
}

#د_که_پای
