// mallocsharp.cpp

#include <iostream>

#include <dpp/dpp.h>
#include "../lib/error.hpp"
#include "../lib/file.hpp"

int main(int, char**) {
	// get bot token
	std::string tok;
	{
		auto m_tok = ms::read_file("../token");
		if(!m_tok) {
			std::cerr << "error: " << m_tok.error() << '\n';
			return -1;
		}
		tok = *std::move(m_tok);
	}

	dpp::cluster bot(tok);
	// send log info to cout
	bot.on_log(dpp::utility::cout_logger());

	// this handles when the user types /ping
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if(event.command.get_command_name() == "ping") {
			event.reply("pong");
		}
	});

	// this registers the slash command
	bot.on_ready( [&bot] (const dpp::ready_t& event) {
		if(dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(dpp::slashcommand("ping", "ping pong!", bot.me.id));
		}
	} );

	bot.start(dpp::st_wait);
}