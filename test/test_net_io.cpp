#include "../io/net_io_stream_channel.hpp"

void test_client()
{
	NetIO client("client", "127.0.0.1", 8080);

	std::string message;

	std::getline(std::cin, message);
	client.SendString(message);

	std::getline(std::cin, message);
	client.SendString(message);
}

void test_server()
{
	NetIO server("server", "", 8080); 
	
	std::string message;


	server.ReceiveString(message);
	std::cout << "message from client: " << message << std::endl; 

	server.ReceiveString(message);
	std::cout << "message from client: " << message << std::endl; 
}

void test_netio(std::string party)
{
	if (party == "server")
	{
		test_server(); 
	}

	if (party == "client")
	{
		test_client(); 
	}

}

int main()
{

    std::string party; 
    std::getline(std::cin, party); // first receiver (acts as server), then sender (acts as client)
	test_netio(party);

	return 0; 
}