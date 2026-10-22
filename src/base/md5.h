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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENSSL
#ಅನ್ತರ್ಭಾವ <openssl/md5.h>
#ಅಥವಾಆದರೆ !defined(_MD5_H)
#ವ್ಯಾಖ್ಯಾನಿಸು _MD5_H

/* Any 32-bit or wider unsigned integer data type will do */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ MD5_u32plus;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಬಫರ್[64];
	MD5_u32plus ಖಂಡ[16];
} MD5_CTX;

ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Init(MD5_CTX *ctx);
ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Update(MD5_CTX *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2);
ಬಾಹ್ಯ ಶೂನ್ಯ MD5_Final(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ಫಲಿತಾಂಶ, MD5_CTX *ctx);

#ಆದರೆ_ಅಂತ್ಯ
