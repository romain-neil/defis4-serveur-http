#include "HttpServer.h"

int main(int argc, char* argv[]) {
	HttpServer http(8080);

	http.describe();
	http.start();

	return 0;
}
