 * Copyright (c) 2000-2016 Apple Inc. All rights reserved.
 * 
 * 
 * 
 * 
#include <machine/spl.h>
#include <kern/kalloc.h>	/* kalloc()/kfree() */
#include <kern/clock.h>		/* delay_for_interval() */
#include <libkern/OSAtomic.h>	/* OSAddAtomic() */
#include <vm/vm_protos.h>	/* vnode_pager_vrele() */
int	vttoif_tab[9] = {
extern void 		memory_object_mark_io_tracking(
    void * array,
    size_t nmembers,
    size_t member_size,
    int (*)(const void *, const void *));
extern kern_return_t adjust_vm_object_cache(vm_size_t oval, vm_size_t nval);
__private_extern__ kern_return_t reset_vmobjectcache(unsigned int val1,
			unsigned int val2);
static void vnode_dropiocount (vnode_t);
		struct vnode_attr *vap, uint32_t flags, int fmode, uint32_t *statusp, vfs_context_t ctx);
errno_t rmdir_remove_orphaned_appleDouble(vnode_t, vfs_context_t, int *); 
TAILQ_HEAD(freelst, vnode) vnode_free_list;	/* vnode free list */
TAILQ_HEAD(deadlst, vnode) vnode_dead_list;	/* vnode dead list */
TAILQ_HEAD(ragelst, vnode) vnode_rage_list;	/* vnode rapid age list */
int	rage_limit = 0;
int	ragevnodes = 0;			   
#define RAGE_LIMIT_MIN	100
#define RAGE_TIME_LIMIT	5
struct mntlist mountlist;			/* mounted filesystem list */
#define VLISTCHECK(fun, vp, list)	\
		panic("%s: %s vnode not on %slist", (fun), (list), (list));
