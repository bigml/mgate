#include "mheads.h"
#include "lheads.h"

/* LERR_XXX start from 25 */

/*
 * static error (layout.html used)
 */
int LERR_NOTLOGIN  = 0;            /* 25 */
int LERR_LOGINPSW  = 0;            /* 26 */
int LERR_LIMIT     = 0;            /* 27 */
int LERR_MISS_DATA = 0;            /* 28 */
int LERR_MISS_TPL  = 0;            /* 29 */
int LERR_ATTACK    = 0;            /* 30 */
int LERR_REDIRECT  = 0;            /* 31 */


static int lerrInited = 0;

NEOERR* lerr_init()
{
    NEOERR *err;

    if (lerrInited == 0) {
        err = nerr_init();
        if (err != STATUS_OK) return nerr_pass(err);

        err = merr_init((MeventLog)mtc_msg);
        if (err != STATUS_OK) return nerr_pass(err);

        err = nerr_register(&LERR_NOTLOGIN, "请登录后操作");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_LOGINPSW, "密码错误");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_LIMIT, "用户无权限");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_MISS_DATA, "请求的资源不存在");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_MISS_TPL, "找不到渲染模板(忘记了/json ?)");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_ATTACK, "太过频繁，请稍后请求！");
        if (err != STATUS_OK) return nerr_pass(err);
        err = nerr_register(&LERR_REDIRECT, "页面重定向！");
        if (err != STATUS_OK) return nerr_pass(err);

        lerrInited = 1;
    }

    return STATUS_OK;
}

void lerr_opfinish_json(NEOERR *err, HDF *hdf)
{
    if (err == STATUS_OK) {
        hdf_set_value(hdf, PRE_SUCCESS, "1");
        mcs_set_int_attr(hdf, PRE_SUCCESS, "type", CNODE_TYPE_INT);
        return;
    }

    hdf_remove_tree(hdf, PRE_SUCCESS);

    NEOERR *neede = mcs_err_valid(err);
    /* set PRE_ERRXXX with the most recently err */
    mcs_set_int_value_with_type(hdf, PRE_ERRCODE, neede->error, CNODE_TYPE_INT);
    if (!hdf_get_obj(hdf, PRE_ERRMSG)) {
        hdf_set_valuef(hdf, "%s=%s:%d %s",
                       PRE_ERRMSG, neede->file, neede->lineno, neede->desc);
    }

    STRING str; string_init(&str);
    nerr_error_traceback(err, &str);
    mtc_err("%s", str.buf);
    hdf_set_value(hdf, PRE_ERRTRACE, str.buf);
    nerr_ignore(&err);
    string_clear(&str);
}
