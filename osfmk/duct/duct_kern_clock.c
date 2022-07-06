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
#include "duct_kern_clock.h"

#include <mach/mach_time.h>
#include <kern/clock.h>
#include <kern/waitq.h>

#include "duct_post_xnu.h"

#include <linux/delay.h>
#include <darling/debug_print.h>

/* WC - todo wrapper */
void duct_clock_init (void)
{
        clock_oldinit ();
}


// kern/clock.c
kern_return_t duct_mach_timebase_info_trap (struct mach_timebase_info_trap_args * args)
{
        mach_timebase_info_data_t       info;

        // arm/rtclock.c
        info.numer      = 1;
        info.denom      = 1;

        // WC - XNU didn't check return value
        copyout ((void *) &info, args->info, sizeof (mach_timebase_info_data_t));

        return KERN_SUCCESS;
}

void clock_absolutetime_interval_to_deadline (uint64_t abstime, uint64_t * result)
{
        *result     = mach_absolute_time () + abstime;
}

void clock_get_uptime (uint64_t * result)
{
        *result     = mach_absolute_time ();
}

void clock_get_calendar_microtime (clock_sec_t *secs, clock_usec_t *microsecs)
{
	// passing in `microsecs` is fine; `clock_usec_t` and `clock_nsec_t` are the same type
	clock_get_calendar_nanotime(secs, microsecs);
	*microsecs /= NSEC_PER_USEC;
}

#if defined (XNU_USE_MACHTRAP_WRAPPERS_TIMEKEEPING)
kern_return_t xnusys_mach_timebase_info_trap (struct mach_timebase_info_trap_args * args)
{
        debug_msg( "- args->info: 0x%llx\n", args->info);

        // debug_msg( "xnusys_mach_timebase_info_trap: is invalid\n");
        kern_return_t   retval  = duct_mach_timebase_info_trap (args);
        // debug_msg( "- retval: %d", retval);
        return retval;
}

kern_return_t xnusys_mach_wait_until_trap (struct mach_wait_until_trap_args * args)
{
        debug_msg( "- args->deadline: 0x%x\n", (unsigned int) args->deadline);

        debug_msg( "xnusys_mach_wait_until_trap: not implemented\n");
        // kern_return_t   retval  = mach_mach_wait_until_trap (args);
        // debug_msg( "- retval: %d", retval);
        // return retval;
        return 0;
}
#endif

void delay(int usec)
{
    udelay(usec);
}

uint64_t ml_get_timebase(void)
{
    return get_cycles();
}

// yes, i know it's weird that this is here and `mach_absolute_time` is in `duct_machine_rtclock.c`, but that's how XNU has it
uint64_t mach_continuous_time(void) {
	return ktime_to_ns(ktime_get_boottime());
};

// <copied from="xnu://6153.61.1/osfmk/kern/clock.c">

void
clock_continuoustime_interval_to_deadline(
	uint64_t                        conttime,
	uint64_t                        *result)
{
	*result = mach_continuous_time() + conttime;
}

void
clock_interval_to_deadline(
	uint32_t                        interval,
	uint32_t                        scale_factor,
	uint64_t                        *result)
{
	uint64_t        abstime;

	clock_interval_to_absolutetime_interval(interval, scale_factor, &abstime);

	*result = mach_absolute_time() + abstime;
}

// </copied>

void clock_get_calendar_nanotime(clock_sec_t* secs, clock_nsec_t* nanosecs) {
	uint64_t ns_since_epoch = ktime_get_real_ns();
	*secs = ns_since_epoch / NSEC_PER_SEC;
	*nanosecs = ns_since_epoch - ((*secs) * NSEC_PER_SEC);
};
