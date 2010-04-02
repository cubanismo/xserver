/*
 * Copyright ©2010 NVIDIA
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

#include "misync.h"
#include "misyncstr.h"

void
miSyncTriggerFence(SyncFence* pFence)
{
    SyncTriggerList *ptl, *pNext;
    CARD64 unused;

    pFence->triggered = TRUE;

    XSyncIntToValue(&unused, 0L);

    /* run through triggers to see if any fired */
    for (ptl = pFence->sync.pTriglist; ptl; ptl = pNext)
    {
	pNext = ptl->next;
	if ((*ptl->pTrigger->CheckTrigger)(ptl->pTrigger, unused))
	    (*ptl->pTrigger->TriggerFired)(ptl->pTrigger);
    }
}
