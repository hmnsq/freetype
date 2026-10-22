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

#جيڪڏهن_وصف_ٿيل HAVE_OPENSSL
#ضمن <openssl/md5.h>
#وإلاإذا !defined(_MD5_H)
#وصف_ڪريو _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
تعريفنوع بنانشان صحيح MD5_u32plus;

تعريفنوع بنية {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	بنانشان حرف بفر[64];
	MD5_u32plus بلاڪ[16];
} MD5_CTX;

خارجي فراغ MD5_Init(MD5_CTX *ctx);
خارجي فراغ MD5_Update(MD5_CTX *ctx, ثابت فراغ *معلومات, بنانشان ڊگهو سائيز);
خارجي فراغ MD5_Final(بنانشان حرف *نتيجو, MD5_CTX *ctx);

#جيڪڏهن_پورو
