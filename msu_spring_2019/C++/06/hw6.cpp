#include <iostream>
#include <string>
#include <fstream>
#include <thread>

using file_ptr = std::unique_ptr<FILE, int(*)(FILE*)>;

std::string input = "input.bin";
std::string output = "output.bin";

uint64_t file_size(const std::string &filename)
{
    file_ptr f(fopen(filename.c_str(), "rb"), &fclose);
    fseek(f.get(),0,SEEK_END);
    int size = ftell(f.get());
    return size;
}

void mergefile(const std::string &sort_file1, const std::string &sort_file2, const std::string &sorted_file)
{
    file_ptr out(fopen(sorted_file.c_str(), "wb"), &fclose);
    file_ptr sort1(fopen(sort_file1.c_str(), "rb"), &fclose);
    file_ptr sort2(fopen(sort_file2.c_str(), "rb"), &fclose);

    uint64_t sort1_size = file_size(sort_file1) / sizeof(uint64_t);
    uint64_t sort2_size = file_size(sort_file2) / sizeof(uint64_t);
    uint64_t temp1, temp2;

    fread(&temp1, sizeof(temp1), 1, sort1.get());
    fread(&temp2, sizeof(temp2), 1, sort2.get());
    while(sort1_size != 0 || sort2_size != 0)
    {
        if (sort1_size == 0)
        {
            while(sort2_size != 0)
            {
                sort2_size--;
                fwrite(&temp2, sizeof(temp2), 1, out.get());
                if(sort2_size > 0)
                    fread(&temp2, sizeof(temp2), 1, sort2.get());
            }
        }
        else if (sort2_size == 0)
        {
            while(sort1_size != 0)
            {
                sort2_size--;
                fwrite(&temp1, sizeof(temp1), 1, out.get());
                if(sort1_size > 0)
                    fread(&temp1, sizeof(temp1), 1, sort1.get());
            }
        }
        else if(temp1 <= temp2)
        {
            sort1_size--;
            fwrite(&temp1, sizeof(temp1), 1, out.get());
            if(sort1_size > 0)
                fread(&temp1, sizeof(temp1), 1, sort1.get());
        }
        else
        {
            sort2_size--;
            fwrite(&temp2, sizeof(temp2), 1, out.get());
            if(sort2_size > 0)
                fread(&temp2, sizeof(temp2), 1, sort2.get());
        }
    }
}

std::string mergesort_file(uint64_t left, uint64_t right, uint step)
{
    uint64_t middle = (left + right) / 2;
    std::string sort_file1;
    std::string sort_file2;
    if (left == right)
    {
        std::string sorted_file = std::string(std::to_string(left) + '-' + std::to_string(right) + '-' + std::to_string(step));
        file_ptr in(fopen(input.c_str(), "rb"), &fclose);
        file_ptr out(fopen(sorted_file.c_str(), "rb"), &fclose);

        fseek(in.get(), sizeof(uint64_t) * left, SEEK_SET);
        uint64_t temp;
        fread(&temp, sizeof(temp), 1, in.get());
        fwrite(&temp, sizeof(temp), 1, out.get());
        fclose(in.get());
        fclose(out.get());
        return sorted_file;
    }
    sort_file1 = mergesort_file(left, middle, step + 1);
    sort_file2 = mergesort_file(middle + 1, right, step + 1);
    std::string sorted_file = std::string(std::to_string(left) + '-' + std::to_string(right) + '-' + std::to_string(step));
    mergefile(sort_file1, sort_file2, sorted_file);
    return sorted_file;
}

int main(void)
{
    uint64_t count_num = file_size(input) / sizeof(uint64_t);
    std::thread file_left(mergesort_file, 0, 0, (count_num - 1) / 2);
    std::thread file_right(mergesort_file, 0, (count_num - 1) / 2 + 1, count_num - 1);
    file_left.join();
    file_right.join();
    std::string str1 = std::string(std::to_string(0) + '-' + std::to_string((count_num - 1) / 2) + '-' + std::to_string(0));
    std::string str2 = std::string(std::to_string((count_num - 1) / 2 + 1) + '-' + std::to_string(count_num - 1) + '-' + std::to_string(0));
    mergefile(str1, str2, output);
    return 0;
}