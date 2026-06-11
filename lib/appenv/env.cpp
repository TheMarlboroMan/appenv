#include <appenv/env.h>
#include <tools/file_utils.h>
#include <lm/log.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>

#ifdef WINBUILD
#include <filesystem>
#endif

using namespace appenv;

env::env(
	const std::string& _app_name,
	lm::logger* _logger
):
	logger{_logger}
{

#ifndef WINBUILD

	std::array<char, 1024> buff;

	int bytes=readlink("/proc/self/exe", buff.data(), 1024);
	if(-1==bytes) {

		std::stringstream ss;
		ss<<"could not locate proc/self/exe, error "<<errno<<std::endl;
		throw std::runtime_error(ss.str());
	}

	std::string executable_path{std::begin(buff), std::begin(buff)+bytes};
	//Remove executable filename...
	auto last_slash=executable_path.find_last_of("/");
	exec_dir=executable_path.substr(0, last_slash);
	exec_dir.append(1, '/');

	user_dir=std::string{getenv("HOME")};
	user_dir+="/"+_app_name;
	user_dir.append(1, '/');

#else

	auto curpath=std::filesystem::current_path();
	exec_dir=curpath.string();
	user_dir=exec_dir+"/"+_app_name;

#endif
}

void env::appimagefy() {

	exec_dir+="/../share/";
	lm::log(*logger).info()<<"appimagefying, exec_dir is now "<<exec_dir<<std::endl;
}

void env::create_user_dir() const {
	
	if(!tools::filesystem::exists(user_dir)) {

		if(nullptr!=logger) {
			
			lm::log(*logger).info()<<"will create the "<<user_dir<<" directory"<<std::endl;
		}		

		tools::filesystem::create_directory(user_dir);
	}
}

void env::copy_from_app_to_home(
	const std::string& _origin_path,
	const std::string& _destination_path
) const {

	std::string origin=build_app_path(_origin_path),
				destination=build_user_path(_destination_path);

	if(!tools::filesystem::exists(destination)) {
	
		if(nullptr!=logger) {

			lm::log(*logger).info()<<"will copy "<<origin<<" to "<<destination<<std::endl;
			tools::filesystem::copy(origin, destination);
		};
	}
}

std::string env::build_app_path(
	const std::string& _file
) const {

	return exec_dir+_file;
}

std::string env::build_user_path(
	const std::string& _file
) const {

	return user_dir+_file;
}
