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

#నిర్వచితం_కాకపోతే HAVE_OPENSSL

#అన్తర్భావ <string.h>

#అన్తర్భావ "md5.h"

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#నిర్వచించు F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#నిర్వచించు G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#నిర్వచించు H(x, y, z)			(((x) ^ (y)) ^ (z))
#నిర్వచించు H2(x, y, z)			((x) ^ ((y) ^ (z)))
#నిర్వచించు I(x, y, z)			((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#నిర్వచించు అడుగు(f, a, b, c, d, x, t, s) \
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
#ఒకవేళ defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#నిర్వచించు సమితి_2(n) \
	(*(MD5_u32plus *)&ptr[(n) * 4])
#నిర్వచించు GET(n) \
	సమితి_2(n)
#లేకపోతే
#నిర్వచించు సమితి_2(n) \
	(ctx->ఖండం[(n)] = \
	(MD5_u32plus)ptr[(n) * 4] | \
	((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
#నిర్వచించు GET(n) \
	(ctx->ఖండం[(n)])
#అయితే_ముగింపు

/*
 * This processes one or more 64-byte data blocks, but does NOT update the bit
 * counters.  There are no alignment requirements.
 */
స్థిర స్థిరరాశి శూన్య *body(MD5_CTX *ctx, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని దీర్ఘ కొలత)
{
	స్థిరరాశి గుర్తులేని అక్షరం *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = (స్థిరరాశి గుర్తులేని అక్షరం *)దత్తాంశం;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	చేయి {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

/* Round 1 */
		అడుగు(F, a, b, c, d, సమితి_2(0), 0xd76aa478, 7)
		అడుగు(F, d, a, b, c, సమితి_2(1), 0xe8c7b756, 12)
		అడుగు(F, c, d, a, b, సమితి_2(2), 0x242070db, 17)
		అడుగు(F, b, c, d, a, సమితి_2(3), 0xc1bdceee, 22)
		అడుగు(F, a, b, c, d, సమితి_2(4), 0xf57c0faf, 7)
		అడుగు(F, d, a, b, c, సమితి_2(5), 0x4787c62a, 12)
		అడుగు(F, c, d, a, b, సమితి_2(6), 0xa8304613, 17)
		అడుగు(F, b, c, d, a, సమితి_2(7), 0xfd469501, 22)
		అడుగు(F, a, b, c, d, సమితి_2(8), 0x698098d8, 7)
		అడుగు(F, d, a, b, c, సమితి_2(9), 0x8b44f7af, 12)
		అడుగు(F, c, d, a, b, సమితి_2(10), 0xffff5bb1, 17)
		అడుగు(F, b, c, d, a, సమితి_2(11), 0x895cd7be, 22)
		అడుగు(F, a, b, c, d, సమితి_2(12), 0x6b901122, 7)
		అడుగు(F, d, a, b, c, సమితి_2(13), 0xfd987193, 12)
		అడుగు(F, c, d, a, b, సమితి_2(14), 0xa679438e, 17)
		అడుగు(F, b, c, d, a, సమితి_2(15), 0x49b40821, 22)

/* Round 2 */
		అడుగు(G, a, b, c, d, GET(1), 0xf61e2562, 5)
		అడుగు(G, d, a, b, c, GET(6), 0xc040b340, 9)
		అడుగు(G, c, d, a, b, GET(11), 0x265e5a51, 14)
		అడుగు(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
		అడుగు(G, a, b, c, d, GET(5), 0xd62f105d, 5)
		అడుగు(G, d, a, b, c, GET(10), 0x02441453, 9)
		అడుగు(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
		అడుగు(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
		అడుగు(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
		అడుగు(G, d, a, b, c, GET(14), 0xc33707d6, 9)
		అడుగు(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
		అడుగు(G, b, c, d, a, GET(8), 0x455a14ed, 20)
		అడుగు(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
		అడుగు(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
		అడుగు(G, c, d, a, b, GET(7), 0x676f02d9, 14)
		అడుగు(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
		అడుగు(H, a, b, c, d, GET(5), 0xfffa3942, 4)
		అడుగు(H2, d, a, b, c, GET(8), 0x8771f681, 11)
		అడుగు(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
		అడుగు(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
		అడుగు(H, a, b, c, d, GET(1), 0xa4beea44, 4)
		అడుగు(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
		అడుగు(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
		అడుగు(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
		అడుగు(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
		అడుగు(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
		అడుగు(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
		అడుగు(H2, b, c, d, a, GET(6), 0x04881d05, 23)
		అడుగు(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
		అడుగు(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
		అడుగు(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
		అడుగు(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

/* Round 4 */
		అడుగు(I, a, b, c, d, GET(0), 0xf4292244, 6)
		అడుగు(I, d, a, b, c, GET(7), 0x432aff97, 10)
		అడుగు(I, c, d, a, b, GET(14), 0xab9423a7, 15)
		అడుగు(I, b, c, d, a, GET(5), 0xfc93a039, 21)
		అడుగు(I, a, b, c, d, GET(12), 0x655b59c3, 6)
		అడుగు(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
		అడుగు(I, c, d, a, b, GET(10), 0xffeff47d, 15)
		అడుగు(I, b, c, d, a, GET(1), 0x85845dd1, 21)
		అడుగు(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
		అడుగు(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
		అడుగు(I, c, d, a, b, GET(6), 0xa3014314, 15)
		అడుగు(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
		అడుగు(I, a, b, c, d, GET(4), 0xf7537e82, 6)
		అడుగు(I, d, a, b, c, GET(11), 0xbd3af235, 10)
		అడుగు(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
		అడుగు(I, b, c, d, a, GET(9), 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} అయితే (కొలత -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	తిరిగి ptr;
}

శూన్య MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

శూన్య MD5_Update(MD5_CTX *ctx, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని దీర్ఘ కొలత)
{
	MD5_u32plus saved_lo;
	గుర్తులేని దీర్ఘ వాడిన, అందుబాటు;

	saved_lo = ctx->lo;
	ఒకవేళ ((ctx->lo = (saved_lo + కొలత) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += కొలత >> 29;

	వాడిన = saved_lo & 0x3f;

	ఒకవేళ (వాడిన) {
		అందుబాటు = 64 - వాడిన;

		ఒకవేళ (కొలత < అందుబాటు) {
			జ్ఞాపకం_నకలు(&ctx->బఫరు[వాడిన], దత్తాంశం, కొలత);
			తిరిగి;
		}

		జ్ఞాపకం_నకలు(&ctx->బఫరు[వాడిన], దత్తాంశం, అందుబాటు);
		దత్తాంశం = (స్థిరరాశి గుర్తులేని అక్షరం *)దత్తాంశం + అందుబాటు;
		కొలత -= అందుబాటు;
		body(ctx, ctx->బఫరు, 64);
	}

	ఒకవేళ (కొలత >= 64) {
		దత్తాంశం = body(ctx, దత్తాంశం, కొలత & ~(గుర్తులేని దీర్ఘ)0x3f);
		కొలత &= 0x3f;
	}

	జ్ఞాపకం_నకలు(ctx->బఫరు, దత్తాంశం, కొలత);
}

#నిర్వచించు బహిర్గతం_2(dst, src) \
	(dst)[0] = (గుర్తులేని అక్షరం)(src); \
	(dst)[1] = (గుర్తులేని అక్షరం)((src) >> 8); \
	(dst)[2] = (గుర్తులేని అక్షరం)((src) >> 16); \
	(dst)[3] = (గుర్తులేని అక్షరం)((src) >> 24);

శూన్య MD5_Final(గుర్తులేని అక్షరం *ఫలితం, MD5_CTX *ctx)
{
	గుర్తులేని దీర్ఘ వాడిన, అందుబాటు;

	వాడిన = ctx->lo & 0x3f;

	ctx->బఫరు[వాడిన++] = 0x80;

	అందుబాటు = 64 - వాడిన;

	ఒకవేళ (అందుబాటు < 8) {
		memset(&ctx->బఫరు[వాడిన], 0, అందుబాటు);
		body(ctx, ctx->బఫరు, 64);
		వాడిన = 0;
		అందుబాటు = 64;
	}

	memset(&ctx->బఫరు[వాడిన], 0, అందుబాటు - 8);

	ctx->lo <<= 3;
	బహిర్గతం_2(&ctx->బఫరు[56], ctx->lo)
	బహిర్గతం_2(&ctx->బఫరు[60], ctx->hi)

	body(ctx, ctx->బఫరు, 64);

	బహిర్గతం_2(&ఫలితం[0], ctx->a)
	బహిర్గతం_2(&ఫలితం[4], ctx->b)
	బహిర్గతం_2(&ఫలితం[8], ctx->c)
	బహిర్గతం_2(&ఫలితం[12], ctx->d)

	memset(ctx, 0, పరిమాణం(*ctx));
}

#అయితే_ముగింపు
