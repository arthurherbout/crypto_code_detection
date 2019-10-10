/*

Dumps decrypted iPhone Applications to a file - better solution than those GDB scripts for non working GDB versions
(C) Copyright 2011-2014 Stefan Esser

iPod:~ root# DYLD_INSERT_LIBRARIES=dumpdecrypted.dylib /var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/Scan.app/Scan
mach-o decryption dumper

DISCLAIMER: This tool is only meant for security research purposes, not for application crackers.

[+] Found encrypted data at address 00002000 of length 1826816 bytes - type 1.
[+] Opening /private/var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/Scan.app/Scan for reading.
[+] Reading header
[+] Detecting header type
[+] Executable is a FAT image - searching for right architecture
[+] Correct arch is at offset 2408224 in the file
[+] Opening Scan.decrypted for writing.
[-] Failed opening. Most probably a sandbox issue. Trying something different.
[+] Opening /private/var/mobile/Applications/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx/tmp/Scan.decrypted for writing.
[+] Copying the not encrypted start of the file
[+] Dumping the decrypted data into the file
[+] Copying the not encrypted remainder of the file
[+] Closing original file
[+] Closing dump file

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <mach-o/dyld.h>

#define swap32(value) (((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24) )

void dumptofile(const char *path, const struct mach_header *mh) {
	struct load_command *lc;
	struct encryption_info_command *eic;
	struct fat_header *fh;
	struct fat_arch *arch;
	char buffer[1024];
	char rpath[4096],npath[4096]; /* should be big enough for PATH_MAX */
	unsigned int fileoffs = 0, off_cryptid = 0, restsize;
	int i,fd,outfd,r,n,toread;
	char *tmp;

	if (realpath(path, rpath) == NULL) {
		strlcpy(rpath, path, sizeof(rpath));
	}

	/* extract basename */
	tmp = strrchr(rpath, '/');
	printf("\n\n");
	if (tmp == NULL) {
		printf("[-] Unexpected error with filename.\n");
		_exit(1);
	} else {
		printf("[+] Dumping %s\n", tmp+1);
	}

	/* detect if this is a arm64 binary */
	if (mh->magic == MH_MAGIC_64) {
		lc = (struct load_command *)((unsigned char *)mh + sizeof(struct mach_header_64));
		printf("[+] detected 64bit ARM binary in memory.\n");
	} else { /* we might want to check for other errors here, too */
		lc = (struct load_command *)((unsigned char *)mh + sizeof(struct mach_header));
		printf("[+] detected 32bit ARM binary in memory.\n");
	}
	
	/* searching all load commands for an LC_ENCRYPTION_INFO load command */
	for (i=0; i<mh->ncmds; i++) {
		/*printf("Load Command (%d): %08x\n", i, lc->cmd);*/
		
		if (lc->cmd == LC_ENCRYPTION_INFO || lc->cmd == LC_ENCRYPTION_INFO_64) {
			eic = (struct encryption_info_command *)lc;
			
			/* If this load command is present, but data is not crypted then exit */
			if (eic->cryptid == 0) {
				break;
			}
			off_cryptid=(off_t)((void*)&eic->cryptid - (void*)mh);
			printf("[+] offset to cryptid found: @%p(from %p) = %x\n", &eic->cryptid, mh, off_cryptid);

			printf("[+] Found encrypted data at address %08x of length %u bytes - type %u.\n", eic->cryptoff, eic->cryptsize, eic->cryptid);

			printf("[+] Opening %s for reading.\n", rpath);
			fd = open(rpath, O_RDONLY);
			if (fd == -1) {
				printf("[-] Failed opening.\n");
				_exit(1);
			}
			
			printf("[+] Reading header\n");
			n = read(fd, (void *)buffer, sizeof(buffer));
			if (n != sizeof(buffer)) {
				printf("[W] Warning read only %d bytes\n", n);
			}
			
			printf("[+] Detecting header type\n");
			fh = (struct fat_header *)buffer;
			
			/* Is this a FAT file - we assume the right endianess */
			if (fh->magic == FAT_CIGAM) {
				printf("[+] Executable is a FAT image - searching for right architecture\n");
				arch = (struct fat_arch *)&fh[1];
				for (i=0; i<swap32(fh->nfat_arch); i++) {
					if ((mh->cputype == swap32(arch->cputype)) && (mh->cpusubtype == swap32(arch->cpusubtype))) {
						fileoffs = swap32(arch->offset);
						printf("[+] Correct arch is at offset %u in the file\n", fileoffs);
						break;
					}
					arch++;
				}
				if (fileoffs == 0) {
					printf("[-] Could not find correct arch in FAT image\n");
					_exit(1);
				}
			} else if (fh->magic == MH_MAGIC || fh->magic == MH_MAGIC_64) {
				printf("[+] Executable is a plain MACH-O image\n");
			} else {
				printf("[-] Executable is of unknown type\n");
				_exit(1);
			}

			strlcpy(npath, tmp+1, sizeof(npath));
			strlcat(npath, ".decrypted", sizeof(npath));
			strlcpy(buffer, npath, sizeof(buffer));

			printf("[+] Opening %s for writing.\n", npath);
			outfd = open(npath, O_RDWR|O_CREAT|O_TRUNC, 0644);
			if (outfd == -1) {
				if (strncmp("/private/var/mobile/Applications/", rpath, 33) == 0) {
					printf("[-] Failed opening. Most probably a sandbox issue. Trying something different.\n");
					
					/* create new name */
					strlcpy(npath, "/private/var/mobile/Applications/", sizeof(npath));
					tmp = strchr(rpath+33, '/');
					if (tmp == NULL) {
						printf("[-] Unexpected error with filename.\n");
						_exit(1);
					}
					tmp++;
					*tmp++ = 0;
					strlcat(npath, rpath+33, sizeof(npath));
					strlcat(npath, "tmp/", sizeof(npath));
					strlcat(npath, buffer, sizeof(npath));
					printf("[+] Opening %s for writing.\n", npath);
					outfd = open(npath, O_RDWR|O_CREAT|O_TRUNC, 0644);
				}
				if (outfd == -1) {
					perror("[-] Failed opening");
					printf("\n");
					_exit(1);
				}
			}
			
			/* calculate address of beginning of crypted data */
			n = fileoffs + eic->cryptoff;
			
			restsize = lseek(fd, 0, SEEK_END) - n - eic->cryptsize;			
			lseek(fd, 0, SEEK_SET);
			
			printf("[+] Copying the not encrypted start of the file\n");
			/* first copy all the data before the encrypted data */
			while (n > 0) {
				toread = (n > sizeof(buffer)) ? sizeof(buffer) : n;
				r = read(fd, buffer, toread);
				if (r != toread) {
					printf("[-] Error reading file\n");
					_exit(1);
				}
				n -= r;
				
				r = write(outfd, buffer, toread);
				if (r != toread) {
					printf("[-] Error writing file\n");
					_exit(1);
				}
			}
			
			/* now write the previously encrypted data */
			printf("[+] Dumping the decrypted data into the file\n");
			r = write(outfd, (unsigned char *)mh + eic->cryptoff, eic->cryptsize);
			if (r != eic->cryptsize) {
				printf("[-] Error writing file\n");
				_exit(1);
			}
			
			/* and finish with the remainder of the file */
			n = restsize;
			lseek(fd, eic->cryptsize, SEEK_CUR);
			printf("[+] Copying the not encrypted remainder of the file\n");
			while (n > 0) {
				toread = (n > sizeof(buffer)) ? sizeof(buffer) : n;
				r = read(fd, buffer, toread);
				if (r != toread) {
					printf("[-] Error reading file\n");
					_exit(1);
				}
				n -= r;
				
				r = write(outfd, buffer, toread);
				if (r != toread) {
					printf("[-] Error writing file\n");
					_exit(1);
				}
			}

			if (off_cryptid) {
				uint32_t zero=0;
				off_cryptid+=fileoffs;
				printf("[+] Setting the LC_ENCRYPTION_INFO->cryptid to 0 at offset %x\n", off_cryptid);
				if (lseek(outfd, off_cryptid, SEEK_SET) != off_cryptid || write(outfd, &zero, 4) != 4) {
					printf("[-] Error writing cryptid value\n");
				}
			}

			printf("[+] Closing original file\n");
			close(fd);
			printf("[+] Closing dump file\n");
			close(outfd);
		}
		
		lc = (struct load_command *)((unsigned char *)lc+lc->cmdsize);		
	}
	printf("[-] This mach-o file is not encrypted. Nothing was decrypted.\n");
}

static void image_added(const struct mach_header *mh, intptr_t slide) {
	Dl_info image_info;
	int result = dladdr(mh, &image_info);
	dumptofile(image_info.dli_fname, mh);
}

__attribute__((constructor))
static void dumpexecutable() {
	printf("mach-o decryption dumper\n\n");
	printf("DISCLAIMER: This tool is only meant for security research purposes, not for application crackers.");
	_dyld_register_func_for_add_image(&image_added);
}
