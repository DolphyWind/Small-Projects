#pragma once
#include <iostream>
#include <cstdio>
#include <cstring>
#include <curl/curl.h>

class Request
{
    CURL *curl;
    CURLcode res;

    typedef struct
    {
        char *memory;
        std::size_t size;
    } MemoryStruct;

    MemoryStruct chunk;
    
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
public:
    Request();
    ~Request();
    std::string get(std::string url);
};