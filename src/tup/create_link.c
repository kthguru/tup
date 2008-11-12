#include "fileio.h"
#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int link_cb(void *id, int argc, char **argv, char **col);

int find_link(const char *from, const char *to)
{
	tupid_t id = -1;
	int rc;

	rc = tup_db_select(link_cb, &id,
			   "select from_id from link where from_id in (select id from node where name='%q') and to_id in (select id from node where name='%q')",
			   from, to);
	if(rc == 0 && id != -1)
		return 0;

	return -1;
}

static int link_cb(void *id, int argc, char **argv, char **col)
{
	int x;
	tupid_t *iptr = id;

	for(x=0; x<argc; x++) {
		if(strcmp(col[x], "from_id") == 0) {
			*iptr = atoll(argv[x]);
			return 0;
		}
	}
	return -1;
}
