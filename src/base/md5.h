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

#నిర్వచితమైతే HAVE_OPENSSL
#అన్తర్భావ <openssl/md5.h>
#లేదావేళ !defined(_MD5_H)
#నిర్వచించు _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
రకనిర్వచనం గుర్తులేని పూర్ణాంకం MD5_u32plus;

రకనిర్వచనం నిర్మాణం {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	గుర్తులేని అక్షరం బఫరు[64];
	MD5_u32plus ఖండం[16];
} MD5_CTX;

బాహ్య శూన్య MD5_Init(MD5_CTX *ctx);
బాహ్య శూన్య MD5_Update(MD5_CTX *ctx, స్థిరరాశి శూన్య *దత్తాంశం, గుర్తులేని దీర్ఘ కొలత);
బాహ్య శూన్య MD5_Final(గుర్తులేని అక్షరం *ఫలితం, MD5_CTX *ctx);

#అయితే_ముగింపు
