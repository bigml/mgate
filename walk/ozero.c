#include "mheads.h"
#include "lheads.h"
#include "ozero.h"

NEOERR* zero_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "aic");
    NEOERR *err;

    MCS_NOT_NULLB(cgi->hdf, evt);

    err = ldml_render("zero", "video", cgi->hdf, mcs_fetch_nodef(cgi->hdf, PRE_OUTPUT));
    if (err != STATUS_OK) return nerr_pass(err);

    return STATUS_OK;
}

NEOERR* zero_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "aic");

    MCS_NOT_NULLB(cgi->hdf, evt);

    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".302=%s", "http://www.google.cn/");

    return nerr_raise(LERR_REDIRECT, "redirect");
}

NEOERR* zero_image_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    char fname[LEN_MD5];
    int ftype;
	NEOERR *err;

    err = mimg_accept(cgi, "upfile", ROOT_IMG, fname, &ftype);
	if (err != STATUS_OK) return nerr_pass(err);

    char tok[3] = {0}; strncpy(tok, fname, 2);

    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=%s%s/%s.%s",
                   URL_IMG, tok, fname, mimg_type_int2str(ftype));
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.%s",
                   fname, mimg_type_int2str(ftype));

    return STATUS_OK;
}
