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

struct list_element_s *cb_list_element_get_next(struct list_element_s *e)
{
    struct list_element_s *n=NULL;
    struct list_header_s *h=e->h;

    if (h==NULL) return NULL;

    if ((* h->lops->lock_element)(h, e, LIST_OP_LOCK_TYPE_NEXT)) {

        n=e->n;
        (* h->lops->unlock_element)(h, e, LIST_OP_LOCK_TYPE_NEXT);

    }

    return (* n->ops->get_self)(n);
}

struct list_element_s *cb_list_element_get_prev(struct list_element_s *e)
{
    struct list_element_s *p=NULL;
    struct list_header_s *h=e->h;

    if (h==NULL) return NULL;

    if ((* h->lops->lock_element)(h, e, LIST_OP_LOCK_TYPE_PREV)) {

        p=e->p;
        (* h->lops->unlock_element)(h, e, LIST_OP_LOCK_TYPE_PREV);

    }

    return (* p->ops->get_self)(p);
}

/* get next element of the (system) head element
    NOTE: this element is a system element, and is part of the header, and never of the list itself
        it's declared here to have an uniform interface */

struct list_element_s *cb_list_element_get_next_head(struct list_element_s *e)
{
    struct list_header_s *h=e->h;
    return cb_list_element_get_next(e);
}

/* get prev element of the (system) tail element
    NOTE: this element is a system element, and is part of the header, and never of the list itself
        it's declared here to have an uniform interface */

struct list_element_s *cb_list_element_get_prev_tail(struct list_element_s *e)
{
    struct list_header_s *h=e->h;
    return cb_list_element_get_prev(e);
}
