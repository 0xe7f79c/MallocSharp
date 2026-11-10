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
		std::string err;
		err.reserve(sizeof(b_err) + std::strlen(path.data()) + 1);
		err += b_err;
		err += path.data();
		err += '\"';
		return err;
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