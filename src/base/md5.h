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
#અન્તર્ભાવ <openssl/md5.h>
#અથવાજો !defined(_MD5_H)
#define _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક MD5_u32plus;

પ્રકારવ્યાખ્યા સંરચના {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned અક્ષર buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;

બાહ્ય શૂન્ય MD5_Init(MD5_CTX *ctx);
બાહ્ય શૂન્ય MD5_Update(MD5_CTX *ctx, અચલ શૂન્ય *data, unsigned long size);
બાહ્ય શૂન્ય MD5_Final(unsigned અક્ષર *result, MD5_CTX *ctx);

#endif
