#pragma once
// file.hpp

#include "error.hpp"

#include <cstring>
#include <cstdio>

namespace ms {

ms::maybe<std::string> read_file(std::string_view path) {
	std::FILE* fp = std::fopen(path.data(), "rb");
	if(!fp) {
		constexpr char b_err[] = "cannot open file \"";
		ms::error err;
		err.w.reserve(sizeof(b_err) + std::strlen(path.data()) + 1);
		err.w += b_err;
		err.w += path.data();
		err.w += '\"';
		return std::unexpected(err);
	}
	std::string f_s;

	std::fseek(fp, 0, SEEK_END);
	f_s.resize(std::ftell(fp));
	std::rewind(fp);
	std::fread(f_s.data(), 1, f_s.size(), fp);

	std::fclose(fp);
	return f_s;
}

} // namespace ms