#pragma once

#include <lm/logger.h>
#include <string>

namespace appenv {

std::string get_lib_version();

class env {

	public:
	            env(
					const std::string& _app_name,
					lm::logger* = nullptr
				);
/**
 * changes the execution dir so it matches those given in an appimage.
 */
	void                appimagefy();

/**
 * creates the application directory in the user's homedir if needed. 
 * */
	void                create_user_dir() const;

/**
 * copies a file from the app dir to the user dir. The first parameter can be
 * considered the execution dir so it should be followed by assets/, data/, etc.
 * The second parameter must be considered as the application directory inside
 * the user's home dir. Subdirectories must already exist! No copy will be 
 * performed if the destination already exists. No leading slashes are needed. 
 */
	void                copy_from_app_to_home(const std::string&, const std::string&) const;

/**
* Returns the string where the installed application data can be
* found (configuration, mostly), ending in a slash.
*/
	std::string         build_app_path(const std::string&) const;
/**
 *returns the user data path for this application under $HOME, ending in a slash.
 */
	std::string         build_user_path(const std::string&) const;

	private:

	lm::logger*         logger;
	std::string         exec_dir;
	std::string         user_dir;
};

}
