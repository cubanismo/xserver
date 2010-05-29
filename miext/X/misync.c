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

#include "scrnintstr.h"
#include "misync.h"
#include "misyncstr.h"

static int syncScreenPrivateKeyIndex;
static DevPrivateKey syncScreenPrivateKey = &syncScreenPrivateKeyIndex;

#define SYNC_SCREEN_PRIV(pScreen) 				\
    (SyncScreenPrivPtr) dixLookupPrivate(&pScreen->devPrivates,	\
					 syncScreenPrivateKey)

typedef struct _syncScreenPriv {
    /* Wrappable sync-specific screen functions */
    SyncScreenFuncsRec		funcs;

    /* Wrapped screen functions */
    CloseScreenProcPtr		CloseScreen;
} SyncScreenPrivRec, *SyncScreenPrivPtr;

/* Default implementations of the sync screen functions */
void
miSyncScreenCreateFence(ScreenPtr pScreen, SyncFence* pFence,
                        Bool initially_triggered)
{
    (void)pScreen;

    pFence->triggered = initially_triggered;
}

void miSyncScreenDestroyFence(ScreenPtr pScreen, SyncFence* pFence)
{
    (void)pScreen;
    (void)pFence;
}

/* Default implementations of the per-object functions */
static void
miSyncFenceSetTriggered(SyncFence* pFence)
{
    pFence->triggered = TRUE;
}

static void
miSyncFenceReset(SyncFence* pFence)
{
    pFence->triggered = FALSE;
}

static Bool
miSyncFenceCheckTriggered(SyncFence* pFence)
{
    return pFence->triggered;
}

static void
miSyncFenceAddTrigger(SyncTrigger* pTrigger)
{
    (void)pTrigger;

    return;
}

static void
miSyncFenceDeleteTrigger(SyncTrigger* pTrigger)
{
    (void)pTrigger;

    return;
}

/* Machine independent portion of the fence sync object implementation */
void
miSyncInitFence(ScreenPtr pScreen, SyncFence* pFence, Bool initially_triggered)
{
    SyncScreenPrivPtr pScreenPriv = SYNC_SCREEN_PRIV(pScreen);
    static const SyncFenceFuncsRec miSyncFenceFuncs = {
	&miSyncFenceSetTriggered,
	&miSyncFenceReset,
	&miSyncFenceCheckTriggered,
	&miSyncFenceAddTrigger,
	&miSyncFenceDeleteTrigger
    };

    pFence->pScreen = pScreen;
    pFence->funcs = miSyncFenceFuncs;

    pScreenPriv->funcs.CreateFence(pScreen, pFence, initially_triggered);
}

void
miSyncDestroyFence(SyncFence* pFence)
{
    ScreenPtr pScreen = pFence->pScreen;
    SyncScreenPrivPtr pScreenPriv = SYNC_SCREEN_PRIV(pScreen);

    pScreenPriv->funcs.DestroyFence(pScreen, pFence);
}

void
miSyncTriggerFence(SyncFence* pFence)
{
    SyncTriggerList *ptl, *pNext;
    CARD64 unused;

    pFence->funcs.SetTriggered(pFence);

    XSyncIntToValue(&unused, 0L);

    /* run through triggers to see if any fired */
    for (ptl = pFence->sync.pTriglist; ptl; ptl = pNext)
    {
	pNext = ptl->next;
	if ((*ptl->pTrigger->CheckTrigger)(ptl->pTrigger, unused))
	    (*ptl->pTrigger->TriggerFired)(ptl->pTrigger);
    }
}

SyncScreenFuncsPtr miSyncGetScreenFuncs(ScreenPtr pScreen)
{
    SyncScreenPrivPtr pScreenPriv = SYNC_SCREEN_PRIV(pScreen);

    return &pScreenPriv->funcs;
}

static Bool
SyncCloseScreen (int i, ScreenPtr pScreen)
{
    SyncScreenPrivPtr pScreenPriv = SYNC_SCREEN_PRIV(pScreen);

    pScreen->CloseScreen = pScreenPriv->CloseScreen;
    xfree(pScreenPriv);

    return (*pScreen->CloseScreen) (i, pScreen);
}

Bool
miSyncSetup(ScreenPtr pScreen)
{
    SyncScreenPrivPtr	pScreenPriv = SYNC_SCREEN_PRIV(pScreen);

    static const SyncScreenFuncsRec miSyncScreenFuncs = {
	&miSyncScreenCreateFence,
	&miSyncScreenDestroyFence
    };

    if (pScreenPriv)
	return TRUE;

    if (!(pScreenPriv = xalloc (sizeof(SyncScreenPrivRec))))
	return FALSE;

    pScreenPriv->funcs = miSyncScreenFuncs;

    /* Wrap CloseScreen to clean up */
    pScreenPriv->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = SyncCloseScreen;

    dixSetPrivate(&pScreen->devPrivates, syncScreenPrivateKey, pScreenPriv);

    return TRUE;
}
