#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <curl/curl.h>

static int __init graphql_module_init(void)
{
    CURL *curl;
    CURLcode res;

    const char *graphql_query = "{\"query\":\"{ user(id: \\\"1\\\") { name } }\"}";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/graphql");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, graphql_query);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            printk(KERN_INFO "GraphQL request failed: %s\n", curl_easy_strerror(res));

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}

static void __exit graphql_module_exit(void)
{
    printk(KERN_INFO "GraphQL module exited.\n");
}

module_init(graphql_module_init);
module_exit(graphql_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GraphQL Linux Kernel Module");
MODULE_AUTHOR("Mehdi");