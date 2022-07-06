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
#include "duct_vm_kern.h"

#include <vm/vm_map.h>

#include "duct_post_xnu.h"

kern_return_t duct_copyinmap (vm_map_t map, vm_map_offset_t fromaddr, void * todata, vm_size_t length)
{
        kern_return_t    kr = KERN_SUCCESS;

#if defined (__DARLING__)
        if (current_map () == map) {
                if (copyin(fromaddr, todata, length) != 0) {
                        kr = KERN_INVALID_ADDRESS;
                }
        }
        else {
                printk (KERN_NOTICE "copyinmap: not implemented current_map: 0x%p != map: 0x%p", current_map (), map);
                kr = KERN_NOT_SUPPORTED;
        }
#else
        // if (vm_map_pmap(map) == pmap_kernel())
        // {
        //     /* assume a correct copy */
        //     memcpy(todata, CAST_DOWN(void *, fromaddr), length);
        // }
        // else if (current_map() == map)
        // {
        //     if (copyin(fromaddr, todata, length) != 0)
        //         kr = KERN_INVALID_ADDRESS;
        // }
        // else
        // {
        //     vm_map_reference(map);
        //     oldmap = vm_map_switch(map);
        //     if (copyin(fromaddr, todata, length) != 0)
        //         kr = KERN_INVALID_ADDRESS;
        //     vm_map_switch(oldmap);
        //     vm_map_deallocate(map);
        // }
#endif
        return kr;
}


kern_return_t duct_copyoutmap (vm_map_t map, void * fromdata, vm_map_offset_t toaddr, vm_size_t length)
{
#if defined (__DARLING__)
        if (current_map () == map) {
                if (copyout (fromdata, toaddr, length) != 0) {
                        return KERN_INVALID_ADDRESS;
                }
        }
        else {
                printk (KERN_NOTICE "copyout: not implemented current_map: 0x%p != map: 0x%p", current_map (), map);
                return KERN_NOT_SUPPORTED;
        }
#else
        // if (vm_map_pmap(map) == pmap_kernel()) {
        //     /* assume a correct copy */
        //     memcpy(CAST_DOWN(void *, toaddr), fromdata, length);
        //     return KERN_SUCCESS;
        // }
        //
        // if (current_map() != map)
        //     return KERN_NOT_SUPPORTED;
        //
        // if (copyout(fromdata, toaddr, length) != 0)
        //     return KERN_INVALID_ADDRESS;
#endif
        return KERN_SUCCESS;
}

void
kmem_free(
    vm_map_t    map,
    vm_offset_t addr,
    vm_size_t   size)
{
	vfree((void*) addr);
}

kern_return_t kmem_alloc_external(vm_map_t map, vm_offset_t* addrp, vm_size_t size) {
	return kmem_alloc(map, addrp, size, 0);
}

kern_return_t
kmem_alloc(
    vm_map_t    map,
    vm_offset_t *addrp,
    vm_size_t   size,
    vm_tag_t    tag)
{
	return kernel_memory_allocate(map, addrp, size, 0, 0, tag);
}

kern_return_t    kernel_memory_allocate(
                vm_map_t    map,
                vm_offset_t *addrp,
                vm_size_t   size,
                vm_offset_t mask,
                int     flags,
                vm_tag_t        tag)
{
        *addrp = (vm_offset_t) vmalloc(size);
        if (*addrp)
		return KERN_SUCCESS;
	else
		return KERN_NO_SPACE;
}
