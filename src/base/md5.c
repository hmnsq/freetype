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

#यदि_अपरिभाषितम् HAVE_OPENSSL

#अन्तर्भाव <string.h>

#अन्तर्भाव "md5.h"

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#परिभाषय F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#परिभाषय G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#परिभाषय H(x, y, z)			(((x) ^ (y)) ^ (z))
#परिभाषय H2(x, y, z)			((x) ^ ((y) ^ (z)))
#परिभाषय I(x, y, z)			((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#परिभाषय चरणः(f, a, b, c, d, x, t, s) \
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
#यदि defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#परिभाषय समुच्चयः_2(n) \
	(*(MD5_u32plus *)&ptr[(n) * 4])
#परिभाषय GET(n) \
	समुच्चयः_2(n)
#अन्यथा
#परिभाषय समुच्चयः_2(n) \
	(ctx->खण्डम्[(n)] = \
	(MD5_u32plus)ptr[(n) * 4] | \
	((MD5_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD5_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD5_u32plus)ptr[(n) * 4 + 3] << 24))
#परिभाषय GET(n) \
	(ctx->खण्डम्[(n)])
#यदि_अन्तः

/*
 * This processes one or more 64-byte data blocks, but does NOT update the bit
 * counters.  There are no alignment requirements.
 */
