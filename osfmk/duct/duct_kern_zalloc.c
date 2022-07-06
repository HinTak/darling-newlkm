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

#include "duct.h"
#include "duct_pre_xnu.h"
#include "duct_kern_zalloc.h"
#include <linux/version.h>
// #include "duct_machine_routines.h"

// #include <mach/mach_types.h>
// #include <ipc/ipc_init.h>
// #include <kern/locks.h>
// #include <kern/thread.h>
// #include <libkern/version.h>

// setup zinfo_per_task in zone_bootstrap

zone_t duct_zinit (vm_size_t size, vm_size_t max, vm_size_t alloc, const char * name)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0)
        return (zone_t)
        kmem_cache_create (name, size, 0 /* align */, SLAB_HWCACHE_ALIGN /* flags */, NULL /* ctor */);
#else
        return (zone_t)
        kmem_cache_create_usercopy (name, size, 0 /* align */, SLAB_HWCACHE_ALIGN /* flags */, 0, size, NULL /* ctor */);
#endif
}

void duct_zone_change (zone_t zone, unsigned int item, boolean_t value)
{ ;
}

void * duct_zalloc (zone_t zone)
{
        return kmem_cache_alloc ((struct kmem_cache *) zone, GFP_KERNEL);
}

void duct_zfree (zone_t zone, void * elem)
{
        kmem_cache_free ((struct kmem_cache *) zone, elem);
}

boolean_t kdp_is_in_zone(void* addr, const char* zone_name) {
	// this isn't too important; it's only used in debug/development mode inside asserts
	// TODO: how are we actually supposed to check this? as far as i can tell,
	// Linux doesn't have a way to lookup a slab via an address
	// well, since we would only lookup caches that we ourselves allocate, we could keep a list of
	// the ones we have, but that's a lot of unnecessary work for something that is only
	// ever really used in development
	return 1;
};
