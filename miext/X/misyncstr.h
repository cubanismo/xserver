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

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifndef _MISYNCSTR_H_
#define _MISYNCSTR_H_

#include "dix.h"
#include <X11/extensions/syncconst.h>

#define CARD64 XSyncValue /* XXX temporary! need real 64 bit values for Alpha */

/* Sync object types */
#define SYNC_COUNTER		0
#define SYNC_FENCE		1

typedef struct _SyncObject {
    ClientPtr		client;	/* Owning client. 0 for system counters */
    XID			id;		/* resource ID */
    struct _SyncTriggerList *pTriglist;	/* list of triggers */
    unsigned char	type;		/* SYNC_* */
    Bool		beingDestroyed;	/* in process of going away */
} SyncObject;

typedef struct _SyncCounter {
    SyncObject		sync;		/* Common sync object data */
    CARD64		value;		/* counter value */
    struct _SysCounterInfo *pSysCounterInfo; /* NULL if not a system counter */
} SyncCounter;

struct _SyncFence {
    SyncObject		sync;		/* Common sync object data */
    BOOL		triggered;	/* counter value */
};

typedef struct _SyncTrigger {
    SyncObject *pSync;
    CARD64	wait_value;	/* wait value */
    unsigned int value_type;     /* Absolute or Relative */
    unsigned int test_type;	/* transition or Comparision type */
    CARD64	test_value;	/* trigger event threshold value */
    Bool	(*CheckTrigger)(
				struct _SyncTrigger * /*pTrigger*/,
				CARD64 /*newval*/
				);
    void	(*TriggerFired)(
				struct _SyncTrigger * /*pTrigger*/
				);
    void	(*CounterDestroyed)(
				struct _SyncTrigger * /*pTrigger*/
				    );
} SyncTrigger;

typedef struct _SyncTriggerList {
    SyncTrigger *pTrigger;
    struct _SyncTriggerList *next;
} SyncTriggerList;

#endif /* _MISYNCSTR_H_ */

