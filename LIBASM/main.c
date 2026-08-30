#include "libasm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

// Utility per stampare l'esito dei test
void print_result(const char *test_name, int condition) {
    if (condition) {
        printf("  %s[OK]%s %s\n", GREEN, RESET, test_name);
    } else {
        printf("  %s[KO]%s %s\n", RED, RESET, test_name);
    }
}

// Funzione di comparazione per ft_list_sort e ft_list_remove_if
int cmp_string(void *a, void *b) {
    return strcmp((char *)a, (char *)b);
}

// Funzione di free personalizzata per ft_list_remove_if
void free_content(void *content) {
    // Se il contenuto fosse allocato dinamicamente, andrebbe fatto free(content).
    // In questo test usiamo stringhe letterali o allocate, gestiamo coerentemente.
    (void)content;
}

// ==========================================
// TEST MANDATORY
// ==========================================
void test_ft_strlen(void) {
    printf("%s--- Testing ft_strlen ---%s\n", CYAN, RESET);
    
    char *s1 = "Hello, World!";
    char *s2 = "";
    char *s3 = "A";
    char *s4 = "0123456789abcdefghijklmnopqrstuvwxyz";

    print_result("Standard string", ft_strlen(s1) == strlen(s1));
    print_result("Empty string", ft_strlen(s2) == strlen(s2));
    print_result("Single character", ft_strlen(s3) == strlen(s3));
    print_result("Long alphanumeric string", ft_strlen(s4) == strlen(s4));
}

void test_ft_strcpy(void) {
    printf("%s--- Testing ft_strcpy ---%s\n", CYAN, RESET);

    char dest1[50];
    char dest2[50];
    char *src1 = "Test string!";
    char *src2 = "";

    print_result("Standard copy return value", ft_strcpy(dest1, src1) == dest1);
    print_result("Standard copy content", strcmp(dest1, src1) == 0);

    print_result("Empty string copy return value", ft_strcpy(dest2, src2) == dest2);
    print_result("Empty string copy content", strcmp(dest2, src2) == 0);
}

void test_ft_strcmp(void) {
    printf("%s--- Testing ft_strcmp ---%s\n", CYAN, RESET);

    char *s1 = "Hello";
    char *s2 = "Hello";
    char *s3 = "Hell";
    char *s4 = "Hello World";
    char *empty = "";

    print_result("Identical strings", (ft_strcmp(s1, s2) == 0) && (strcmp(s1, s2) == 0));
    print_result("S1 shorter than S2", (ft_strcmp(s3, s1) < 0) && (strcmp(s3, s1) < 0));
    print_result("S1 longer than S3", (ft_strcmp(s1, s3) > 0) && (strcmp(s1, s3) > 0));
    print_result("Prefix vs Full string", (ft_strcmp(s1, s4) < 0) && (strcmp(s1, s4) < 0));
    print_result("Empty string vs non-empty", (ft_strcmp(empty, s1) < 0) && (strcmp(empty, s1) < 0));
    print_result("Empty strings", (ft_strcmp(empty, empty) == 0) && (strcmp(empty, empty) == 0));
}

