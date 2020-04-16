#ifndef RXUSRP_H
#define RXUSRP_H

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
class RxUsrp
{
public:
    RxUsrp();
};

#endif // RXUSRP_H
