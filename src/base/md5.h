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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENSSL
#ਅਨ੍ਤਰ੍ਭਾਵ <openssl/md5.h>
#ਜਾਂਜੇ !defined(_MD5_H)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ MD5_u32plus;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਫਰ[64];
	MD5_u32plus ਖੰਡ[16];
} MD5_CTX;

ਬਾਹਰੀ ਖਾਲੀ MD5_Init(MD5_CTX *ctx);
ਬਾਹਰੀ ਖਾਲੀ MD5_Update(MD5_CTX *ctx, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ);
ਬਾਹਰੀ ਖਾਲੀ MD5_Final(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਨਤੀਜਾ, MD5_CTX *ctx);

#ਜੇ_ਅੰਤ
