/* Get a web page with libcurl, extract the title with libxml.
  
   gcc -Wall -I/usr/include/libxml2 -I/usr/include/curl title.c
   -o web -lcurl -lxml2

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>

//  Case-insensitive string comparison

#ifdef _MSC_VER
#define COMPARE(a, b) (!stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

struct mem_vec {
    char *memory;
    size_t size;
};

//  libxml callback context structure

struct Context
{
    int add_title;
    char *title;
};

//  libcurl variables for error strings and returned data

static char errorBuffer[CURL_ERROR_SIZE];
struct mem_vec chunk;

//  libcurl write callback function

static size_t writer(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct mem_vec *mem = userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "realloc() returned NULL\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

//  libcurl connection initialization

static int init(CURL **conn, char *url)
{
    CURLcode code;

    *conn = curl_easy_init();

    if (*conn == NULL) {
        fprintf(stderr, "Failed to create CURL connection\n");

        exit(EXIT_FAILURE);
    }

    code = curl_easy_setopt(*conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set error buffer [%d]\n", code);

        return 0;
    }

    code = curl_easy_setopt(*conn, CURLOPT_URL, url);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);

        return 0;
    }

    code = curl_easy_setopt(*conn, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);

        return 0;
    }

    code = curl_easy_setopt(*conn, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);

        return 0;
    }

    code = curl_easy_setopt(*conn, CURLOPT_WRITEDATA, &chunk);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);

        return 0;
    }

    return 1;
}

//  libxml start element callback function

static void StartElement(void *void_context,
        const xmlChar *name,
        const xmlChar **attributes)
{
    struct Context *context = void_context;

    if (COMPARE((char *)name, "TITLE")) {
        context->title = "";
        context->add_title = 1;
    }
    (void) attributes;
}

//  libxml end element callback function

static void EndElement(void *void_context,
        const xmlChar *name)
{
    struct Context *context = void_context;

    if (COMPARE((char *)name, "TITLE"))
        context->add_title = 0;
}

//  Text handling helper function

static void handleCharacters(struct Context *context, const xmlChar *chars, 
                             int length)
{

    if (context->add_title) {
        context->title = malloc(length);
        memcpy(context->title, chars, length);
    }
}

//  libxml PCDATA callback function

static void Characters(void *void_context,
        const xmlChar *chars,
        int length)
{
   struct Context *context = void_context;

    handleCharacters(context, chars, length);
}

//  libxml CDATA callback function

static void cdata(void *void_context,
        const xmlChar *chars,
        int length)
{
   struct Context *context = void_context;

    handleCharacters(context, chars, length);
}

//  libxml SAX callback structure

static htmlSAXHandler saxHandler =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    StartElement,
    EndElement,
    NULL,
    Characters,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    cdata,
    NULL
};

//  Parse given (assumed to be) HTML text and return the title

static void parseHtml(const char *html, size_t size, char **title)
{
    htmlParserCtxtPtr ctxt;
    struct Context context;
    context.add_title = 0;

    ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",
            XML_CHAR_ENCODING_NONE);
    htmlParseChunk(ctxt, html, size, 0);
    htmlParseChunk(ctxt, "", 0, 1);

    htmlFreeParserCtxt(ctxt);

    *title = context.title;
}

int main(int argc, char *argv[])
{
    CURL *conn = NULL;
    CURLcode code;
    char *title;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // Ensure one argument is given

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Initialize CURL connection

    if (!init(&conn, argv[1])) {
        fprintf(stderr, "Connection initializion failed\n");

        exit(EXIT_FAILURE);
    }

    // Retrieve content for the URL

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to get '%s' [%s]\n", argv[1], errorBuffer);
        exit(EXIT_FAILURE);
    }

    // Parse the (assumed) HTML code
    chunk.memory[chunk.size] = '\0';
    parseHtml(chunk.memory, chunk.size, &title);

    // Display the extracted title

    printf("Title: %s\n", title);

    return EXIT_SUCCESS;
}
