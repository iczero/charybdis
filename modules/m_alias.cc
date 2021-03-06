/* modules/m_alias.c - main module for aliases
 * Copyright (c) 2016 Elizabeth Myers <elizabeth@interlinked.me>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice is present in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ircd/stdinc.h>
#include <ircd/client.h>
#include <ircd/parse.h>
#include <ircd/msg.h>
#include <ircd/modules.h>
#include <ircd/s_conf.h>
#include <ircd/s_serv.h>
#include <ircd/hash.h>
#include <ircd/ircd.h>
#include <ircd/match.h>
#include <ircd/numeric.h>
#include <ircd/send.h>
#include <ircd/packet.h>

using namespace ircd;

static const char alias_desc[] = "Provides the system for services aliases";

static int _modinit(void);
static void _moddeinit(void);
static int reload_aliases(hook_data *);
static void m_alias(struct MsgBuf *, struct Client *, struct Client *, int, const char **);

mapi_hfn_list_av1 alias_hfnlist[] = {
	{ "rehash", (hookfn)reload_aliases },
	{ NULL, NULL },
};

DECLARE_MODULE_AV2(alias, _modinit, _moddeinit, NULL, NULL, alias_hfnlist, NULL, NULL, alias_desc);

static rb_dlink_list alias_messages;
static const struct MessageEntry alias_msgtab[] =
	{{m_alias, 2}, {m_alias, 2}, mg_ignore, mg_ignore, mg_ignore, {m_alias, 2}};

static inline void
create_aliases(void)
{
	s_assert(rb_dlink_list_length(&alias_messages) == 0);

	for (auto it = alias_dict.begin(); it != alias_dict.end(); it++)
	{
		std::shared_ptr<alias_entry> alias = it->second;
		struct Message *message = (Message *)rb_malloc(sizeof(*message) + alias->name.size() + 1);
		char *cmd = (char*)message + sizeof(*message);

		/* copy the alias name as it will be freed early on a rehash */
		strcpy(cmd, alias->name.c_str());
		message->cmd = cmd;
		memcpy(message->handlers, alias_msgtab, sizeof(alias_msgtab));

		mod_add_cmd(message);
		rb_dlinkAddAlloc(message, &alias_messages);
	}
}

static inline void
destroy_aliases(void)
{
	rb_dlink_node *ptr, *nptr;

	RB_DLINK_FOREACH_SAFE(ptr, nptr, alias_messages.head)
	{
		mod_del_cmd((struct Message *)ptr->data);
		rb_free(ptr->data);
		rb_dlinkDestroy(ptr, &alias_messages);
	}
}

static int
_modinit(void)
{
	create_aliases();
	return 0;
}

static void
_moddeinit(void)
{
	destroy_aliases();
}

static int
reload_aliases(hook_data *data)
{
	destroy_aliases(); /* Clear old aliases */
	create_aliases();
	return 0;
}

/* The below was mostly taken from the old do_alias */
static void
m_alias(struct MsgBuf *msgbuf, struct Client *client_p, struct Client *source_p, int parc, const char **parv)
{
	struct Client *target_p;
	std::shared_ptr<alias_entry> aptr = alias_dict[msgbuf->cmd];
	char *str;

	if(aptr == NULL)
	{
		/* This shouldn't happen... */
		if(IsPerson(client_p))
			sendto_one(client_p, form_str(ERR_UNKNOWNCOMMAND),
				me.name, client_p->name, msgbuf->cmd);

		return;
	}

	if(!IsFloodDone(client_p) && client_p->localClient->receiveM > 20)
		flood_endgrace(client_p);

	auto pos = aptr->target.find('@');
	if(pos != std::string::npos)
	{
		/* user@server */
		target_p = find_server(NULL, aptr->target.substr(pos + 1).c_str());
		if(target_p != NULL && IsMe(target_p))
			target_p = NULL;
	}
	else
	{
		/* nick, must be +S */
		target_p = find_named_person(aptr->target.c_str());
		if(target_p != NULL && !IsService(target_p))
			target_p = NULL;
	}

	if(target_p == NULL)
	{
		sendto_one_numeric(client_p, ERR_SERVICESDOWN, form_str(ERR_SERVICESDOWN), aptr->target.c_str());
		return;
	}

	str = reconstruct_parv(parc - 1, &parv[1]);
	if(EmptyString(str))
	{
		sendto_one(client_p, form_str(ERR_NOTEXTTOSEND), me.name, target_p->name);
		return;
	}

	sendto_one(target_p, ":%s PRIVMSG %s :%s",
			get_id(client_p, target_p),
			pos != std::string::npos ? aptr->target.c_str() : get_id(target_p, target_p),
			str);
}
