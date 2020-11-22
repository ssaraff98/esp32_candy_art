#include "myserver_config.h"
#include "mytask.h"

char pixelinfo_query[1024];
int pixelinfo_flag = 0;
int reset_flag = 0;
int initialize_flag = 0;

TaskHandle_t testtaskhandle = NULL;
TaskHandle_t steppertask = NULL;


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
			ESP_LOGI(TAG,"Found spiffs files");
		}
		else{
			ESP_LOGI(TAG,"Can't find spiffs files");
		}
	}
}

int readFile(char *fname,httpd_req_t *req){
	int res;
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
}

esp_err_t send_indexhtml(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("main page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/index.html",req);
	
	return ESP_OK;
}

esp_err_t send_senthtml(httpd_req_t *req){
	char* indexbuf;
	size_t indexbuf_len;
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	
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
			printf("Task Created");
			pixelinfo_flag = 1;
			xTaskCreate(&test_task, "test_task", 2048, NULL, 1, &testtaskhandle);
		}

		free(indexbuf);
	}
	return ESP_OK;
}

esp_err_t send_drawinghtml(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("drawing page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/pixeldrawing.html",req);
	
	return ESP_OK;
}

esp_err_t send_pickinghtml(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("picking page requested\r\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/imagepicking.html",req);
	
	return ESP_OK;
}

esp_err_t send_indexscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
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
	printf("picking script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/pickingscript.js",req);
	return ESP_OK;
}

esp_err_t send_sentscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("sent script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/scriptforsent.js",req);
	return ESP_OK;
}

esp_err_t send_css(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	
	printf("css requested\r\n");
	httpd_resp_set_type(req,"text/css");
	readFile("/spiffs/style.css",req);
	
	return ESP_OK;
}

esp_err_t send_bootstrapcss(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("bootstrap css requested\r\n");
	httpd_resp_set_type(req,"text/css");
	readFile("/spiffs/bootstrap.min.css",req);
	
	return ESP_OK;
}

esp_err_t send_jqueryscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("jquery script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/jquery-3.5.1.min.js",req);
	
	return ESP_OK;
}

esp_err_t send_bootstrapscript(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("bootstrap script requested\r\n");
	httpd_resp_set_type(req,"text/javascript");
	readFile("/spiffs/bootstrap.min.js",req);

	return ESP_OK;
}

esp_err_t send_image1(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("image1 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/person.png",req);

	return ESP_OK;
}

esp_err_t send_image2(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("image2 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/pikachu.png",req);

	return ESP_OK;
}

esp_err_t send_image3(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	printf("image3 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/sonic.png",req);

	return ESP_OK;
}

esp_err_t send_image4(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	
	printf("image4 requested\r\n");
	httpd_resp_set_type(req,"image/png");
	readFile("/spiffs/kirby.png",req);

	return ESP_OK;
}

esp_err_t reset_req(httpd_req_t *req){
	ESP_LOGI(TAG, "url %s was hit", req->uri);
	
	printf("Restarting in 3 sec\n");
	httpd_resp_set_type(req,"text/html");
	readFile("/spiffs/reset.html",req);
	initialize_flag = 1;
	vTaskDelete(testtaskhandle);
	vTaskDelete(steppertask);
	
	printf("Task aborted");
	
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

	httpd_uri_t sent_script = {
		.uri = "/scriptforsent.js",
		.method = HTTP_GET,
		.handler = send_sentscript
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

	httpd_uri_t reset = {
		.uri = "/reset.html",
		.method = HTTP_GET,
		.handler = reset_req
	};
	
	httpd_register_uri_handler(server, &index_html);
	httpd_register_uri_handler(server, &sent_html);
	httpd_register_uri_handler(server, &drawing_html);
	httpd_register_uri_handler(server, &picking_html);
	httpd_register_uri_handler(server, &index_script);
	httpd_register_uri_handler(server, &drawing_script);
	httpd_register_uri_handler(server, &picking_script);
	httpd_register_uri_handler(server, &sent_script);
	httpd_register_uri_handler(server, &css_style);
	httpd_register_uri_handler(server, &jquery_script);
	httpd_register_uri_handler(server, &bootstrap_script);
	httpd_register_uri_handler(server, &bootstrap_css);
	httpd_register_uri_handler(server, &image1);
	httpd_register_uri_handler(server, &image2);
	httpd_register_uri_handler(server, &image3);
	httpd_register_uri_handler(server, &image4);
	httpd_register_uri_handler(server, &reset);
}