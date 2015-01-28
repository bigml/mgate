#ifndef __LCFG_H__
#define __LCFG_H__

#include "mheads.h"

__BEGIN_DECLS

#define SITE_DOMAIN    "voov.com"
#define TC_ROOT        "/var/log/moon/voov/"

#define URL_DATA       "http://www.voov.com/data/"
#define URL_IMG         URL_DATA"images/"

#define ROOT_DATA       "/var/www/voov/fly/data/"
#define ROOT_IMG        ROOT_DATA"images/"

/*
 * SITE_CONFIG 可以在命令行指定
 * PATH_SITE 可以在配置文件中通过"Config.site_path"指定
 * PATH_XXX 首先以配置文件中Config.site_path/PATH_XXX为准，否则使用默认配置 PATH_SITE/PATH_XXX
 */
#define PATH_SITE   "/var/www/voov/"
#define SITE_CONFIG PATH_SITE"config.hdf"

#define PATH_MODE   "walk/"     /* modler */
#define PATH_CGI    "run/"      /* controller */
#define PATH_DOC    "fly/"      /* viewer */

#define PATH_MTL    "mtl/"      /* operation tools */
#define PATH_TPL    "tpl/"      /* template */
#define PATH_DATA   "dml/"      /* data modifier */

#define PATH_PAGER  "pager/"
/* pub */
/* mevent */
/* xport */
/* lab */
/* doc */

#define PRE_REQ_IP      PRE_CGI".RemoteAddress"
#define PRE_REQ_URI     PRE_CGI".ScriptName"    /* lutil_file_access() */
#define PRE_REQ_URI_RW  PRE_QUERY".ScriptName"  /* lutil_file_access_rewrited() */
#define PRE_REQ_AJAX_FN PRE_QUERY".JsonCallback"/* main() */

#define PRE_CFG_OUTPUT      "Output"
#define PRE_CFG_LAYOUT      "Layout"
#define PRE_CFG_DATASET     "Dataset"
#define PRE_CFG_REQLIMIT    "ClientReqLimit"
#define PRE_CFG_FILECACHE   "FileCache"
#define PRE_CFG_DATAER      "DataGeter"

#define PRE_WALK_SACTION    "SpecialAction"

#define PRE_MMC_COUNT        "Ttnum"

#define REXP_EMAIL "^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$"

__END_DECLS
#endif    /* __LCFG_H__ */
