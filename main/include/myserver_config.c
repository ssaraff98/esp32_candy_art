#include "myserver_config.h"

char pixelinfo_query[1024];
int pixelinfo_flag = 0;

void spiffs_setup(){
	ESP_LOGI(TAG, "Initializing SPIFFS");
	esp_vfs_spiffs_conf_t conf ={
		.base_path = "/spiffs",
		.partition_label = NULL,
		.max_files = 10,
		.format_if_mount_failed = true

	};

	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if(ret != ESP_OK){
		if(ret == ESP_FAIL){
			ESP_LOGE(TAG,"Failed to mount or format filesystem");
		}
		else if(ret == ESP_ERR_NOT_FOUND){
			ESP_LOGE(TAG,"Failed to find SPIFFS partition");
		}
		else{
			ESP_LOGE(TAG,"Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return;
	}
	size_t total = 0, used = 0;
	ret = esp_spiffs_info(conf.partition_label,&total,&used);
	if (ret != ESP_OK){
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	}
	else{
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d",total, used);
		struct stat st;
		if(stat("/spiffs/index.html",&st) == 0){
			ESP_LOGI(TAG,"Found index.html file");
		}
		else{
			ESP_LOGI(TAG,"Can't find index.html file");
		}
		if(stat("/spiffs/style.css",&st) == 0){
			ESP_LOGI(TAG,"Found style.css file");
		}
		else{
			ESP_LOGI(TAG,"Can't find style.css file");
		}
		if(stat("/spiffs/script.js",&st) == 0){
			ESP_LOGI(TAG,"Found script.js file");
		}
		else{
			ESP_LOGI(TAG,"Can't find script.js file");
		}

	}
}

int readFile(char *fname,httpd_req_t *req){
	int res;
	//int size;
	char buf[1024];
	
	FILE *fd = fopen(fname,"rb");
	if(fd == NULL){
		ESP_LOGE(TAG,"ERROR opening file (%d) %s\n", errno,strerror(errno));
		httpd_resp_send_404(req);
		return 0;
	}
	do{
		res = fread(buf,1,sizeof(buf),fd);
		if(res > 0){
			httpd_resp_send_chunk(req,buf,res);
			printf("Read %d\n", res);
		}
	}while(res>0);
	httpd_resp_send_chunk(req,NULL,0);
	res = fclose(fd);
	if(res){
		printf("Error closing file\n");
	}
	return 1;
	/*
	if(size == 0){
		ESP_LOGE(TAG,"ERROR - file size zero");
		return NULL;
	}
	buf = (char*) calloc(size,sizeof(char*));
	if(buf == NULL){
		ESP_LOGE(TAG,"ERROR allocating read buffer");
		free(buf);
		fclose(fd);
		return NULL;
	}
	res = size;
	res = fread(buf,1,size,fd);
	if(res <= 0){
		ESP_LOGE(TAG,"ERROR reading from file");
	}
	else{
		ESP_LOGI(TAG,"%d bytes read",res);
		buf[res] = '\0';
	}
	return ((char*) buf);

	res = fclose(fd);
	if(res){
		ESP_LOGE(TAG,"ERROR closing file");
	}
	free(buf);
	return ((char*) buf);*/

}

esp_err_t send_indexhtml(httpd_req_t *req){
	//char* indexbuf;
	//size_t indexbuf_len;
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	
	/*
	char* resp_str = (char*) readFile("/spiffs/index.html");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/html; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	free(resp_str);
	*/
	printf("main page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/index.html",req);
	/*
	indexbuf_len = httpd_req_get_url_query_len(req) + 1;
	if(indexbuf_len > 1){
		indexbuf = malloc(indexbuf_len);
		if(httpd_req_get_url_query_str(req,indexbuf,indexbuf_len) == ESP_OK){
			ESP_LOGI(TAG,"Found URL query => %s",indexbuf);
		}
		free(indexbuf);
	}
	*/
	return ESP_OK;
}

esp_err_t send_senthtml(httpd_req_t *req){
	char* indexbuf;
	size_t indexbuf_len;
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	
	/*
	char* resp_str = (char*) readFile("/spiffs/index.html");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/html; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	free(resp_str);
	*/
	printf("sent_html page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/sent.html",req);
	indexbuf_len = httpd_req_get_url_query_len(req) + 1;
	if(indexbuf_len > 1){
		indexbuf = malloc(indexbuf_len);
		if(httpd_req_get_url_query_str(req,indexbuf,indexbuf_len) == ESP_OK){
			ESP_LOGI(TAG,"Found URL query => %s",indexbuf);
			char param[1024];

			if(httpd_query_key_value(indexbuf,"pixelarray",param,sizeof(param)) == ESP_OK){
				ESP_LOGI(TAG,"FOUND URL query parameter => %s",param);
			}
			strcpy(pixelinfo_query,param);
			pixelinfo_flag = 1;
			/*
			int x = 0;
			int y = 1;
			pixelinfo[0][0] = toupper(param[0]);
			
			for (int i =1; i < strlen(param); i++){
				if(param[i] == 'C'){
					pixelinfo[x][y] = toupper(param[i + 1]);
					y++;
					if(y == 8){
						y = 0;
						x++;
					}
				}
			}
			for (int h = 0; h < 8; h++){
				for (int j = 0; j < 8; j++){
					printf("%c",pixelinfo[h][j]);
					
				}
				printf("\n");
			}
			//printf("pixelinfo array: %c",pixelinfo[]);
			*/
		}

		free(indexbuf);
	}
	return ESP_OK;
}

esp_err_t send_drawinghtml(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	
	/*
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	const char* resp_str = (const char*) readFile("/spiffs/pixeldrawing.html");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/html; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("drawing page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/pixeldrawing.html",req);
	
	return ESP_OK;
}

esp_err_t send_pickinghtml(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	
	/*
	const char* resp_str = (const char*) readFile("/spiffs/imagepicking.html");
	
	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/html; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("picking page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/imagepicking.html",req);
	
	return ESP_OK;
}

esp_err_t send_indexscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/script.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");
	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("index script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/script.js",req);
	return ESP_OK;
}

esp_err_t send_drawingscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/drawingscript.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("drawing script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/drawingscript.js",req);

	return ESP_OK;
}

esp_err_t send_pickingscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/pickingscript.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("picking script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/pickingscript.js",req);
	return ESP_OK;
}

esp_err_t send_css(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/style.css");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/css; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("css requested\r\n");
	httpd_resp_set_type(req,"text/css");
	readFile("/spiffs/style.css",req);
	
	return ESP_OK;
}

esp_err_t send_bootstrapcss(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.css");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/css; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("bootstrap css requested\r\n");
	httpd_resp_set_type(req,"text/css");
	readFile("/spiffs/bootstrap.min.css",req);
	
	return ESP_OK;
}

esp_err_t send_jqueryscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/jquery-3.5.1.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("jquery script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/jquery-3.5.1.min.js",req);
	
	return ESP_OK;
}

esp_err_t send_bootstrapscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("bootstrap script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/bootstrap.min.js",req);

	return ESP_OK;
}

esp_err_t send_image1(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("image1 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/person.png",req);

	return ESP_OK;
}

esp_err_t send_image2(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("image2 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/pikachu.png",req);

	return ESP_OK;
}

esp_err_t send_image3(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("image3 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/sonic.png",req);

	return ESP_OK;
}

esp_err_t send_image4(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	//char *message = "hello world!";
	//httpd_resp_send(req,message,strlen(message));
	/*
	const char* resp_str = (const char*) readFile("/spiffs/bootstrap.min.js");

	//seting response headers
	httpd_resp_set_hdr(req,"status","200");
	httpd_resp_set_hdr(req,"content-type","text/javascript; charset=UTF-8");
	//httpd_resp_set_hdr(req,"server","ESP32-10103");

	httpd_resp_send(req,resp_str, strlen(resp_str));
	*/
	printf("image4 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/kirby.png",req);

	return ESP_OK;
}

void serverconfig(){
    httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.max_uri_handlers = 20;
	config.uri_match_fn = httpd_uri_match_wildcard;
	ESP_LOGI(TAG, "starting server");
	if(httpd_start(&server,&config) != ESP_OK){
		ESP_LOGE(TAG, "COULD NOT START SERVER");
	}

	httpd_uri_t index_html = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = send_indexhtml
	};

	httpd_uri_t sent_html = {
		.uri = "/sent*",
		.method = HTTP_GET,
		.handler = send_senthtml
	};

	httpd_uri_t drawing_html = {
		.uri = "/pixeldrawing.html",
		.method = HTTP_GET,
		.handler = send_drawinghtml
	};

	httpd_uri_t picking_html = {
		.uri = "/imagepicking.html",
		.method = HTTP_GET,
		.handler = send_pickinghtml
	};


	httpd_uri_t index_script = {
		.uri = "/script.js",
		.method = HTTP_GET,
		.handler = send_indexscript
	};
	
	httpd_uri_t drawing_script = {
		.uri = "/drawingscript.js",
		.method = HTTP_GET,
		.handler = send_drawingscript
	};

	httpd_uri_t picking_script = {
		.uri = "/pickingscript.js",
		.method = HTTP_GET,
		.handler = send_pickingscript
	};

	httpd_uri_t css_style = {
		.uri = "/style.css",
		.method = HTTP_GET,
		.handler = send_css
	};

	httpd_uri_t bootstrap_css = {
		.uri = "/bootstrap.min.css",
		.method = HTTP_GET,
		.handler = send_bootstrapcss
	};

	httpd_uri_t jquery_script = {
		.uri = "/bootstrap.min.js",
		.method = HTTP_GET,
		.handler = send_jqueryscript
	};

	httpd_uri_t bootstrap_script = {
		.uri = "/jquery-3.5.1.min.js",
		.method = HTTP_GET,
		.handler = send_bootstrapscript
	};

	httpd_uri_t image1 = {
		.uri = "/person.png",
		.method = HTTP_GET,
		.handler = send_image1
	};

	httpd_uri_t image2 = {
		.uri = "/pikachu.png",
		.method = HTTP_GET,
		.handler = send_image2
	};

	httpd_uri_t image3 = {
		.uri = "/sonic.png",
		.method = HTTP_GET,
		.handler = send_image3
	};

	httpd_uri_t image4 = {
		.uri = "/kirby.png",
		.method = HTTP_GET,
		.handler = send_image4
	};
	
	httpd_register_uri_handler(server, &index_html);
	httpd_register_uri_handler(server, &sent_html);
	httpd_register_uri_handler(server, &drawing_html);
	httpd_register_uri_handler(server, &picking_html);
	httpd_register_uri_handler(server, &index_script);
	httpd_register_uri_handler(server, &drawing_script);
	httpd_register_uri_handler(server, &picking_script);
	httpd_register_uri_handler(server, &css_style);
	httpd_register_uri_handler(server, &jquery_script);
	httpd_register_uri_handler(server, &bootstrap_script);
	httpd_register_uri_handler(server, &bootstrap_css);
	httpd_register_uri_handler(server, &image1);
	httpd_register_uri_handler(server, &image2);
	httpd_register_uri_handler(server, &image3);
	httpd_register_uri_handler(server, &image4);
}