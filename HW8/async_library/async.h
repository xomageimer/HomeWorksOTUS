#pragma once

#include <cstddef>

#include <thread>
#include <queue>
#include <map>
#include <future>
#include <string>

#include "../src/Bulk.h"

namespace async {
    using request_queue = std::queue<std::pair<const char *, size_t>>;
    using handle_t = void *;

    struct EventHandler{
    public:
        static EventHandler & Instance() {
            static EventHandler eh;
            return eh;
        }

        handle_t make_connect(size_t s);
        void make_disconnect(handle_t h);
        void make_receive(handle_t h, const char * msg, std::size_t msg_size);

    private:

        void worker();

        EventHandler();
        ~EventHandler();

        std::mutex m;
        std::thread handle_thread;

        std::map<Bulk *, request_queue> handlers;
        bool quit = false;
    };

    handle_t connect(std::size_t bulk);
    void receive(handle_t handle, const char *data, std::size_t size);
    void disconnect(handle_t handle);

}
