#include "mheads.h"
#include "lheads.h"

#ifdef __MACH__
int ldml_config(struct dirent *ent)
#else
int ldml_config(const struct dirent *ent)
#endif
{
    if (reg_search(".*.hdf", ent->d_name))
        return 1;
    else
        return 0;
}

NEOERR* ldml_parse_file(char *dir, char *name, HASH *outhash)
{
    char fname[_POSIX_PATH_MAX], *attrval = NULL;
    HDF *node, *child, *dhdf;
    STRING str;
    NEOERR *err;

    memset(fname, 0x0, sizeof(fname));
    snprintf(fname, sizeof(fname), "%s/%s", dir, name);

    err = hdf_init(&node);
    if (err != STATUS_OK) return nerr_pass(err);

    err = hdf_read_file(node, fname);
    if (err != STATUS_OK) return nerr_pass(err);

    child = hdf_obj_child(node);
    while (child != NULL) {
        mtc_dbg("parse node %s", hdf_obj_name(child));
        string_init(&str);

        attrval = mcs_obj_attr(child, "merge");
        if (attrval) {
            ULIST *list;
            string_array_split(&list, attrval, ",", 10);
            ITERATE_MLIST(list) {
                snprintf(fname, sizeof(fname), "%s/%s",
                         dir, neos_strip((char*)list->items[t_rsv_i]));
                err = hdf_init(&dhdf);
                JUMP_NOK(err, wnext);
                err = hdf_read_file(dhdf, fname);
                JUMP_NOK(err, wnext);
                err = hdf_copy(child, NULL, dhdf);
                JUMP_NOK(err, wnext);
            }
            uListDestroy(&list, ULIST_FREE);
        }

    wnext:
        string_clear(&str);
        child = hdf_obj_next(child);
    }

    err = hash_insert(outhash, (void*)strdup(name), (void*)node);
    JUMP_NOK(err, wnext);

    return STATUS_OK;
}

NEOERR* ldml_parse_dir(char *dir, HASH *outhash)
{
    struct dirent **eps = NULL;
    int n;
    NEOERR *err;

    if (!dir) return nerr_raise(NERR_ASSERT, "can't read null directory");

    n = scandir(dir, &eps, ldml_config, alphasort);
    for (int i = 0; i < n; i++) {
        mtc_dbg("parse file %s", eps[i]->d_name);
        err = ldml_parse_file(dir, eps[i]->d_name, outhash);
        TRACE_NOK(err);
        free(eps[i]);
    }

    if (n > 0) free(eps);
    else mtc_warn("no .hdf file found in %s", dir);

    return STATUS_OK;
}

NEOERR* ldml_init(HASH **datah, char *path)
{
    HASH *ldatah = NULL;
    char path_data[_POSIX_PATH_MAX], *temps;
    NEOERR *err;

    *datah = NULL;

    temps = hdf_get_value(g_cfg, PRE_CONFIG".site_path", NULL);
    if (!temps || *temps == '\0') temps = PATH_SITE;

    snprintf(path_data, sizeof(path_data), "%s/%s/", temps, PATH_DATA);

    path = path ? path: path_data;

    err = hash_init(&ldatah, hash_str_hash, hash_str_comp, hash_str_free);
    if (err != STATUS_OK) return nerr_pass(err);

    err = ldml_parse_dir(path, ldatah);
    if (err != STATUS_OK) return nerr_pass_ctx(err, "pase dir %s", path);

    *datah = ldatah;
    return STATUS_OK;
}

NEOERR* ldml_render(char *filename, char *keyname, HDF *datanode, HDF *outnode)
{
    HASH *datah;
    HDF *confignode;

    MCS_NOT_NULLC(filename, keyname, outnode);

    datah = hash_lookup(g_datah, "runtime_dml");
    MCS_NOT_NULLA(datah);

    confignode = hash_lookupf(datah, "%s.hdf", filename);
    if (!confignode) return nerr_raise(NERR_ASSERT, "data file %s nexist", filename);

    confignode = hdf_get_obj(confignode, keyname);
    if (!confignode) return nerr_raise(NERR_ASSERT, "%s.hdf %s nexist",
                                       filename, keyname);

    return nerr_pass(mcs_data_rend(confignode, datanode, outnode));
}

void ldml_destroy(HASH *datah)
{
    char *key, *buf;

    if (!datah) return;

    buf = (char*)hash_next(datah, (void**)&key);

    while (buf != NULL) {
        /* TODO free them */
        free(buf);
        buf = hash_next(datah, (void**)&key);
    }

    hash_destroy(&datah);
}
