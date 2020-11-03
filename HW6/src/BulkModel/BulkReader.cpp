#include "BulkReader.h"

BulkReader::BulkReader(std::istream & in, std::shared_ptr<struct IController> contr, int c) : input(in), controller(contr), counter_cmd(c) {

}

void BulkReader::ParseStandard(int n) {
    handler = std::make_shared<StandardHandler>(n);
    handler->SetController(controller);
    handler->Execute(this);
}

void BulkReader::ParseDynamic(int n) {
    handler = std::make_shared<DynamicHandler>(n);
    handler->SetController(controller);
    handler->Execute(this);
}

std::string BulkReader::GetCmd() const {
    std::string tmp;
    std::getline(input, tmp);
    return tmp;
}

bool BulkReader::CheckSymb(char delim) const {
    char c = input.get();
    if (c == delim)
        return true;
    else {
        input.putback(c);
        return false;
    }
}

void StandardHandler::Execute(BulkReader *Application) {
    if (Application->CheckSymb('{')) {
        counter_cmd = 0;
        controller->HandleEvent(job());
        Application->ParseDynamic(1);
    } else if (Application->CheckSymb(EOF)){
        counter_cmd = 0;
        controller->HandleEvent(job());
    } else {
        if (job().type == Event::Type::WRITE){
            controller->HandleEvent(job(), Application->GetCmd());
            Application->ParseStandard(Application->counter_cmd);
        } else {
            auto str = Application->GetCmd();
            controller->HandleEvent(job(), str);     // игнорирование пустых строк
            Application->ParseStandard((!str.empty()) ? counter_cmd - 1 : counter_cmd);
        }
    }
}

Event StandardHandler::job() {
    Event event{};
    if (counter_cmd > 1) {
        event.type = Event::Type::READ;
    } else {
        event.type = Event::Type::WRITE;
    }
    return event;
}

void DynamicHandler::Execute(BulkReader *Application) {
    if (Application->CheckSymb(EOF)) {
        counter_cmd = 0;
        controller->HandleEvent(job());
        return;
    }
    if (Application->CheckSymb('{')) {
        counter_cmd += 1;
    } else if (Application->CheckSymb('}')) {
        counter_cmd -= 1;
    }
    if (job().type == Event::Type::WRITE){
        controller->HandleEvent(job());
        Application->ParseStandard(Application->counter_cmd);
    } else {
        controller->HandleEvent(job(), Application->GetCmd());
        Application->ParseDynamic(counter_cmd);
    }
}

Event DynamicHandler::job() {
    Event event{};
    if (counter_cmd != 0) {
        event.type = Event::Type::READ;
    } else {
        event.type = Event::Type::WRITE;
    }
    return event;
}

void IHandler::SetController(std::shared_ptr<struct IController> cont) {
    controller = cont;
}
