/*
 * Do not allow operators to be kicked from +M channels.
 *     -- kaniini
 */

#include <ircd/stdinc.h>
#include <ircd/modules.h>
#include <ircd/hook.h>
#include <ircd/client.h>
#include <ircd/ircd.h>
#include <ircd/send.h>
#include <ircd/hash.h>
#include <ircd/s_conf.h>
#include <ircd/s_user.h>
#include <ircd/s_serv.h>
#include <ircd/numeric.h>
#include <ircd/privilege.h>
#include <ircd/s_newconf.h>
#include <ircd/chmode.h>

using namespace ircd;

static const char chm_operpeace_desc[] =
	"Adds channel mode +M which prohibits operators from being kicked";

static void hdl_can_kick(hook_data_channel_approval *);

mapi_hfn_list_av1 chm_operpeace_hfnlist[] = {
	{ "can_kick", (hookfn) hdl_can_kick },
	{ NULL, NULL }
};

static unsigned int mymode;

static int
_modinit(void)
{
	mymode = cflag_add('M', CHM_D, chm_hidden);
	if (mymode == 0)
		return -1;

	return 0;
}

static void
_moddeinit(void)
{
	cflag_orphan('M');
}

DECLARE_MODULE_AV2(chm_operpeace, _modinit, _moddeinit, NULL, NULL, chm_operpeace_hfnlist, NULL, NULL, chm_operpeace_desc);

static void
hdl_can_kick(hook_data_channel_approval *data)
{
	struct Client *source_p = data->client;
	struct Client *who = data->target;
	struct Channel *chptr = data->chptr;

	if(IsOper(source_p))
		return;

	if((chptr->mode.mode & mymode) && IsOper(who))
	{
		sendto_realops_snomask(SNO_GENERAL, L_NETWIDE, "%s attempted to kick %s from %s (which is +M)",
			source_p->name, who->name, chptr->chname);
		sendto_one_numeric(source_p, ERR_ISCHANSERVICE, "%s %s :Cannot kick IRC operators from that channel.",
			who->name, chptr->chname);
		data->approved = 0;
	}
}
