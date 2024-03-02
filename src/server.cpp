#include "server.h"

#include <crypto.h>

std::shared_ptr<Client> Server::add_client(std::string id) {
    if (id2client.contains(id)) {
        id.append(std::to_string(dis(gen)));
    }
    std::shared_ptr<Client> new_client = std::make_shared<Client>(id, *this);
    id2client[id] = new_client;
    clients[new_client] = 5.0;
    return new_client;
}

bool Server::add_pending_trx(const std::string& trx, const std::string& signature) {
    std::string sender, receiver;
    double amount;
    if (parse_trx(trx, sender, receiver, amount)) {
        std::shared_ptr<Client> sender_client = get_client(sender);
        if (amount <= get_wallet(sender) && crypto::verifySignature(sender_client->get_publickey(), trx, signature)) {
            pending_trxs.emplace_back(trx);
            return true;
        }
    }
    return false;
}

std::shared_ptr<Client> Server::get_client(const std::string& id) const {
    auto result = id2client.find(id);
    if (result != id2client.end()) {
        return result->second;
    }
    return nullptr;
}

double Server::get_wallet(const std::string& id) {
    return clients[get_client(id)];
}

size_t Server::mine() {
    std::string mempool{};
    for (const std::string& trx : pending_trxs) {
        mempool.append(trx);
    }

    while(1) {
        for(const auto& pair : id2client) {
            auto client = pair.second;
            size_t nonce = client->generate_nonce();
            std::string sha256 = crypto::sha256(std::move(mempool + std::to_string(nonce)));
            int count = 0;
            for (const char &c : sha256.substr(0, 10)) {
                if (c != '0') {
                    count = 0;
                } else {
                    count++;
                }

                if (count >= 3) {
                    std::cout << "success mine: " << pair.first << " " << nonce;
                    clients[client] += 6.25;
                    for (const std::string& trx: pending_trxs) {
                        std::string sender, receiver;
                        double money;
                        parse_trx(trx, sender, receiver, money);
                        auto sender_client = id2client[sender];
                        auto receiver_client = id2client[receiver];
                        clients[sender_client] -= money;
                        clients[receiver_client] += money;
                    }
                    pending_trxs.clear();
                    return nonce;
                }
            }
        }
    }
}

bool Server::parse_trx(const std::string& trx, std::string& sender, std::string& receiver, double& value) {
    size_t start = 0;
    std::vector<std::string> parse_result{};
    for (size_t i = 0; i < trx.length(); i++) {
        if (trx[i] == '-') {
            parse_result.emplace_back(std::move(trx.substr(start, i - start)));
            start = i + 1;
        }
    }

    if (parse_result.size() != 2) {
        throw std::runtime_error("");
    }

    value = std::stod(trx.substr(start, trx.length()));
    sender = parse_result[0];
    receiver = parse_result[1];

    return true;
}

std::map<std::shared_ptr<Client>, double> Server::getClients() const {
    return this->clients;
}

Server::Server() : gen(rd()), dis(1000, 9999) {}