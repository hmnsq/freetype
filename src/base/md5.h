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
#ಅನ್ತರ್ಭಾವ <openssl/md5.h>
#ನೋಚೇದ್ಯದಿ !defined(_MD5_H)
#define _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ MD5_u32plus;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned ವರ್ಣ buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;

ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Init(MD5_CTX *ctx);
ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Update(MD5_CTX *ctx, ಧ್ರುವ ಶೂನ್ಯ *data, unsigned long size);
ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Final(unsigned ವರ್ಣ *result, MD5_CTX *ctx);

#endif
