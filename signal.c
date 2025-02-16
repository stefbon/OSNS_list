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

static pthread_mutex_t list_mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t list_cond=PTHREAD_COND_INITIALIZER;

struct event_shared_signal_s list_shared_signal_initializer;

#define LIST_SIGNAL_INITIALIZER_INIT_DONE                       1

struct list_signal_initializer_s {
    unsigned int status;
    struct event_shared_signal_s *(* cb_get_event_signal)();
};

static struct event_shared_signal_s *list_signal_get_default_init();
static struct event_shared_signal_s *list_signal_get_default();

static struct list_signal_initializer_s initializer = {
    .status                     = 0,
    .cb_get_event_signal        = list_signal_get_default_init,
};

static struct event_shared_signal_s *list_signal_get_default_init()
{

    pthread_mutex_lock(&list_mutex);

    if ((initializer.status & LIST_SIGNAL_INITIALIZER_INIT_DONE)==0) {

        initializer.status |= LIST_SIGNAL_INITIALIZER_INIT_DONE;
        initializer.cb_get_event_signal = list_signal_get_default;

        list_shared_signal_initializer = EVENT_SHARED_SIGNAL_CUSTOM_INIT;
        list_shared_signal_initializer.mutex=&list_mutex;
        list_shared_signal_initializer.cond=&list_cond;

    }

    pthread_mutex_unlock(&list_mutex);

    return (* initializer.cb_get_event_signal)();

}

static struct event_shared_signal_s *list_signal_get_default()
{
    return &list_shared_signal_initializer;
}

struct event_shared_signal_s *LIST_signal_get_default()
{
    return (* initializer.cb_get_event_signal)();
}
