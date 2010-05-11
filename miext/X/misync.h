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

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _MISYNC_H_
#define _MISYNC_H_

typedef struct _SyncFence SyncFence;

typedef void (*SyncScreenCreateFenceFunc) (ScreenPtr pScreen,
					   SyncFence* pFence,
					   Bool initially_triggered);
typedef void (*SyncScreenDestroyFenceFunc) (ScreenPtr pScreen,
					    SyncFence* pFence);

typedef struct _syncScreenFuncs {
    SyncScreenCreateFenceFunc	CreateFence;
    SyncScreenDestroyFenceFunc	DestroyFence;
} SyncScreenFuncsRec, *SyncScreenFuncsPtr;

extern _X_EXPORT void
miSyncScreenCreateFence(ScreenPtr pScreen, SyncFence* pFence,
			Bool initially_triggered);
extern _X_EXPORT void
miSyncScreenDestroyFence(ScreenPtr pScreen, SyncFence* pFence);

typedef void (*SyncFenceSetTriggeredFunc) (SyncFence* pFence);
typedef void (*SyncFenceResetFunc) (SyncFence* pFence);
typedef Bool (*SyncFenceCheckTriggeredFunc) (SyncFence* pFence);

typedef struct _syncFenceFuncs {
    SyncFenceSetTriggeredFunc	SetTriggered;
    SyncFenceResetFunc		Reset;
    SyncFenceCheckTriggeredFunc	CheckTriggered;
} SyncFenceFuncsRec, *SyncFenceFuncsPtr;

extern _X_EXPORT void
miSyncFenceSetTriggered(SyncFence* pFence);
extern _X_EXPORT void
miSyncFenceReset(SyncFence* pFence);
extern _X_EXPORT Bool
miSyncFenceCheckTriggered(SyncFence* pFence);

extern _X_EXPORT void
miSyncInitFence(ScreenPtr pScreen, SyncFence* pFence, Bool initially_triggered);
extern _X_EXPORT void
miSyncDestroyFence(SyncFence* pFence);
extern _X_EXPORT void
miSyncTriggerFence(SyncFence* pFence);

extern _X_EXPORT SyncScreenFuncsPtr
miSyncGetScreenFuncs(ScreenPtr pScreen);
extern _X_EXPORT Bool
miSyncSetup(ScreenPtr pScreen);

#endif /* _MISYNC_H_ */
