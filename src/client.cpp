#include "client.h"

#include <crypto.h>
#include <server.h>
#include <random>

size_t Client::generate_nonce() {
    return dis(gen);
}

std::string Client::get_id() const {
    return this->id;
}

std::string Client::get_publickey() const {
    return this->public_key;
}

double Client::get_wallet() const {
    return const_cast<Server *>(server)->get_wallet(id);
}

std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) {
    if (server->get_client(receiver) == nullptr) {
        return false;
    }
    std::string trx = this->id + "-" + receiver + "-" + std::to_string(value);
    return const_cast<Server *>(server)->add_pending_trx(trx, sign(trx));
}

Client::Client(std::string id, const Server &server) : id(id), server(&server), gen(rd()), dis(0, SIZE_MAX) {
    crypto::generate_key(public_key, private_key);
}






