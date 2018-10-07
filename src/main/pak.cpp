/*
   Feet of Fury

   pak.cc

   Copyright (C)2002 Cryptic Allusion, LLC
*/

#include "global.h"
#include "otp.h"
#include "pak.h"

#if 0
#include <zlib/zlib.h>
#include <kos/fs_ramdisk.h>
#endif

// Some routines to handle "pak" files (romdisk images)
//
// This code is generally NOT thread safe.
//

// Is "Swap CD" mode enabled? This will allow the user to swap out
// the main FoF CD to put a new one in with their own songs and step
// files. The big technical issue here is that we need to pre-load
// several pak files and keep them cached until Swap CD mode is
// disabled again.
bool pak_swap_enabled = false;

#if 0
// These load functions _only_ load the PAK file into the cache. In theory
// this will be the slowest part of the whole operation. What we'll do is
// load them up and decrypt them, but keep them compressed in the cache.
// When a new section of the game needs its data files, we'll uncompress
// it and mount it as a romdisk temporarily.
static int pakLoadInternal(const char * pakname, const char * prefix, bool decrypt) {
	// Figure out the pak name from the pak name step
	char pfn[64];
	if (pakname[0] != '/')
		sprintf(pfn, "pakfiles/%s.pak", pakname);
	else	
		strcpy(pfn, pakname);
	
	printf("pak_mount: loading pak file %s into the cache\n", pfn);

	uint64 ts, te;

	ts = timer_ms_gettime64();

	uint8 * buf;
	int buflen = -1;

	// Get the "original size" header from before the gzip'd data
	file_t fd = fs_open(pfn, O_RDONLY);
	if (fd == FILEHND_INVALID) {
		printf("pak_load: unable to open pak file %s\n", pfn);
		printf("  cwd is '%s'\n", fs_getwd());
		return -1;
	}

	uint32 origsize;
	char magic[5] = {0};
	fs_read(fd, magic, 4);
	buf = (uint8 *)&magic;
	for (int i=0; i<4; i++)
		buf[i] = otp[buf[i]];
	if (strcmp(magic, "CAPK")) {
		fs_close(fd);
		printf("pak_load: invalid signature for %s\n", pfn);
		return -1;
	}

	fs_read(fd, &origsize, 4);
	buf = (uint8 *)&origsize;
	for (int i=0; i<4; i++)
		buf[i] = otp[buf[i]];
	printf("origsize is %lu\n", origsize);
	if (origsize > 10000000) {
		fs_close(fd);
		return -1;
	}

	buflen = fs_total(fd) - 8;
	buf = (uint8 *)malloc(buflen);
	fs_read(fd, buf, buflen);
	fs_close(fd);

	te = timer_ms_gettime64();
	printf("load time = %d msec\n", (int)(te - ts));

	// Decrypt
	ts = timer_ms_gettime64();
	if (decrypt) {
		for (int i=0; i<buflen; i++) {
			buf[i] = otp[buf[i]];
		}
	}
	te = timer_ms_gettime64();
	printf("decrypt time = %d msec\n", (int)(te - ts));

	// Also write out a little metafile with the original size
	sprintf(pfn, "/ram/%s.size", prefix);
	fd = fs_open(pfn, O_WRONLY | O_TRUNC);
	fs_write(fd, &origsize, 4);
	fs_close(fd);

	// Attach it to the ramdisk so we can use it with zlib
	printf("attaching to ramdisk:\n");
	sprintf(pfn, "/%s.pak", prefix);
	if (fs_ramdisk_attach(pfn, buf, buflen) < 0) {
		printf("pak_mount: unable to attach pak file to ramdisk at %s\n", pfn);
		free(buf);
		return -1;
	}

	return 0;
}
#endif

int pakLoad(const char * pakname) {
//	return pakLoadInternal(pakname, pakname, true);
	return -1;
}

int pakLoadUnencr(const char * pakname, const char * prefix) {
//	return pakLoadInternal(pakname, prefix, false);
	return -1;
}

int pakUnload(const char * pakname) {
#if 0
	printf("pakUnload(%s)\n", pakname);

	char pfn[64];
	sprintf(pfn, "/ram/%s.pak", pakname);
	fs_unlink(pfn);
	sprintf(pfn, "/ram/%s.size", pakname);
	fs_unlink(pfn);
#endif

	return 0;
}

int pakMount(const char * pakname) {
#if 0
	// Figure out the pak name from the pak name step
	printf("pak_mount: mounting pak file %s\n", pakname);

	uint64 ts, te;

	// Open the size metafile
	char pfn[64];
	sprintf(pfn, "/ram/%s.size", pakname);
	file_t fd = fs_open(pfn, O_RDONLY);
	if (fd == FILEHND_INVALID) {
		printf("pak_mount: unable to load size metafile %s\n", pfn);
		return -1;
	}

	uint32 origsize;
	fs_read(fd, &origsize, 4);
	fs_close(fd);

	if (origsize > 10000000) {
		printf("origsize is %lu\n", origsize);
		return -1;
	}

	// Open it up with zlib
	ts = timer_ms_gettime64();
	printf("opening for decompress:\n");
	sprintf(pfn, "/ram/%s.pak", pakname);
	gzFile gzf = gzopen(pfn, "rb");
	if (gzf == Z_NULL) {
		printf("pak_mount: unable to open pak file %s for decompress\n", pfn);
		return -1;
	}

	// Allocate the output space for it
	printf("alloc'ing space:\n");
	uint8 * buf = (uint8 *)malloc(origsize);

	printf("decompressing:\n");
	int rb, total = 0;
	for ( ; ; ) {
		rb = gzread(gzf, buf + total, 4096);
		total += rb;
		if (rb < 4096)
			break;
	}
	if ((uint32)total < origsize) {
		printf("pak_mount: WARNING: short read on %s\n", pfn);
	}
	gzclose(gzf);
	te = timer_ms_gettime64();
	printf("decompress time = %d msec\n", (int)(te - ts));

	// Mount it
	printf("mount:\n");
	sprintf(pfn, "%s/%s", fs_getwd(), pakname);
	if (fs_romdisk_mount(pfn, buf, 1) < 0) {
		printf("pak_mount: unable to mount pak file on %s\n", pfn);
		return -1;
	}
#endif

	return 0;
}

