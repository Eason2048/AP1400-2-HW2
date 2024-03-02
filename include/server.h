#ifndef SERVER_H
#define SERVER_H

#include <client.h>
#include <iostream>
#include <map>
#include <random>
#include <string>

class Client;

class Server
{
public:
    Server();
    std::shared_ptr<Client> add_client(std::string id);
    std::shared_ptr<Client> get_client(const std::string& id) const;
    double get_wallet(const std::string& id);
    static bool parse_trx(const std::string& trx, std::string &sender, std::string& receiver, double& value);
    bool add_pending_trx(const std::string& trx, const std::string& signature);
    size_t mine();
    std::map<std::shared_ptr<Client>, double> getClients() const;

private:
    std::map<std::string, std::shared_ptr<Client>> id2client;
    std::map<std::shared_ptr<Client>, double> clients;
    // generate the random number
    std::random_device rd;
    std::mt19937_64 gen;
    std::uniform_int_distribution<size_t> dis;
};

inline std::vector<std::string> pending_trxs;

inline void show_pending_transactions()
{
    std::cout  <<  std::string(20, '*') <<  std::endl;
    for(const  auto& trx : pending_trxs)
        std::cout << trx <<  std::endl;
    std::cout  <<  std::string(20, '*') <<  std::endl;
}

inline void show_wallets(const  Server& server)
{
    std::cout << std::string(20, '*') << std::endl;
    for(const auto& client: server.getClients())
        std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}

#endif //SERVER_H