#ifndef ADVENTURE2016_CONNECTIONMANAGER_HPP
#define ADVENTURE2016_CONNECTIONMANAGER_HPP

namespace networking {
class Connection;
}


class ConnectionManager {
public:
    virtual void disconnectClient(const networking::Connection& connection) = 0;
};

#endif //ADVENTURE2016_CONNECTIONMANAGER_HPP
