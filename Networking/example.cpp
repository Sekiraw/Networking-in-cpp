#include <iostream>

#ifdef _WIN32
#define _WIN_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);
void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (int i = 0; i < length; i++) {
					std::cout << vBuffer[i];
				}

				GrabSomeData(socket);
			}
		}
	);

}


int main() 
{
	asio::error_code ec;

	// Create the context
	asio::io_context context;

	// Get the address we want to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("172.217.14.206", ec), 80);

	// Create a socket
	asio::ip::tcp::socket socket(context);

	// Tell the socket to try and connect
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open()) {
		std::string sRequest =
			"GET /intex.html HTTP/1.1\r\n"
			"Host: facebook.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// Wait for data
		GrabSomeData(socket);
	}

	system("pause");
	return 0;
}