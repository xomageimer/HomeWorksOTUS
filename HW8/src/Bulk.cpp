#include "Bulk.h"

#include <sstream>

void Bulk::SetBulkModel(std::istream &input) {
    inp = &input;
}

void Bulk::SetController(std::shared_ptr<BulkController> controller) {
    this->data.controller = controller;
}

void Bulk::SubscribeLogger(const std::string & sub_name, std::shared_ptr<ILogger> logger) {
    this->data.controller->manager.subscribe(sub_name, logger);
}

bool Bulk::run() {
    return data.model->Process();
}

void Bulk::run(int c) {
    data.model.emplace(*inp, data.controller, c);
    this->data.model->ParseStandard(c);
    while (data.model->Process()) {}
}

void Bulk::build(char * p) {
    int c;
    try {
        c = std::stoi(p);
    } catch (const std::exception & excp){
        restart:
        std::cerr << "Wrong input!\nPlease try again!" << std::endl;
        try {
            std::cin >> p;
            c = std::stoi(p);
        }catch (...) {
            goto restart;
        }
    }
    data.model.emplace(*inp, data.controller, c);
    this->data.model->ParseStandard(c);
}

void Bulk::GetStr(const char * str, size_t s) {
    dynamic_cast<std::stringstream *>(inp)->write(str, s);
}

void Bulk::SetDefaultBulkMode() {
    inp = new std::stringstream ();
}

