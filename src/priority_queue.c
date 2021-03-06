/*
 * Description : A special type of FIFO (first-in first-out).
 * Usage       : This queue is used if the data has priority
 *               and the we need to get the higher one always.
 * Author      : Baron Leonardo <mohamedayman.fcis@zoho.com>
 * License     : Copyright (C) 2016 RTOS From Scratch
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "priority_queue.h"

PQueue *PQueue_new( size_t queue_length )
{
    PQueue *p_queue = malloc( sizeof(PQueue) +
                              queue_length * sizeof(__PQueue_Node) );

    // default values
    p_queue->length = queue_length;
    p_queue->curr_index = -1;

    return p_queue;
}

uint32_t PQueue_push(PQueue *queue, int8_t priority, void *data)
{
    // TODO: check length first
    if(! queue) return -1;

    queue->queue[++queue->curr_index].priority = priority;
    queue->queue[queue->curr_index].data = data;

    __swim_LastElement( queue );

    // return current length
    return queue->curr_index + 1;
}

bool PQueue_isEmpty(PQueue *p_queue)
{
    if(p_queue->curr_index == -1) return true;
    else return false;
}

void *PQueue_popHead(PQueue *p_queue)
{
    void* head_data = p_queue->queue[0].data;
    p_queue->queue[0] = p_queue->queue[p_queue->curr_index];
    p_queue->curr_index--;
    // sink start from the root node
    __PQueue_Node_sink(p_queue, 0);

    return head_data;
}

size_t PQueue_getLength(PQueue *p_queue)
{
    return p_queue->curr_index;
}

void *PQueue_getHeadData(PQueue *p_queue)
{
    return p_queue->queue[0].data;
}

int8_t PQueue_getHeadPriority(PQueue *p_queue)
{
    if( PQueue_isEmpty(p_queue) )
        return -1;
    else
        return p_queue->queue[0].priority;
}

bool PQueue_remove(PQueue *p_queue, void* data)
{
    __PQueue_Node* queue = p_queue->queue;

    int16_t indexToBeRemove = __PQueue_find(p_queue, data);

    // move last node into the node needed to be removed
    __PQueue_Node_move(&queue[p_queue->curr_index], &queue[indexToBeRemove]);
    p_queue->curr_index--;

    __PQueue_Node_sink(p_queue, indexToBeRemove);

    return true;
}

void PQueue_clean(PQueue *p_queue)
{
    p_queue->curr_index = -1;
    free(p_queue);
}