int pakUnmount(const char * pakname) {
#if 0
	char pfn[64];
	sprintf(pfn, "%s/%s", fs_getwd(), pakname);
	
	// Unmount it
	if (fs_romdisk_unmount(pfn) < 0) {
		printf("pak_mount: unable to unmount pak file from %s\n", pfn);
		return -1;
	}

	printf("pak_mount: unmounted pak file from %s\n", pfn);
#endif
	return 0;
}

int pakLMOldSkool(const char * pakname, const char * prefix) {
#if 0
	char pfn[64];
	if (pakname[0] != '/')
		sprintf(pfn, "pakfiles/%s.pak", pakname);
	else	
		strcpy(pfn, pakname);

	printf("pak_mount: mounting old-skool pak file %s\n", pfn);

	uint8 * buf;
	int buflen = -1;
	buflen = fs_load(pfn, (void **)&buf);
	if (buflen < 0) {
		printf("pak_mount: unable to load pak file %s\n", pfn);
		printf("  cwd is '%s'\n", fs_getwd());
		return -1;
	}

	// Mount it
	sprintf(pfn, "%s/%s", fs_getwd(), prefix);
	if (fs_romdisk_mount(pfn, buf, 1) < 0) {
		printf("pak_mount: unable to mount pak file on %s\n", pfn);
		free(buf);
		return -1;
	}
#endif

	return 0;
}

void pakSwapMode(bool enabled) {
}

int pakCacheEncrypted(const char * inpfn, const char * outpfn) {
#if 0
	printf("pakCacheEncrypted: loading '%s' to '%s'\n", inpfn, outpfn);

	// Make sure any previous file was deleted
	char tmpname[10 + strlen(outpfn)];
	sprintf(tmpname, "/ram/%s", outpfn);
	fs_unlink(tmpname);

	// Load the file
	uint8 * buf;
	int buflen;
	buflen = fs_load(inpfn, (void **)&buf);
	if (buflen < 0) {
		printf("pakCacheEncrypted: can't load file '%s'\n", inpfn);
		return -1;
	}

	// Decrypt it
	for (int i=0; i<buflen; i++)
		buf[i] = otp[buf[i]];

	// Attach it to the ramdisk
	if (fs_ramdisk_attach(outpfn, buf, buflen) < 0) {
		printf("pakCacheEncrypted: can't attach to file '%s'\n", outpfn);
		free(buf);
		return -1;
	}
#endif

	// Should be all good
	return 0;
}

//////////////////////////////////////////////////////////////////////////

#if 0

static void * pakfs_open(vfs_handler_t * vfs, const char *fn, int mode) {
	if ((mode & O_MODE_MASK) != O_RDONLY)
		return NULL;

	// Try to open it through the normal VFS
	file_t f = fs_open(fn, mode);
	if (f == FILEHND_INVALID)
		return NULL;

	// Pass it back through
	return (void *)f;
}

static void pakfs_close(void * fd) {
	fs_close((file_t)fd);
}

static ssize_t pakfs_read(void * fd, void *buf, size_t bytes) {
	ssize_t r;

	r = fs_read((file_t)fd, buf, bytes);
	if (r >= 0) {
		// We got some data, decrypt it
		for (uint32 i=0; i<r; i++)
			((uint8 *)buf)[i] = otp[((uint8 *)buf)[i]];
	}

	return r;
}

static off_t pakfs_seek(void * fd, off_t offset, int whence) {
	return fs_seek((file_t)fd, offset, whence);
}

static off_t pakfs_tell(void * fd) {
	return fs_tell((file_t)fd);
}

static size_t pakfs_total(void * fd) {
	return fs_total((file_t)fd);
}

static dirent_t * pakfs_readdir(void * fd) {
	return fs_readdir((file_t)fd);
}

static vfs_handler_t vh = {
	/* Name handler */
	{
		"/pd",	/* name */
		0,	/* tbfi */
		0x00010000,	/* Version 1.0 */
		0,	/* flags */
		NMMGR_TYPE_VFS,
		NMMGR_LIST_INIT
	},

	0, NULL,

	pakfs_open,
	pakfs_close,
	pakfs_read,
	NULL,		// write
	pakfs_seek,
	pakfs_tell,
	pakfs_total,
	pakfs_readdir,	// readdir
	NULL,		// ioctl
	NULL,		// rename
	NULL,		// unlink
	NULL,		// mmap
	NULL,		// complete
	NULL,		// stat
	NULL,		// mkdir
	NULL		// rmdir
};
#endif

bool pakInit() {
	// Register our "VFS" for opening encrypted files on the fly
	/* if (nmmgr_handler_add(&vh.nmmgr) < 0)
		return false; */

	return true;
}

bool pakShutdown() {
//	nmmgr_handler_remove(&vh.nmmgr);

	return true;
}
