#ifndef __LDML_H__
#define __LDML_H__
#include "mheads.h"

__BEGIN_DECLS

#ifdef __MACH__
int ldml_config(struct dirent *ent);
#else
int ldml_config(const struct dirent *ent);
#endif

NEOERR* ldml_parse_file(char *dir, char *name, HASH *outhash);
NEOERR* ldml_parse_dir(char *dir, HASH *outhash);
NEOERR* ldml_init(HASH **datah, char *path);

NEOERR* ldml_render(char *filename, char *keyname, HDF *datanode, HDF *outnode);

void ldml_destroy(HASH *datah);

__END_DECLS
#endif    /* __LDML_H__ */
