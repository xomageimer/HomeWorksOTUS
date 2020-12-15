#include "Controller.h"

#include <utility>

void IController::HandleEvent([[maybe_unused]]const Event & event, std::string str) {
    command = std::move(str);
}

void BulkController::HandleEvent(const Event & e, std::string str) {
    command = std::move(str);
    switch (e.type){
        case Event::Type ::WRITE:
            ActionWrite();
            break;
        case Event::Type ::READ:
            ActionRead();
            break;
    }
}

void BulkController::ActionWrite() {
    manager.notify(command);
    manager.drop();
}

void BulkController::ActionRead() {
    manager.notify(command);
}
