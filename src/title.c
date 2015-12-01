/* get_title(): Get a web page with libcurl, extract the title with libxml. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include "title.h"

/* libcurl writer() needs vector type */
struct mem_vec {
    char *memory;
    size_t size;
};

/*  libxml callback context structure */
struct context {
    int add_title;
    char *title;
    size_t len;
};

/*  libcurl write callback function */
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

/*  libcurl connection initialization */
static int init(CURL **conn, const char *url, struct mem_vec *chunk, 
                                                        char *curl_err_buff)
{
    CURLcode code;

    *conn = curl_easy_init();

    if (*conn == NULL) {
        fprintf(stderr, "Failed to create CURL connection\n");
        exit(EXIT_FAILURE);
    }
    code = curl_easy_setopt(*conn, CURLOPT_ERRORBUFFER, curl_err_buff);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set error buffer [%d]\n", code);
        return 0;
    }
    code = curl_easy_setopt(*conn, CURLOPT_URL, url);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set URL [%s]\n", curl_err_buff);
        return 0;
    }
    code = curl_easy_setopt(*conn, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set redirect option [%s]\n", curl_err_buff);
        return 0;
    }
    code = curl_easy_setopt(*conn, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set writer [%s]\n", curl_err_buff);
        return 0;
    }
    code = curl_easy_setopt(*conn, CURLOPT_WRITEDATA, chunk);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set write data [%s]\n", curl_err_buff);
        return 0;
    }
    code = curl_easy_setopt(*conn, CURLOPT_SSL_VERIFYPEER, 0);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set write data [%s]\n", curl_err_buff);
        return 0;
    }
    return 1;
}

/*  libxml start element callback function */
static void start_elem(void *void_ctxt, const xmlChar *name, 
                                        const xmlChar **attributes)
{
    struct context *ctxt = void_ctxt;
    if (!strcasecmp((char *)name, "TITLE"))
        ctxt->add_title = 1;
}

/*  libxml end element callback function */
static void end_elem(void *void_ctxt, const xmlChar *name)
{
    struct context *ctxt = void_ctxt;

    if (!strcasecmp((char *)name, "TITLE")) {
        ctxt->add_title = 0;
        ctxt->title[ctxt->len] = '\0';
    }
}

/*  Text handling helper function */
static void handle_chars(struct context *ctxt, const xmlChar *chars, int length)
{
    if (ctxt->add_title) {
        ctxt->title = realloc(ctxt->title, ctxt->len + length);
        memcpy(ctxt->title + ctxt->len, chars, length);
        ctxt->len += length;
    }
}

/*  libxml PCDATA callback function */
static void chars(void *void_ctxt, const xmlChar *chars, int length)
{
   struct context *ctxt = void_ctxt;

    handle_chars(ctxt, chars, length);
}

/*  libxml CDATA callback function */
static void cdata(void *void_ctxt, const xmlChar *chars, int length)
{
   struct context *ctxt = void_ctxt;

    handle_chars(ctxt, chars, length);
}

/*  libxml SAX callback structure */
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
    start_elem,
    end_elem,
    NULL,
    chars,
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

/*  Parse given (assumed to be) HTML text and return the title */
static void parseHtml(const char *html, size_t size, char **title)
{
    htmlParserCtxtPtr ctxt;
    struct context context;
    
    context.add_title = 0;
    context.len = 0;
    context.title = NULL;
    ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",
                                                        XML_CHAR_ENCODING_UTF8);
    htmlParseChunk(ctxt, html, size, 0);
    htmlParseChunk(ctxt, "", 0, 1);
    htmlFreeParserCtxt(ctxt);
    *title = context.title;
}

char *get_title(const char *url)
{
    char curl_err_buff[CURL_ERROR_SIZE];
    struct mem_vec chunk;

    CURL *conn = NULL;
    CURLcode code;
    char *title;
    char *title_prefixed;
    char *prefix = "Title: "; /* strlen(prefix) == 7 */
    
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Initialize CURL connection */

    if (!init(&conn, url, &chunk, (char *) &curl_err_buff)) {
        fprintf(stderr, "Connection initializion failed\n");
        return NULL;
    }

    /* Retrieve content for the URL */

    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to get '%s' [%s]\n", url, curl_err_buff);
        return NULL;
    }

    /* Parse the (assumed) HTML code */
    chunk.memory[chunk.size] = '\0';
    parseHtml(chunk.memory, chunk.size, &title);
    free(chunk.memory);
    if (!title)
        return NULL;
    title_prefixed = malloc(strlen(prefix) + strlen(title) + 1);
    strcpy(title_prefixed, prefix);
    strcat(title_prefixed, title);
    free(title);
    return title_prefixed;
}
