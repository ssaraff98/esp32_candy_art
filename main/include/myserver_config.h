#include <errno.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include "esp_http_client.h"
#include "esp_http_server.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#define TAG "MY SERVER"

void spiffs_setup();
int readFile(char *fname,httpd_req_t *req);
esp_err_t send_indexhtml(httpd_req_t *req);
esp_err_t send_senthtml(httpd_req_t *req);
esp_err_t send_drawinghtml(httpd_req_t *req);
esp_err_t send_pickinghtml(httpd_req_t *req);
esp_err_t send_indexscript(httpd_req_t *req);
esp_err_t send_drawingscript(httpd_req_t *req);
esp_err_t send_pickingscript(httpd_req_t *req);
esp_err_t send_sentscript(httpd_req_t *req);
esp_err_t send_css(httpd_req_t *req);
esp_err_t send_bootstrapcss(httpd_req_t *req);
esp_err_t send_jqueryscript(httpd_req_t *req);
esp_err_t send_bootstrapscript(httpd_req_t *req);
esp_err_t send_image1(httpd_req_t *req);
esp_err_t send_image2(httpd_req_t *req);
esp_err_t send_image3(httpd_req_t *req);
esp_err_t send_image4(httpd_req_t *req);
esp_err_t reset_req(httpd_req_t *req);
void serverconfig();