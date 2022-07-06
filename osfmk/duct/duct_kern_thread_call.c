/*
 * Darling Mach Linux Kernel Module
 * Copyright (C) 2015-2017 Lubos Dolezel
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <duct/duct.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <duct/duct_pre_xnu.h>
#include <duct/duct_kern_kalloc.h>

#include <mach/mach_types.h>
#include <mach/thread_act.h>

#include <kern/kern_types.h>
#include <kern/zalloc.h>
#include <kern/sched_prim.h>
#include <kern/clock.h>
#include <kern/task.h>
#include <kern/thread.h>
#include <kern/waitq.h>

#include <vm/vm_pageout.h>

#include <kern/thread_call.h>
#include <kern/call_entry.h>
#include <kern/timer_call.h>

#include <libkern/OSAtomic.h>
#include <darling/debug_print.h>
#include <darling/task_registry.h>

static struct workqueue_struct* _thread_call_wq = NULL;

static void
thread_call_worker(struct work_struct* work);

static void
thread_call_wq_init(struct work_struct* work);
static void
thread_call_wq_exit(struct work_struct* work);

extern kern_return_t
duct_thread_create(task_t task, thread_t* new_thread);
extern void
duct_thread_destroy(thread_t thread);

extern task_t kernel_task;
static thread_t my_thread;

//#define thread_call_debug_msg(...) printk(KERN_ERR __VA_ARGS__)
#define thread_call_debug_msg(...)

/*
 *  thread_call_initialize:
 *
 *  Initialize this module, called
 *  early during system initialization.
 */
void
thread_call_initialize(void)
{
	struct work_struct ws;

	assert(_thread_call_wq == NULL);
	_thread_call_wq = create_singlethread_workqueue("mach_thread_call");

	INIT_WORK(&ws, thread_call_wq_init);
	queue_work(_thread_call_wq, &ws);
	flush_work(&ws);
}

void
thread_call_deinitialize(void)
{
	struct work_struct ws;

	assert(_thread_call_wq != NULL);

	INIT_WORK(&ws, thread_call_wq_exit);
	queue_work(_thread_call_wq, &ws);
	flush_work(&ws);

	destroy_workqueue(_thread_call_wq);
	_thread_call_wq = NULL;
}

static void
thread_call_wq_init(struct work_struct* work)
{
	// Setup an XNU thread, so that IPC works from this thread
	// Note: It seems that Linux recreates the kthread backing the workqueue
	// sometimes, so we register ourselves lazily now.
	duct_thread_create(kernel_task, &my_thread);
	thread_deallocate(my_thread);
	
	// darling_thread_register(thread);
}

static void
thread_call_wq_exit(struct work_struct* work)
{
	// thread_t thread = darling_thread_get_current();
	//if (thread != NULL)
	//{
		duct_thread_destroy(my_thread);
		// darling_thread_deregister(thread);
	// }
}

void
thread_call_setup(
    thread_call_t           call,
    thread_call_func_t      func,
    thread_call_param_t     param0)
{
	thread_call_debug_msg("thread_call_setup(call=%p)\n", call);
	call_entry_setup((call_entry_t) call, func, param0);
	INIT_DELAYED_WORK(&call->tc_work, thread_call_worker);
	call->free = FALSE;
}

thread_call_t
thread_call_allocate(
        thread_call_func_t      func,
        thread_call_param_t     param0)
{
	thread_call_t call = kalloc(sizeof(struct thread_call));
	thread_call_setup(call, func, param0);
	return call;
}

// <copied from="xnu://6153.61.1/osfmk/kern/thread_call.c" modified="true">

thread_call_t
thread_call_allocate_with_options(
	thread_call_func_t              func,
	thread_call_param_t             param0,
	thread_call_priority_t          pri,
	thread_call_options_t           options)
{
	thread_call_t call = thread_call_allocate(func, param0);

	switch (pri) {
	case THREAD_CALL_PRIORITY_HIGH:
		call->tc_index = THREAD_CALL_INDEX_HIGH;
		break;
	case THREAD_CALL_PRIORITY_KERNEL:
		call->tc_index = THREAD_CALL_INDEX_KERNEL;
		break;
	case THREAD_CALL_PRIORITY_USER:
		call->tc_index = THREAD_CALL_INDEX_USER;
		break;
	case THREAD_CALL_PRIORITY_LOW:
		call->tc_index = THREAD_CALL_INDEX_LOW;
		break;
	case THREAD_CALL_PRIORITY_KERNEL_HIGH:
		call->tc_index = THREAD_CALL_INDEX_KERNEL_HIGH;
		break;
	default:
		panic("Invalid thread call pri value: %d", pri);
		break;
	}

	if (options & THREAD_CALL_OPTIONS_ONCE) {
		call->tc_flags |= THREAD_CALL_ONCE;
	}
	if (options & THREAD_CALL_OPTIONS_SIGNAL) {
		call->tc_flags |= THREAD_CALL_SIGNAL | THREAD_CALL_ONCE;
	}

	return call;
}

// </copied>

boolean_t
thread_call_free(
        thread_call_t       call)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
	if (atomic_long_read(&call->tc_work.work.data) & (WORK_STRUCT_PENDING | WORK_STRUCT_INACTIVE))
