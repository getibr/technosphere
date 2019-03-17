#include "numbers.dat"
#include <iostream>

int foo(int v_1, int v_2)
{
    int flag = 0;
    int flag_2 = 0;
    int ch = 0;

    for (int i = 0; i < Size; i++)
    {
        if (Data[i] == v_1)
        {
            ch = i;
            flag_2 = 1;
            break;
        }
    }
    for (int i = ch; i < Size; i++)
    {
        if ((Data[i] == v_2) && (flag_2 != 0))
            flag = 1;
    }
    return (flag * ch);
}

int	ft_is_prime(int nb)
{
	int n;

	n = 2;
	if (nb <= 1)
		return (0);
	while ((n * n <= nb) && (n < 46340))
	{
		if (nb % n == 0)
		{
			return (0);
		}
		n++;
	}
	return (1);
}


int main(int argc, char* argv[])
{
    int first_ind;
    int last_ind;
    int count;

    if ((argc % 2 == 0) || (argc == 1))
    {
        return(-1);
    }
    for (int i = 1; i < argc; i++)
    {
        int v_1 = std::atoi(argv[i]);
        i++;
        int v_2 = std::atoi(argv[i]);
        first_ind = foo(v_1, v_2);
        if ((first_ind == 0) && (v_1 != Data[0]))
        {
            printf("0 \n");
            return(0);
        }
        for (int j = first_ind; j < Size; j++)
        {
            if (Data[j] == v_2)
            {
                last_ind = j;
                break;
            }
        }
        count = 0;
        for (int j = first_ind; j <= last_ind; j++)
        {
            if (ft_is_prime(Data[j]) == 1)
                count++;
        }
        printf("%d\n", count);
    }
    return 0;
}
