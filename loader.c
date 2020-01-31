#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

extern char **environ;

struct exec {
	uintptr_t a_midmag;
	uintptr_t a_text;
	uintptr_t a_data;
	uintptr_t a_bss;
	uintptr_t a_syms;
	uintptr_t a_entry;
};

int main(int argc, char **argv)
{
	int fd = open(argv[1], O_RDONLY|O_CLOEXEC);
	struct exec a;
	read(fd, &a, sizeof a);
	uintptr_t maplen = a.a_text + a.a_data + a.a_bss;
	void *p = mmap((void *)4096, maplen, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
	mmap((void *)4096, a.a_text, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED, fd, 0);
	mmap((void *)(4096+a.a_text), a.a_data, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED, fd, a.a_text);
	mmap((void *)(4096+a.a_text+a.a_data), a.a_bss, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANON, -1, 0);
	close(fd);
	__asm__ __volatile__(
		"mov %0,%%esp ; push %3 ; push %0 ; push %2 ; jmp *%1"
		: : "r"(argv+2), "r"(a.a_entry), "r"(argc-2), "r"(environ));
}
