

#include "v0.hh"
//https://gist.github.com/alan-mushi/19546a0e2c6bd4e059fd
//https://json-c.github.io/json-c/json-c-current-release/doc/html/index.html#using
//https://aleen42.gitbooks.io/wiki/content/Programming/c/CJson/CJson.html
//https://alirumane.github.io/2019/03/17/working-with-json-data-in-c
//https://www.geeksforgeeks.org/cjson-json-file-write-read-modify-in-c/
//https://github.com/suve/copydeps/tree/release-v.5.0.1



static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


int v0_init(void)
{
    printf("\nIniciando servidor de desarrollo..\n");
    Resource favicon("favicon.ico",favicon_request,false);
    serv_devel.root.branch.insert(std::pair(favicon.name_string,favicon));
    Resource loging("loging",default_loging,false);
    Resource tdd("tdd",TDD,false);
    Resource rccheck("check",hcheck,false);
    Resource rcincrement("increment",hincrement,false);
    Resource rcap02("ap02",hap02,false);
    Resource rcap03("ap03",hap03,false);
    Resource prueba1("prueba1",default_page,true);
    Resource prueba2("prueba2",default_page,true);
    Resource prueba21("prueba21",default_page,true);
    Resource prueba22("prueba22",default_page,true);
    serv_devel.root.branch.insert(std::pair(loging.name_string,loging));
    serv_devel.root.branch.insert(std::pair(tdd.name_string,tdd));
    serv_devel.root.branch.insert(std::pair(rccheck.name_string,rccheck));
    serv_devel.root.branch.insert(std::pair(rcincrement.name_string,rcincrement));
    serv_devel.root.branch.insert(std::pair(rcap02.name_string,rcap02));
    serv_devel.root.branch.insert(std::pair(rcap03.name_string,rcap03));
    prueba2.branch.insert(std::pair(prueba21.name_string,prueba21));
    prueba2.branch.insert(std::pair(prueba22.name_string,prueba22));
    serv_devel.root.branch.insert(std::pair(prueba1.name_string,prueba1));
    serv_devel.root.branch.insert(std::pair(prueba2.name_string,prueba2));

    serv_devel.load_certificate("tests/server.pem","tests/server.key");
    serv_devel.start();

	return 0;
}

int v0_clean(void)
{
    serv_devel.stop();
    printf("\nFinalizando servidor de desarrollo..\n");
	return 0;
}

void curl_query(CURL* curl_handle,const char* url,MemoryStruct* chunk)
{
    CURLcode res;

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    //login
    curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
    curl_easy_setopt(curl_handle, CURLOPT_USERNAME, "root");
    curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, "123456");
    /*
     * If you want to connect to a site who is not using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    /*
     * If the site you are connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl refuses to connect. You can skip this
     * check, but it makes the connection insecure.
     */
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);

    /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        //return;
    }
    else
    {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
        //printf(">>%s<<",chunk.memory);
    }
}

void curl_build1(CURL* curl_handle,const char* url,MemoryStruct* chunk)
{
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    //login
    curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);
    curl_easy_setopt(curl_handle, CURLOPT_USERNAME, "root");
    curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, "123456");
    /*
     * If you want to connect to a site who is not using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    /*
     * If the site you are connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl refuses to connect. You can skip this
     * check, but it makes the connection insecure.
     */
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);

    /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

void v0_developing()
{
    CURL *curl_handle;

    struct MemoryStruct chunk;

    curl_global_init(CURL_GLOBAL_ALL);
    /* init the curl session */
    curl_handle = curl_easy_init();

    //write test code
    {
        chunk.memory = (char*)malloc(1);  /* grown as needed by the realloc above */
        chunk.size = 0;    /* no data at this point */
#ifdef OCTETOS_SERVER
        curl_query(curl_handle,"https://localhost:8081/prueba2?system=linux&application=muposys&code=100",&chunk);
#else
        curl_query(curl_handle,"http://192.168.1.102:8081:8081/prueba2?system=linux&application=muposys&code=100",&chunk);
#endif // OCTETOS_SERVER

        CU_ASSERT(curl_handle != NULL)
        //printf("Recived : %llu\n",chunk.size);
        CU_ASSERT(chunk.size == 36)

        free(chunk.memory);
    }


    //write test code
    {
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
#ifdef OCTETOS_SERVER
        curl_build1(curl_handle,"https://localhost:8081/increment?number=100",&chunk);
#else
        curl_build1(curl_handle,"https://localhost:8081/increment?number=100",&chunk);
#endif // OCTETOS_SERVER

        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            //return;
        }
        else
        {
            //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            //printf(">>%s<<",chunk.memory);
        }

        CU_ASSERT(curl_handle != NULL)
        //printf("Recived text: '%s'\n",chunk.memory);
        //printf("Recived size : '%llu'\n",chunk.size);
        CU_ASSERT(strcmp(chunk.memory,"101") == 0)

        free(chunk.memory);
    }


    //write test code
    {
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
#ifdef OCTETOS_SERVER
        curl_build1(curl_handle,"https://localhost:8081/ap03?number=100",&chunk);
#else
        curl_build1(curl_handle,"https://localhost:8081/increment?number=100",&chunk);
#endif // OCTETOS_SERVER

        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            //return;
        }
        else
        {
            //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            //printf(">>%s<<",chunk.memory);
        }

        CU_ASSERT(curl_handle != NULL)
        //printf("Recived text: '%s'\n",chunk.memory);
        //printf("Recived size : '%llu'\n",chunk.size);
        float number = std::stof(chunk.memory);
        //printf("number : %f\n",number);
        CU_ASSERT(core::equal(number,10010.0f));

        free(chunk.memory);
    }


    //write test code
    {
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
#ifdef OCTETOS_SERVER
        curl_build1(curl_handle,"https://localhost:8081/check?number=100",&chunk);
#else
        curl_build1(curl_handle,"https://localhost:8081/increment?number=100",&chunk);
#endif // OCTETOS_SERVER

        CURLcode res = curl_easy_perform(curl_handle);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            //return;
        }
        else
        {
            //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            //printf(">>%s<<",chunk.memory);
        }

        CU_ASSERT(curl_handle != NULL)
        //printf("Recived text: '%s'\n",chunk.memory);
        //printf("Recived size : '%llu'\n",chunk.size);
        CU_ASSERT(strcmp(chunk.memory,"101") == 0)

        free(chunk.memory);
    }


    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    //free(chunk.memory);
    /* we are done with libcurl, so clean it up */
    curl_global_cleanup();
}

