//
//  Logger.h
//  Nomoku
//
//  Used as logger
//

#ifndef Logger_h
#define Logger_h
#include <string>

enum{ LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };
void logEvent(int level, std::string log_info, ...);

#endif /* Logger_h */