#define VLISTNONE(vp)	\
	do {	\
		(vp)->v_freelist.tqe_next = (struct vnode *)0;	\
		(vp)->v_freelist.tqe_prev = (struct vnode **)0xdeadb;	\
#define VONLIST(vp)	\
#define VREMFREE(fun, vp)	\
	do {	\
		VLISTCHECK((fun), (vp), "free");	\
		TAILQ_REMOVE(&vnode_free_list, (vp), v_freelist);	\
		VLISTNONE((vp));	\
		freevnodes--;	\
#define VREMDEAD(fun, vp)	\
	do {	\
		VLISTCHECK((fun), (vp), "dead");	\
		TAILQ_REMOVE(&vnode_dead_list, (vp), v_freelist);	\
		VLISTNONE((vp));	\
		vp->v_listflag &= ~VLIST_DEAD;	\
		deadvnodes--;	\
#define VREMASYNC_WORK(fun, vp)	\
	do {	\
		VLISTCHECK((fun), (vp), "async_work");	\
		TAILQ_REMOVE(&vnode_async_work_list, (vp), v_freelist);	\
		VLISTNONE((vp));	\
		vp->v_listflag &= ~VLIST_ASYNC_WORK;	\
		async_work_vnodes--;	\
#define VREMRAGE(fun, vp)	\
	do {	\
	        if ( !(vp->v_listflag & VLIST_RAGE))			\
		        panic("VREMRAGE: vp not on rage list");		\
		VLISTCHECK((fun), (vp), "rage");			\
		TAILQ_REMOVE(&vnode_rage_list, (vp), v_freelist);	\
		VLISTNONE((vp));		\
		vp->v_listflag &= ~VLIST_RAGE;	\
		ragevnodes--;			\

/*
 * vnodetarget hasn't been used in a long time, but
 * it was exported for some reason... I'm leaving in
 * place for now...  it should be deprecated out of the
 * exports and removed eventually.
 */
u_int32_t vnodetarget;		/* target for vnreclaim() */
#define VNODE_FREE_TARGET	20	/* Default value for vnodetarget */

/*
 * We need quite a few vnodes on the free list to sustain the
 * rapid stat() the compilation process does, and still benefit from the name
 * cache. Having too few vnodes on the free list causes serious disk
 * thrashing as we cycle through them.
 */
#define VNODE_FREE_MIN		CONFIG_VNODE_FREE_MIN	/* freelist should have at least this many */


	thread_t	thread = THREAD_NULL;
	if (!vnodetarget)
		vnodetarget = VNODE_FREE_TARGET;

	if (rage_limit < RAGE_LIMIT_MIN)
	        rage_limit = RAGE_LIMIT_MIN;
	
	/*
	 * Scale the vm_object_cache to accomodate the vnodes 
	 * we want to cache
	 */
	(void) adjust_vm_object_cache(0, desiredvnodes - VNODE_FREE_MIN);
/* Reset the VM Object Cache with the values passed in */
__private_extern__ kern_return_t
reset_vmobjectcache(unsigned int val1, unsigned int val2)
{
	vm_size_t oval = val1 - VNODE_FREE_MIN;
	vm_size_t nval;
	
	if (val1 == val2) {
		return KERN_SUCCESS;
	}

	if(val2 < VNODE_FREE_MIN)
		nval = 0;
	else
		nval = val2 - VNODE_FREE_MIN;

	return(adjust_vm_object_cache(oval, nval));
}


vnode_waitforwrites(vnode_t vp, int output_target, int slpflag, int slptimeout, const char *msg) {
        int error = 0;
	        slpflag |= PDROP;

	        vnode_lock_spin(vp);
		        if (output_target)
			        vp->v_flag |= VTHROTTLED;
			else
			        vp->v_flag |= VBWAIT;
			ts.tv_sec = (slptimeout/100);
			ts.tv_nsec = (slptimeout % 1000)  * 10 * NSEC_PER_USEC * 1000 ;
vnode_startwrite(vnode_t vp) {

        OSAddAtomic(1, &vp->v_numoutput);
	        OSAddAtomic(-1, &vp->v_numoutput);
		if (vp->v_numoutput < 0)
		
		if (need_wakeup)
        struct cl_writebehind *wbp;
	 * state can change right after that. If their 
	 */ 
        if (vp->v_dirtyblkhd.lh_first)
	        return (1);
	
	if (!UBCINFOEXISTS(vp))
	        return (0);
	if (wbp && (wbp->cl_number || wbp->cl_scmap))
	        return (1);
	return (0);
	 * state can change right after that. If their 
	 */ 
        if (vp->v_cleanblkhd.lh_first)
	        return (1);
	return (0);
	while (mp->mnt_lflag & MNT_LITER) {
		mp->mnt_lflag |= MNT_LITERWAIT;
		msleep((caddr_t)mp, &mp->mnt_mlock, PVFS, "vnode_iterate_setup", NULL);	
	}


		if (vp->v_type == VDIR)
		if (vp == skipvp)
		if ((flags & SKIPSYSTEM) && ((vp->v_flag & VSYSTEM) || (vp->v_flag & VNOFLUSH)))
		if ((flags & SKIPSWAP) && (vp->v_flag & VSWAP))
		if ((flags & WRITECLOSE) && (vp->v_writecount == 0 || vp->v_type != VREG)) 
			return 1;

			 /* Busy if iocount is > 0 for more than 3 seconds */
			if (vp->v_iocount > 0)
				return 1;
	
	return 0;
/* 
		return (0);
	} 
	if (mp->mnt_newvnodes.tqh_first != NULL)
	return (1);
int 
		
		if (mvp)
		else
		
		if(mp->mnt_vnodelist.tqh_first) 
		else
	return(moved);
	if (mp->mnt_lflag & MNT_LITERWAIT) {
		mp->mnt_lflag &= ~MNT_LITERWAIT;
		wakeup(mp);
	}
static void vnode_iterate_panic_hook(panic_hook_t *hook_)
	extern int kdb_log(const char *fmt, ...);
	
		kdb_log("mp = %p, phys = %p, prev (%p: %p-%p)\n", 
				hook->mp, phys, range.type, range.phys_start,
				range.phys_start + range.len);
		kdb_log("mp = %p, phys = %p, prev (!)\n", hook->mp, phys);
		kdb_log("vp = %p, phys = %p, prev (%p: %p-%p)\n", 
				hook->vp, phys, range.type, range.phys_start,
				range.phys_start + range.len);
		kdb_log("vp = %p, phys = %p, prev (!)\n", hook->vp, phys);
	panic_dump_mem((void *)(((vm_offset_t)hook->mp -4096) & ~4095), 12288);
	      void *arg)
	/* it is returns 0 then there is nothing to do */
	if (retval == 0)  {
		return(ret);
		if ( vget_internal(vp, vid, (flags | VNODE_NODEAD| VNODE_WITHID | VNODE_NOSUSPEND))) {
			continue;	
		        /*
		        if (vnode_reload(vp)) {
			        /* vnode will be recycled on the refcount drop */
			        vnode_put(vp);
			    	continue;
		  case VNODE_RETURNED:
		  case VNODE_RETURNED_DONE:
			  vnode_put(vp);
			  if (retval == VNODE_RETURNED_DONE) {
			  }
			  break;
		  case VNODE_CLAIMED_DONE:
				mount_lock(mp);
				ret = 0;
				goto out;
		  case VNODE_CLAIMED:
		  default:
				break;
	return (ret);
        if ( !locked)
	        mount_lock_spin(mp);
	
        if ( !locked)
	        mount_unlock(mp);
        if ( !locked)
	        mount_lock_spin(mp);
	
	if (mp->mnt_count == 0 && (mp->mnt_lflag & MNT_LDRAIN))
	        wakeup(&mp->mnt_lflag);
        if ( !locked)
	        mount_unlock(mp);
	if (!locked)
	if (!locked)
	return(retval);
	if (!locked)
	if (mp->mnt_iterref < 0)
	else
		retval = 0;	
	if (!locked)
	return(retval);
	while (mp->mnt_iterref)
	if (mp->mnt_iterref == -1)
int 
	if (mp->mnt_lflag & MNT_LDRAIN)
	while (mp->mnt_count)
	if (mp->mnt_vnodelist.tqh_first != NULL)
		 panic("mount_refdrain: dangling vnode"); 
	return(0);
void 
mount_set_noreaddirext(mount_t mp) {
	mount_lock (mp);
	mount_unlock (mp);

	if (mp->mnt_lflag & MNT_LDEAD)
		return (ENOENT);
		        mount_unlock(mp);
			return (ENOENT);
		return (ENOENT);
	/* 
	return (0);
vfs_rootmountfailed(mount_t mp) {

	mount_t	mp;
	return (mp);
        struct vfstable *vfsp;
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next)
	        if (!strncmp(vfsp->vfc_name, fstypename,
			     sizeof(vfsp->vfc_name)))
		        break;
        if (vfsp == NULL)
	        return (ENODEV);
	if (*mpp)
	        return (0);
	return (ENOMEM);
	struct vfs_attr	vfsattr;
	int	error;
	vnode_t	bdevvp_rootvp;
		return (error);
		return (error);
	 * 4951998 - code we call in vfc_mountroot may replace rootvp 
			&& !ISSET(vfsp->vfc_vfsflags, VFC_VFSCANMOUNTROOT)) {
		if (vfsp->vfc_mountroot)
		else
			if ( bdevvp_rootvp != rootvp ) {
			        vnode_rele(bdevvp_rootvp);
			        vnode_put(bdevvp_rootvp);
			if (MNTK_VIRTUALDEV & mp->mnt_kern_flag) speed = 128;
			else if (MNTK_SSD & mp->mnt_kern_flag)   speed = 7*256;
			else                                     speed = 256;
			if (vfs_getattr(mp, &vfsattr, ctx) == 0 && 
					(vfsattr.f_capabilities.valid[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_DIR_HARDLINKS)) {
			if ((vfs_flags(mp) & MNT_MULTILABEL) == 0)
				return (0);
			return (0);
		
		if (error != EINVAL)
	return (ENODEV);
	return (mount_list_lookupby_fsid(fsid, 0, 0));
	return(mount_list_lookupby_fsid(fsid, 1, 0));
					sizeof(mp->mnt_vfsstat.f_mntonname))) {
			if (mount_iterref(retmp, 1))
	return (retmp);

	if (++mntid_gen == 0)
		if (++mntid_gen == 0)
extern int (**dead_vnodeop_p)(void *);
	if ( (lmp = vp->v_mount) != NULL && lmp != dead_mountp) {
		if ((vp->v_lflag & VNAMED_MOUNT) == 0)
			if (TAILQ_LAST(&lmp->mnt_vnodelist, vnodelst) == vp)
			else if (TAILQ_LAST(&lmp->mnt_newvnodes, vnodelst) == vp)
			else if (TAILQ_LAST(&lmp->mnt_workerqueue, vnodelst) == vp)
		 } else {
		}	
		if ((vp->v_mntvnodes.tqe_next != 0) && (vp->v_mntvnodes.tqe_prev != 0))
		if (mp->mnt_lflag & MNT_LITER)
		else
		if (vp->v_lflag & VNAMED_MOUNT)
	vnode_t	nvp;
	int	error;
		return (ENODEV);
	if ( (error = vnode_create(VNCREATE_FLAVOR, VCREATESIZE, &vfsp, &nvp)) ) {
		return (error);
	nvp->v_tag = VT_NON;	/* set this to VT_NON so during aliasing it can be replaced */
	if ( (error = vnode_ref(nvp)) ) {
		return (error);
	if ( (error = VNOP_FSYNC(nvp, MNT_WAIT, &context)) ) {
		return (error);
	if ( (error = buf_invalidateblks(nvp, BUF_WRITE_DATA, 0, 0)) ) {
		return (error);
	/* 
#endif /* MAC */	
	if ( (error = VNOP_OPEN(nvp, FREAD, &context)) ) {
		return (error);
	return (0);

		        vid = vp->v_id;
	        if (vnode_getwithvid(vp,vid)) {
		        goto loop;
		        vnode_reclaim_internal(vp, 1, 1, 0);
		
					M_SPECINFO, M_WAITOK);
		
		} 
		return (NULLVP);
	if ((vp->v_flag & (VBDEVVP | VDEVFLUSH)) != 0)
		return(vp);
	return (vp);
	if ((vflags & VNODE_WRITEABLE) && (vp->v_writecount == 0))
	        /*
		 * vnode to be returned only if it has writers opened 
	        error = EINVAL;
	else
	        error = vnode_getiocount(vp, vid, vflags);
	return (error);

        return (vnode_ref_ext(vp, 0, 0));
	int	error = 0;
	if (vp->v_iocount <= 0 && vp->v_usecount <= 0) 
	        if (++vp->v_writecount <= 0)
		        panic("vnode_ref_ext: v_writecount");
	        if (++vp->v_kusecount <= 0)
		        panic("vnode_ref_ext: v_kusecount");
	        struct  uthread *ut;
	        ut = get_bsdthread_info(current_thread());
		
	        if ( !(current_proc()->p_lflag & P_LRAGE_VNODES) &&
		     !(ut->uu_flag & UT_RAGE_VNODES)) {
		        /*

	return (error);
	if ( !(vp->v_mount->mnt_kern_flag & MNTK_VIRTUALDEV) && (vp->v_mount->mnt_flag & MNT_LOCAL) )
		return (TRUE);
	return (FALSE);
	if (VONLIST(vp) || (vp->v_lflag & (VL_TERMINATE|VL_DEAD)))

	 * if it is already on a list or non zero references return 
	if (VONLIST(vp) || (vp->v_usecount != 0) || (vp->v_iocount != 0) || (vp->v_lflag & VL_TERMINATE))
		&& (!LIST_EMPTY(&vp->v_cleanblkhd) || !LIST_EMPTY(&vp->v_dirtyblkhd))) {
		++vp->v_iocount;	// Probably not necessary, but harmless
		if ((vp->v_flag & VAGE))
		        TAILQ_INSERT_HEAD(&vnode_rage_list, vp, v_freelist);
		else
		        TAILQ_INSERT_TAIL(&vnode_rage_list, vp, v_freelist);
	        /*
	        if ( (vp->v_lflag & VL_DEAD)) {
		        TAILQ_INSERT_HEAD(&vnode_dead_list, vp, v_freelist);

		} else if ( (vp->v_flag & VAGE) ) {
		        TAILQ_INSERT_HEAD(&vnode_free_list, vp, v_freelist);
		        TAILQ_INSERT_TAIL(&vnode_free_list, vp, v_freelist);
	if (need_dead_wakeup == TRUE)
	        if (vp->v_listflag & VLIST_RAGE)
		        VREMRAGE("vnode_list_remove", vp);
		else if (vp->v_listflag & VLIST_DEAD)
		        VREMDEAD("vnode_list_remove", vp);
		else if (vp->v_listflag & VLIST_ASYNC_WORK)
		        VREMASYNC_WORK("vnode_list_remove", vp);
		else
		        VREMFREE("vnode_list_remove", vp);
        /*
	 * the not-on-list state to the on-list 
	        vnode_list_lock();
		 * hold the vnode_list_lock... this 
        vnode_rele_internal(vp, 0, 0, 0);
        vnode_rele_internal(vp, fmode, dont_reenter, 0);

	if ( !locked)
	        vnode_lock_spin(vp);
	else
	if (--vp->v_usecount < 0)
		panic("vnode_rele_ext: vp %p usecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp,  vp->v_usecount, vp->v_tag, vp->v_type, vp->v_flag);
		if (--vp->v_writecount < 0)
			panic("vnode_rele_ext: vp %p writecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp,  vp->v_writecount, vp->v_tag, vp->v_type, vp->v_flag);
	        if (--vp->v_kusecount < 0)
		        panic("vnode_rele_ext: vp %p kusecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp,  vp->v_kusecount, vp->v_tag, vp->v_type, vp->v_flag);
	if (vp->v_kusecount > vp->v_usecount)
		panic("vnode_rele_ext: vp %p kusecount(%d) out of balance with usecount(%d).  v_tag = %d, v_type = %d, v_flag = %x.",vp, vp->v_kusecount, vp->v_usecount, vp->v_tag, vp->v_type, vp->v_flag);
			if ( !(vp->v_lflag & (VL_TERMINATE | VL_DEAD | VL_MARKTERM)) ) {
				
        vp->v_lflag &= ~VL_NEEDINACTIVE;
	if ( (vp->v_iocount == 1) && (vp->v_usecount == 0) &&
	     ((vp->v_lflag & (VL_MARKTERM | VL_TERMINATE | VL_DEAD)) == VL_MARKTERM)) {
	        struct  uthread *ut;
	        ut = get_bsdthread_info(current_thread());
		
		        vp->v_defer_reclaimlist = ut->uu_vreclaims;
	        vnode_reclaim_internal(vp, 1, 1, 0);

	if ( !locked)
	        vnode_unlock(vp);
#if DIAGNOSTIC
int busyprt = 0;	/* print out busy vnodes */
#endif
	if (((flags & FORCECLOSE)==0)  && ((mp->mnt_kern_flag & MNTK_UNMOUNT_PREFLIGHT) != 0)) {
			return(EBUSY);
	/* it is returns 0 then there is nothing to do */
	if (retval == 0)  {
		return(retval);

		if ( (vp->v_mount != mp) || (vp == skipvp)) {
				vnode_unlock(vp);
				mount_lock(mp);
				continue;
                 */

		        vp->v_iocount++;	/* so that drain waits for * other iocounts */
				vp->v_iocount++;	/* so that drain waits * for other iocounts */
#if DIAGNOSTIC
		if (busyprt)
			vprint("vflush: busy vnode", vp);
#endif
	if (busy && ((flags & FORCECLOSE)==0) && reclaimed) {
 	if ( vnode_iterate_reloadq(mp) != 0) {
		if (!(busy && ((flags & FORCECLOSE)==0)))
	if (busy && ((flags & FORCECLOSE)==0))
		return (EBUSY);
	return (0);
	if (flags & DOCLOSE)
	if (flags & REVOKEALL)
	
	if (active && (flags & DOCLOSE))
		if (vp->v_tag == VT_NFS)
		else
		if (UBCINFOEXISTS(vp))
		        /*
		        (void)ubc_msync(vp, (off_t)0, ubc_getsize(vp), NULL, UBC_PUSHALL | UBC_INVALIDATE | UBC_SYNC);
	if (active || need_inactive) 
	    
		
		/* 
	if (vp->v_resolve)
	if (VNOP_RECLAIM(vp, ctx))
	
	vnode_update_identity(vp, NULLVP, NULL, 0, 0, VNODE_UPDATE_PARENT | VNODE_UPDATE_NAME | VNODE_UPDATE_PURGE);
	        vp->v_lflag &= ~VL_TERMINATE;
		        vp->v_lflag &= ~VL_TERMWANT;
	if ((flags & REVOKEALL) == 0)
		if (vp->v_lflag & VL_TERMINATE)
			return(ENOENT);
				    vq->v_type != vp->v_type || vp == vq)
				if (vnode_getwithvid(vq,vid)){
					SPECHASH_LOCK();	
		return (ENOENT);
	return (0);
		return(0);
	} 
	return (1);
		return(0);
	} 
	if (vp->v_iocount <= 0)
	return (1);
			SPECHASH_LOCK();
			if (*vp->v_hashchain == vp) {
				*vp->v_hashchain = vp->v_specnext;
			} else {
				for (vq = *vp->v_hashchain; vq; vq = vq->v_specnext) {
					if (vq->v_specnext != vp)
						continue;
					vq->v_specnext = vp->v_specnext;
					break;
			if (vq == NULL)
			if (vp->v_specflags & SI_ALIASED) {
				vx = NULL;
				for (vq = *vp->v_hashchain; vq; vq = vq->v_specnext) {
					if (vq->v_rdev != vp->v_rdev ||
				    	vq->v_type != vp->v_type)
						continue;
					if (vx)
						break;
					vx = vq;
				if (vx == NULL)
					panic("missing alias");
				if (vq == NULL)
					vx->v_specflags &= ~SI_ALIASED;
				vp->v_specflags &= ~SI_ALIASED;
			SPECHASH_UNLOCK();
			{
			FREE_ZONE((void *)tmp, sizeof(struct specinfo), M_SPECINFO);
			}
	vnode_t	vp;
		if (dev != vp->v_rdev || type != vp->v_type)
		if (vnode_getwithvid(vp,vid))
			if ((*errorp = vfs_mountedon(vp)) != 0)
		} else
		return(rc);
	return (0);
		return (vp->v_usecount - vp->v_kusecount);		
	if (!vnode_isaliased(vp))
	        return (vp->v_specinfo->si_opencount);
			if ((vq->v_usecount == 0) && (vq->v_iocount == 1)  && vq != vp) {
	return (count);
int	prtactive = 0;		/* 1 => print out reclaim of active vnodes */
   { "VNON", "VREG", "VDIR", "VBLK", "VCHR", "VLNK", "VSOCK", "VFIFO", "VBAD" };
	if (label != NULL)
	printf("type %s, usecount %d, writecount %d",
	       typename[vp->v_type], vp->v_usecount, vp->v_writecount);
	if (vp->v_flag & VROOT)
	if (vp->v_flag & VTEXT)
	if (vp->v_flag & VSYSTEM)
	if (vp->v_flag & VNOFLUSH)
	if (vp->v_flag & VBWAIT)
	if (vnode_isaliased(vp))
	if (sbuf[0] != '\0')
		printf(" flags (%s)", &sbuf[1]);
static char *extension_table=NULL;
    return (strlen((const char *)a) - strlen((const char *)b));
    char *new_exts, *old_exts;
    int error;
    
    if (nentries <= 0 || nentries > 1024 || maxwidth <= 0 || maxwidth > 255) {
	return EINVAL;
    }


    // allocate one byte extra so we can guarantee null termination
    MALLOC(new_exts, char *, (nentries * maxwidth) + 1, M_TEMP, M_WAITOK);
    if (new_exts == NULL) {
    	return ENOMEM;
    }
    
    error = copyin(data, new_exts, nentries * maxwidth);
    if (error) {
	FREE(new_exts, M_TEMP);
	return error;
    }
    new_exts[(nentries * maxwidth)] = '\0';   // guarantee null termination of the block
    qsort(new_exts, nentries, maxwidth, extension_cmp);
    lck_mtx_lock(pkg_extensions_lck);
    old_exts        = extension_table;
    extension_table = new_exts;
    nexts           = nentries;
    max_ext_width   = maxwidth;
    lck_mtx_unlock(pkg_extensions_lck);
    if (old_exts) {
	FREE(old_exts, M_TEMP);
    }
    return 0;
int is_package_name(const char *name, int len)
    int i, extlen;
    const char *ptr, *name_ext;
    
    if (len <= 3) {
	return 0;
    }
    name_ext = NULL;
    for(ptr=name; *ptr != '\0'; ptr++) {
	if (*ptr == '.') {
	    name_ext = ptr;
    }
    // if there is no "." extension, it can't match
    if (name_ext == NULL) {
	return 0;
    }
    // advance over the "."
    name_ext++;
    lck_mtx_lock(pkg_extensions_lck);
    // now iterate over all the extensions to see if any match
    ptr = &extension_table[0];
    for(i=0; i < nexts; i++, ptr+=max_ext_width) {
	extlen = strlen(ptr);
	if (strncasecmp(name_ext, ptr, extlen) == 0 && name_ext[extlen] == '\0') {
	    // aha, a match!
	    lck_mtx_unlock(pkg_extensions_lck);
	    return 1;
    }
    lck_mtx_unlock(pkg_extensions_lck);
    // if we get here, no extension matched
    return 0;
    char *ptr, *end;
    int comp=0;
    
    *component = -1;
    if (*path != '/') {
	return EINVAL;
    }
    end = path + 1;
    while(end < path + pathlen && *end != '\0') {
	while(end < path + pathlen && *end == '/' && *end != '\0') {
	    end++;
	ptr = end;
	while(end < path + pathlen && *end != '/' && *end != '\0') {
	    end++;
	}
	if (end > path + pathlen) {
	    // hmm, string wasn't null terminated 
	    return EINVAL;
	}
	*end = '\0';
	if (is_package_name(ptr, end - ptr)) {
	    *component = comp;
	    break;
	end++;
	comp++;
    }

    return 0;
/* 
int vn_searchfs_inappropriate_name(const char *name, int len) {
	for(i=0; i < (int) (sizeof(bad_names) / sizeof(bad_names[0])); i++) {
		   CTLFLAG_RD | CTLFLAG_KERN | CTLFLAG_LOCKED,
		   &vfs_nummntops, 0, "");
		   user_addr_t oldp __unused, size_t *oldlenp __unused,
           user_addr_t newp __unused, size_t newlen __unused, proc_t p __unused);
		   user_addr_t oldp __unused, size_t *oldlenp __unused,
           user_addr_t newp __unused, size_t newlen __unused, proc_t p __unused)
	return (EINVAL);
	switch(selector_name) {
		case VFS_CTL_QUERY:
		case VFS_CTL_TIMEO:
		case VFS_CTL_NOLOCKS:
		case VFS_CTL_NSTATUS:
		case VFS_CTL_SADDR:
		case VFS_CTL_DISC:
		case VFS_CTL_SERVERINFO:
			return 1;
		default:
			break;
		switch(selector_name) {
			case SMBFS_SYSCTL_REMOUNT:
			case SMBFS_SYSCTL_REMOUNT_INFO:
			case SMBFS_SYSCTL_GET_SERVER_SHARE:
				return 1;
		switch(selector_name) {
			case AFPFS_VFS_CTL_GETID:
			case AFPFS_VFS_CTL_NETCHANGE:
			case AFPFS_VFS_CTL_VOLCHANGE:
				return 1;
	
	if (namelen < 1)
		return (EISDIR); /* overloaded */
	
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next)
	
		return (ENOTSUP);
			    vq->v_type != vp->v_type)
	return (error);
	int	u_errs;	// Total failed unmounts
	int	u_busy;	// EBUSY failed unmounts
	mount_iterdrop(mp);	// avoid vfs_iterate deadlock in dounmount()
	if (mntname)
		if (error == EBUSY)
	if (mntname)
	return (VFS_RETURNED);
	if (mounts == 0)
	if (ui.u_busy > 0) {		// Busy mounts - wait & retry
		if (sec <= 32)
	} else if (ui.u_errs < mounts)	{
/*  
        struct ubc_info *uip;
	int	error;
	off_t	readblockcnt = 0;
	off_t	writeblockcnt = 0;
	off_t	readmaxcnt = 0;
	off_t	writemaxcnt = 0;
	off_t	readsegcnt = 0;
	off_t	writesegcnt = 0;
	off_t	readsegsize = 0;
	off_t	writesegsize = 0;
	off_t	alignment = 0;
	if (devvp == rootvp)
				(caddr_t)&blksize, 0, ctx)))
		return (error);
	        if (isvirtual)
		        mp->mnt_kern_flag |= MNTK_VIRTUALDEV;
	        if (isssd)
		        mp->mnt_kern_flag |= MNTK_SSD;
				(caddr_t)&features, 0, ctx)))
		return (error);
				(caddr_t)&readblockcnt, 0, ctx)))
		return (error);
				(caddr_t)&writeblockcnt, 0, ctx)))
		return (error);
				(caddr_t)&readmaxcnt, 0, ctx)))
		return (error);
				(caddr_t)&writemaxcnt, 0, ctx)))
		return (error);
				(caddr_t)&readsegcnt, 0, ctx)))
		return (error);
				(caddr_t)&writesegcnt, 0, ctx)))
		return (error);
				(caddr_t)&readsegsize, 0, ctx)))
		return (error);
				(caddr_t)&writesegsize, 0, ctx)))
		return (error);
				(caddr_t)&alignment, 0, ctx)))
		return (error);
				(caddr_t)&ioqueue_depth, 0, ctx)))
		return (error);
	if (readmaxcnt)
		if (temp < mp->mnt_maxreadcnt)
	if (writemaxcnt)
		if (temp < mp->mnt_maxwritecnt)
	        temp = (readsegcnt > UINT16_MAX) ? UINT16_MAX : readsegcnt;
		if (temp > UINT16_MAX)
	        temp = (writesegcnt > UINT16_MAX) ? UINT16_MAX : writesegcnt;
		if (temp > UINT16_MAX)
	if (readsegsize)
	        temp = (readsegsize > UINT32_MAX) ? UINT32_MAX : readsegsize;
	else
	        temp = mp->mnt_maxreadcnt;
	if (writesegsize)
	        temp = (writesegsize > UINT32_MAX) ? UINT32_MAX : writesegsize;
	else
	        temp = mp->mnt_maxwritecnt;
	if (alignment)
	        temp = (alignment > PAGE_SIZE) ? PAGE_MASK : alignment - 1;
	else
	        temp = 0;
	if (ioqueue_depth > MNT_DEFAULT_IOQUEUE_DEPTH)
	else
	mp->mnt_ioscale = (mp->mnt_ioqueue_depth + (MNT_DEFAULT_IOQUEUE_DEPTH - 1)) / MNT_DEFAULT_IOQUEUE_DEPTH;
	if (mp->mnt_ioscale > 1)
	if (features & DK_FEATURE_FORCE_UNIT_ACCESS)
	        mp->mnt_ioflags |= MNT_IOFLAGS_FUA_SUPPORTED;
	
	if (VNOP_IOCTL(devvp, DKIOCCORESTORAGE, (caddr_t)&cs_info, 0, ctx) == 0)
		if (cs_present == TRUE)
		if ((cs_info.flags & DK_CORESTORAGE_PIN_YOUR_METADATA))
        if (iosched_enabled && (features & DK_FEATURE_PRIORITY)) {
                mp->mnt_ioflags |= MNT_IOFLAGS_IOSCHED_SUPPORTED;
	return (error);

			if (data)
				mp->mnt_kern_flag &= ~MNT_LNOTRESP;	// Now responding
			else
				mp->mnt_kern_flag |= MNT_LNOTRESP;	// Not responding
	return(mount_getvfscnt());
	return (ret);

	int actual=0;
	return (actual);

		if (*actual <= count)
	return (*actual <= count ? 0 : ENOMEM);
		__unused int arg2, struct sysctl_req *req)
	if (req->newptr != USER_ADDR_NULL)
		return (EPERM);
	if (space < req->oldlen)
		return (ENOMEM);
	MALLOC(fsidlst, fsid_t *, req->oldlen, M_TEMP, M_WAITOK);
		return (ENOMEM);
	return (error);
		struct sysctl_req *req)
	int error=0, gotref=0;
	proc_t p = req->p;	/* XXX req->p != current_proc()? */
	if (error)
				    NULL, USER_ADDR_NULL, 0, 
			}
			else {
		}
		else {
			    NULL, USER_ADDR_NULL, 0, 
		}
		else {
		if (error)
		}
		else {
		if (error)
		    (error = vfs_update_vfsstat(mp, ctx, VFS_USER_EVENT)))
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSTYPENAMELEN);
            
		}
		else {
				int		shift;
					if ((sp->f_blocks >> shift) <= INT_MAX)
					if ((((long long)sp->f_bsize) << (shift + 1)) > INT_MAX)
#define __SHIFT_OR_CLIP(x, s)	((((x) >> (s)) > INT_MAX) ? INT_MAX : ((x) >> (s)))
#ifdef NFS_CLIENT
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSTYPENAMELEN);
            
	if(gotref != 0)
	return (error);
}

static int	filt_fsattach(struct knote *kn);
static void	filt_fsdetach(struct knote *kn);
static int	filt_fsevent(struct knote *kn, long hint);
static int	filt_fstouch(struct knote *kn, struct kevent_internal_s *kev);
static int	filt_fsprocess(struct knote *kn, struct filt_process_s *data, struct kevent_internal_s *kev);
struct filterops fs_filtops = {
        .f_attach = filt_fsattach,
        .f_detach = filt_fsdetach,
        .f_event = filt_fsevent,
filt_fsattach(struct knote *kn)
	/* 
	 * filter only sees future events, 
	return (0);
	return (kn->kn_fflags != 0);
filt_fstouch(struct knote *kn, struct kevent_internal_s *kev)
	if ((kn->kn_status & KN_UDATA_SPECIFIC) == 0)
		kn->kn_udata = kev->udata;
filt_fsprocess(struct knote *kn, struct filt_process_s *data, struct kevent_internal_s *kev)
#pragma unused(data)
	int res;
	res = (kn->kn_fflags != 0);
	if (res) {
		*kev = kn->kn_kevent;
		kn->kn_flags |= EV_CLEAR; /* automatic */
		kn->kn_fflags = 0;
		kn->kn_data = 0;
}	
		__unused void *arg1, __unused int arg2, struct sysctl_req *req)
	if (req->newptr == USER_ADDR_NULL)
		return (EINVAL);
	if (error)
		return (error);
	if (p == NULL)
		return (ESRCH);
		return (error);
		return (EPERM);
	if (pid < 0)
	else
	return (0);
	struct vfsconf vfsc;
	
	
		return (EISDIR);
		return (ENOTDIR);
			
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next)
		if (vfsp->vfc_typenum == name[0])
	
		return (ENOTSUP);
			
			
	return (SYSCTL_OUT(req, &vfsc, sizeof(struct vfsconf)));
		   CTLFLAG_RD | CTLFLAG_KERN | CTLFLAG_LOCKED,
		   &maxvfstypenum, 0, "");
SYSCTL_INT(_vfs_generic, OID_AUTO, sync_timeout, CTLFLAG_RW | CTLFLAG_LOCKED, &sync_timeout, 0, "");
		   CTLFLAG_RD | CTLFLAG_LOCKED,
		   sysctl_vfs_generic_conf, "");
	/* 
	 */ 
	if ((vpid != vp->v_id) || (vp->v_usecount != 0) || (vp->v_iocount != 0) || 
		
		return (NULLVP);
	if ( (vp->v_lflag & (VL_NEEDINACTIVE | VL_MARKTERM)) == VL_NEEDINACTIVE ) {
	        /*
		 * vnode_put will either leave us off 
	        vp->v_iocount++;
		return (NULLVP);
	 */ 
			
			return (NULLVP);
		if (vp->v_lflag & VL_DEAD)
			if ((VONLIST(vp)))
			    (vp->v_lflag & (VNAMED_UBC | VNAMED_MOUNT | VNAMED_FSHASH)))
			if ((vp->v_mntvnodes.tqe_prev != 0) && (vp->v_mntvnodes.tqe_next != 0))
			if ( !LIST_EMPTY(&vp->v_nclinks) || !TAILQ_EMPTY(&vp->v_ncchildren))
	return (vp);
	int	deferred;
	vnode_t	vp;

		if ( TAILQ_EMPTY(q) ) {
	
			
		if (vp != NULLVP)
	vnode_t	vp;
	uint32_t retries = 0, max_retries = 100;		/* retry incase of tablefull */
        struct timeval initial_tv;
        struct timeval current_tv;
	if (need_reliable_vp == TRUE)
		if ( !TAILQ_EMPTY(&vnode_dead_list)) {
		VLISTNONE(vp);		/* avoid double queue removal */
		if (mac_vnode_label_init_needed(vp))
	if ( !TAILQ_EMPTY(&vnode_rage_list) &&
	     (ragevnodes >= rage_limit ||
	      (current_tv.tv_sec - rage_tv.tv_sec) >= RAGE_TIME_LIMIT)) {

			if ( !(vp->v_listflag & VLIST_RAGE))
			// may not be updated if there are multiple devnode layers 
			if ((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL || 
				 * mechanism to first scan for a reliable vnode before forcing 
				if (need_reliable_vp == FALSE || vnode_on_reliable_media(vp) == TRUE)
	        /*

			// may not be updated if there are multiple devnode layers 
			if ((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL || 
				 * mechanism to first scan for a reliable vnode before forcing 
				if (need_reliable_vp == FALSE || vnode_on_reliable_media(vp) == TRUE)
			
			"%d free, %d dead, %d async, %d rage\n",
		        desiredvnodes, numvnodes, freevnodes, deadvnodes, async_work_vnodes, ragevnodes);
		if (bootarg_no_vnode_jetsam)
		/* 
		 * Now that we've killed someone, wait a bit and continue looking 
		retries = 0;	
		return (ENFILE);
			int	elapsed_msecs;
			if (initial_tv.tv_sec == 0)
			
				 * we've waited long enough... 100ms is 
	assert ((vp->v_lflag & VL_LABELWAIT) != VL_LABELWAIT);
	assert ((vp->v_lflag & VL_LABEL) != VL_LABEL);
	if (vp->v_lflag & VL_LABELED) {
	        vnode_lock_convert(vp);
	        vnode_lock_convert(vp);
        vp->v_references = 0;
	return (0);
        int retval;
        vnode_lock_spin(vp);
	return(retval);	
		return(ENOENT);	
	vp->v_iocount++;
	return (0);
        return(vget_internal(vp, vid, ( VNODE_NODEAD | VNODE_WITHID | VNODE_DRAINO )));
        return(vget_internal(vp, vid, ( VNODE_NODEAD | VNODE_WITHID )));
        return(vget_internal(vp, 0, 0));
        return(vget_internal(vp, 0, VNODE_ALWAYS));
        int retval;
	return(retval);
	vfs_context_t ctx = vfs_context_current();	/* hoist outside loop */
	if (vp->v_iocount < 1) 
	if ((vp->v_usecount > 0) || (vp->v_iocount > 1))  {
		return(0);

	        vp->v_lflag &= ~VL_NEEDINACTIVE;
	        vnode_unlock(vp);
        vp->v_lflag &= ~VL_NEEDINACTIVE;
	        vnode_lock_convert(vp);
	        vnode_reclaim_internal(vp, 1, 1, 0);
	return(0);
int 
	return(vnode_isinuse_locked(vp, refcnt, 0));
int vnode_usecount(vnode_t vp)
int vnode_iocount(vnode_t vp)
static int 
	if (!locked)
	if ((vp->v_type != VREG) && ((vp->v_usecount - vp->v_kusecount) >  refcnt)) {
	if (vp->v_type == VREG)  {
		
	if (!locked)
	return(retval);
errno_t 

	        vp->v_lflag &= ~VL_SUSPENDED;
	return(0);
		return(EBUSY);
	/* 
	 * xxx is this sufficient to check if a vnode_drain is 
	return(0);
					
	if (vp->v_flag & VLOCKLOCAL)
					
static errno_t 
	
		return(ENOENT);
	while (vp->v_iocount > 1)
		msleep(&vp->v_iocount, &vp->v_lock, PVFS, "vnode_drain", NULL);
	return(0);
#define UNAGE_DIRTYTHRESHHOLD    6    
	        if (nodead && (vp->v_lflag & VL_DEAD) && ((vp->v_type == VBAD) || (vp->v_data == 0))) {
			return(ENOENT);
		if ((vp->v_lflag & (VL_SUSPENDED | VL_DRAIN | VL_TERMINATE)) == 0 ) {
			return(ENOENT);
		        break;
		
		if (always != 0) 
			if (beatdrain)
			if (withvid && (vp->v_mount) && vfs_isunmount(vp->v_mount))
				return (ENODEV);
			error = msleep(&vp->v_lflag,   &vp->v_lock,
			   (PVFS | sleepflg), "vnode getiocount", NULL);
			if (error)
				return (error);
		} else
		return(ENOENT);
	        vp->v_references = 0;
	return(0);	
vnode_dropiocount (vnode_t vp)
	if (vp->v_iocount < 1)
	if ((vp->v_lflag & (VL_DRAIN | VL_SUSPENDED)) && (vp->v_iocount <= 1))
	if (!locked)
	if (vp->v_type != VBAD)
		vgone(vp, flags);		/* clean and reclaim the vnode */

	if (vp->v_data)
	if (vp->v_numoutput)
	if (UBCINFOEXISTS(vp))
	if (vp->v_parent)
	        panic("vnode_reclaim_internal: vparent not removed");
	if (vp->v_name)
	        panic("vnode_reclaim_internal: vname not removed");
	        /*
	        vnode_list_add(vp);
	if (!locked)
	        vnode_unlock(vp);
        struct  uthread *ut;
		if ((error = new_vnode(&vp)) ) {
			return (error);
			return (0);
	if (param->vnfs_markroot)
	if (param->vnfs_marksystem)
			return(error);
		if (param->vnfs_mp->mnt_ioflags & MNT_IOFLAGS_IOSCHED_SUPPORTED)
		/* 
		 * mount if successful, which we would need to undo on a 
			return (error);
		vp->v_tag = VT_DEVFS;		/* callers will reset if needed (bdevvp) */

		if ( (nvp = checkalias(vp, param->vnfs_rdev)) ) {
			vp->v_data = NULL; 
			if (maj < (u_int)nchrdev && cdevsw[maj].d_type == D_TTY)
			sizeof(*fip), M_TEMP, M_WAITOK);
		bzero(fip, sizeof(struct fifoinfo ));
			if (param->vnfs_mp->mnt_kern_flag & MNTK_LOCK_LOCAL)
				vp->v_flag |= VLOCKLOCAL;
			if ((vp->v_freelist.tqe_prev != (struct vnode **)0xdeadb))
		} else
		if ((cnp->cn_flags & UNIONCREATED) == UNIONCREATED)
	    (ut->uu_flag & UT_RAGE_VNODES)) {
		    (! (vfs_flags(vnode_mount(vp)) & MNT_ROOTFS))) {
		 * 	+ dyld_shared_cache_arm64*
		 * 	+ Camera
	return (0);
	return (error);
	return (vnode_create_internal(flavor, size, data, vpp, 1));
	return (vnode_create_internal(VNCREATE_FLAVOR, VCREATESIZE, NULL,
	    vpp, 0));
	return (vnode_create_internal(flavor, size, data, vpp, 1));
	if (vp->v_lflag & VNAMED_FSHASH)
	if ((vp->v_freelist.tqe_prev != (struct vnode **)0xdeadb))
	        panic("addfsref: vp on the free list\n");
	return(0);

	if ((vp->v_lflag & VNAMED_FSHASH) == 0)
	return(0);

	mount_t	mp;
	int count, actualcount,  i;
	} else /* Head first by default */ {
	for (i=indx_start; i != indx_stop; i += indx_incr) {

		if(mp == (struct mount *)0)
		if (mp->mnt_lflag & (MNT_LDEAD | MNT_LUNMOUNT)) {
		
		if (!cb_dropref)
	return (ret);
	struct vfs_attr	va;
	int		error;
		return(error);
		if (error != 0)
			return (error);
		mp->mnt_vfsstat.f_bsize = mp->mnt_devblocksize;	/* default from the device block size */
		mp->mnt_vfsstat.f_iosize = 1024 * 1024;		/* 1MB sensible I/O size */
	if (VFSATTR_IS_SUPPORTED(&va, f_blocks))
	if (VFSATTR_IS_SUPPORTED(&va, f_bfree))
	if (VFSATTR_IS_SUPPORTED(&va, f_bavail))
	if (VFSATTR_IS_SUPPORTED(&va, f_bused))
	if (VFSATTR_IS_SUPPORTED(&va, f_files))
	if (VFSATTR_IS_SUPPORTED(&va, f_ffree))
	if (VFSATTR_IS_SUPPORTED(&va, f_fssubtype))
	return(0);
		TAILQ_INSERT_TAIL(&mountlist, mp, mnt_list);	
				if (mount_iterref(cur_mount, 1))  {
	return(cur_mount);
mount_t 
	if (!locked)
	TAILQ_FOREACH(mp, &mountlist, mnt_list) 
		if (mp->mnt_vfsstat.f_fsid.val[0] == fsid->val[0] &&
		    mp->mnt_vfsstat.f_fsid.val[1] == fsid->val[1]) {
			retmp = mp;
			if (withref) {
				if (mount_iterref(retmp, 1)) 
					retmp = (mount_t)0;
			goto out;
	if (!locked)
	return (retmp);
vnode_lookup(const char *path, int flags, vnode_t *vpp, vfs_context_t ctx)
	if (flags & VNODE_LOOKUP_NOFOLLOW)
	else
	if (flags & VNODE_LOOKUP_NOCROSSMOUNT)
	if (flags & VNODE_LOOKUP_CROSSMOUNTNOWAIT)
	       CAST_USER_ADDR_T(path), ctx);
	if ((error = namei(&nd)))
		return (error);
	
	return (0);
	if (ctx == NULL) {		/* XXX technically an error */
	if (fmode & O_NOFOLLOW)
	if (lflags & VNODE_LOOKUP_NOFOLLOW)
	else
	if (lflags & VNODE_LOOKUP_NOCROSSMOUNT)
	
	if (lflags & VNODE_LOOKUP_CROSSMOUNTNOWAIT)
	       CAST_USER_ADDR_T(path), ctx);
	if ((error = vn_open(&nd, fmode, cmode)))
	else
	
	return (error);
	
	return (error);
	struct vnode_attr	va;
	int			error;
	if (!error)
	struct vnode_attr	va;
	int			error;
	if (!error)
	struct vnode_attr	va;
	int			error;
	if (!error)
	return(error);
	struct vnode_attr	va;
	return(vnode_setattr(vp, &va, ctx));
int 
 *		
 *		
 *		
 *					
	errno_t	error, old_error;
	if (flags & VN_CREATE_NOINHERIT) 
	if (flags & VN_CREATE_NOAUTH) 
	switch(vap->va_type) {
		if (error)

	return(error);
static kauth_scope_t	vnode_scope;
static int	vnode_authorize_callback(kauth_cred_t credential, void *idata, kauth_action_t action,
    uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);
static int	vnode_authorize_callback_int(__unused kauth_cred_t credential, __unused void *idata, kauth_action_t action,
	vnode_t		vp;
	vnode_t		dvp;
	vfs_context_t	ctx;
	int		flags;
	int		flags_valid;
#define _VAC_IS_OWNER		(1<<0)
#define _VAC_IN_GROUP		(1<<1)
#define _VAC_IS_DIR_OWNER	(1<<2)
#define _VAC_IN_DIR_GROUP	(1<<3)
#define VATTR_PREPARE_DEFAULTED_UID		0x1
#define VATTR_PREPARE_DEFAULTED_GID		0x2
#define VATTR_PREPARE_DEFAULTED_MODE		0x4
			 oacl,
			 &nacl,
			 vap->va_type == VDIR,
			 ctx)) != 0) {
			return(error);
	
	} 
		if (oacl)  {
	 * Normally, unlinking of directories is not supported. 
			!(vp->v_mount->mnt_kern_flag & MNTK_DIR_HARDLINKS)) {
		return (EPERM);	/* POSIX */
	if (!error)
	if (!error)
		if (error)
			return (error);
	if ( (fmode & O_DIRECTORY) && vp->v_type != VDIR ) {
		return (ENOTDIR);
		return (EOPNOTSUPP);	/* Operation not supported on socket */
		return (ELOOP);		/* O_NOFOLLOW was specified and the target is a symbolic link */
		return (EISDIR);
			return (ENOTDIR);
	/* If a file being opened is a shadow file containing 
	 * namedstream data, ignore the macf checks because it 
	 * is a kernel internal file and access should always 
			return (error);
		if (vnode_isshadow(vp) && vnode_isnamedstream (vp)) {
		if (error)
			return (error);
	if (error)
		return (error);
	return (vnode_authorize(dvp, NULL, KAUTH_VNODE_ADD_FILE, ctx));
vn_authorize_rename(struct vnode *fdvp,  struct vnode *fvp,  struct componentname *fcnp,
					struct vnode *tdvp,  struct vnode *tvp,  struct componentname *tcnp,
					vfs_context_t ctx, void *reserved)
vn_authorize_renamex(struct vnode *fdvp,  struct vnode *fvp,  struct componentname *fcnp,
					 struct vnode *tdvp,  struct vnode *tvp,  struct componentname *tcnp,
					 vfs_context_t ctx, vfs_rename_flags_t flags, void *reserved)
	     (fcnp->cn_namelen == 1 && fcnp->cn_nameptr[0] == '.') ||
	     ((fcnp->cn_flags | tcnp->cn_flags) & ISDOTDOT)) ) {
		if (error) 
	if (error)
		if (error)
			if (vnode_isdir(fvp))
			if (vnode_isdir(tvp))
		if (error)
		if (error)
		if (fdvp == tdvp)
		else {
			if (error)
		if (error)
			if (tvp != fdvp)
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE | KAUTH_VNODE_ADD_SUBDIRECTORY, ctx)) != 0)
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE, ctx)) != 0)
							NULL, 
							vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE,
							ctx)) != 0) {
							vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE, ctx)) != 0)
				((error = vnode_authorize(tvp, tdvp, KAUTH_VNODE_DELETE, ctx)) != 0)) {
		panic("reserved not NULL in vn_authorize_mkdir()");	
		if (error)
	if (error)
  	/* authorize addition of a directory to the parent */
  	if ((error = vnode_authorize(dvp, NULL, KAUTH_VNODE_ADD_SUBDIRECTORY, ctx)) != 0)
  		goto out;
 	
	} 
	
	} 
	
			vp, cnp);
	if (error)
	int	error, result;
	if (vp->v_type == VBAD)
		return(0);
	
		   (uintptr_t)ctx, (uintptr_t)vp, (uintptr_t)dvp, (uintptr_t)&error);
	if (result == EPERM)		/* traditional behaviour */
	if ((result != 0) && (error != 0))
	        return(error);
	return(result);
	int	mask;
	if (append)
	if ((vap->va_flags & mask) != 0)
		return(EPERM);
	return(0);
	
	return(result);
	int	error;
	int	result;
		if (error == ENOENT)
	if (!error)
	return(error);
	return(result);
	int	error;
		
	return(error);
	return(result);
	int	error;
	return(error);
# define _SETWHERE(c)	where = c;
	
	
	if (action & VREAD)
	if (action & VWRITE)
	if (action & VEXEC)
	if (action & VREAD)
	if (action & VWRITE)
	if (action & VEXEC)
	if (action & VREAD)
	if (action & VWRITE)
	if (action & VEXEC)
		if (!owner_ok)
	ismember = 0;	/* Default to allow, if the target has no group owner */
		if (!group_ok)
		if (!group_ok)
	if (!world_ok)
	return(error);
	struct vnode_attr	*vap = vcp->vap;
	struct vnode_attr	*dvap = vcp->dvap;
	kauth_cred_t		cred = vcp->ctx->vc_ucred;
	struct kauth_acl_eval	eval;
	int			error, ismember;
		if (vauth_file_owner(vcp))
		if ((error = vauth_file_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT)
			return (error);
		if (error == ENOENT)
		else if (ismember)
			return (error);
		switch(eval.ae_result) {
			return (EACCES);
			return (0);
		return (0);
		if (vauth_dir_owner(vcp))
		if ((error = vauth_dir_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT)
			return(error);
		if (error == ENOENT)
		else if (ismember)
			return (error);
		switch(eval.ae_result) {
			return (EACCES);
			return (0);
			return (EACCES);
		return (EACCES);
	return (0);
	
	struct vnode_attr	*vap = vcp->vap;
	kauth_cred_t		cred = vcp->ctx->vc_ucred;
	struct kauth_acl_eval	eval;
	int			error, ismember;
	mode_t			posix_action;
	if (vauth_file_owner(vcp))
	    (acl_rights & KAUTH_VNODE_WRITE_SECURITY))
	
		return(0);
		if (vauth_file_owner(vcp))
		if ((error = vauth_file_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT)
			return(error);
		if (error == ENOENT)
		else if (ismember)
		
			return(error);
		
		switch(eval.ae_result) {
			return(EACCES);		/* deny, deny, counter-allege */
			return(0);
	if (vauth_file_owner(vcp))
	
		return(0);
	}		
	
		return(EACCES);
		return(EACCES);
	if (eval.ae_residual & KAUTH_VNODE_DELETE)
		KAUTH_VNODE_LIST_DIRECTORY |
		KAUTH_VNODE_READ_EXTATTRIBUTES))
		KAUTH_VNODE_ADD_FILE |
		KAUTH_VNODE_ADD_SUBDIRECTORY |
		KAUTH_VNODE_DELETE_CHILD |
		KAUTH_VNODE_WRITE_ATTRIBUTES |
		KAUTH_VNODE_WRITE_EXTATTRIBUTES))
		KAUTH_VNODE_SEARCH))
	
		return(vnode_authorize_posix(vcp, posix_action, 0 /* !on_dir */));
	return(0);
vnode_authorize_checkimmutable(vnode_t vp, struct vnode_attr *vap, int rights, int ignore)
	mount_t mp;
	switch(vp->v_type) {
		
		mp = vp->v_mount;
	
				KAUTH_DEBUG("%p    DENIED - filesystem disallowed extended attributes", vp);
		/* 
		 * check for file immutability. first, check if the requested rights are 
		if (vp->v_type == VDIR) {
			if ((rights & (KAUTH_VNODE_ADD_FILE | KAUTH_VNODE_ADD_SUBDIRECTORY | KAUTH_VNODE_WRITE_EXTATTRIBUTES)) == rights)
			if ((rights & (KAUTH_VNODE_APPEND_DATA | KAUTH_VNODE_WRITE_EXTATTRIBUTES)) == rights)
			KAUTH_DEBUG("%p    DENIED - file is immutable", vp);
	return(error);
	int	error;
	switch(vp->v_type) {
		return(0);
	if ((action & KAUTH_VNODE_ACCESS) && !vfs_authopaqueaccess(vp->v_mount))
		return(0);
		return(1);
	
			return(1);
	return(1);
vnode_authorize_callback(kauth_cred_t cred, void *idata, kauth_action_t action,
			 uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
	vfs_context_t	ctx;
	vnode_t		cvp = NULLVP;
	vnode_t		vp, dvp;
	int		result = KAUTH_RESULT_DENY;
	int		parent_iocount = 0;
	int		parent_action; /* In case we need to use namedstream's data fork for cached rights*/
	 * this point which rights to look at based on the 
        if (dvp && vp)
	        goto defer;
	        cvp = dvp;
		/* 
		 * on the parent means that we don't recompute for the named stream and that if 
		 * as if there were no cached rights (passing the named stream vnode and desired rights to 
		 * On an opaquely authorized volume, we don't know the relationship between the 
		 * stream. On a VNOP_SETATTR() for the parent that may invalidate cached authorization, we 

	 	result = KAUTH_RESULT_ALLOW;
        result = vnode_authorize_callback_int(cred, idata, action, arg0, arg1, arg2, arg3);
	        vnode_cache_authorized_action(cvp, ctx, action);
vnode_authorize_callback_int(__unused kauth_cred_t unused_cred, __unused void *idata, kauth_action_t action,
    uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
	vauth_ctx		vcp;
	vfs_context_t		ctx;
	vnode_t			vp, dvp;
	kauth_cred_t		cred;
	kauth_ace_rights_t	rights;
	struct vnode_attr	va, dva;
	int			result;
	int			*errorp;
	int			noimmutable;
	boolean_t		parent_authorized_for_delete_child = FALSE;
	boolean_t		found_deny = FALSE;
	boolean_t		parent_ref= FALSE;
	boolean_t		is_suser = FALSE;
	ctx = vcp->ctx = (vfs_context_t)arg0;
	vp = vcp->vp = (vnode_t)arg1;
	dvp = vcp->dvp = (vnode_t)arg2;
	errorp = (int *)arg3;
	if ((ctx == NULL) || (vp == NULL) || (cred == NULL))
	    (action & KAUTH_VNODE_ACCESS)		? "access" : "auth",
	    (action & KAUTH_VNODE_READ_DATA)		? vnode_isdir(vp) ? " LIST_DIRECTORY" : " READ_DATA" : "",
	    (action & KAUTH_VNODE_WRITE_DATA)		? vnode_isdir(vp) ? " ADD_FILE" : " WRITE_DATA" : "",
	    (action & KAUTH_VNODE_EXECUTE)		? vnode_isdir(vp) ? " SEARCH" : " EXECUTE" : "",
	    (action & KAUTH_VNODE_DELETE)		? " DELETE" : "",
	    (action & KAUTH_VNODE_APPEND_DATA)		? vnode_isdir(vp) ? " ADD_SUBDIRECTORY" : " APPEND_DATA" : "",
	    (action & KAUTH_VNODE_DELETE_CHILD)		? " DELETE_CHILD" : "",
	    (action & KAUTH_VNODE_READ_ATTRIBUTES)	? " READ_ATTRIBUTES" : "",
	    (action & KAUTH_VNODE_WRITE_ATTRIBUTES)	? " WRITE_ATTRIBUTES" : "",
	    (action & KAUTH_VNODE_READ_EXTATTRIBUTES)	? " READ_EXTATTRIBUTES" : "",
	    (action & KAUTH_VNODE_WRITE_EXTATTRIBUTES)	? " WRITE_EXTATTRIBUTES" : "",
	    (action & KAUTH_VNODE_READ_SECURITY)	? " READ_SECURITY" : "",
	    (action & KAUTH_VNODE_WRITE_SECURITY)	? " WRITE_SECURITY" : "",
	    (action & KAUTH_VNODE_CHANGE_OWNER)		? " CHANGE_OWNER" : "",
	    (action & KAUTH_VNODE_NOIMMUTABLE)		? " (noimmutable)" : "",
 
		if (dvp == NULL)
		if (vnode_cache_is_authorized(dvp, ctx, KAUTH_VNODE_DELETE_CHILD) == TRUE)
		        parent_authorized_for_delete_child = TRUE;
		dvp = NULL;
	
	    ((vp->v_type == VREG) || (vp->v_type == VDIR) || 
	     (vp->v_type == VLNK) || (vp->v_type == VCPLX) || 
	     (rights & KAUTH_VNODE_DELETE) || (rights & KAUTH_VNODE_DELETE_CHILD))) {
	if ((vp->v_mount->mnt_kern_flag & MNTK_AUTH_OPAQUE) && vnode_authorize_opaque(vp, &result, action, ctx))
		if (!(rights & (KAUTH_VNODE_EXECUTE | KAUTH_VNODE_WRITE_RIGHTS)))
	if (dvp) {
		if ((result = vnode_getattr(dvp, &dva, ctx)) != 0) {

	/*
	 * Check for immutability.
	 *
	 * In the deletion case, parent directory immutability vetoes specific
	 * file rights.
	 */
	if ((result = vnode_authorize_checkimmutable(vp, &va, rights, noimmutable)) != 0)
		goto out;
	if ((rights & KAUTH_VNODE_DELETE) &&
	    parent_authorized_for_delete_child == FALSE &&
	    ((result = vnode_authorize_checkimmutable(dvp, &dva, KAUTH_VNODE_DELETE_CHILD, 0)) != 0))
		goto out;

	/*
	 * Clear rights that have been authorized by reaching this point, bail if nothing left to
	 * check.
	 */
	rights &= ~(KAUTH_VNODE_LINKTARGET | KAUTH_VNODE_CHECKIMMUTABLE);
	if (rights == 0)
		goto out;

	/*
	 * If we're not the superuser, authorize based on file properties;
	 * note that even if parent_authorized_for_delete_child is TRUE, we
	 * need to check on the node itself.
	 */
	if (!is_suser) {
		/* process delete rights */
		if ((rights & KAUTH_VNODE_DELETE) &&
		    ((result = vnode_authorize_delete(vcp, parent_authorized_for_delete_child)) != 0))
		    goto out;

		/* process remaining rights */
		if ((rights & ~KAUTH_VNODE_DELETE) &&
		    (result = vnode_authorize_simple(vcp, rights, rights & KAUTH_VNODE_DELETE, &found_deny)) != 0)
			goto out;
	} else {

		/*
		 * Execute is only granted to root if one of the x bits is set.  This check only
		 * makes sense if the posix mode bits are actually supported.
		 */
		if ((rights & KAUTH_VNODE_EXECUTE) &&
		    (vp->v_type == VREG) &&
		    VATTR_IS_SUPPORTED(&va, va_mode) &&
		    !(va.va_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
			result = EPERM;
			KAUTH_DEBUG("%p    DENIED - root execute requires at least one x bit in 0x%x", vp, va.va_mode);
			goto out;
		}
		
		/* Assume that there were DENYs so we don't wrongly cache KAUTH_VNODE_SEARCHBYANYONE */
		found_deny = TRUE;
		
		KAUTH_DEBUG("%p    ALLOWED - caller is superuser", vp);
	}
	if (VATTR_IS_SUPPORTED(&va, va_acl) && (va.va_acl != NULL))
	if (VATTR_IS_SUPPORTED(&dva, va_acl) && (dva.va_acl != NULL))
		if (parent_ref)
		return(KAUTH_RESULT_DENY);
	        /*
		 * deny execute, we can synthesize a global right that allows anyone to 
	        if (!VATTR_IS_SUPPORTED(&va, va_mode) ||
		     (S_IXUSR | S_IXGRP | S_IXOTH))) {
		        vnode_cache_authorized_action(vp, ctx, KAUTH_VNODE_SEARCHBYANYONE);
	if (parent_ref)
	return(KAUTH_RESULT_ALLOW);
int 
	int		error;
	int		has_priv_suser, ismember, defaulted_owner, defaulted_group, defaulted_mode, inherit_restricted;
	kauth_cred_t	cred;
	guid_t		changer;
	mount_t		dmp;
	inherit_restricted = 0;
	
	if ((error = vnode_getattr(dvp, &dva, ctx)) != 0)
	if (!VATTR_IS_ACTIVE(vap, va_flags))
	if (VATTR_IS_SUPPORTED(&dva, va_flags) &&
	    (dva.va_flags & SF_RESTRICTED)) {
		inherit_restricted = 1;
	
	if (vfs_authopaque(dvp->v_mount))
	    goto out;
out:	
	if (inherit_restricted) {
		VATTR_SET(vap, va_flags, (vap->va_flags | SF_RESTRICTED));
	return(error);
	kauth_action_t	required_action;
	int		error, has_priv_suser, ismember, chowner, chgroup, clear_suid, clear_sgid;
	guid_t		changer;
	gid_t		group;
	uid_t		owner;
	mode_t		newmode;
	kauth_cred_t	cred;
	uint32_t	fdelta;
	if (vfs_authopaque(vp->v_mount))
	
	
	    VATTR_IS_ACTIVE(vap, va_backup_time)) {

#if 0	/* enable this when we support UUIDs as official owners */
		
			KAUTH_DEBUG("ATTR - size change, requiring WRITE_DATA");
			required_action |= KAUTH_VNODE_WRITE_DATA;
	    VATTR_IS_ACTIVE(vap, va_backup_time)) {
			required_action |= KAUTH_VNODE_CHECKIMMUTABLE;	/* always required */
			required_action |= KAUTH_VNODE_CHECKIMMUTABLE;	/* always required */
	    
	 * 
		if (!has_priv_suser && (kauth_cred_getuid(cred) != vap->va_uid)) {
			KAUTH_DEBUG("  DENIED - non-superuser cannot change ownershipt to a third party");
			error = EPERM;
			goto out;
		chowner = 1;
	}
	
		if (!has_priv_suser) {
			if ((error = kauth_cred_ismember_gid(cred, vap->va_gid, &ismember)) != 0) {
				KAUTH_DEBUG("  ERROR - got %d checking for membership in %d", error, vap->va_gid);
				goto out;
			}
			if (!ismember) {
				KAUTH_DEBUG("  DENIED - group change from %d to %d but not a member of target group",
				    ova.va_gid, vap->va_gid);
				error = EPERM;
				goto out;
		chgroup = 1;
	}
			if (kauth_guid_equal(&vap->va_uuuid, &ova.va_uuuid))
			
				    kauth_guid_equal(&vap->va_uuuid, &uid_guid))
				    	goto no_uuuid_change;
		
			if (kauth_guid_equal(&vap->va_guuid, &ova.va_guuid))
				    kauth_guid_equal(&vap->va_guuid, &gid_guid))
				    	goto no_guuid_change;
			if (kauth_guid_equal(&vap->va_guuid, &kauth_null_guid))
			else if ((error = kauth_cred_ismember_guid(cred, &vap->va_guuid, &ismember)) != 0) {
			
			
		 * setattrlist executed by a root process to set <uid, gid, mode> on a file:
			if (!VATTR_IS_ACTIVE(vap, va_mode) || !has_priv_suser) {
					newmode, newmode & ~(S_ISUID | S_ISGID));


					sizeof(struct kauth_ace) * vap->va_acl->acl_entrycount)) {
	if (VATTR_IS_ACTIVE(vap, va_encoding))
	
	if (VATTR_IS_SUPPORTED(&ova, va_acl) && (ova.va_acl != NULL))
	if (error == 0)
	return(error);
	return (VNODE_RETURNED);

	int64_t	max_swappin_avail = 0;
	if (vp->v_mount->mnt_ioflags & MNT_IOFLAGS_SWAPPIN_SUPPORTED)
	return (max_swappin_avail);
	while ((vp->v_flag & VISUNION) == VISUNION)
	if (!locked)
	if((vp->v_flag & VISUNION) == VISUNION) {
	if (!locked)
/* 
 * 2. Call VNOP_READDIR() till the end of directory is reached.  
 * 3. Check if the directory entries returned are regular files with name starting with "._".  If not, return ENOTEMPTY.  
errno_t rmdir_remove_orphaned_appleDouble(vnode_t vp , vfs_context_t ctx, int * restart_flag) 

	char uio_buf[ UIO_SIZEOF(1) ];
   	void *dir_pos;
	if (error == EBUSY)
	if (error != 0)
		return (error);
	if (rbuf)
				&uio_buf[0], sizeof(uio_buf));
	uio_setoffset(auio,0);
	if ((error = VNOP_OPEN(vp, FREAD, ctx))) 
		goto outsc; 	
	else
		if((error = VNOP_READDIR(vp, auio, 0, &eofflag, &nentries, ctx)))
		if (uio_resid(auio) != 0) 
		if (dir_pos == dir_end)
			if (dp->d_ino != 0 && 
					!((dp->d_namlen == 1 && dp->d_name[0] == '.') ||
					    (dp->d_namlen == 2 && dp->d_name[0] == '.' && dp->d_name[1] == '.'))) {
				if ( dp->d_namlen < 2 ||
						strncmp(dp->d_name,"._",2) ||
						(dp->d_namlen >= 4 && !strncmp(&(dp->d_name[2]), "._",2))) {
		
		 * workaround for HFS/NFS setting eofflag before end of file 
		if (vp->v_tag == VT_HFS && nentries > 2)
			eofflag=0;
			else if (!eofflag && full_erase_flag)
				full_erase_flag = 0;

		if (error != 0) 
		if (uio_resid(auio) != 0) 
		
		if (dir_pos == dir_end)
	
			if (dp->d_ino != 0 && 
					!((dp->d_namlen == 1 && dp->d_name[0] == '.') ||
					    (dp->d_namlen == 2 && dp->d_name[0] == '.' && dp->d_name[1] == '.'))
					  ) {
	
				if (error &&  error != ENOENT) {

		
		 * workaround for HFS/NFS setting eofflag before end of file 
		if (vp->v_tag == VT_HFS && nentries > 2)
			eofflag=0;
			else if (!eofflag && full_erase_flag)
				full_erase_flag = 0;

	if (open_flag)
	if (auio)

	return(error);

void 
lock_vnode_and_post(vnode_t vp, int kevent_num) 
static const char *__vtype(uint16_t vtype)
static char *__vpath(vnode_t vp, char *str, int len, int depth)
	if (len <= 0)
	if (!vp->v_name)
				       str, len, depth+1);
		*(dst-1) = '/';
extern int kdb_printf(const char *format, ...) __printflike(1,2);

void panic_print_vnodes(void)
	kdb_printf("\n***** VNODES *****\n"
		   "TYPE UREF ICNT PATH\n");
	vname[sizeof(vname)-1] = '\0';

			kdb_printf("Unable to iterate the mount list %p - encountered an invalid mount pointer %p \n",
				&mountlist, mnt);

				kdb_printf("Unable to iterate the vnode list %p - encountered an invalid vnode pointer %p \n",
					&mnt->mnt_vnodelist, vp);
				
			if (++nvnodes > SANE_VNODE_PRINT_LIMIT)
			nm = __vpath(vp, vname, sizeof(vname)-1, 0);
			kdb_printf("%s %0d %0d %s\n",
				   type, vp->v_usecount, vp->v_iocount, nm);
void panic_print_vnodes(void)
static void record_vp(vnode_t vp, int count) {
        struct uthread *ut;
	if (vp->v_resolve)
	if ((vp->v_flag & VSYSTEM))
	        return;
        ut->uu_iocount += count;
	return (((uint64_t)seq) << 32) |		
	       (((uint64_t)(aux & 0x0fffffff)) << 4) |	
	       (uint64_t)(stat & 0x0000000F);	
	return (result & 0x0000000F);
		return (EINVAL);
		return (EINVAL);
		if (stat == RESOLVER_RESOLVED)
		else
	return (0);
	
	if (tinfo->vnt_data)
	if (rp == NULL)
		return (ENOMEM);
	return (result);

	mount_t	mp;
	} 
	
	OSAddAtomic(-1, &mp->mnt_numtriggers);	
		if (status == RESOLVER_UNRESOLVED)
		return (0);
		return (0);
	 * 
		if (status == RESOLVER_RESOLVED)
	return (status == RESOLVER_ERROR ? vfs_resolver_auxiliary(result) : 0);
		return (0);
		return (0);
		if (status == RESOLVER_UNRESOLVED)
	return (status == RESOLVER_ERROR ? vfs_resolver_auxiliary(result) : 0);
		if (mp->mnt_flag & MNT_ROOTFS)
		if (vp == NULLVP)
	return (match);
	vfs_context_t	ctx;
	mount_t		top_mp;
	vnode_t		trigger_vp;
	mount_t		trigger_mp;
	uint32_t	trigger_vid;
	int		flags;
	if (mp == infop->top_mp)
		return (VFS_RETURNED_DONE);
		return (VFS_RETURNED);
	if ( !mountedtrigger || !triggerisdescendant(mp, infop->top_mp) )
		return (VFS_RETURNED);
		
				mp->mnt_vfsstat.f_mntonname);
			return (VFS_RETURNED);
				infop->trigger_mp, vp->v_mountedhere);
				vp->v_mountedhere ? vp->v_mountedhere->mnt_vfsstat.f_mntonname :
				"???", error);
			return (VFS_RETURNED_DONE); /* stop iteration on errors */
	return (VFS_RETURNED);
		if (recursive)
	
	/* 
	 * Must be called for trigger callback, wherein rwlock is held 
	/* 
		CAST_USER_ADDR_T(relpath), ctx);
	
	/* 
vm_offset_t kdebug_vnode(vnode_t vp)
SYSCTL_INT (_kern, OID_AUTO, flush_cache_on_write,
			CTLFLAG_RW | CTLFLAG_LOCKED, &flush_cache_on_write, 0,
			"always flush the drive cache on writes to uncached files");
int vnode_should_flush_after_write(vnode_t vp, int ioflag)
	return (flush_cache_on_write
			&& (ISSET(ioflag, IO_NOCACHE) || vnode_isnocache(vp)));
	long path[MAXPATHLEN / sizeof (long) + 1]; /* + 1 in case sizeof (long) does not divide MAXPATHLEN */
static int vnode_trace_path_callback(struct vnode *vp, void *arg) {
	len = sizeof (ctx->path);
		kdebug_lookup_gen_events(ctx->path, len, vp, TRUE);
static int vfs_trace_paths_callback(mount_t mp, void *arg) {
	if (mp->mnt_flag & MNT_LOCAL)
	if (!kauth_cred_issuser(kauth_cred_get()))
	if (!kdebug_enable || !kdebug_debugid_enabled(VFS_LOOKUP))
	bzero(&ctx, sizeof (struct vnode_trace_paths_context));