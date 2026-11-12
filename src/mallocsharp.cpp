// mallocsharp.cpp

#include <iostream>
#include <vector>
#include <memory>

#include <dpp/dpp.h>
#include "../lib/error.hpp"
#include "../lib/file.hpp"

struct slash_command {
	slash_command() : cmd() {}
	slash_command(dpp::slashcommand&& i_cmd) : cmd( std::move(i_cmd) ) {}
	slash_command(const dpp::slashcommand& i_cmd) : cmd(i_cmd) {}
public:
	virtual void operator()(dpp::cluster&, const dpp::slashcommand_t&) = 0;
	dpp::slashcommand cmd;
};

struct pong_command : slash_command {
	pong_command() : slash_command() {}
	pong_command(dpp::cluster& c) : slash_command(dpp::slashcommand("ping", "ping pong!", c.me.id)) {}
public:
	void operator()(dpp::cluster& c, const dpp::slashcommand_t& e_sc) override {
		n_ps++;
		std::string reply = "pong! # of times replied = " + std::to_string(n_ps);
		e_sc.reply(reply);
	}
	int n_ps = 0;
};

int main(int, char**) {
	// get bot token
	std::string tok;
	{
		auto m_tok = ms::read_file("../token");
		if(!m_tok) {
			std::cerr << "error: " << m_tok.error().w << '\n';
			return -1;
		}
		tok = *std::move(m_tok);
	}

	dpp::cluster b(tok);
	std::vector<std::unique_ptr<slash_command>> scs; // we have to do this because of the polymorphism instead of std::vector<slash_command>

	scs.emplace_back( std::make_unique<pong_command>(b) );

	// send log info to cout
	b.on_log(dpp::utility::cout_logger());

	b.on_ready([&](const dpp::ready_t& e_r) {
		for(const auto& sc : scs)
			b.global_command_create(sc->cmd);
	});

	b.on_slashcommand([&](const dpp::slashcommand_t& e_sc) {
		for(const auto& sc : scs)
			if(e_sc.command.get_command_name() == sc->cmd.name)
				(*sc)(b, e_sc);
	});

	b.start(dpp::st_wait);
}