/*
 * Remote oper up notices.
 */

#include <ircd/stdinc.h>
#include <ircd/modules.h>
#include <ircd/client.h>
#include <ircd/hook.h>
#include <ircd/ircd.h>
#include <ircd/send.h>
#include <ircd/s_conf.h>
#include <ircd/snomask.h>

using namespace ircd;

static const char sno_desc[] =
	"Adds server notices for remote oper up";

static void h_sgo_umode_changed(void *);

mapi_hfn_list_av1 sgo_hfnlist[] = {
	{ "umode_changed", (hookfn) h_sgo_umode_changed },
	{ NULL, NULL }
};

DECLARE_MODULE_AV2(sno_globaloper, NULL, NULL, NULL, NULL, sgo_hfnlist, NULL, NULL, sno_desc);

static void
h_sgo_umode_changed(void *vdata)
{
	hook_data_umode_changed *data = (hook_data_umode_changed *)vdata;
	struct Client *source_p = data->client;

	if (MyConnect(source_p) || !HasSentEob(source_p->servptr))
		return;

	if (!(data->oldumodes & UMODE_OPER) && IsOper(source_p))
		sendto_realops_snomask_from(SNO_GENERAL, L_ALL, source_p->servptr,
				"%s (%s@%s) is now an operator",
				source_p->name, source_p->username, source_p->host);
}
