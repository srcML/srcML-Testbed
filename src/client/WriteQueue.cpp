// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file WriteQueue.cpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <WriteQueue.hpp>
#include <srcml_write.hpp>

WriteQueue::WriteQueue(TraceLog& log, const srcml_output_dest& destination, bool ordered)
       : log(log), destination(destination), ordered(ordered), maxposition(0), q(
            [](std::shared_ptr<ParseRequest> r1, std::shared_ptr<ParseRequest> r2) {
                return r1->position > r2->position;
            }) {

    write_thread = std::thread(&WriteQueue::process, this);
}

/* writes out the current srcml */
void WriteQueue::schedule(std::shared_ptr<ParseRequest> pvalue) {

    // push the value on the priority queue
    {
        std::lock_guard<std::mutex> lock(qmutex);

        // record max position for eos()
        if (pvalue->position > maxposition)
            maxposition = pvalue->position;

        // put this request into the queue
        q.push(pvalue);
    }

    // let the write processing know there is something
    cv.notify_one();
}

void WriteQueue::stop() {

    {
        std::unique_lock<std::mutex> lock(qmutex);

        completed = true;
    }

    cv.notify_one();

    write_thread.join();
}

void WriteQueue::process() {

    int position = 0;
    while (1) {

        // get a parse request to handle
        std::shared_ptr<ParseRequest> pvalue(0);
        {
            std::unique_lock<std::mutex> lock(qmutex);

            while (q.empty() || (ordered && (q.top()->position != position + 1))) {
                if (q.empty() && completed)
                    return;
                cv.wait(lock);
            }

            pvalue = q.top();
            q.pop();
        }
        ++position;

        // record real units written
        if (pvalue->status == SRCML_STATUS_OK)
            ++total;

        // finally write it out
        srcml_write_request(pvalue, log, destination);
    }
}
