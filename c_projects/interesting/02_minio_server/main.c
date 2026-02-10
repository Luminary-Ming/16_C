#include "minio_server.h"

int main()
{
    MinioConfig *config = minio_create();

    //int ret = minio_upload(config, "Yeah.jpg", "Yeah.jpg");
    char * url = minio_preview_url(config, "Yeah.jpg");
    printf("返回结果 : %s\n", url);

    return 0;
}