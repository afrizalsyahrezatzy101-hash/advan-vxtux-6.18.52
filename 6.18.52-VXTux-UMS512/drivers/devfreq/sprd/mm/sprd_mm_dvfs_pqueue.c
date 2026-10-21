// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * The priority queue of the mm DVFS driver.
 *
 * A singly linked list sorted by the requested frequency, the highest
 * frequency first.  AddNode() keeps the list sorted by walking it until
 * the first node whose frequency is lower than the one being inserted;
 * the caller is expected to hold whatever lock protects the queue.
 *
 * The node payload is 24 bytes (pid, freq, index, next), matching the
 * kmem_cache_alloc_trace() size of the stock implementation.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>

#include "sprd_mm_dvfs_pqueue.h"

/**
 * InitPQueue - reset a queue to the empty state
 * @q: the queue
 */
void InitPQueue(struct pqueue *q)
{
	if (q)
		q->front = NULL;
}
EXPORT_SYMBOL_GPL(InitPQueue);

/**
 * CreateNode - allocate a queue node
 * @data: the payload to copy into the new node
 *
 * Returns the new node, or NULL if the allocation failed.  The node is
 * not linked into any queue yet.
 */
struct pqueue_node *CreateNode(struct pqueue_data *data)
{
	struct pqueue_node *node;

	node = kmalloc(sizeof(*node), GFP_KERNEL);
	if (!node)
		return NULL;

	node->pid = data->pid;
	node->freq = data->freq;
	node->index = data->index;
	node->next = NULL;

	return node;
}
EXPORT_SYMBOL_GPL(CreateNode);

/**
 * AddNode - insert a node into the queue, keeping it frequency-sorted
 * @q: the queue
 * @data: the payload of the node to insert
 *
 * The highest frequency stays at the front, so the front node is the one
 * that drives the IP.
 */
void AddNode(struct pqueue *q, struct pqueue_data *data)
{
	struct pqueue_node *node, **link;

	if (!q || !data)
		return;

	node = CreateNode(data);
	if (!node)
		return;

	/* link always points at the pointer that references the next node */
	link = &q->front;
	while (*link && data->freq > (*link)->freq)
		link = &(*link)->next;

	node->next = *link;
	*link = node;
}
EXPORT_SYMBOL_GPL(AddNode);

/**
 * DelNode - drop the node at the front of the queue
 * @q: the queue
 */
void DelNode(struct pqueue *q)
{
	struct pqueue_node *node;

	if (!q || !q->front)
		return;

	node = q->front;
	q->front = node->next;
	kfree(node);
}
EXPORT_SYMBOL_GPL(DelNode);

/**
 * Destroy - free every node of the queue
 * @q: the queue
 */
void Destroy(struct pqueue *q)
{
	struct pqueue_node *node, *next;

	if (!q)
		return;

	node = q->front;
	while (node) {
		next = node->next;
		kfree(node);
		node = next;
	}
}
EXPORT_SYMBOL_GPL(Destroy);

/**
 * GetFrontNodeFreq - the frequency of the node at the front
 * @q: the queue
 *
 * Returns 0 if the queue is empty.
 */
u64 GetFrontNodeFreq(struct pqueue *q)
{
	if (!q || !q->front) {
		pr_err("dvfs power off\n");
		return 0;
	}

	return q->front->freq;
}
EXPORT_SYMBOL_GPL(GetFrontNodeFreq);

/**
 * IsEmpty - test whether the queue holds any node
 * @q: the queue
 */
bool IsEmpty(struct pqueue *q)
{
	return !q || !q->front;
}
EXPORT_SYMBOL_GPL(IsEmpty);

/**
 * MatchPid_RemoveInvalidNode - add or drop a client in the queue
 * @q: the queue
 * @data: the payload identifying the client
 *
 * When data->index is zero the client is dropping its request and any
 * node with a matching pid is unlinked.  Otherwise an existing node with
 * the same pid is replaced and, if there was none, the new node is
 * inserted in frequency order.
 *
 * Returns 0.
 */
int MatchPid_RemoveInvalidNode(struct pqueue *q, struct pqueue_data *data)
{
	struct pqueue_node *node, **link, *new;

	if (!q || !data)
		return 0;

	/* find the node of this pid, remembering where it is linked from */
	link = &q->front;
	node = q->front;
	while (node && node->pid != data->pid) {
		link = &node->next;
		node = node->next;
	}

	if (node) {
		/* unlink the stale node */
		*link = node->next;
		kfree(node);
		/* the list changed under us, restart the ordered insert */
		link = &q->front;
	}

	if (data->index == 0)
		return 0;

	new = CreateNode(data);
	if (!new)
		return 0;

	while (*link && data->freq > (*link)->freq)
		link = &(*link)->next;

	new->next = *link;
	*link = new;

	return 0;
}
EXPORT_SYMBOL_GPL(MatchPid_RemoveInvalidNode);

/**
 * ShowQueue - dump the queue contents to the kernel log
 * @q: the queue
 */
void ShowQueue(struct pqueue *q)
{
	struct pqueue_node *node;

	if (!q)
		return;

	node = q->front;
	while (node) {
		pr_info("pid:%d,freq:%llu,work_index=%u\n",
			(int)node->pid, node->freq, node->index);
		node = node->next;
	}
}
EXPORT_SYMBOL_GPL(ShowQueue);