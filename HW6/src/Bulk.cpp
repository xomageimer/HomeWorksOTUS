#include "Bulk.h"

void Bulk::SetBulkModel(std::istream &input) {
    inp = &input;
}

void Bulk::SetController(std::shared_ptr<BulkController> controller) {
    this->data.controller = controller;
}

void Bulk::SubscribeLogger(const std::string & sub_name, std::shared_ptr<ILogger> logger) {
    this->data.controller->manager.subscribe(sub_name, logger);
}

void Bulk::run(int c) {
    data.model.emplace(*inp, data.controller, c);
    this->data.model->ParseStandard(c);
}
