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
 * See md5.c for more information.
 */

#ifdef HAVE_OPENSSL
#অন্তর্ভাব <openssl/md5.h>
#নতুবা_যদি !defined(_MD5_H)
#define _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
প্রকারলেখ unsigned পূর্ণসংখ্যা MD5_u32plus;

প্রকারলেখ কাঠামো {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned অক্ষর বাফার[64];
	MD5_u32plus block[16];
} MD5_CTX;

বাহ্যিক শূন্য MD5_Init(MD5_CTX *ctx);
বাহ্যিক শূন্য MD5_Update(MD5_CTX *ctx, ধ্রুবক শূন্য *তথ্য, unsigned long আকার_2);
বাহ্যিক শূন্য MD5_Final(unsigned অক্ষর *ফলাফল, MD5_CTX *ctx);

#endif
