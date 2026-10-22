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
#ଅନ୍ତର୍ଭାଵ <openssl/md5.h>
#ନଚେତଯଦି !defined(_MD5_H)
#define _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା MD5_u32plus;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned ଅକ୍ଷର buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;

ବାହ୍ୟ ଶୂନ୍ୟ MD5_Init(MD5_CTX *ctx);
ବାହ୍ୟ ଶୂନ୍ୟ MD5_Update(MD5_CTX *ctx, ଅଚଳ ଶୂନ୍ୟ *data, unsigned long size);
ବାହ୍ୟ ଶୂନ୍ୟ MD5_Final(unsigned ଅକ୍ଷର *result, MD5_CTX *ctx);

#endif
