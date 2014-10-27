#include "mheads.h"
#include "lheads.h"

HDF *g_cfg = NULL;
HASH *g_datah = NULL;

int main(int argc, char *argv[])
{
    NEOERR *err;
    char *temps, fname[_POSIX_PATH_MAX];

    temps = argv[1] ? argv[1] : SITE_CONFIG;

    err = mcfg_parse_file(temps, &g_cfg);
    DIE_NOK_MTL(err);

    temps = hdf_get_value(g_cfg, PRE_CONFIG".trace_path", NULL);
    if (!temps || *temps == '\0') temps = TC_ROOT;
    snprintf(fname, sizeof(fname), "%s/static", temps);

    mtc_init(fname, hdf_get_int_value(g_cfg, PRE_CONFIG".trace_level", TC_DEFAULT_LEVEL));

    temps = hdf_get_value(g_cfg, PRE_CONFIG".site_path", NULL);
    if (!temps || *temps == '\0') temps = PATH_SITE;

    snprintf(fname, sizeof(fname), "%s/%s/config/static/oncetime", temps, PATH_TPL);
    err = ltpl_parse_dir(fname, NULL);
    DIE_NOK_MTL(err);

    return 0;
}
