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

#अगर_घोषित HAVE_OPENSSL
#समावेश <openssl/md5.h>
#या_अगर !defined(_MD5_H)
#घोषणा _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
प्रकार अचिन्हित पूर्णांक MD5_u32plus;

प्रकार काष्ठा {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	अचिन्हित अक्षर बफर[64];
	MD5_u32plus block[16];
} MD5_CTX;

बाह्य व्योम MD5_Init(MD5_CTX *ctx);
बाह्य व्योम MD5_Update(MD5_CTX *ctx, स्थिर व्योम *तथ्य, अचिन्हित दीर्घ आकार);
बाह्य व्योम MD5_Final(अचिन्हित अक्षर *परिणाम, MD5_CTX *ctx);

#पूर्ण_अगर
