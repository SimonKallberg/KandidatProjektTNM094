#include "libwebsockets.h"
#include <fstream>


#ifndef Serverhandler
#define Serverhandler

namespace ServerHandler
{
	const int MAX_PLAYERS = 100;
	const int CONTROLS_SIZE = 100;

	#define EXAMPLE_RX_BUFFER_BYTES (MAX_PLAYERS*CONTROLS_SIZE+10)

	struct lws *web_socket;
	struct lws_context *context;

	typedef void(*MessageCallbackFunction)(const char *, size_t);
	MessageCallbackFunction msgCallFun = NULL;

	bool server_notified = false;

	int callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
	{
		switch (reason)
		{
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			std::cout << "Connecting to server...\n";
			break;

		case LWS_CALLBACK_CLIENT_RECEIVE:
			//std::cout << reinterpret_cast<const char *>(in);
			if (msgCallFun)
				msgCallFun(reinterpret_cast<const char *>(in),len);

			break;

		case LWS_CALLBACK_CLIENT_WRITEABLE:
		{
			if (!server_notified) {
				std::cout << "Server connection established.\n";
				unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + EXAMPLE_RX_BUFFER_BYTES + LWS_SEND_BUFFER_POST_PADDING];
				unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
				size_t n = sprintf((char *)p, "GAME CONNECTION ESTABLISHED");
				lws_write(wsi, p, n, LWS_WRITE_TEXT);
				server_notified = true;
			}
			break;
		}

		case LWS_CALLBACK_CLOSED:
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			web_socket = NULL;
			server_notified = false;
			break;

		default:
			break;
		}

		return 0;
	}

	enum protocols
	{
		PROTOCOL_EXAMPLE = 0,
		PROTOCOL_COUNT
	};

	const struct lws_protocols protocols[]
	{
		{
			"example-protocol",
			callback,
			0,
			EXAMPLE_RX_BUFFER_BYTES,
		},
		{ NULL, NULL, 0, 0 } /* terminator */
	};

	
	bool connect() {
		struct lws_context_creation_info info;
		memset(&info, 0, sizeof(info));

		info.port = CONTEXT_PORT_NO_LISTEN;
		info.protocols = protocols;
		info.gid = -1;
		info.uid = -1;

		context = lws_create_context(&info);

		struct lws_client_connect_info ccinfo = { 0 };
		ccinfo.context = context;

		std::ifstream ifs("../server_ip.txt", std::ifstream::in);
		if (!ifs) {
			std::cout << "Cannot open ../server_ip.txt. Defaulting to localhost:80\n";
			ccinfo.address = "localhost";
			ccinfo.port = 80;
		}
		else {
			std::cout << "Found ../server_ip.txt...\n";

			char ip[128] = "failed";
			ifs >> ip;
			ccinfo.address = ip;

			int port = -1;
			ifs >> port;
			ccinfo.port = port;

			std::cout << "Attempting connection to: " << ip << ":" << port << "\n";
		}

		ccinfo.path = "/";
		ccinfo.host = lws_canonical_hostname(context);
		ccinfo.origin = "origin";
		ccinfo.protocol = protocols[PROTOCOL_EXAMPLE].name;
		web_socket = lws_client_connect_via_info(&ccinfo);

		return web_socket;
	}

	void setMessageCallback(MessageCallbackFunction fn) {
		msgCallFun = fn;
	}

	void service() {
		lws_service(context, 0);
	}

}

#endif