#else
	if (atomic_long_read(&call->tc_work.work.data) & (WORK_STRUCT_PENDING | WORK_STRUCT_DELAYED))
#endif
		return FALSE;

	kfree(call, sizeof(struct thread_call));
	return TRUE;
}

/*
 *  thread_call_cancel:
 *
 *  Dequeue a callout entry.
 *
 *  Returns TRUE if the call was
 *  on a queue.
 */
boolean_t
thread_call_cancel(
        thread_call_t       call)
{
	assert(_thread_call_wq != NULL);
	if (_thread_call_wq != NULL)
	{
		thread_call_debug_msg("thread_call_cancel(%p)\n", call);
		return cancel_delayed_work(&call->tc_work);
	}
	else
	{
		return FALSE;
	}
}

boolean_t
thread_call_cancel_wait(thread_call_t call) {
	assert(_thread_call_wq != NULL);

	if (_thread_call_wq != NULL)
	{
		thread_call_debug_msg("thread_call_cancel_wait(%p)\n", call);
		return cancel_delayed_work_sync(&call->tc_work);
	}
	else
	{
		return FALSE;
	}
};

boolean_t
thread_call_enter_delayed_with_leeway(
        thread_call_t       call,
        thread_call_param_t param1,
        uint64_t            deadline,
        uint64_t leeway,
        uint32_t flags)
{
    // We don't do leeways
    ((call_entry_t)call)->param1 = param1;
		if ((flags & THREAD_CALL_CONTINUOUS) != 0) {
			call->tc_flags |= THREAD_CALL_CONTINUOUS;
		} else {
			call->tc_flags &= ~THREAD_CALL_CONTINUOUS;
		}
    return thread_call_enter_delayed(call, deadline);
}

static uint64_t deadline_to_delay(uint64_t deadline, bool continuous) {
	int64_t delay = deadline - (continuous ? mach_continuous_time() : mach_absolute_time());
	if (delay < 0)
		delay = 0;
	return (uint64_t)delay;
};

/*
 *  thread_call_enter_delayed:
 *
 *  Enqueue a callout entry to occur
 *  at the stated time.
 *
 *  Returns TRUE if the call was
 *  already on a queue.
 */
boolean_t
thread_call_enter_delayed(
        thread_call_t       call,
        uint64_t            deadline)
{
	uint64_t delay;

	thread_call_debug_msg("thread_call_enter_delayed(call=%p, deadline=%lld)\n", call, deadline);
	assert(_thread_call_wq != NULL);

	if (_thread_call_wq == NULL)
	{
		return FALSE;
	}

	delay = deadline_to_delay(deadline, (call->tc_flags & THREAD_CALL_CONTINUOUS) != 0);

	thread_call_debug_msg("... delayed by %llu ns\n", delay);

	if (queue_delayed_work(_thread_call_wq, &call->tc_work, nsecs_to_jiffies(delay)) == 0)
	{
		if ((call->tc_flags & THREAD_CALL_ONCE) != 0) {
			// tell the worker to reschedule itself when it's done
			call->tc_flags |= THREAD_CALL_RESCHEDULE;
			call->tc_deadline = deadline;
		} else {
			// Re-schedule
			thread_call_debug_msg("... mod timer expiry\n");
			// Important details: if thread_call_enter_delayed() returns TRUE, then no new
			// callout was scheduled. So we may not re-queue the work, we may only
			// change the delay.
			// Whether the delay took effect or it was too late, is caller's problem.
			call->tc_work.timer.expires = jiffies + nsecs_to_jiffies(delay);
		}
		return TRUE;
	}
	return FALSE;
}

boolean_t
thread_call_enter1_delayed(
        thread_call_t           call,
        thread_call_param_t     param1,
        uint64_t            deadline)
{
    ((call_entry_t)call)->param1 = param1;
	return thread_call_enter_delayed(call, deadline);
}

boolean_t
thread_call_enter(
        thread_call_t       call)
{
	return thread_call_enter_delayed(call, 0);
}

boolean_t
thread_call_enter1(
        thread_call_t           call,
        thread_call_param_t     param1)
{
	((call_entry_t)call)->param1 = param1;
	return thread_call_enter(call);
}

static void
thread_call_worker(struct work_struct* work)
{
	thread_call_t call;

	call = container_of(to_delayed_work(work), struct thread_call, tc_work);
	thread_call_debug_msg("thread_call_worker() invoked on call %p\n", call);
	
	darling_thread_register(my_thread);
	call->tc_call.func(call->tc_call.param0, call->tc_call.param1);
	darling_thread_deregister(my_thread);

	if (call->free)
		kfree(call, sizeof(struct thread_call));
	else if ((call->tc_flags & THREAD_CALL_RESCHEDULE) != 0) {
		thread_call_debug_msg("... asked to reschedule\n");
		call->tc_flags &= ~THREAD_CALL_RESCHEDULE;
		queue_delayed_work(_thread_call_wq, &call->tc_work, deadline_to_delay(call->tc_deadline, (call->tc_flags & THREAD_CALL_CONTINUOUS) != 0));
	}
}

