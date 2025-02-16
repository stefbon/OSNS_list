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

void cb_list_element_remove(struct list_element_s *e)
{
    struct list_header_s *h=e->h;

    if (h==NULL) return;

    /* remove by linking the n to the p and vice versa */

    if ((* h->lops->lock_element)(h, e, LIST_OP_LOCK_TYPE_NEXT)) {

        if ((* h->lops->lock_element)(h, e, LIST_OP_LOCK_TYPE_PREV)) {
            struct list_element_s *n=e->n;
            struct list_element_s *p=e->p;

            if ((* h->lops->lock_element)(h, n, LIST_OP_LOCK_TYPE_PREV)) {

                if ((* h->lops->lock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT)) {

                    n->p=p;
                    p->n=n;
                    LIST_element_init(e, NULL);

                    if ((* h->lops->lock_header)(h, LIST_OP_LOCK_TYPE_SELF)) {

                        h->count--;
                        (* h->lops->unlock_header)(h, LIST_OP_LOCK_TYPE_SELF);

                    }

                    (* h->lops->unlock_element)(h, p, LIST_OP_LOCK_TYPE_NEXT);

                }

                (* h->lops->unlock_element)(h, n, LIST_OP_LOCK_TYPE_PREV);

            }

            (* h->lops->unlock_element)(h, e, LIST_OP_LOCK_TYPE_PREV);

        }

        (* h->lops->unlock_element)(h, e, LIST_OP_LOCK_TYPE_NEXT);

    }

}
