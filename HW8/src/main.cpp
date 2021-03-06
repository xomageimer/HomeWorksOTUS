#include "Bulk.h"


int main( [[maybe_unused]] int argc, char* argv[]){
    Bulk blk;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(std::cin);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(std::cout));
    blk.SubscribeLogger("File_Outputer", std::make_shared<FileLogger>(std::filesystem::current_path(), 2));

    blk.build(argv[1]);
    while (blk.run())
        continue;
}