/*
 * Copyright © 2010 NVIDIA
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NVIDIA
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _SYNCSDK_H_
#define _SYNCSDK_H_

#include "misync.h"

extern _X_EXPORT int
SyncVerifyFence(SyncFence **ppFence, XID fid, ClientPtr client, Mask mode);

#define VERIFY_SYNC_FENCE(pFence, fid, client, mode)			\
    do {								\
	int rc;								\
	rc = SyncVerifyFence(&(pFence), (fid), (client), (mode));	\
	if (Success != rc) return rc;					\
    } while (0)

#define VERIFY_SYNC_FENCE_OR_NONE(pFence, fid, client, mode)		\
    do {								\
        pFence = 0;							\
        if (None != fid)						\
	    VERIFY_SYNC_FENCE((pFence), (fid), (client), (mode));	\
    } while (0);

#endif /* _SYNCSDK_H_ */

