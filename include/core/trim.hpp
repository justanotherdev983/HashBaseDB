#include <string>

namespace Trim {
    const std::string whiteSpaces( " \f\n\r\t\v" );
    void trim_right(std::string& str, const std::string& trimChars = whiteSpaces) {
        std::string::size_type pos = str.find_last_not_of( trimChars );
        str.erase( pos + 1 );    
    }
    void trim_left( std::string& str, const std::string& trimChars = whiteSpaces) {
        std::string::size_type pos = str.find_first_not_of( trimChars );
        str.erase( 0, pos );
    }


    std::string trim(std::string& str, const std::string& trimChars = whiteSpaces) {
        trim_right(str, trimChars);
        trim_left(str, trimChars);
        return str;
    } 
}
