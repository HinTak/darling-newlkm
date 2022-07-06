/*
Copyright (c) 2014-2017, Wenqi Chen

Shanghai Mifu Infotech Co., Ltd
B112-113, IF Industrial Park, 508 Chunking Road, Shanghai 201103, China


All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/

#ifndef DUCT_KERN_THREAD_H
#define DUCT_KERN_THREAD_H

#include <mach/mach_types.h>
#include <mach/mach_traps.h>

extern void duct_thread_bootstrap (void);
extern void duct_thread_init (void);

extern kern_return_t duct_thread_create (task_t task, thread_t * new_thread);


// WC - todo: to rename to duct_current_thread
extern thread_t current_thread (void);

extern void duct_thread_deallocate (thread_t thread);
extern void duct_thread_destroy (thread_t thread);

extern kern_return_t thread_set_cthread_self (uint32_t cthread);

#define XNU_USE_MACHTRAP_WRAPPERS_THREAD

extern struct task_struct* thread_get_linux_task(thread_t thread);

#if defined (XNU_USE_MACHTRAP_WRAPPERS_THREAD)
extern kern_return_t xnusys_thread_set_cthread_self (uint32_t cthread);
#else
#define xnusys_thread_set_cthread_self          thread_set_cthread_self
#endif

#endif // DUCT_KERN_THREAD_H
