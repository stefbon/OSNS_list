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

/* DEFAULT - INSERT AFTER */

void cb_list_element_insert_after(struct list_element_s *p, struct list_element_s *e)
{
    struct list_header_s *h=NULL;

    if ((p==NULL) || (p->h==NULL) || (e==NULL)) {

        logoutput_debug("%s: previous, header and/or element not defined", __FUNCTION__);
        return;

    }

    // logoutput_debug("%s: previous ops %s", __FUNCTION__, p->ops->name);

    h=p->h;
    LIST_element_init(e, h);

    /* insert after p (and before n) */

    if ((* h->lops->lock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT)) {
        struct list_element_s *n=p->n;

        if ((* h->lops->lock_element)(h, n, LIST_OP_LOCK_TYPE_PREV)) {

            n->p=e;
            p->n=e;
            e->p=p;
            e->n=n;

            if ((* h->lops->lock_header)(h, LIST_OP_LOCK_TYPE_SELF)) {

                h->count++;
                (* h->lops->unlock_header)(h, LIST_OP_LOCK_TYPE_SELF);

            }

            (* h->lops->unlock_element)(h, n, LIST_OP_LOCK_TYPE_PREV);

        }

        (* h->lops->unlock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT);

    }

}

/* DEFAULT - INSERT BEFORE */

void cb_list_element_insert_before(struct list_element_s *n, struct list_element_s *e)
{
    struct list_header_s *h=NULL;

    if ((n==NULL) || (n->h==NULL) || (e==NULL)) {

        logoutput_debug("%s: next, header and/or element not defined", __FUNCTION__);
        return;

    }

    // logoutput_debug("%s: next ops %s", __FUNCTION__, n->ops->name);

    h=n->h;
    LIST_element_init(e, h);

    /* insert before n (and after p) */

    if ((* h->lops->lock_element)(h, n, LIST_OP_LOCK_TYPE_PREV)) {
        struct list_element_s *p=n->p;

        if ((* h->lops->lock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT)) {

            p->n=e;
            n->p=e;
            e->n=n;
            e->p=p;

            if ((* h->lops->lock_header)(h, LIST_OP_LOCK_TYPE_SELF)) {

                h->count++;
                (* h->lops->unlock_header)(h, LIST_OP_LOCK_TYPE_SELF);

            }

            (* h->lops->unlock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT);

        }

        (* h->lops->unlock_element)(h, n, LIST_OP_LOCK_TYPE_PREV);

    }

}

void cb_list_element_insert_null(struct list_element_s *n, struct list_element_s *e)
{}