स्थिर ध्रुव शून्य *body(MD5_CTX *ctx, ध्रुव शून्य *दत्त, अचिह्नित दीर्घ आकार)
{
	ध्रुव अचिह्नित वर्ण *ptr;
	MD5_u32plus a, b, c, d;
	MD5_u32plus saved_a, saved_b, saved_c, saved_d;

	ptr = (ध्रुव अचिह्नित वर्ण *)दत्त;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	कुरु {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

/* Round 1 */
		चरणः(F, a, b, c, d, समुच्चयः_2(0), 0xd76aa478, 7)
		चरणः(F, d, a, b, c, समुच्चयः_2(1), 0xe8c7b756, 12)
		चरणः(F, c, d, a, b, समुच्चयः_2(2), 0x242070db, 17)
		चरणः(F, b, c, d, a, समुच्चयः_2(3), 0xc1bdceee, 22)
		चरणः(F, a, b, c, d, समुच्चयः_2(4), 0xf57c0faf, 7)
		चरणः(F, d, a, b, c, समुच्चयः_2(5), 0x4787c62a, 12)
		चरणः(F, c, d, a, b, समुच्चयः_2(6), 0xa8304613, 17)
		चरणः(F, b, c, d, a, समुच्चयः_2(7), 0xfd469501, 22)
		चरणः(F, a, b, c, d, समुच्चयः_2(8), 0x698098d8, 7)
		चरणः(F, d, a, b, c, समुच्चयः_2(9), 0x8b44f7af, 12)
		चरणः(F, c, d, a, b, समुच्चयः_2(10), 0xffff5bb1, 17)
		चरणः(F, b, c, d, a, समुच्चयः_2(11), 0x895cd7be, 22)
		चरणः(F, a, b, c, d, समुच्चयः_2(12), 0x6b901122, 7)
		चरणः(F, d, a, b, c, समुच्चयः_2(13), 0xfd987193, 12)
		चरणः(F, c, d, a, b, समुच्चयः_2(14), 0xa679438e, 17)
		चरणः(F, b, c, d, a, समुच्चयः_2(15), 0x49b40821, 22)

/* Round 2 */
		चरणः(G, a, b, c, d, GET(1), 0xf61e2562, 5)
		चरणः(G, d, a, b, c, GET(6), 0xc040b340, 9)
		चरणः(G, c, d, a, b, GET(11), 0x265e5a51, 14)
		चरणः(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
		चरणः(G, a, b, c, d, GET(5), 0xd62f105d, 5)
		चरणः(G, d, a, b, c, GET(10), 0x02441453, 9)
		चरणः(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
		चरणः(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
		चरणः(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
		चरणः(G, d, a, b, c, GET(14), 0xc33707d6, 9)
		चरणः(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
		चरणः(G, b, c, d, a, GET(8), 0x455a14ed, 20)
		चरणः(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
		चरणः(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
		चरणः(G, c, d, a, b, GET(7), 0x676f02d9, 14)
		चरणः(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

/* Round 3 */
		चरणः(H, a, b, c, d, GET(5), 0xfffa3942, 4)
		चरणः(H2, d, a, b, c, GET(8), 0x8771f681, 11)
		चरणः(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
		चरणः(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
		चरणः(H, a, b, c, d, GET(1), 0xa4beea44, 4)
		चरणः(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
		चरणः(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
		चरणः(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
		चरणः(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
		चरणः(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
		चरणः(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
		चरणः(H2, b, c, d, a, GET(6), 0x04881d05, 23)
		चरणः(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
		चरणः(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
		चरणः(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
		चरणः(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

/* Round 4 */
		चरणः(I, a, b, c, d, GET(0), 0xf4292244, 6)
		चरणः(I, d, a, b, c, GET(7), 0x432aff97, 10)
		चरणः(I, c, d, a, b, GET(14), 0xab9423a7, 15)
		चरणः(I, b, c, d, a, GET(5), 0xfc93a039, 21)
		चरणः(I, a, b, c, d, GET(12), 0x655b59c3, 6)
		चरणः(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
		चरणः(I, c, d, a, b, GET(10), 0xffeff47d, 15)
		चरणः(I, b, c, d, a, GET(1), 0x85845dd1, 21)
		चरणः(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
		चरणः(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
		चरणः(I, c, d, a, b, GET(6), 0xa3014314, 15)
		चरणः(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
		चरणः(I, a, b, c, d, GET(4), 0xf7537e82, 6)
		चरणः(I, d, a, b, c, GET(11), 0xbd3af235, 10)
		चरणः(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
		चरणः(I, b, c, d, a, GET(9), 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} यावत् (आकार -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	प्रत्यागम ptr;
}

शून्य MD5_Init(MD5_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

शून्य MD5_Update(MD5_CTX *ctx, ध्रुव शून्य *दत्त, अचिह्नित दीर्घ आकार)
{
	MD5_u32plus saved_lo;
	अचिह्नित दीर्घ प्रयुक्तम्, उपलब्धम्;

	saved_lo = ctx->lo;
	यदि ((ctx->lo = (saved_lo + आकार) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += आकार >> 29;

	प्रयुक्तम् = saved_lo & 0x3f;

	यदि (प्रयुक्तम्) {
		उपलब्धम् = 64 - प्रयुक्तम्;

		यदि (आकार < उपलब्धम्) {
			स्मृतिं_प्रतिलिख(&ctx->बफर[प्रयुक्तम्], दत्त, आकार);
			प्रत्यागम;
		}

		स्मृतिं_प्रतिलिख(&ctx->बफर[प्रयुक्तम्], दत्त, उपलब्धम्);
		दत्त = (ध्रुव अचिह्नित वर्ण *)दत्त + उपलब्धम्;
		आकार -= उपलब्धम्;
		body(ctx, ctx->बफर, 64);
	}

	यदि (आकार >= 64) {
		दत्त = body(ctx, दत्त, आकार & ~(अचिह्नित दीर्घ)0x3f);
		आकार &= 0x3f;
	}

	स्मृतिं_प्रतिलिख(ctx->बफर, दत्त, आकार);
}

#परिभाषय बहिर्गमनम्_2(dst, src) \
	(dst)[0] = (अचिह्नित वर्ण)(src); \
	(dst)[1] = (अचिह्नित वर्ण)((src) >> 8); \
	(dst)[2] = (अचिह्नित वर्ण)((src) >> 16); \
	(dst)[3] = (अचिह्नित वर्ण)((src) >> 24);

शून्य MD5_Final(अचिह्नित वर्ण *परिणाम, MD5_CTX *ctx)
{
	अचिह्नित दीर्घ प्रयुक्तम्, उपलब्धम्;

	प्रयुक्तम् = ctx->lo & 0x3f;

	ctx->बफर[प्रयुक्तम्++] = 0x80;

	उपलब्धम् = 64 - प्रयुक्तम्;

	यदि (उपलब्धम् < 8) {
		स्मृतिं_पूरय(&ctx->बफर[प्रयुक्तम्], 0, उपलब्धम्);
		body(ctx, ctx->बफर, 64);
		प्रयुक्तम् = 0;
		उपलब्धम् = 64;
	}

	स्मृतिं_पूरय(&ctx->बफर[प्रयुक्तम्], 0, उपलब्धम् - 8);

	ctx->lo <<= 3;
	बहिर्गमनम्_2(&ctx->बफर[56], ctx->lo)
	बहिर्गमनम्_2(&ctx->बफर[60], ctx->hi)

	body(ctx, ctx->बफर, 64);

	बहिर्गमनम्_2(&परिणाम[0], ctx->a)
	बहिर्गमनम्_2(&परिणाम[4], ctx->b)
	बहिर्गमनम्_2(&परिणाम[8], ctx->c)
	बहिर्गमनम्_2(&परिणाम[12], ctx->d)

	स्मृतिं_पूरय(ctx, 0, परिमाण(*ctx));
}

#यदि_अन्तः
