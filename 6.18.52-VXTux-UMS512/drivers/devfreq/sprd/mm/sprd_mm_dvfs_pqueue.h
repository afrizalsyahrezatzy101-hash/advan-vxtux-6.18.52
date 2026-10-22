/* SPDX-License-Identifier: GPL-2.0 */
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
 * The priority queue used by the mm DVFS driver to track the frequency
 * requests of the camera clients.
 *
 * The queue is a singly linked list kept sorted by the requested
 * frequency, the highest frequency first.  The list head lives in the
 * queue structure itself, so the queue does not have to be allocated.
 */

#ifndef __SPRD_MM_DVFS_PQUEUE_H__
#define __SPRD_MM_DVFS_PQUEUE_H__

#include <linux/types.h>

/**
 * struct pqueue_node - one entry of the priority queue
 * @pid: the client that requested the frequency
 * @freq: the requested frequency, in Hz
 * @index: the index of the requested frequency in the IP's table
 * @next: the next node, towards the lower frequencies
 */
struct pqueue_node {
	pid_t pid;
	u64 freq;
	u32 index;
	struct pqueue_node *next;
};

/**
 * struct pqueue - the priority queue
 * @front: the head of the list, the node with the highest frequency
 * @size: the number of nodes in the queue
 */
struct pqueue {
	struct pqueue_node *front;
	unsigned int size;
};

/**
 * struct pqueue_data - the payload of a queue node
 * @pid: the client that requested the frequency
 * @freq: the requested frequency, in Hz
 * @index: the index of the requested frequency in the IP's table
 */
struct pqueue_data {
	pid_t pid;
	u64 freq;
	u32 index;
};

void InitPQueue(struct pqueue *q);
struct pqueue_node *CreateNode(struct pqueue_data *data);
void AddNode(struct pqueue *q, struct pqueue_data *data);
void DelNode(struct pqueue *q);
void Destroy(struct pqueue *q);
u64 GetFrontNodeFreq(struct pqueue *q);
bool IsEmpty(struct pqueue *q);
int MatchPid_RemoveInvalidNode(struct pqueue *q, struct pqueue_data *data);
void ShowQueue(struct pqueue *q);

#endif /* __SPRD_MM_DVFS_PQUEUE_H__ */
