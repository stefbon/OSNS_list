/*
  2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Stef Bon <stefbon@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "libosns-basic-system-headers.h"

#include "libosns-log.h"
#include "libosns-event.h"

#include "list.h"
#include "header.h"
#include "lock.h"

#include "insert.h"
#include "next.h"
#include "remove.h"

#include "signal.h"

/* HEADER lock ops */

struct list_header_lock_ops_s dummy_header_lock_ops = {
    .type                                       = LIST_LOCKING_METHOD_NONE,
    .lock_header                                = LIST_header_lock_dummy,
    .unlock_header                              = LIST_header_unlock_dummy,
    .lock_element                               = LIST_element_lock_op_dummy,
    .unlock_element                             = LIST_element_unlock_op_dummy,
};

struct list_header_lock_ops_s lockflag_header_lock_ops = {
    .type                                       = LIST_LOCKING_METHOD_LOCKFLAG,
    .lock_header                                = LIST_header_lock,
    .unlock_header                              = LIST_header_unlock,
    .lock_element                               = LIST_element_lock_op,
    .unlock_element                             = LIST_element_unlock_op,
};

/* HEADER initializer */

static void cb_change_dummy(struct list_header_s *h, unsigned char opcode)
{}

struct list_header_s LIST_header_initializer = {
    .flags                              = 0,
    .lockflags                          = 0,
    .threadid                           = 0,
    .count                              = 0,
    .cb_change                          = cb_change_dummy,
    .ptr                                = NULL,
    .esignal                            = LIST_SHARED_SIGNAL_INIT_P,
    .head.h                             = NULL,
    .head.n                             = NULL,
    .head.p                             = NULL,
    .head.ops                           = NULL,
    .head.use                           = 0,
    .head.lockflags                     = 0,
    .tail.h                             = NULL,
    .tail.n                             = NULL,
    .tail.p                             = NULL,
    .tail.ops                           = NULL,
    .tail.use                           = 0,
    .tail.lockflags                     = 0,
    .lops                               = &dummy_header_lock_ops,
};

/* HEADER init */

static void LIST_header_init_headtail_elements(struct list_header_s *h, struct list_element_s *e, unsigned char tail)
{
    unsigned char dolock=(h->flags & LIST_HEADER_FLAG_NOLOCK) ? 0 : 1;

    LIST_element_init(e, h);
    e->ops=(tail) ? LIST_element_get_ops_tail(dolock) : LIST_element_get_ops_head(dolock);
}

void LIST_header_init(struct list_header_s *h, unsigned int lockingmethod, void (* cb_change)(struct list_header_s *h, unsigned char opcode))
{
    struct list_element_s *head=NULL;
    struct list_element_s *tail=NULL;

    if (h==NULL) {

	logoutput_warning("%s: header empty", __FUNCTION__);
	return;

    } else if (h->flags & LIST_HEADER_FLAG_INIT) {

        logoutput_warning("%s: header already initialized?", __FUNCTION__);

    }

    h->flags = LIST_HEADER_FLAG_INIT;

    h->lockflags=0;
    h->count=0;
    h->ptr=0;
    h->cb_change= (cb_change ? cb_change :cb_change_dummy);

    h->esignal=LIST_signal_get_default();

    if (lockingmethod==LIST_LOCKING_METHOD_LOCKFLAG) {

        h->lops=&lockflag_header_lock_ops;

    } else {

        if (lockingmethod != LIST_LOCKING_METHOD_NONE) {

            logoutput_warning("%s: locking method %u not reckognized ... falling back to none", __FUNCTION__, lockingmethod);
            lockingmethod = LIST_LOCKING_METHOD_NONE;

        }

        h->lops=&dummy_header_lock_ops;
        h->flags |= LIST_HEADER_FLAG_NOLOCK;

    }

    head=&h->head;
    tail=&h->tail;

    LIST_header_init_headtail_elements(h, head, 0);
    LIST_header_init_headtail_elements(h, tail, 1);

    head->n=tail;
    head->p=tail;
    tail->p=head;
    tail->n=head;

}

/* user functions */

struct list_element_s *LIST_header_get_first(struct list_header_s *h)
{
    struct list_element_s *e=&h->head;
    return (* e->ops->get_next)(e);
}

struct list_element_s *LIST_header_get_last(struct list_header_s *h)
{
    struct list_element_s *e=&h->tail;
    return (* e->ops->get_prev)(e);
}

struct list_element_s *LIST_header_remove_first(struct list_header_s *h)
{
    struct list_element_s *e=LIST_header_get_first(h);
    if (e) LIST_element_remove(e);
    return e;
}

struct list_element_s *LIST_header_remove_last(struct list_header_s *h)
{
    struct list_element_s *e=LIST_header_get_last(h);
    if (e) LIST_element_remove(e);
    return e;
}

void LIST_header_add_last(struct list_header_s *h, struct list_element_s *e)
{
    struct list_element_s *tail=&h->tail;
    (* tail->ops->insert_before)(tail, e);
}

void LIST_header_add_first(struct list_header_s *h, struct list_element_s *e)
{
    struct list_element_s *head=&h->head;
    (* head->ops->insert_after)(head, e);
}
