// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ParseQueue.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef PARSE_QUEUE_HPP
#define PARSE_QUEUE_HPP

#include <ParseRequest.hpp>
#include <WriteQueue.hpp>
#include <ctpl_stl.h>
#include <srcml_consume.hpp>
#include <srcml_utilities.hpp>
#include <atomic>

class ParseQueue {
public:

    ParseQueue(int max_threads, WriteQueue* write_queue)
        : pool(max_threads), wqueue(write_queue) {}

    inline void schedule(ParseRequest&& value) {

        value.position = ++counter;

        // error passthrough to output for proper output in trace
        if (value.status) {
            value.unit = 0;
            wqueue->schedule(std::move(value));
            return;
        }

        pool.push(srcml_consume, std::move(value), wqueue);
    }

    inline void wait() {

        pool.stop(true);
    }

private:
    ctpl::thread_pool pool;
    WriteQueue* wqueue = nullptr;
    std::atomic<int> counter = 0;
};

#endif
