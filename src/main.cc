#include <glog/logging.h>

int main(int argc, char * argv[])
{
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "lamiglowka";

    return 0;
}

