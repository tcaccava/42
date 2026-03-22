#include <unistd.h>

int is_sum_target(int *tmp, int tmp_index, int target)
{
    int sum = 0;
    int i = 0;
    while (i < tmp_index) // gli array di int,a differenza di quelli di char,non sono terminati da null terminator,quindi hai sempre bisogno di una lunghezza o indice per scorrerli fino alla fine
    {
        sum += tmp[i];
        i++;
    }
    return (sum == target);
}
void print(int *tmp, int tmp_index)
{
    int i = 0;
    while (i < tmp_index)
    {
        char c = tmp[i] + '0';
        write(1, &c, 1);
        i++;
    }
    write(1, "\n", 1);
}

void target_sum(int *array, int size, int index, int *tmp, int tmp_index, int target)
{
    if (index == size)
    {
        if (is_sum_target(tmp, tmp_index, target))
            print(tmp, tmp_index);
        return;
    }
    target_sum(array, size, index + 1, tmp, tmp_index, target);
    tmp[tmp_index] = array[index];
    target_sum(array, size, index + 1, tmp, tmp_index + 1, target);
}

int main()
{
    int array[7] = {1, 2, 4, 5, 6, 7, 8};
    int tmp[7];
    target_sum(array, 7, 0, tmp, 0, 10);
}