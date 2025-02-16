/*

  2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017 Stef Bon <stefbon@gmail.com>

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

#ifndef LIB_LIST_HEADER_H
#define LIB_LIST_HEADER_H

#include 					                "list.h"

extern struct list_header_s LIST_header_initializer;
#define	INIT_LIST_HEADER		                        LIST_header_initializer
#define	LIST_HEADER_INIT		                        LIST_header_initializer

/* prototypes */

void LIST_header_init(struct list_header_s *h, unsigned int lockingmethod, void (* cb_change)(struct list_header_s *h, unsigned char opcode));

struct list_element_s *LIST_header_get_first(struct list_header_s *h);
struct list_element_s *LIST_header_remove_first(struct list_header_s *h);

struct list_element_s *LIST_header_get_last(struct list_header_s *h);
struct list_element_s *LIST_header_remove_last(struct list_header_s *h);

void LIST_header_add_last(struct list_header_s *h, struct list_element_s *e);
void LIST_header_add_first(struct list_header_s *h, struct list_element_s *e);

#endif
