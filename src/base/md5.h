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

#اگر_تعریف_شدہ HAVE_OPENSSL
#شامل <openssl/md5.h>
#وگرنہ_اگر !defined(_MD5_H)
#تعریف_کرو _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
قسمتعریف غیرعلامتی صحیح MD5_u32plus;

قسمتعریف ساخت {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	غیرعلامتی حرف بفر[64];
	MD5_u32plus بلاک[16];
} MD5_CTX;

خارجی خالی MD5_Init(MD5_CTX *ctx);
خارجی خالی MD5_Update(MD5_CTX *ctx, ثابت خالی *ڈیٹا, غیرعلامتی طویل سائز);
خارجی خالی MD5_Final(غیرعلامتی حرف *نتیجہ, MD5_CTX *ctx);

#اگر_ختم
