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

#यदि_परिभाषितम् HAVE_OPENSSL
#अन्तर्भाव <openssl/md5.h>
#नोचेद्यदि !defined(_MD5_H)
#परिभाषय _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
प्रकारनिर्देश अचिह्नित पूर्णाङ्क MD5_u32plus;

प्रकारनिर्देश संरचना {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	अचिह्नित वर्ण बफर[64];
	MD5_u32plus खण्डम्[16];
} MD5_CTX;

बाह्य शून्य MD5_Init(MD5_CTX *ctx);
बाह्य शून्य MD5_Update(MD5_CTX *ctx, ध्रुव शून्य *दत्त, अचिह्नित दीर्घ आकार);
बाह्य शून्य MD5_Final(अचिह्नित वर्ण *परिणाम, MD5_CTX *ctx);

#यदि_अन्तः