void test_ft_write(void) {
    printf("%s--- Testing ft_write ---%s\n", CYAN, RESET);

    int fd = open("test_write.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char *msg = "Writing test via ft_write.\n";
    
    ssize_t ret_ft = ft_write(fd, msg, strlen(msg));
    close(fd);

    // Leggiamo il file creato per confermare
    fd = open("test_write.txt", O_RDONLY);
    char buf[100] = {0};
    read(fd, buf, sizeof(buf));
    close(fd);
    unlink("test_write.txt");

    print_result("ft_write return value (bytes written)", ret_ft == (ssize_t)strlen(msg));
    print_result("ft_write file content check", strcmp(buf, msg) == 0);

    // Edge case: File Descriptor non valido (deve settare errno e ritornare -1)
    errno = 0;
    ssize_t err_ft = ft_write(-99, "fail", 4);
    int err_code_ft = errno;

    errno = 0;
    ssize_t err_libc = write(-99, "fail", 4);
    int err_code_libc = errno;

    print_result("ft_write invalid FD return (-1)", err_ft == -1);
    print_result("ft_write invalid FD errno match", err_code_ft == err_code_libc);
}

void test_ft_read(void) {
    printf("%s--- Testing ft_read ---%s\n", CYAN, RESET);

    int fd = open("test_read.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Reading test content.", 21);
    close(fd);

    fd = open("test_read.txt", O_RDONLY);
    char buf1[50] = {0};
    char buf2[50] = {0};

    ssize_t r1 = ft_read(fd, buf1, 21);
    close(fd);

    fd = open("test_read.txt", O_RDONLY);
    ssize_t r2 = read(fd, buf2, 21);
    close(fd);
    unlink("test_read.txt");

    print_result("ft_read return value check", r1 == r2);
    print_result("ft_read buffer content check", strcmp(buf1, buf2) == 0);

    // Edge case: File Descriptor non valido
    errno = 0;
    ssize_t err_ft = ft_read(-99, buf1, 10);
    int err_code_ft = errno;

    errno = 0;
    ssize_t err_libc = read(-99, buf2, 10);
    int err_code_libc = errno;

    print_result("ft_read invalid FD return (-1)", err_ft == -1);
    print_result("ft_read invalid FD errno match", err_code_ft == err_code_libc);
}

void test_ft_strdup(void) {
    printf("%s--- Testing ft_strdup ---%s\n", CYAN, RESET);

    char *original = "Duplication test string with 42 Roma.";
    char *dup_ft = ft_strdup(original);
    char *dup_libc = strdup(original);

    print_result("ft_strdup content match", strcmp(dup_ft, dup_libc) == 0);
    print_result("ft_strdup allocation check (not same pointer)", dup_ft != original);

    free(dup_ft);
    free(dup_libc);

    // Edge case: stringa vuota
    char *empty_orig = "";
    char *empty_ft = ft_strdup(empty_orig);
    char *empty_libc = strdup(empty_orig);
    print_result("ft_strdup empty string match", strcmp(empty_ft, empty_libc) == 0);
    free(empty_ft);
    free(empty_libc);
}

// ==========================================
// TEST BONUS
// ==========================================
void test_ft_atoi_base(void) {
    printf("%s--- Testing ft_atoi_base (Bonus) ---%s\n", CYAN, RESET);

    // Decimali
    print_result("Decimal base '42'", ft_atoi_base("42", "0123456789") == 42);
    print_result("Decimal base with spaces and sign '-42'", ft_atoi_base("   -42", "0123456789") == -42);
    print_result("Decimal base with plus sign '+1337'", ft_atoi_base("+1337", "0123456789") == 1337);

    // Binari
    print_result("Binary base '1010' -> 10", ft_atoi_base("1010", "01") == 10);
    print_result("Binary base '-1111' -> -15", ft_atoi_base("-1111", "01") == -15);

    // Esadecimali
    print_result("Hex base '2a' -> 42", ft_atoi_base("2a", "0123456789abcdef") == 42);
    print_result("Hex base uppercase '2A' -> 42", ft_atoi_base("2A", "0123456789ABCDEF") == 42);

    // Edge cases di basi non valide (devono ritornare 0)
    print_result("Invalid base: empty", ft_atoi_base("42", "") == 0);
    print_result("Invalid base: single char", ft_atoi_base("42", "0") == 0);
    print_result("Invalid base: contains duplicate", ft_atoi_base("42", "01234567890") == 0);
    print_result("Invalid base: contains '+'", ft_atoi_base("42", "0123+56789") == 0);
    print_result("Invalid base: contains '-'", ft_atoi_base("42", "0123-56789") == 0);
    print_result("Invalid base: contains space", ft_atoi_base("42", "0123 56789") == 0);
}

void test_ft_list(void) {
    printf("%s--- Testing List Functions (Bonus) ---%s\n", CYAN, RESET);

    t_list *head = NULL;

    // 1. ft_list_size su lista vuota
    print_result("ft_list_size on empty list", ft_list_size(head) == 0);

    // 2. ft_list_push_front e ft_list_size
    ft_list_push_front(&head, "Charlie");
    ft_list_push_front(&head, "Bravo");
    ft_list_push_front(&head, "Alpha");
    
    print_result("ft_list_size after 3 pushes", ft_list_size(head) == 3);

    // 3. ft_list_sort
    ft_list_sort(&head, cmp_string);
    // Ora la lista dovrebbe essere: Alpha -> Bravo -> Charlie
    print_result("ft_list_sort order check (head is Alpha)", strcmp((char *)head->data, "Alpha") == 0);
    print_result("ft_list_sort order check (middle is Bravo)", strcmp((char *)head->next->data, "Bravo") == 0);
    print_result("ft_list_sort order check (tail is Charlie)", strcmp((char *)head->next->next->data, "Charlie") == 0);

    // 4. ft_list_remove_if
    // Rimuoviamo un elemento in mezzo ("Bravo")
    ft_list_remove_if(&head, "Bravo", cmp_string, free_content);
    print_result("ft_list_size after remove element", ft_list_size(head) == 2);
    print_result("ft_list_remove_if integrity check (head is still Alpha)", strcmp((char *)head->data, "Alpha") == 0);
    print_result("ft_list_remove_if integrity check (next is now Charlie)", strcmp((char *)head->next->data, "Charlie") == 0);

    // Pulizia finale della memoria della lista
    t_list *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(void) {
    printf("==========================================\n");
    printf("       LIBASM COMPREHENSIVE TEST SUITE    \n");
    printf("==========================================\n\n");

    // Mandatories
    test_ft_strlen();
    test_ft_strcpy();
    test_ft_strcmp();
    test_ft_write();
    test_ft_read();
    test_ft_strdup();

    printf("\n");

    // Bonus
    test_ft_atoi_base();
    test_ft_list();

    printf("\n==========================================\n");
    printf("             TESTS COMPLETED              \n");
    printf("==========================================\n");

    return 0;
}