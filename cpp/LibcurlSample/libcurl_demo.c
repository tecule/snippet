/**
 * use libcurl to download data into local memory
 * see http://curl.haxx.se/libcurl/c/getinmemory.html
 */

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct response {
	size_t size;
	char* data;
};

/**
 * callback function when receiving data
 * this function maybe called multiple times in one visit, so keep track of the data received
 */
size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
	struct response *resp = (struct response *)userdata;
	size_t data_size = size * nmemb;

	resp->data = realloc(resp->data, resp->size + data_size + 1);	/* +1 for '\0' */
	if (resp->data == NULL) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	 memcpy(&(resp->data[resp->size]), ptr, data_size);
	 resp->size += data_size;
	 resp->data[resp->size] = '\0';

	 return data_size;
}

char *visit_url(char* url) {
	CURL *curl_handle;
	CURLcode res;

	struct response resp;
	resp.size = 0;
	resp.data = malloc(1);
	if (NULL == resp.data) {
		fprintf(stderr, "Failed to allocate memory.\n");
		return NULL;
	}
	resp.data[0] = '\0';

	/* init the curl session */
	curl_handle = curl_easy_init();
	if (curl_handle) {
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		/* send all data to this function  */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
		/* pass response struct to the callback function */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &resp);

		/* get it! */
		res = curl_easy_perform(curl_handle);

		/* check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		}

		 /* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
	}

	return resp.data;
}

int main(int argc, char* argv[]) {
	char* response_data;

	response_data = visit_url("http://www.thomas-bayer.com/sqlrest/CUSTOMER/18/");

	if (response_data) {
		printf("%s\n", response_data);
		free(response_data);
	}

	return 0;
}
