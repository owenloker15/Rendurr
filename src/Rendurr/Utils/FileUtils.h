#pragma once

#include <Rendurr/Core/Log.h>
#include <stdio.h>

namespace rnd
{
    namespace Utils
    {
        inline const char* read_file(const char* path)
        {
            FILE* file = fopen(path, "rb");
            if (!file)
                return NULL;

            fseek(file, 0, SEEK_END);
            size_t size = ftell(file);
            fseek(file, 0, SEEK_SET);

            char* data = (char*)malloc(size + 1);
            if (!data) {
                fclose(file);
                return NULL;
            }

            fread(data, 1, size, file);
            fclose(file);

            data[size] = '\0';

            return data;
        }
    } // namespace Utils
} // namespace rnd
